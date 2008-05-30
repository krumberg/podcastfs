/*
 *    podcastfs - a simple fuse filesystem for mounting podcast
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

#include <assert.h>
#include <glib.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <debuglog/debuglog.h>
#include <podcast/podcast.h>

struct Podcast {
	GHashTable* podcastitem_hash; /* <filename, PodcastEntry> */
	const char* folder_name;
	const char* url;
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

        for (int i=0; i < obj->nodesetval->nodeNr; i++) {
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
        pcast->podcastitem_hash = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

        GList* etitle_list = *p_etitle_list;
        GList* url_list    = *p_url_list;
        GList* size_list   = *p_size_list;

        while(TRUE) {
                if (!etitle_list || !url_list || !size_list) break;

                puts(etitle_list->data);
                puts(url_list->data);
                puts(size_list->data);

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

static gchar* steal_string(GList* list)
{
        assert(list!=NULL);
        gchar* str = (gchar*) list->data;
        list->data = NULL;
        return str;
}

Podcast* podcast_new_from_file(const char* file)
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
        pcast->folder_name = steal_string(gtitle_list);

        podcast_create_hashtable(pcast, &etitle_list, &url_list, &size_list);

cleanup:
        g_list_free_all(&gtitle_list);
        g_list_free_all(&etitle_list);
        g_list_free_all(&url_list);
        g_list_free_all(&size_list);
        xmlFreeDoc(doc);

        return pcast;
}

Podcast* podcast_new_from_url(const char* url)
{
        return NULL;
}

void podcast_foreach_item(Podcast* pcast, pc_foreachname_callback callback)
{
}

void podcast_has_item(Podcast* pcast, const char* item_name)
{
}

const char* podcast_folder_name(Podcast* pcast)
{
        return pcast->folder_name;
}

