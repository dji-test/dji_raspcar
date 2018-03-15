// ConsoleApplication6.cpp : 定义控制台应用程序的入口点。
//

#include "camera.h"

#define VIDEO_BUFFER_LEN 1024*4096
#define DATA_BUFFER_LEN 1024
#define POST_BUFFER_LEN 1024
#define UPDATE_BUFFEr_LEN 200

#define MYPORT 1234    // the port users will be connecting to
#define BACKLOG 5     // how many pending connections queue will hold
#define BUF_SIZE DATA_BUFFER_LEN

using namespace std;
using namespace cv;


enum{
    DATA,
    VIDEO,
    POST,
    UPDATE
};
typedef struct arg_struct
{
    int client_port;
    int server_port;
    int type;
    char ip[20];

    /* data */
}arg_struct;


FILE *fp;
int recv_length=0;
/*
static void *server_function(void *arg);
int curl_post(char *send_data,int length,char *recv_buffer,int *num);

static void* client_fun(void *arg);
*/

//char data_msg[DATA_BUFFER_LEN];  
char video_msg[VIDEO_BUFFER_LEN];
char post_msg[POST_BUFFER_LEN];  
char update_msg[UPDATE_BUFFEr_LEN];
int recv_analyse(char data[],int lenth);


	Mat img;
	Mat result_img;
	unsigned int count = 0;

	string data;

	int iLowH = 35;
	int iHighH = 77;

	int iLowS = 33;
	int iHighS = 255;

	int iLowV = 46;
	int iHighV = 255;
//	waitKey(50);
	std::vector<std::vector<cv::Point> > contours;
	static cv::VideoCapture frame(0);

void log(char *str,FILE *fp){
	time_t t;
	time(&t);
	if(fp==NULL)
	{
	    printf("fp is NULL %s\r\n",ctime(&t));
	    return;
	}
	if(fputs(str,fp)==NULL){
	    printf("log error %s\r\n",ctime(&t));
	    return;
	}
	else{
	    fputs(ctime(&t),fp);
	}
	fflush(fp);
}

void ToHSV(cv::Mat& image, cv::Mat& result)  //产出是一个mask
{
	cv::Mat hsv_image;        //转HSV
	hsv_image.create(image.size(), image.type());
	cv::cvtColor(image, hsv_image, CV_BGR2HSV);

	vector<cv::Mat> channels;
	cv::split(hsv_image, channels);


	int num_row = image.rows;
	int num_col = image.cols;

	for (int r = 0; r < num_row; r++)
	{
		const cv::Vec3b* curr_r_image = image.ptr<const cv::Vec3b>(r);
		const uchar* curr_r_hue = channels[0].ptr<const uchar>(r);
		const uchar* curr_r_satur = channels[1].ptr<const uchar>(r);
		const uchar* curr_r_value = channels[2].ptr<const uchar>(r);
		uchar* curr_r_result = result.ptr<uchar>(r);
		for (int c = 0; c < num_col; c++)
		{
			if (((curr_r_hue[c] <= 2 && curr_r_hue[c] >= 0) || (curr_r_hue[c] <= 180 && curr_r_hue[c] >= 150)) && curr_r_value[c]>130 && curr_r_satur[c]>35 && curr_r_satur[c]<150) //找颜色
			{
				curr_r_result[c] = 255;
			}
			else
			{
				curr_r_result[c] = 0;
			}

		}
	}
}


void camera_init()
{

	if((fp=fopen("transport.log","at"))== NULL){
		printf("cann't open log file\r\n");
		exit(1);
	}
	log("main",fp);
	printf("main\n");
	int res;
/*	
	pthread_t post;

	arg_struct payload;

	memset(&payload,0x00,sizeof(payload));

	payload.server_port=1234;
	payload.client_port=0000;
	payload.type=POST;
	//memcpy(payload.ip,argv[1],strlen(argv[1]));

	res = pthread_create(&post,NULL,server_function,(void*)&payload);
	if(res!=0){
		log("post_thread create failure\r\n",fp);
		printf("video_thread create failure\n");
        return 1;
	}
	log("post_thread create success\r\n",fp);
	printf("post_thread create success\n");
*/	
}

int camera_getAngle()
{
	
	frame >> img;
	if (img.empty())
	{
		return 0;
	}
	result_img = img.clone();
	cv::cvtColor(img, result_img, CV_BGR2HSV);
	Mat imgThresholded;
	inRange(result_img, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
	int g_nStructElementSize = 8; //结构元素(内核矩阵)的尺寸  
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * g_nStructElementSize + 1, 2 * g_nStructElementSize + 1),
		Point(g_nStructElementSize, g_nStructElementSize));
	erode(imgThresholded, imgThresholded, element);
	g_nStructElementSize = 3;
	element = getStructuringElement(MORPH_RECT,
		Size(2 * g_nStructElementSize + 1, 2 * g_nStructElementSize + 1),
		Point(g_nStructElementSize, g_nStructElementSize));
	dilate(imgThresholded, imgThresholded, element);
	
	//imshow("1", img);
	//imshow("2", result_img);
	//imshow("3", imgThresholded);
	//GaussianBlur(imgThresholded, imgThresholded, Size(10, 10), 0, 0, BORDER_DEFAULT);
	Mat dst;
	Laplacian(imgThresholded, dst, CV_8U, 3, 1, 0, BORDER_DEFAULT);
	//imshow("Lap", dst);

	
	findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat result(dst.size(), CV_8U, Scalar(0));
	
//	drawContours(result, contours, -1, Scalar(255), 2);
	int max_area = 0;
	int max_index = 0;
	Point2f mc;
	int delt;
	if ((int)contours.size()>0)
	{
		for (int i = 0; i < (int)contours.size(); i++)
		{
			double g_dConArea = contourArea(contours[i], false);
			if (g_dConArea>max_area)
			{
				max_area = g_dConArea;
				max_index = i;
			}
		//	cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积为：】" << g_dConArea << endl;
			Rect r = boundingRect(Mat(contours[i]));

		//	rectangle(result,r, Scalar(255), 2);

		}

		Moments mu;
		mu = moments(contours[max_index], false);

		
		mc = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);

//		cout << mc.x << " , " << mc.y << endl;
		circle(result, mc, 4, Scalar(255), -1, 8, 0);
/*		
		if (mc.x < (img.cols/2-150) )
		{
			cout << "detection on the left" << endl;
		}
		else if (mc.x > (img.cols / 2 + 150))
		{
			cout << "detection on the right" << endl;
		}
		else
		{
			cout << "detection on the center" << endl;
		}
*/

		delt = mc.x - img.cols/2;
		if (delt > -150 && delt < 150)
			return 0;
		else if (delt < -150)
			return (delt + 150)/10;
		else
			return (delt -150)/10;
	}
	else{
//		cout<<"no detection"<<endl;
		return -1;
	}
	
	
	//imshow("contours", result);
//		cv::waitKey(40);

}
