#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <highgui.h>
#include <cv.h>

typedef struct {
  int r;
  int g;
  int b;
  int r_sum;
  int g_sum;
  int b_sum;
  unsigned int count;
} Cluster;

void kmean (unsigned char* imgHead, int steps, Cluster* clHead, int cl_num){
  for(unsigned char* img = imgHead; img != imgHead+steps*3; img+=3){
    int r = (int) img[0];
    int g = (int) img[1];
    int b = (int) img[2];
    int sum = 0;
    Cluster* cl = NULL;
    for (Cluster* c = clHead; c!=clHead+cl_num; c++){
      int _sum = abs(c->r-r) + abs(c->g-g) + abs(c->b-b);
      if (_sum > sum) {
        sum = _sum;
        cl = c;
      }
    }
    cl->r_sum += r;
    cl->g_sum += g;
    cl->b_sum += b;
    cl->count ++;
  }
  for (Cluster* c = clHead; c != clHead+cl_num; c++){
    if (c->count !=0){
      c->r = c->r_sum/ c->count;
      c->g = c->g_sum/ c->count;
      c->b = c->b_sum/ c->count;
      c->r_sum = 0;
      c->g_sum = 0;
      c->b_sum = 0;
      c->count = 0;
    }
  }
}

void dumpCluster(Cluster* clHead, int cl_num){
  for (Cluster* c = clHead; c!=clHead+cl_num; c++){
    printf("Cluster [%d]:\n",(int)(c-clHead));
    printf("r = %d\ng = %d\nb = %d\n",c->r,c->g,c->b);
  }
  printf("\n");
}

    

int main(int argc, char** argv){
  IplImage* img = cvLoadImage(argv[1],CV_LOAD_IMAGE_COLOR);
  unsigned char* imgData = (unsigned char*)img->imageData;
  Cluster cluster[5];
  for (Cluster* c = cluster; c!=cluster+5; c++){
    c->r=0;
    c->g=0;
    c->b=0;
    c->r_sum=0;
    c->g_sum=0;
    c->b_sum=0;
    c->count=0;
  }
  cluster[0].r=255;
  cluster[1].g=255;
  cluster[2].b=255;
  cluster[3].r=255;
  cluster[3].g=255;
  for (int i=0;i<5;i++){
    kmean(imgData,img->width*img->height,cluster, 5);
    dumpCluster(cluster,5);
  }
  return 0;
}
