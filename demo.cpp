// ------------------------------------------------------------------
// Copyright (c) HWater. All rights reserved HWater. if any questions about
// this, please mail to hsqaihkl@163.com
// ------------------------------------------------------------------
// Project Name : RawMultiFrameHDR
// Author       : HWater
// Date         : 2022-12-18
// Description  : Raw HDR demo
// ------------------------------------------------------------------
#include<iostream>
#include<opencv2/opencv.hpp>

cv::Mat ReadRaw(std::string path,int heigh,int width)
{
    int size = heigh * width;
    cv::Mat raw;
    raw.create(cv::Size(width, heigh), CV_16UC1);
    ushort* raw_ptr = raw.ptr<ushort>();
    FILE* fpsrc = NULL;
    if ((fpsrc = fopen(path.data(), "rb")) == NULL) {
        printf("Can't open the raw image!\n");
        
    }
    fread(raw_ptr, 1, size * sizeof(ushort), fpsrc);
    fclose(fpsrc);
    return raw;
}

int WriteRaw(std::string path, cv::Mat raw)
{
    int height = raw.rows;
    int width = raw.cols;
	int pixel_num=height*width;
    ushort* raw_ptr = raw.ptr<ushort>();
    FILE* pFile = fopen(path.data(), "wb");//
    if (pFile == nullptr) {
        printf("%s file can't open!\n", path.data());
        return 0;
    }
    else {
        fwrite(raw_ptr, 2, pixel_num, pFile);
    }

    fclose(pFile);
    return 1;
}

std::string changePathWithPostfix(std::string path, std::string postfix)
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char extc[_MAX_EXT];
    _splitpath(path.data(), drive, dir, fname, extc);
    std::string ext = extc;
    int len = path.size();
    int ext_len = ext.size();

    std::string dst = path;
    dst = dst.insert(len - ext_len, postfix);
    return dst;
}

void main(int partc, char* partv[])
{
	std::string postfix = "_hdr";
	std::string path = "..//data//raw//my_house_4640x3472_GBRG.raw";
	std::string path_save = "";
	path_save = changePathWithPostfix(path, postfix);

    int white_value = 1023;
    float detapixel = 1.0 / float(white_value);

    int heigh=3472;
	int width=4640;

    float rgain = 1.435f;
    float bgain = 2.052f;
	
    float gamma = 2.2;

	cv::Mat bayer_image;

	if (partc > 1)
	{
		
		path = partv[1];
		path_save = changePathWithPostfix(path, postfix);

	}
	bayer_image =ReadRaw(path,heigh,width);
    

    cv::Mat dst_demosaic_image,bay;
    bayer_image.convertTo(bay, CV_32FC1, detapixel);

    //demosaic
    cv::cvtColor(bayer_image, dst_demosaic_image, cv::COLOR_BayerGB2BGR);
    dst_demosaic_image.convertTo(dst_demosaic_image, CV_32FC3, detapixel);

    //awb
    cv::Scalar awb(bgain, 1.0, rgain);
    dst_demosaic_image = dst_demosaic_image.mul(awb);

    //gamma
    cv::pow(dst_demosaic_image, 1.0 / gamma, dst_demosaic_image);
}