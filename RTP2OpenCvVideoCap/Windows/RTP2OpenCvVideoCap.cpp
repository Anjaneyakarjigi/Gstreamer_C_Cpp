#include <stdio.h>
#include <stdlib.h>
#include <opencv2\opencv.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
	cv::VideoCapture cap;
	if(argc > 3 || argc < 3)
	{
		printf("Error Arguments, please input all arguments");
		return 1;
	}
	
	char *IpAddress = argv[1];
	int port = 0;
	if (argv[2] > 0)
	port = atoi(argv[2]);
	
	char pipelineToOpen[30] = {0};
	sprintf(pipelineToOpen,"rtp://@%s:%d",IpAddress,port);
	
	cap.open(pipelineToOpen);
	if(!cap.isOpened())
	{
		printf("Unable to open videoCap\n");
	}
	
	cv::Mat image; 
	while(1)
	{
		cap.read(image);
		cv::imshow("received Image",image);
		waitKey(5);
	}
	getchar();
	return 0;
}
