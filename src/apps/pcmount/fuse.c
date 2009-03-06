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

#define FUSE_USE_VERSION 26
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <debuglog/debuglog.h>
#include <podcast/podcastlist.h>

static int podcastfs_getattr(const char* path, struct stat *stbuf)
{
	debuglog("Inside podcastfs_getattr");
	debuglog(path);

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		return 0;
	}

	PodcastList* list = podcastlist_get_instance();
	if (podcastlist_is_podcast_folder(list, (path + 1))) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (podcastlist_is_podcast_track(list, path)) {
		stbuf->st_mode = S_IFREG | 0644;
		stbuf->st_nlink = 1;
		stbuf->st_size = podcastlist_get_track_size(list, path);
	} else {
		debuglog("podcastfs_getattr failed on path");
		debuglog(path);
		return -ENOENT;
	}

	return 0;
}

static int podcastfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info* fi)
{
	debuglog("Inside podcastfs_readdir");
	debuglog(path);

	void callback(const char* item)
	{
		debuglog(item);
		filler(buf, item, NULL, 0);
	}

	PodcastList* list = podcastlist_get_instance();

	if (strcmp(path, "/") == 0) {
		callback(".");
		callback("..");
		podcastlist_foreach_foldername(list, callback);
	} else if (podcastlist_is_podcast_folder(list, (path + 1))) {
		podcastlist_foreach_trackname_in_folder(list, path, callback);
	} else {
		debuglog("podcastfs_readdir failed on path");
		debuglog(path);
		return -ENOENT;
	}
	return 0;
}

static int podcastfs_open(const char* path, struct fuse_file_info* fi)
{
	debuglog("Inside podcastfs_open");
	debuglog(path);

	PodcastList* list = podcastlist_get_instance();

	if (podcastlist_is_podcast_track(list, path) && ((fi->flags & 3) == O_RDONLY) ) {
      		debuglog("File found");
      		return 0;
	}
	return -EACCES;
}

static int podcastfs_read(const char* path, char* buf, size_t size, off_t offset,
		      struct fuse_file_info* fi)
{
	debuglog("Inside podcastfs_read");
	debuglog(path);

	PodcastList* list = podcastlist_get_instance();

	if (podcastlist_is_podcast_track(list, path)) {
		size_t n = podcastlist_read_track(list, path, buf, size, offset);
		debuglog("Read bytes");
       		return n;
	}
	debuglog("Read failed");
	return -ENOENT;
}

/* static int podcastfs_write(const char* path, const char* buf, size_t size, off_t offset,  struct fuse_file_info *) */
/* { */
/*	 return 0; */
/* } */

static struct fuse_operations podcastfs_oper = {
	.getattr	= podcastfs_getattr,
	.readdir	= podcastfs_readdir,
	.open		= podcastfs_open,
	.read		= podcastfs_read,
/*	 .write	  = podcastfs_write */
};

int podcastfs_init(int argc, char* argv[])
{
	debuglog("About to call fuse_main");

	return fuse_main(argc, argv, &podcastfs_oper, NULL);
}

