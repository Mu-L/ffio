# -*- coding=utf-8 -*-
# Library: ffio
# Author: dongrixinyu, koisi
# License: MIT
# Email: dongrixinyu.66@gmail.com
# Github: https://github.com/dongrixinyu/ffio
# Description: An easy-to-use Python wrapper for FFmpeg-C-API.
# Website: http://www.jionlp.com

#  Usage:
#     python3 decode_frames.py test.flv

# Strongly recommend to use this piece of code in a sub-process.
# Cause decoding an online video stream consumes about 100M memory and 40% CPU.


# Note:
#   - C log file is written in ~/.cache/ffio/clog-%d.txt.%s

import pdb
import sys
import ffio


if len(sys.argv) < 2:
  print("Please provide a target to read.")
  sys.exit(1)

video_path = sys.argv[1]
read_num   = 1000
read_idx   = 0
while read_idx < read_num:

  """
    Infinite trying to open ffio, until ffio_state returns success.
  """
  while True:
    params = ffio.CCodecParams()
    # params.flags2 = b"showall"
    decoder = ffio.FFIO(
      video_path, mode=ffio.FFIOMode.DECODE, hw_enabled=False,
      pix_fmt_hw_enabled=True, codec_params=params)

    if decoder:
      print('stream width: ', decoder.width)
      print('stream height: ', decoder.height)
      break

  """
    Loop to decode frames from video.
  """
  while read_idx < read_num:
    frame = decoder.decode_one_frame()
    if not frame:
      """
        When `decode_one_frame()` failed to get frame,
        you should release current FFIO instance by calling `release_memory()`.
        Than retry to create new instance as you need.
      """
      # break
      print("error.")
      continue

    else:
      read_idx += 1
      print(f"success get frame: [{read_idx}].")
      image = frame.as_image()
      # pdb.set_trace()
      # print("image shape: ", image.shape)
      image.save(f"output-{read_idx}.jpeg", 'JPEG')
      pdb.set_trace()

  decoder.release_memory()
