ffmpeg-demuxer
==============

Split Audio and Video bitstreams based on FFmpeg

## Features
* Demux a FLV file to H.264 bitstream and mp3 bitstream.
* Use cmake for cross-platform build.

## Notes
* It doesn't init Output Video/Audio stream's AVFormatContext. It just writes AVPacket's data to files directly.
* Tt's not suitable for some kind of bitstreams. For example, AAC bitstream in FLV/MP4/MKV Container Format(data in AVPacket lack of 7 bytes of ADTS header).

## Dependencies
* libavformat
* libavcodec

## Build
* `./start_build.sh`
* build target is located in build/ffmpeg_demuxer.

## Usage
* `ffmpeg_demuxer`