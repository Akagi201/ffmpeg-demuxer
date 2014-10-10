ffmpeg-demuxer
==============

Split Audio and Video bitstreams based on FFmpeg

## Features
* demux1: Demux a FLV file to H.264 bitstream and mp3 bitstream.
* demux2: Demux a MPEG2TS file to H.264 bitstream and AAC bitstream.
* Use cmake for cross-platform build.

## Notes
### demux1
* It doesn't init Output Video/Audio stream's AVFormatContext. It just writes AVPacket's data to files directly.
* Tt's not suitable for some kind of bitstreams. For example, AAC bitstream in FLV/MP4/MKV Container Format(data in AVPacket lack of 7 bytes of ADTS header).

## Dependencies

### demux1
* libavformat
* libavcodec

### demux2
* libavformat
* libavcodec
* libavutil

## Build
* `./start_build.sh`
* build target is located in build/.

## Usage
* `demux1`
* `demux2`