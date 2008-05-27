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

#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <debuglog/debuglog.h>
#include <podcast/podcastlist.h>
#include <podcast/podcast.h>

struct PodcastList {
	GHashTable* podcast_hash;
};

static void podcastlist_add_default_podcasts(PodcastList* list) 
{
        debuglog("Enter podcastlist_add_default_podcasts");
        return;

        //Podcast* p = podcast_new_from_url("http://www.sr.se/Podradio/xml/Ekots_lordagsintervju.xml");
        Podcast* p = podcast_new_from_file("Ekots_lordagsintervju.xml");
        if (NULL == p) {
                debuglog("ERROR: Unable to fetch URL");
                exit(-1);
        }
        g_hash_table_insert(list->podcast_hash, g_strdup(podcast_folder_name(p)), p); 
}

PodcastList* podcastlist_get_instance()
{
        debuglog("Enter podcastlist_get_instance");
	static PodcastList* list = NULL;
	if (list == NULL) {
	        PodcastList* list = g_new(PodcastList, 1);
		list->podcast_hash = g_hash_table_new_full(g_str_hash, (GEqualFunc)strcmp, g_free, g_free);
	        
                podcastlist_add_default_podcasts(list);
	}
	return list;
}

int podcastlist_is_podcast_folder(PodcastList* list, const char* name)
{
	/* /FOLDER */
        if (!strcmp(name, "/ekot")) return 1;
        return 0;
}

int podcastlist_is_podcast_item(PodcastList* list, const char* folder_and_item)
{
	/* /FOLDER/SONG.MP3 */
        if (!strcmp(folder_and_item, "/ekot/song.mp3")) return 1;
        return 0;
}

void podcastlist_foreach_itemname_in_folder(PodcastList* list, const char* name,
                                            pc_foreachname_callback callback)
{
	/* SONG.MP3 */
        callback("song.mp3");
}

void podcastlist_foreach_foldername(PodcastList* list, pc_foreachname_callback callback)
{
        callback("ekot");
}

size_t podcastlist_get_item_size(PodcastList* list, const char* folder_and_item)
{
        return 6;
}

int podcastlist_read_item(PodcastList* list, const char* folder_and_item, char* buf,
                          size_t size, size_t offset)
{
	/* /FOLDER/SONG.MP3 */
        const char* song = "/ekot/song.mp3";
        const char* text = "Hejsan";

	size_t len;
	if(strcmp(folder_and_item, song) != 0) {
		return -1;
        }

	len = strlen(text);
	if (offset < len) {
		if (offset + size > len) {
			size = len - offset;
                }
		memcpy(buf, text + offset, size);
	} else {
		size = 0;
        }

	return size;
}

