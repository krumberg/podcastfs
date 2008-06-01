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

#ifndef __PODCAST_H
#define __PODCAST_H

#include <glib.h>

typedef void (*pc_foreachname_callback)(const gchar* name);

typedef struct Podcast Podcast;

Podcast* podcast_new_from_file(const gchar* file);
Podcast* podcast_new_from_url(const gchar* url);
void podcast_free(Podcast* pcast);

void podcast_foreach_trackname(Podcast* pcast, pc_foreachname_callback callback);
gboolean podcast_has_track(Podcast* pcast, const gchar* track_name);
const gchar* podcast_folder_name(Podcast* pcast);

#endif

