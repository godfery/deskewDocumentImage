/*
 *	Author: John Hany
 *	Website: http://johnhany.net
 *	Source code updates: https://github/johnhany/textRotCorrect
 *	If you have any advice, you could contact me at: johnhany@163.com
 *	Need OpenCV environment!
 *
 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>


//#include<io.h>
using namespace cv;
using namespace std;

#define GRAY_THRESH 150
#define HOUGH_VOTE 100

//#define DEGREE 27


char *scat(const char *a,const char *b)     /*返回合并的字符串的地址*/
{
    char *rt=(char *)malloc(sizeof(char)*(strlen(a)+strlen(b)));
    strcpy(rt,a);
    return strcat(rt,b);
}
void deskew( char* filename, double angle)
{
    cv::Mat img = cv::imread(filename, 0);
    
    // cv::bitwise_not(img, img);
    
    std::vector<cv::Point> points;
    cv::Mat_<uchar>::iterator it = img.begin<uchar>();
    cv::Mat_<uchar>::iterator end = img.end<uchar>();
    for (; it != end; ++it)
        if (*it)
            points.push_back(it.pos());
    
    cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
    cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, angle, 1);
    cv::Mat rotated;
    cv::warpAffine(img, rotated, rot_mat, img.size(), cv::INTER_CUBIC);
    
    cv::Size box_size = box.size;
    
    box_size.width = 125;
    box_size.height=20;
    
//    if (box.angle < -45.)
//        std::swap(box_size.width, box_size.height);
    cv::Mat cropped;
    cv::getRectSubPix(rotated, box_size, box.center, cropped);
    
    
    char* save_name, *pos;
    int name_len;
    name_len = strlen(filename);
    pos = filename + name_len;
    while(*pos != '/' && pos != filename)
        pos --;
    if(pos == filename)
    {
        save_name = filename+1;
        //filename return save_name;
    }
    name_len = name_len-(pos-filename);
    save_name = (char*) malloc(name_len+1);
    memcpy(save_name,pos+1,name_len);
    
    
    char *save ,*filesave = "/www/ocr/cap2/r_";
    
    save = scat(filesave, save_name);
    
    
    imwrite(save,cropped);
    
    
    //    cv::imshow("Original", img);
    //    cv::imshow("Rotated", rotated);
    //    cv::imshow("Cropped", cropped);
    //  cv::waitKey(0);
}

void compute_skew( char* filename)
{
    // Load in grayscale.
    cv::Mat src = cv::imread(filename, 0);
    cv::Size size = src.size();
    
    // cv::bitwise_not(src, src);
    //    cv::imshow(filename, src);
    //    cv::waitKey(0);
    std::vector<cv::Vec4i> lines;
    
    cv::HoughLinesP(src, lines, 5, CV_PI/180, 150, 10, 20);
    
    
    cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
    double angle = 0.;
    unsigned nb_lines = lines.size();
    for (unsigned i = 0; i < nb_lines; ++i)
    {
        cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]),
                 cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0 ,0));
        angle += atan2((double)lines[i][3] - lines[i][1],
                       (double)lines[i][2] - lines[i][0]);
    }
    angle /= nb_lines; // mean angle, in radians.
    
    std::cout << "File " << filename << ": " << angle * 180 / CV_PI << std::endl;
    
    deskew(filename, angle * 180 / CV_PI);
    
    
    // cv::imshow(filename, disp_lines);
    //cv::waitKey(0);
    cv::destroyWindow(filename);
}

int main(int argc, char **argv)
{
    
    char *filePath = "/www/ocr/cap2/";
    
    vector<string> files;
    
    ////获取该路径下的所有文件
    
    //Read a single-channel image
    char* filename = argv[1];//"/www/ocr/cap2/im21.png";
    //     char* filename = "/www/ocr/cap2/im1016.png";
    
    compute_skew(filename);
    
    return 0;
}

