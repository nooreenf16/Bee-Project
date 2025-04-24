// keeping only highest pixels - black in the frames

#include <opencv2/opencv.hpp>
#include <iostream>

#include "merge.h"
#include "timestamp.h"
using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap;

    cap.open("Visit 2.mp4");
    if (!cap.isOpened())
    {
        cerr << "Failed to open video!" << endl;
        return -1;
    }
    double fps = cap.get(CAP_PROP_FPS); // fps of video

    int width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

    VideoWriter video("output.mp4",
                      VideoWriter::fourcc('M', 'J', 'P', 'G'),
                      30,
                      Size(width, height),
                      true);

    Mat frame, frame_difference, fgMask;
    Mat previous_frame;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5)); // Rectangular kernel of size 3x3
    Mat dilatedImage;
    int iter = 0, frame_count = 0, frame_enter = 0, frame_exit = 0, final_frame_exit = 0;

    while (true)
    {
        // 1. read frame
        cap >> frame;
        if (frame.empty())
        {
            cout << "End of video stream" << endl;
            break;
        }

        // 2. MASKS

        Mat green_mask;
        inRange(frame, Scalar(0, 0, 0), Scalar(70, 255, 70), green_mask);

        Mat processed = frame.clone(); // making green areas white so they dont interfere with black mask
        processed.setTo(Scalar(255, 255, 255), green_mask);

        Mat dark_mask;
        inRange(processed, Scalar(30, 40, 30), Scalar(90, 70, 90), dark_mask);

        if (iter == 0) // first frame
        {
            iter++;
            dark_mask.copyTo(previous_frame); // copy frame
            continue;
        }

        // 3. FRAME DIFF, DILATE, CONTOURS, TRACKING

        absdiff(previous_frame, dark_mask, frame_difference);
        dilate(frame_difference, dilatedImage, kernel);

        dark_mask.copyTo(previous_frame); // copy frame for tracking

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(dilatedImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

        // draw rectangles
        vector<Rect> boxes;
        // 84 + 73 + 60 => rgb
        for (const auto &c : contours)
        {
            if (contourArea(c) < 3800)
                continue;

            // getting average intensity of contours and keeping only the max values boxes
            Mat mask = Mat::zeros(frame.size(), CV_8UC1);
            vector<vector<Point>> singleContour = {c};
            drawContours(mask, singleContour, -1, Scalar(255), FILLED);

            Scalar mean_cont = mean(frame, mask);
            float curr = (mean_cont[0] + mean_cont[1] + mean_cont[2]) / 3;

            // cout << mean_cont[0] << " " << mean_cont[1] << " " << mean_cont[2] << endl;
            if (curr > 72 && mean_cont[2] > 85 && mean_cont[1] > 72 && mean_cont[0] > 63)
            {
                Rect r = boundingRect(c);
                boxes.push_back(r);
            }
        }

        // merging rectangles
        vector<Rect> merged = mergeOverlappingBoxes(boxes);
        vector<Rect> merged2 = mergeCloseBoxes(merged, 150);

        for (const auto &r : merged2)
        {

            rectangle(frame, r, Scalar(150, 225, 135), 5);
        }

        // 4. TIMESTAMPS
        if (frame_enter == 0 && merged2.size() >= 1)
            frame_enter = frame_count;

        else if (merged2.size() == 0)
            final_frame_exit = frame_count; // in case some noise appears after the bee has left

        else if (merged2.size() <= 1)
            frame_exit = frame_count;
        frame_count++;

        video.write(frame);
        imshow("frame", frame);

        if (waitKey(1) == 27)
            break;
    }
    cap.release();
    destroyAllWindows();

    cout << "Frame entered: " << frame_enter << " Frame exited: " << frame_exit << endl;

    // Format the time in "minute:second" format
    pair<int, int> result_enter = timestamp(frame_enter, fps);
    cout << "Time entered: " << result_enter.first << ":" << (result_enter.second < 10 ? "0" : "") << result_enter.second << " ";

    pair<int, int> result_exit = timestamp((final_frame_exit < frame_exit ? final_frame_exit : frame_exit), fps);
    cout << "Time exit: " << result_exit.first << ":" << (result_exit.second < 10 ? "0" : "") << result_exit.second << endl;

    return 0;
}