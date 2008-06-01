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

#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
        char buf[30];
        strcpy(buf, "/tmp/EkotXXXXXX");

        close(mkstemp(buf));

        puts(buf);

        g_type_init();

        GFile* xml_url_file = g_file_new_for_uri("http://www.sr.se/Podradio/xml/Ekots_lordagsintervju.xml");

        if (NULL == xml_url_file) {
                fprintf(stderr, "Unable to open file\n");
                exit(-1);
        }
        GFile* outfile = g_file_new_for_path("outfile.txt");

        g_file_copy(xml_url_file, outfile, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL);

/*         g_file_close(xml_url_file); */
/*         g_file_close(outfile); */

        return 0;
}