#ifndef MAINPF_HPP
#define MAINPF_HPP



void initPF();
int mainParticleFilter(cv::Mat frame);
void initMainParticleFilter(cv::Mat frame, cv::Rect selectROI);

float getNorm();


#endif
