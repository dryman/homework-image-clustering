#include <stdio.h>
#include <highgui.h>
#include <cv.h>

int main(int argc, char** argv){
  IplImage* img = cvLoadImage(argv[1],CV_LOAD_IMAGE_COLOR);
  printf("chanel of jpg is %d\n",img->nChannels);
  return 0;
}
