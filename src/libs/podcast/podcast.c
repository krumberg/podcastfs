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
	GHashTable* podcastitem_hash;
	const char* folder_name;
	const char* url;
};

Podcast* podcast_new_from_file(const char* file)
{
        Podcast* pcast = g_new0(Podcast, 1);

        debuglog("Creating XML-object");

        xmlDocPtr doc = xmlParseFile(file);
        xmlXPathContextPtr xml_context = xmlXPathNewContext(doc); 

        const xmlChar* attr_url_exp = "//enclosure/attribute::url";
        xmlXPathObjectPtr obj = xmlXPathEval(attr_url_exp, xml_context);
       
        debuglog("Survived");

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

