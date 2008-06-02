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

#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

struct MemoryStruct {
        char *memory;
        size_t size;
};

static void *myrealloc(void *ptr, size_t size)
{
        /* There might be a realloc() out there that doesn't like reallocing
           NULL pointers, so we take care of it here */
        if(ptr)
                return realloc(ptr, size);
        else
                return malloc(size);
}

static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
        size_t realsize = size * nmemb;
        struct MemoryStruct *mem = (struct MemoryStruct *)data;

        mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
        if (mem->memory) {
                memcpy(&(mem->memory[mem->size]), ptr, realsize);
                mem->size += realsize;
                mem->memory[mem->size] = 0;
        }
        return realsize;
}

int main(int argc, char **argv)
{
        CURL *curl_handle;

        struct MemoryStruct chunk;

        chunk.memory=NULL; /* we expect realloc(NULL, size) to work */
        chunk.size = 0;    /* no data at this point */

        curl_global_init(CURL_GLOBAL_ALL);

        /* init the curl session */
        curl_handle = curl_easy_init();

        /* specify URL to get */
        curl_easy_setopt(curl_handle, CURLOPT_URL, "http://www.sr.se/Podradio/xml/Ekots_lordagsintervju.xml");

        curl_easy_setopt(curl_handle, CURLOPT_RANGE, "3-20");

        /* send all data to this function  */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

        /* some servers don't like requests that are made without a user-agent
           field, so we provide one */
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* get it! */
        curl_easy_perform(curl_handle);

        /* cleanup curl stuff */
        curl_easy_cleanup(curl_handle);

        /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         *
         * Do something nice with it!
         *
         * You should be aware of the fact that at this point we might have an
         * allocated data block, and nothing has yet deallocated that data. So when
         * you're done with it, you should free() it as a nice application.
         */

        puts(chunk.memory);

        if(chunk.memory)
                free(chunk.memory);

        /* we're done with libcurl, so clean it up */
        curl_global_cleanup();

        return 0;
}
