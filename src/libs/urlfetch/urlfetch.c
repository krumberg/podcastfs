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
#include <unistd.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <urlfetch/urlfetch.h>

typedef struct {
        int size;
        char* buf;
} DataBuffer;

static size_t write_buffer_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
        size_t realsize = size * nmemb;
        DataBuffer *dbuf = (DataBuffer*) data;

        memcpy(&(dbuf->buf[dbuf->size]), ptr, realsize);
        dbuf->size+= realsize;

        return realsize;
}

int urlfetch_data_in_range(const char* url, char* buf, size_t size, size_t offset)
{
        CURL *curl_handle;

        DataBuffer dbuf;
        dbuf.size = 0;
        dbuf.buf = buf;

        curl_handle = curl_easy_init();

        curl_easy_setopt(curl_handle, CURLOPT_URL, url);

        char srange[32];
        sprintf(srange, "%d-%d", 3 + offset, 3 + offset + size);
        curl_easy_setopt(curl_handle, CURLOPT_RANGE, srange);

        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_buffer_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &dbuf);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        curl_easy_perform(curl_handle);

        curl_easy_cleanup(curl_handle);

        return 0;
}


static size_t write_file_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
        FILE* file = (FILE*) data;
        fwrite(ptr, size, nmemb, file);
        return (nmemb * size);
}

int urlfetch_download_tmpfile(const char* url, char* tmppath)
{
        CURL *curl_handle;

        strcpy(tmppath, "/tmp/rssfileXXXXXX");
        close(mkstemp(tmppath));
        FILE* file = fopen(tmppath, "wb");
        if (NULL == file) {
                return -1;
        }

        curl_handle = curl_easy_init();

        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_file_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, file);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        curl_easy_perform(curl_handle);

        curl_easy_cleanup(curl_handle);
        fclose(file);

        return 0;
}
