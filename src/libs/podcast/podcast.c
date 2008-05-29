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
                printf("%s\n", val);
                list = g_list_prepend(list, g_strdup((const gchar*)val));
	        xmlFree(val);
	}

	xmlXPathFreeObject(obj);
        xmlXPathFreeContext(xpath_context);
	
        return list;
}

static void gfree_with_userparam(void* data, void* userparam)
{
        if (data) { 
                g_free(data);
        }
}

static void g_list_free_everything(GList* list)
{
        if (list) { 
                g_list_foreach(list, gfree_with_userparam, NULL);
                g_list_free(list);
        }
}

static void podcast_create_hashtable(Podcast* pcast, GList* etitle_list, GList* url_list, GList* size_list) 
{
        pcast->podcastitem_hash = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

        while(TRUE) {
                if (!etitle_list || !url_list || !size_list) break;
                
                puts(etitle_list->data);
                puts(url_list->data);

                etitle_list = g_list_delete_link(etitle_list, etitle_list);
                url_list    = g_list_delete_link(url_list, url_list);
                size_list   = g_list_delete_link(size_list, size_list);
        }
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
        pcast->folder_name = (gchar*) gtitle_list->data;
        gtitle_list->data = NULL;
        
        podcast_create_hashtable(pcast, etitle_list, url_list, size_list);

        puts("Before cleanup");
cleanup:
        g_list_free_everything(gtitle_list);
        g_list_free_everything(etitle_list);
        g_list_free_everything(url_list);
        g_list_free_everything(size_list);

        puts("Before xmlFreeDoc");

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

