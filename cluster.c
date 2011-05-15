#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <highgui.h>
#include <memory.h>
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
    int sum = INT32_MAX;
    Cluster* cl = NULL;
    for (Cluster* c = clHead; c!=clHead+cl_num; c++){
      int _sum = abs(c->r-r) + abs(c->g-g) + abs(c->b-b);
      if (_sum < sum) {
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

void kmeanApply(unsigned char* dst, const unsigned char* src, int steps, Cluster* clHead, int cl_num){
  for(int i=0;i<steps*3;i++){
    unsigned char* img = (unsigned char*)(src+i);
    unsigned char* img_apply = (unsigned char*)(dst+i);
    int r = (int) img[0];
    int g = (int) img[1];
    int b = (int) img[2];
    int sum = INT32_MAX;
    Cluster* cl = NULL;
    for (Cluster* c = clHead; c!=clHead+cl_num; c++){
      int _sum = abs(c->r-r) + abs(c->g-g) + abs(c->b-b);
      if (_sum < sum) {
        sum = _sum;
        cl = c;
      }
    }
    img_apply[0]=(unsigned char)cl->r;
    img_apply[1]=(unsigned char)cl->g;
    img_apply[2]=(unsigned char)cl->b;
  }
}

void dumpCluster(Cluster* clHead, int cl_num){
  for (Cluster* c = clHead; c!=clHead+cl_num; c++){
    printf("Cluster [%d]:\n",(int)(c-clHead));
    printf("r = %d\ng = %d\nb = %d\n",c->r,c->g,c->b);
  }
  printf("\n");
}

int initCluster(Cluster** cluster, int cl_num){
  if (cl_num == 0){
    cl_num = 5;
    *cluster = (Cluster*)malloc(sizeof(Cluster)*cl_num);
    for (Cluster* c = *cluster; c!=*cluster+cl_num; c++){
      c->r=0;
      c->g=0;
      c->b=0;
      c->r_sum=0;
      c->g_sum=0;
      c->b_sum=0;
      c->count=0;
    }
    (*cluster)[0].r=255;
    (*cluster)[1].r=255;
    (*cluster)[1].g=255;
    (*cluster)[2].r=100;
    (*cluster)[2].g=100;
    (*cluster)[2].b=100;
    (*cluster)[3].r=255;
    (*cluster)[3].g=255;
    (*cluster)[3].b=255;
    return cl_num;
  }
  else {
    *cluster = (Cluster*)malloc(sizeof(Cluster)*cl_num);
    for (Cluster* c = *cluster; c!=*cluster+cl_num; c++){
      c->r=rand() % 256;
      c->g=rand() % 256;
      c->b=rand() % 256;
      c->r_sum=0;
      c->g_sum=0;
      c->b_sum=0;
      c->count=0;
    }
    return cl_num;
  }
}


    

int main(int argc, char** argv){
  IplImage* img = cvLoadImage(argv[1],CV_LOAD_IMAGE_COLOR);
  IplImage* img_apply = cvCreateImage(cvSize(img->width,img->height),8,3);
  cvNamedWindow("cluster demo",1);
  unsigned char* imgData = (unsigned char*)img->imageData;
  unsigned char* img_apply_data = (unsigned char*)img_apply->imageData;

  Cluster* cluster=NULL;
  int cl_num = initCluster(&cluster,atoi(argv[2]));
  for (int i=0;i<30;i++){
    kmean(imgData,img->width*img->height,cluster, cl_num);
    dumpCluster(cluster,5);
    kmeanApply(img_apply_data,imgData,img->width*img->height,cluster,cl_num);
    cvShowImage("cluster demo",img_apply);
    cvWaitKey(100);
  }
  cvWaitKey(0);
  
  return 0;
}
