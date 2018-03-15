#ifndef __CAMERA_H
#define __CAMERA_H

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "opencv2/imgproc/imgproc.hpp" 
#include<iostream>
#include<stdio.h>  
#include<stdlib.h>
#include<netinet/in.h>  
#include<sys/socket.h>  
#include<sys/types.h>
#include<arpa/inet.h>  
#include<string.h>  
#include<pthread.h> 
#include<unistd.h>
#include<errno.h>

#include<strings.h>
#include<time.h>

void camera_init(void);
int camera_getAngle(void);

#endif
