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
#include <assert.h>
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

        //Podcast* p = podcast_new_from_url("http://www.sr.se/Podradio/xml/Ekots_lordagsintervju.xml");
        Podcast* p = podcast_new_from_file("/tmp/Ekots_lordagsintervju.xml");
        if (NULL == p) {
                debuglog("ERROR: Unable to fetch URL");
                exit(-1);
        }
        g_hash_table_insert(list->podcast_hash, g_strdup(podcast_folder_name(p)), p);

        debuglog("Left podcastlist_add_default_podcasts");
}

PodcastList* podcastlist_get_instance()
{
        debuglog("Enter podcastlist_get_instance");
	static PodcastList* list = NULL;
	if (list == NULL) {
	        list = g_new(PodcastList, 1);
		list->podcast_hash = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)podcast_free);

                podcastlist_add_default_podcasts(list);
	}
	return list;
}

gboolean podcastlist_is_podcast_folder(PodcastList* list, const gchar* name)
{
        const gchar* key = name + 1;
        if (g_hash_table_lookup(list->podcast_hash, key)) {
                return TRUE;
        }
        return FALSE;
}

gboolean podcastlist_is_podcast_track(PodcastList* list, const gchar* folder_and_track)
{
        gboolean has = FALSE;

        gchar** folder_and_track_array = g_strsplit(folder_and_track + 1, "/", 2);

        if (NULL == folder_and_track) {
                debuglog("Failed to split string");
                goto cleanup;
        }

        debuglog("in podcastlist_is_podcast_track after split which resulted in");
        debuglog(folder_and_track_array[0]);
        debuglog(folder_and_track_array[1]);

        Podcast* pcast = (Podcast*) g_hash_table_lookup(list->podcast_hash, folder_and_track_array[0]);
        if (pcast == NULL) {
                goto cleanup;
        }
        if (podcast_has_track(pcast, folder_and_track_array[1])) {
                has = TRUE;
                goto cleanup;
        }

cleanup:
        if (folder_and_track_array) {
                g_strfreev(folder_and_track_array);
        }
        return has;
}

void podcastlist_foreach_trackname_in_folder(PodcastList* list, const gchar* name,
                                            pc_foreachname_callback callback)
{
        debuglog("Enter podcastlist_foreach_trackname_in_folder");

        debuglog(name);

        Podcast* pcast = (Podcast*) g_hash_table_lookup(list->podcast_hash, (name + 1));

        if (pcast==NULL) {
                debuglog("ERROR no such folder");
                exit(-1);
        }

        debuglog("About to call podcastlist_foreach_trackname");

        podcast_foreach_trackname(pcast, callback);
}

static void foreach_ghash_callback (gpointer key, gpointer value, gpointer user_data)
{
        debuglog("Called foreach_ghash_callback");

        pc_foreachname_callback callback = (pc_foreachname_callback) user_data;

        callback((gchar*)key);

        debuglog("Left foreach_ghash_callback");
}

void podcastlist_foreach_foldername(PodcastList* list, pc_foreachname_callback callback)
{
        debuglog("Enter podcastlist_foreach_foldername");
        assert(list!=NULL);
        assert(list->podcast_hash!=NULL);

        g_hash_table_foreach(list->podcast_hash, foreach_ghash_callback, callback);
        debuglog("Leaving podcastlist_foreach_foldername");
}

size_t podcastlist_get_item_size(PodcastList* list, const gchar* folder_and_item)
{
        return 6;
}

int podcastlist_read_item(PodcastList* list, const gchar* folder_and_item, gchar* buf,
                          size_t size, size_t offset)
{
	/* /FOLDER/SONG.MP3 */
        const gchar* song = "/ekot/song.mp3";
        const gchar* text = "Hejsan";

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

