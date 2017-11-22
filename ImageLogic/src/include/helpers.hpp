#ifndef _HELPERS_HPP_
#define _HELPERS_HPP_
#define CPU_ONLY 1
#include <opencv2/opencv.hpp>

inline cv::Mat cropImage(cv::Mat img, cv::Rect r) {
	//davi
		   if(r.x <= 0){
			   r.x = 0;
			   r.width += std::abs(r.x);
		   }
	//davi
	           if(r.y <= 0){
                           r.y = 0;
                           r.height += std::abs(r.y);
                   }

	//cv::Mat m = cv::Mat::zeros(r.height, r.width, img.type());
	int dx = std::abs(std::min(0, r.x));
	if (dx > 0) { r.x = 0; }
	r.width -= dx;
	int dy = std::abs(std::min(0, r.y));
	if (dy > 0) { r.y = 0; }	
	r.height -= dy;
	int dw = std::abs(std::min(0, img.cols - 1 - (r.x + r.width)));
	r.width -= dw;
	int dh = std::abs(std::min(0, img.rows - 1 - (r.y + r.height)));
	r.height -= dh;

	//david
	if(r.width <= 0 )
		r.width = 5;
	if(r.height <= 0 )
		r.height = 5;
	cv::Mat m = cv::Mat::zeros(r.height, r.width, img.type());
//	std::cout<<"img clos:"<<img.cols<<"img rows:"<<img.rows<<std::endl;
//	std::cout<<"rec clos:"<<r.width<<"img rows:"<<r.height<<std::endl;
//	std::cout<<" dy is:"<<dy<<"dx is :"<<dx<<std::endl;

	if(r.x >= img.cols){
		r.x = img.cols - r.width;}
	if(r.y >= img.rows){
		r.y = img.rows - r.height;}

	//std::cout<<"david222"<<r.width<<std::endl;
	if(r.x + r.width - img.cols > 0){
		if(r.x > (r.x + r.width - img.cols ))
			{r.x -= (r.x + r.width - img.cols );}else{r.x = 0;
			}
		}
	if(r.y + r.height - img.rows > 0 ){
		//std::cout<<"david222"<<std::endl;
		if(r.y > (r.y + r.height - img.rows ))
			{r.y -= (r.y + r.height - img.rows );}else{r.y = 0;
			}
		}
//david
//	std::cout<<r.x<<","<<r.y<<std::endl;
//	std::cout<<"##"<<dx<<","<<dx+r.width<<std::endl;
//	std::cout<<"###"<<dy<<","<<dy+r.height<<std::endl;
	img(r).copyTo(m(cv::Range(dy, dy + r.height), cv::Range(dx, dx + r.width)));
	return m;
}

inline void drawAndShowFace(cv::Mat img, cv::Rect r, const std::vector<cv::Point2f>& pts) {
	cv::Mat outImg;
	img.convertTo(outImg, CV_8UC3);
	cv::rectangle(outImg, r, cv::Scalar(0, 0, 255));
	for (size_t i = 0; i < pts.size(); ++i) {
		cv::circle(outImg, pts[i], 3, cv::Scalar(0, 0, 255));
	}
	cv::circle(outImg, pts[3], 3, cv::Scalar(0, 0, 255));
	cv::imshow("test", outImg);
	cv::waitKey(0);
}

#endif //_HELPERS_HPP_
