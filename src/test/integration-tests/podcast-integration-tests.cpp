#include <gtest/gtest.h>

#include <stdio.h>

extern "C" {
#include <podcast/podcast.h>
}

TEST(podcast_integration_tests, from_file_read_name)
{
    const char* TESTRSS_FILE = "src/test/data/testrss.xml";

    Podcast* podcast = podcast_new_from_file(TESTRSS_FILE);
    ASSERT_TRUE(podcast != NULL);

    ASSERT_FALSE(
        strcmp(
            "The Linux Action Show! MP3",
            podcast_folder_name(podcast)
        )
    );

    podcast_free(podcast);
}

TEST(podcast_integration_tests, from_file_count)
{
    const char* TESTRSS_FILE = "src/test/data/testrss.xml";

    Podcast* podcast = podcast_new_from_file(TESTRSS_FILE);
    ASSERT_TRUE(podcast != NULL);

    ASSERT_EQ(10, podcast_num_tracks(podcast));

    podcast_free(podcast);
}
