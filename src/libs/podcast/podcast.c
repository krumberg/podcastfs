/*
 *    podcastfs - a simple fuse filesystem for mounting podcasts
 *    Copyright (C) 2008,  Kristian Rumberg (kristianrumberg@gmail.com)
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <glib.h>
#include <gio/gio.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <debuglog/debuglog.h>
#include <podcast/podcast.h>
#include <podcast/podcasttrack.h>

struct Podcast {
	GHashTable* podcasttrack_hash; /* <filename, PodcastTrack> */
	gchar* folder_name;
	gchar* url;
};

static GList* xmlXPathEvalExpressionToGList(xmlDocPtr doc, const gchar* expression)
{
        xmlXPathContextPtr xpath_context = xmlXPathNewContext(doc);
        xmlXPathObjectPtr obj = xmlXPathEval((xmlChar*)expression, xpath_context);

        if(xmlXPathNodeSetIsEmpty(obj->nodesetval)){
		xmlXPathFreeObject(obj);
                debuglog("xmlXPathEval failed");
		return NULL;
        }

        GList* list = NULL;

        int i;
        for (i=0; i < obj->nodesetval->nodeNr; i++) {
                xmlChar* val = xmlNodeListGetString(doc, obj->nodesetval->nodeTab[i]->xmlChildrenNode, 1);
                list = g_list_prepend(list, g_strdup((const gchar*)val));
	        xmlFree(val);
	}

	xmlXPathFreeObject(obj);
        xmlXPathFreeContext(xpath_context);

        return list;
}

static void podcast_create_hashtable(Podcast* pcast, GList** p_etitle_list, GList** p_url_list, GList** p_size_list)
{
        pcast->podcasttrack_hash = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)podcasttrack_free);

        GList* etitle_list = *p_etitle_list;
        GList* url_list    = *p_url_list;
        GList* size_list   = *p_size_list;

        while(TRUE) {
                if (!etitle_list || !url_list || !size_list) break;

                PodcastTrack* podct = podcasttrack_new((const gchar*) etitle_list->data, (const gchar*) url_list->data,
                                                       atol((const gchar*) size_list->data));

                g_hash_table_insert(pcast->podcasttrack_hash, g_strdup(podcasttrack_filename(podct)), podct);

                etitle_list = g_list_delete_link(etitle_list, etitle_list);
                url_list    = g_list_delete_link(url_list, url_list);
                size_list   = g_list_delete_link(size_list, size_list);
        }

        *p_etitle_list = NULL;
        *p_url_list = NULL;
        *p_size_list = NULL;
}

static void g_list_free_all(GList** list)
{
        while (*list) {
                if ((*list)->data) {
                        g_free((*list)->data);
                }
                *list = g_list_delete_link(*list, *list);
        }
        *list = NULL;
}

static gchar* steal_string(GList** list)
{
        assert(*list!=NULL);
        gchar* str = (gchar*) (*list)->data;
        *list = g_list_delete_link(*list, *list);
        return str;
}

Podcast* podcast_new_from_file(const gchar* file)
{
        Podcast* pcast = g_new0(Podcast, 1);

        xmlDocPtr doc = xmlParseFile(file);

        GList* gtitle_list = xmlXPathEvalExpressionToGList(doc, "//channel/title");
        GList* etitle_list = xmlXPathEvalExpressionToGList(doc, "//item//title");
        GList* url_list    = xmlXPathEvalExpressionToGList(doc, "//enclosure/attribute::url");
        GList* size_list   = xmlXPathEvalExpressionToGList(doc, "//enclosure/attribute::length");

        if (!gtitle_list || !etitle_list || !url_list || !size_list) {
                debuglog("Unable to read podcast rss information");
                g_free(pcast);
                pcast = NULL;
                goto cleanup;
        }

        /* set name of folder (url will be set in the calling podcast_new_from_url) */
        pcast->folder_name = steal_string(&gtitle_list);

        podcast_create_hashtable(pcast, &etitle_list, &url_list, &size_list);

cleanup:
        g_list_free_all(&gtitle_list);
        g_list_free_all(&etitle_list);
        g_list_free_all(&url_list);
        g_list_free_all(&size_list);
        xmlFreeDoc(doc);

        return pcast;
}

static GFile* g_file_new_tmpfile()
{
        char buf[30];
        strcpy(buf, "/tmp/rssfileXXXXXX");
        close(mkstemp(buf));
        GFile* tmp_file = g_file_new_for_path(buf);

        return tmp_file;
}

Podcast* podcast_new_from_url(const gchar* url_path)
{
        GFile* url_file = NULL;
        GFile* tmp_file = NULL;
        gchar* tmp_file_path = NULL;
        Podcast* podcast = NULL;

        url_file = g_file_new_for_uri(url_path);
        if (NULL == url_file) {
                debuglog("Unable to resolve URL");
                goto cleanup;
        }
        tmp_file = g_file_new_tmpfile();
        if (NULL == tmp_file) {
                debuglog("Unable to create tmpfile");
                goto cleanup;
        }
        /* downloading rss */
        if (FALSE == g_file_copy(url_file, tmp_file, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL)) {
                debuglog("Downloading RSS failed");
                goto cleanup;
        }

        tmp_file_path = g_file_get_path(tmp_file);
        podcast = podcast_new_from_file(tmp_file_path);

cleanup:
        if (tmp_file) {
                g_file_delete(tmp_file, NULL, NULL);
                g_object_unref(G_OBJECT(tmp_file));
        }
        if (tmp_file_path) {
                g_free(tmp_file_path);
        }
        if (url_file) {
                g_object_unref(G_OBJECT(url_file));
        }
        return podcast;
}

void podcast_free(Podcast* pcast)
{
        if (pcast) {
                g_free(pcast->folder_name);
                g_free(pcast->url);
                g_hash_table_destroy(pcast->podcasttrack_hash);
                g_free(pcast);
        }
}

static void foreach_ghash_callback (gpointer key, gpointer value, gpointer user_data)
{
        pc_foreachname_callback callback = (pc_foreachname_callback) user_data;

        callback((gchar*)key);
}

void podcast_foreach_trackname(Podcast* pcast, pc_foreachname_callback callback)
{
        g_hash_table_foreach(pcast->podcasttrack_hash, foreach_ghash_callback, callback);
}

PodcastTrack* podcast_get_track(Podcast* pcast, const gchar* track_name)
{
        return (PodcastTrack*)g_hash_table_lookup(pcast->podcasttrack_hash, track_name);
}

gboolean podcast_has_track(Podcast* pcast, const gchar* item_name)
{
        return (FALSE != podcast_get_track(pcast, item_name));
}

const gchar* podcast_folder_name(Podcast* pcast)
{
        return pcast->folder_name;
}


