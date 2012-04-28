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

// Verify that the correct filename is associated with the correct episode
// If "051_Episode 050: Redefining ‘infrequent’ since 2005.mp3" doesn't
// exist titles without associated urls exists. The title "PodcasAlley.com
// voting" has no associated url and should not be listed
TEST(podcast_integration_tests, titels)
{
	const char* TESTRSS_FILE = "src/test/data/c64test.xml";
	Podcast* podcast = podcast_new_from_file(TESTRSS_FILE);
	ASSERT_TRUE(podcast != NULL);

	ASSERT_FALSE(podcast_has_track(podcast, "003_PodcastAlley.com voting.mp3"));
	ASSERT_TRUE(podcast_has_track(podcast, "051_Episode 050: Redefining ‘infrequent’ since 2005.mp3"));
}

