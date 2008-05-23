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

#include "podcastlist.h"
#include <string.h>

struct Podcastlist {

};

PodcastList* podcastlist_get_instance()
{
        return 0;
}

int podcastlist_is_podcast_folder(PodcastList* list, const char* name)
{
        if (!strcmp(name, "ekot")) return 1;
        return 0;
}

int podcastlist_is_podcast_item(PodcastList* list, const char* folder_and_item)
{
        if (!strcmp(folder_and_item, "ekot/song.mp3")) return 1;
        return 0;
}

void podcastlist_foreach_itemname_in_folder(PodcastList* list, const char* name,
                                            pcl_foreachname_callback fill_item)
{
        fill_item("/ekot/song.mp3");
}

void podcastlist_foreach_foldername(PodcastList* list, pcl_foreachname_callback fill_item)
{
        fill_item("ekot");
}

size_t podcastlist_get_item_size(PodcastList* list, const char* folder_and_item)
{
        return 6;
}

int podcastlist_read_item(PodcastList* list, const char* folder_and_item, char* buf,
                          size_t size, size_t offset)
{
        const char* song = "ekot/song.mp3";
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

