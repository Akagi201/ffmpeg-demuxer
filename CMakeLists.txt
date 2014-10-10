#
# @file CMakeLists.txt
# @author Akagi201
# @date 2014/10/10
#

cmake_minimum_required(VERSION 2.8.4)
project(ffmpeg_demuxer)

set(SOURCE_FILES1 demuxer1.c)

set(SOURCE_FILES2 demuxer2.c)

include_directories("/usr/local/include")

link_directories("/usr/local/lib")

add_executable(demuxer1 ${SOURCE_FILES1})

add_executable(demuxer2 ${SOURCE_FILES2})

target_link_libraries(demuxer1 "libavformat.a")

target_link_libraries(demuxer1 "libavcodec.a")

target_link_libraries(demuxer1 "libavutil.a")

target_link_libraries(demuxer2 "libavformat.a")

target_link_libraries(demuxer2 "libavcodec.a")

target_link_libraries(demuxer2 "libavutil.a")