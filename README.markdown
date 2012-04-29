Podcastfs
====================

A FUSE filesystem that allows a user to mount a podcast rss and view it as a directory of mp3 files

Build project
-------------

<pre><code>
mkdir bin
cmake ../src

make -j3
</code></pre>

Create configuration
--------------------

Create a file ~/.podcastfslist and insert one rss-url per line.

Run pcmount
-----------

<pre><code>
mkdir p

[path to bin directory]/apps/pcmount/pcmount p
</code></pre>

The directory p should now contain your mounted files.

Run tests
---------

Ideally all non-trival code in podcastfs should be covered by automated testcases.

To run the integration tests, move to the root folder and execute (assumes that "bin" is your build folder)

<pre><code>
./bin/test/integration-tests/integration-tests
</code></pre>

To run the unit test, move to the root folder and execute

<pre><code>
./bin/test/unit-tests/unit-tests
</code></pre>
