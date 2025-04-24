// merge.h
#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

std::vector<cv::Rect> mergeOverlappingBoxes(const std::vector<cv::Rect> &boxes);
std::vector<cv::Rect> mergeCloseBoxes(const std::vector<cv::Rect> &boxes, int threshold);
