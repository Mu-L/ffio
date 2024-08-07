/**
 * Library : ffio
 * Author : koisi, dongrixinyu
 * License : MIT
 * Email : dongrixinyu.66@gmail.com
 * Github : https://github.com/dongrixinyu/ffio
 * Description : An easy-to-use Python wrapper for FFmpeg-C-API.
 * Website : http://www.jionlp.com
 */

#include <stdlib.h>
#include <string.h>

#include "../ffio.h"

static void saveRGB2File(const FFIO* ffio){
  FILE *pFile;
  char filename[32];

  snprintf(filename, sizeof(filename), "frame%d.ppm", ffio->frameSeq);
  pFile = fopen(filename, "wb");
  if (pFile == NULL) { return; }
  fprintf(pFile, "P6\n%d %d\n255\n", ffio->imageWidth, ffio->imageHeight);

  fwrite(ffio->rawFrame, 1, ffio->imageByteSize, pFile);
  fclose(pFile);
}

int main(int argc, char *argv[]){
#ifdef DEBUG
  av_log_set_level(AV_LOG_DEBUG);
#else
  av_log_set_level(AV_LOG_INFO);
#endif

  if( argc == 1 ){ printf("Please run with args: i_url [o_url] [gpu|cpu].\n"); exit(1); }
  printf("Running with: %s %s %s.\n", argv[0], argv[1], argc == 3 ? argv[2] : "" );

  char *i_url = argv[1];
  char *o_url = NULL;
  bool hw_enabled = false;
  bool pix_fmt_hw_enabled = false;
  if (argc == 3)
  {
    if( strncmp(argv[2], "gpu", 3) == 0 ){
      hw_enabled = true;
      pix_fmt_hw_enabled = true;
    }
    else{ o_url = argv[2]; }
  }
  else if (argc > 3)
  {
    o_url = argv[2];
    if( strncmp(argv[3], "gpu", 3) == 0 ){ hw_enabled = true; }
  }

  FFIO* i_ffio = newFFIO();
  FFIO* o_ffio = newFFIO();
  // ffmpeg -flags2 showall -f h264 -i raw-1677141437.h264
  //     -c:v h264 -bf 0 -g 90 -profile:v high -b:v 2000k
  //     -maxrate 2000k -an -movflags +faststart  gdr_video2.mp4
  CodecParams i_params = {
      0, 0, 0, 0, 30, 90, 0, FFIO_PTS_TRICK_INCREASE, "", "", "", "", "", "", "", "", {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}, true};
  CodecParams o_params = i_params;
  hw_enabled = true;
  pix_fmt_hw_enabled = true;
  initFFIO(i_ffio, FFIO_MODE_DECODE, i_url,
           hw_enabled, pix_fmt_hw_enabled, "cuda",
           false, NULL, 0, 0, &i_params);
  if(o_url != NULL){
    o_params.width = 1280;
    o_params.height = 720;
    // bool hw_enabled = false;
    initFFIO(o_ffio, FFIO_MODE_ENCODE, o_url,
             hw_enabled, pix_fmt_hw_enabled, "cuda",
             false, NULL, 0, 0, &o_params);
  }

  if( i_ffio->ffioState != FFIO_STATE_READY || o_ffio->ffioState != FFIO_STATE_READY){
    LOG_ERROR(" failed to init ffio.");   exit(1);
  }

  print_avcodec_supported_pix_fmt(o_ffio->avCodec);

  FFIOFrame* frame;
  int ret;
  for(int i=0; i<9000; ++i){
    frame = decodeOneFrame(i_ffio, NULL);
    LOG_INFO("[%d] decodeOneFrame returned %d.", i_ffio->frameSeq, ret);
    if (frame->err!=0){
      continue;
    }
    if( frame->err==0 && i<300){
      // saveRGB2File(i_ffio);
    }
    if( frame->err==0 && o_url!=NULL){
      ret = encodeOneFrame(o_ffio, i_ffio->rawFrame,
                           "\"hello.\"", sizeof("\"hello.\""));
      LOG_INFO("[%d] encodeOneFrame returned %d.", o_ffio->frameSeq, ret);
    }
  }

  finalizeFFIO(i_ffio);
  finalizeFFIO(o_ffio);


  return 0;
}
