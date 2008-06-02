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

#include <string.h>
#include <glib.h>
#include <gio/gio.h>
#include <debuglog/debuglog.h>
#include <podcast/podcasttrack.h>

struct PodcastTrack {
        gchar* filename;
        gchar* url;
        size_t size;
};

PodcastTrack* podcasttrack_new(const gchar* title, const gchar* url, size_t size)
{
        PodcastTrack* podct = g_new(PodcastTrack, 1);

        podct->filename = g_strconcat(title, ".mp3", NULL);
        podct->url      = g_strdup(url);
        podct->size     = size;

        return podct;
}

void podcasttrack_free(PodcastTrack* podct)
{
        if (podct) {
                g_free(podct->filename);
                g_free(podct->url);
                g_free(podct);
        }
}

const gchar* podcasttrack_filename(PodcastTrack* podct)
{
        return podct->filename;
}

size_t podcasttrack_size(PodcastTrack* podct)
{
        return podct->size;
}

int podcasttrack_read(PodcastTrack* podct, char* buf, size_t size, size_t offset)
{
        GFile* track_url_file = NULL;
        GFileInputStream* fis = NULL;
        GInputStream* is = NULL;
        GSeekable* seekable = NULL;
        int bytesread = -1;

        debuglog("Will read %d bytes at offset %d", size, offset);

        track_url_file = g_file_new_for_uri("http://www.sr.se/Podradio/xml/Ekots_lordagsintervju.xml"); //podct->url);
        if (NULL == track_url_file ) {
                debuglog("get_file_from_uri failed");
                goto cleanup;
        }

        fis = g_file_read(track_url_file, NULL, NULL);
        if (NULL == fis) {
                debuglog("g_file_read failed");
                goto cleanup;
        }
        is = G_INPUT_STREAM(fis);
        seekable = G_SEEKABLE(fis);

        if (FALSE == g_seekable_can_seek(seekable)) {
                debuglog("g_seekable_can_seek failed");
                goto cleanup;
        }

        g_seekable_seek(seekable, offset, G_SEEK_SET, NULL, NULL);
        bytesread = g_input_stream_read(is, buf, size, NULL, NULL);

        debuglog("Read %d bytes", bytesread);
cleanup:
        if (is) {
                g_input_stream_close(is, NULL, NULL);
                g_object_unref(is);
        }
        if (track_url_file) {
                g_object_unref(track_url_file);
        }

        return bytesread;
}
