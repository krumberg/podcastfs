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

#ifndef __PODCASTLIST_H
#define __PODCASTLIST_H

#include <stddef.h>
#include "podcast.h"

typedef struct PodcastList PodcastList;

PodcastList* podcastlist_get_instance();

int podcastlist_is_podcast_folder(PodcastList* list, const char* name);
int podcastlist_is_podcast_item(PodcastList* list, const char* folder_and_item);

void podcastlist_foreach_itemname_in_folder(PodcastList* list, const char* name, pc_foreachname_callback callback);
void podcastlist_foreach_foldername(PodcastList* list, pc_foreachname_callback callback);

size_t podcastlist_get_item_size(PodcastList* list, const char* folder_and_item);
int podcastlist_read_item(PodcastList* list, const char* folder_and_item, char* buf, size_t size, size_t offset);

#endif /* __PODCASTLIST.H */
