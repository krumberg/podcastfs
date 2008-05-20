#define FUSE_USE_VERSION 26

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <glib.h>


#include "fuse.h"

int main(int argc, char *argv[])
{
        return podcastfs_init(argc, argv);
}
