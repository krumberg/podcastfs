#ifndef __PODCASTLIST_H
#define __PODCASTLIST_H

typedef struct PodcastList PodcastList;

PodcastList* list = podcastlist_get_instance();

int podcastlist_is_podcast_folder(PodcastList* list, const char* name);
int podcastlist_is_podcast_item(PodcastList* list, const char* folder_and_item);

void (pcl_foreachname_callback*)(const char* name);
void podcastlist_foreach_itemname_in_folder(PodcastList* list, const char* name, pcl_foreachname_callback fill_item);
void podcastlist_foreach_foldername(PodcastList* list, pcl_foreachname_callback fill_item);

size_t podcastlist_get_item_size(PodcastList* list, const char* folder_and_item);
int podcastlist_read_item(PodcastList* list, const char* folder_and_item, char* buf, size_t size, off_t offset);

#endif /* __PODCASTLIST.H */
