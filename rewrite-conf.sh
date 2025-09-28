#!/bin/sh

set -e

cp /usr/share/daqmw/conf/reader-logger.xml .

sed \
-e 's/SampleReader/LatencySender/g' \
-e 's/samplereader/latencysender/g' \
-e 's/SampleLogger/LatencyLogger/g' \
-e 's/latencylogger/latencylogger/g' \
-e 's/MyDaq/Latency/g' \
reader-logger.xml > latency-sender-logger.xml

rm reader-logger.xml

