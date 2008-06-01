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

#ifndef __PODCASTTRACK_H
#define __PODCASTTRACK_H

#include <glib.h>

typedef struct PodcastTrack PodcastTrack;

PodcastTrack* podcasttrack_new(const gchar* title, const gchar* url, size_t size);
void podcasttrack_free(PodcastTrack* podct);

const gchar* podcasttrack_filename(PodcastTrack* podct);
size_t podcasttrack_size(PodcastTrack* podct);

#endif /* __PODCASTTRACK.H */
