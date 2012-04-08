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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pwd.h>
#include <glib.h>
#include <debuglog/debuglog.h>
#include <podcast/podcastlist.h>
#include <podcast/podcast.h>
#include <podcast/util.h>

struct PodcastList {
	GTree* podcast_tree;
};

static void podcastlist_add_podcast(PodcastList* list, const gchar* url)
{
	Podcast* p = podcast_new_from_url(url);
	if (NULL == p) {
		debuglog("ERROR: Unable to fetch URL");
		return;
	}
	g_tree_insert(list->podcast_tree, g_strdup(podcast_folder_name(p)), p);
}

static void podcastlist_add_default_podcasts(PodcastList* list)
{
	struct passwd* pws;
	pws = getpwuid(geteuid());

	char conf_path[512] = {0,};
	sprintf(conf_path, "%s/.podcastfslist", getenv("HOME"));

	FILE* rss_conf_file = fopen(conf_path, "rt");
	if (NULL == rss_conf_file) {
		debuglog("Unable to open config file");
		return;
	}

	char buf[512] = {0,};
	while(1) {
		if (fgets(buf, 511, rss_conf_file) != buf) {
			break;
		}
		if (strlen(buf) > 0) {
			buf[strlen(buf) - 1] = 0;
		}
		const char* p_beg = strstr(buf, "http");
		if (NULL == p_beg) {
			break;
		}
		podcastlist_add_podcast(list, p_beg);
		debuglog("Added podcast %s", p_beg);
	}

	fclose(rss_conf_file);
}

PodcastList* podcastlist_get_instance()
{
	debuglog("Enter podcastlist_get_instance");
	static PodcastList* list = NULL;
	if (list == NULL) {
		list = g_new(PodcastList, 1);
		list->podcast_tree = g_tree_new_full(strcmp_with_data, NULL, g_free, (GDestroyNotify)podcast_free);
		podcastlist_add_default_podcasts(list);
	}
	return list;
}

static Podcast* podcastlist_get_podcast(PodcastList* list, const char* name)
{
	return (Podcast*) g_tree_lookup(list->podcast_tree, name);
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

static gboolean foreach_callback (gpointer key, gpointer value, gpointer user_data)
{
	debuglog("Called foreach_ghash_callback");

	pc_foreachname_callback callback = (pc_foreachname_callback) user_data;

	callback((gchar*)key);

	debuglog("Left foreach_ghash_callback");

	return FALSE;
}

void podcastlist_foreach_foldername(PodcastList* list, pc_foreachname_callback callback)
{
	debuglog("Enter podcastlist_foreach_foldername");
	assert(list!=NULL);
	assert(list->podcast_tree!=NULL);

	g_tree_foreach(list->podcast_tree, foreach_callback, callback);
	debuglog("Leaving podcastlist_foreach_foldername");
}

size_t podcastlist_get_track_size(PodcastList* list, const gchar* folder_and_track)
{
	PodcastTrack* track = podcastlist_get_podcast_track(list, folder_and_track);

	if (NULL == track) {
		debuglog("Error: No such file found");
		return -1;
	}

	return podcasttrack_size(track);
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
