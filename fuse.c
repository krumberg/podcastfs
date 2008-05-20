#include "fuse.h"
#include "podcastlist.h"

#define FUSE_USE_VERSION 26
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>

static int podcastfs_getattr(const char* path, struct stat *stbuf)
{
	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
                return 0;
        }

        PodcastList* list = podcastlist_get_instance();
        if (podcastlist_is_podcast_folder(list, path + 1)) {
                stbuf->st_mode = S_IFDIR | 0755;
                stbuf->st_nlink = 2;
        } else if (podcastlist_is_podcast_item(list, path + 1)) {
                stbuf->st_mode = S_IFREG | 0644;
                stbuf->st_nlink = 1;
                stbuf->st_size = podcastlist_get_item_size(list, path + 1);
        } else {
                return -ENOENT;
        }

        return 0;
}

static int podcastfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info* fi)
{
        void fill_item(const char* item)
        {
                filler(buf, item, NULL, 0);
        }

        PodcastList* list = podcastlist_get_instance();

        fill_item(".");
        fill_item("..");

        if (strcmp(path, "/") == 0) {
                podcastlist_foreach_foldername(list, fill_item);
        } else if (podcastlist_is_podcast_folder(list, path + 1)) {
                podcastlist_foreach_itemname_in_folder(list, path, fill_item);
        }

	return -ENOENT;
}

static int podcastfs_open(const char* path, struct fuse_file_info* fi)
{
        PodcastList* list = podcastlist_get_instance();

        if (podcastlist_is_podcast_item(list, path + 1) && ((fi->flags & 3) == O_RDONLY) ) {
                return 0;
        }
        return -EACCES;
}

static int podcastfs_read(const char* path, char* buf, size_t size, off_t offset,
		      struct fuse_file_info* fi)
{
        PodcastList* list = podcastlist_get_instance();

        if (podcastlist_is_podcast_item(list, path + 1)) {
                return podcastlist_read_item(list, path, buf, size, offset);
        }
        return -ENOENT;
}

/* static int podcastfs_write(const char* path, const char* buf, size_t size, off_t offset,  struct fuse_file_info *) */
/* { */
/*         return 0; */
/* } */

static struct operations podcastfs_oper = {
	.getattr	= podcastfs_getattr,
	.readdir	= podcastfs_readdir,
	.open		= podcastfs_open,
	.read		= podcastfs_read,
/*         .write          = podcastfs_write */
};

void podcastfs_init(int argc, char* argv[])
{
	return fuse_main(argc, argv, &podcastfs_fuse_oper, NULL);
}

