# -*- coding=utf-8 -*-
# Library: ffio
# Author: dongrixinyu
# License: MIT
# Email: dongrixinyu.66@gmail.com
# Github: https://github.com/dongrixinyu/ffio
# Description: An easy-to-use Python wrapper for FFmpeg-C-API.
# Website: http://www.jionlp.com

# Strongly recommend to use this piece of code in a sub-process.
# Cause decoding an online video stream consumes about 100M memory and 40% CPU.

import os
import pdb

import ffio


# replace this rtmp stream path with your custom one.
input_stream_path = "rtmp://ip:port/path/to/your/input/stream"

input_stream_state = False  # to control whether to restart the stream context
while True:

    # initialize the stream context
    while True:
        print('init ... ')
        input_stream_obj = ffio.InputStreamParser(input_stream_path)
        if input_stream_obj.stream_state is True:
            # it means that the stream context has been opened successfully.
            # otherwise, the stream can not be reached,
            # probably the path is wrong or stream is empty
            input_stream_state = input_stream_obj.stream_state
            break

    # to get frames in a loop until encountering an error
    while True:
        frame = input_stream_obj.decode_one_frame(image_format='numpy')
        if type(frame) is int:
            if frame == -5:
                input_stream_state = input_stream_obj.stream_state
                break
            elif frame == -4:
                input_stream_state = input_stream_obj.stream_state
                break
            else:
                input_stream_state = input_stream_obj.stream_state
                break

        else:
            pass
            # process this frame according to your needs.
            # rgb_image.save(os.path.join(dir_path, 'rgb_file_{}.jpg'.format(i)))

    # then reconnect to the input stream, rebuild an input stream context in the next loop.
    # you have to release the memory containing input stream context manually.
    input_stream_obj.release_memory()
    print('successfully release memory of input stream context.')
