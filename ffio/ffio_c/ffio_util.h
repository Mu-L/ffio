#pragma once

// this param is set to return a timeout error when waiting for more than 5 seconds.
#define WAIT_FOR_STREAM_TIMEOUT 5

// write log to file in every PRINT_FRAME_GAP frames to make sure correctness.
#define PRINT_FRAME_GAP 200

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdbool.h>

#include "libavutil/opt.h"
#include "libavutil/bprint.h"
#include "libavutil/imgutils.h"
#include "libavutil/time.h"
#include "libavutil/timestamp.h"

#include "libavutil/hwcontext.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersrc.h"
#include "libavfilter/buffersink.h"
#include "libavutil/mathematics.h"
#include "libswscale/swscale.h"
#include <libavutil/pixdesc.h>

#define LOG_INFO(fmt, ...)       av_log(NULL, AV_LOG_INFO,    "[ffio_c][info ]"     fmt "\n",                 ##__VA_ARGS__)
#define LOG_INFO_T(fmt, ...)     av_log(NULL, AV_LOG_INFO,    "[ffio_c][info ][%s]" fmt "\n", get_str_time(), ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...)    av_log(NULL, AV_LOG_WARNING, "[ffio_c][warn ]"     fmt "\n",                 ##__VA_ARGS__)
#define LOG_WARNING_T(fmt, ...)  av_log(NULL, AV_LOG_WARNING, "[ffio_c][warn ][%s]" fmt "\n", get_str_time(), ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)      av_log(NULL, AV_LOG_ERROR,   "[ffio_c][error]"     fmt "\n",                 ##__VA_ARGS__)
#define LOG_ERROR_T(fmt, ...)    av_log(NULL, AV_LOG_ERROR,   "[ffio_c][error][%s]" fmt "\n", get_str_time(), ##__VA_ARGS__)

typedef struct Clicker
{
    time_t lasttime;
} Clicker;

int interrupt_callback(void *p);

void av_log_ffio_callback(void *avClass, int level, const char *fmt, va_list vl);

void print_avcodec_supported_pix_fmt(AVCodec *codec);
enum AVPixelFormat find_avcodec_1st_sw_pix_fmt(AVCodec *codec);
enum AVPixelFormat find_avcodec_1st_hw_pix_fmt(AVCodec *codec);
