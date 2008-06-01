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

static Podcast* podcastlist_get_podcast(PodcastList* list, const char* name)
{
        return (Podcast*) g_hash_table_lookup(list->podcast_hash, name);
}

gboolean podcastlist_is_podcast_folder(PodcastList* list, const gchar* name)
{
        return (FALSE != podcastlist_get_podcast(list, name));
}

static PodcastTrack* podcastlist_get_podcast_track(PodcastList* list, const gchar* folder_and_track)
{
        Podcast* pcast = NULL;
        PodcastTrack* ptrack = NULL;

        gchar** folder_and_track_array = g_strsplit(folder_and_track + 1, "/", 2);

        if (NULL == folder_and_track) {
                debuglog("Failed to split string");
                goto cleanup;
        }

        debuglog("in podcastlist_get_podcast_track after split from");
        debuglog(folder_and_track);
        debuglog("resulted in");
        debuglog(folder_and_track_array[0]);
        debuglog(folder_and_track_array[1]);

        pcast = podcastlist_get_podcast(list, folder_and_track_array[0]);
        if (NULL == pcast) {
                goto cleanup;
        }
        ptrack = podcast_get_track(pcast, folder_and_track_array[1]);
cleanup:
        if (folder_and_track_array) {
                g_strfreev(folder_and_track_array);
        }
        return ptrack;
}

gboolean podcastlist_is_podcast_track(PodcastList* list, const gchar* folder_and_track)
{
        return (FALSE != podcastlist_get_podcast_track(list, folder_and_track));
}

void podcastlist_foreach_trackname_in_folder(PodcastList* list, const gchar* name,
                                            pc_foreachname_callback callback)
{
        debuglog("Enter podcastlist_foreach_trackname_in_folder");

        debuglog(name);

        Podcast* pcast = podcastlist_get_podcast(list, name + 1);

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

size_t podcastlist_get_track_size(PodcastList* list, const gchar* folder_and_item)
{
        return 6;
}

int podcastlist_read_track(PodcastList* list, const gchar* folder_and_track, gchar* buf,
                          size_t size, size_t offset)
{
        PodcastTrack* track = podcastlist_get_podcast_track(list, folder_and_track);

        if (NULL == track) {
                debuglog("Error: No such file found");
                return -1;
        }

        return podcasttrack_read(track, buf, size, offset);
}
