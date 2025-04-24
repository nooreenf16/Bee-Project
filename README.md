References:
https://docs.opencv.org/3.4/d8/dfe/classcv_1_1VideoCapture.html
https://docs.opencv.org/4.x/d1/dc5/tutorial_background_subtraction.html
https://learnopencv.com/contour-detection-using-opencv-python-c/

https://docs.opencv.org/4.x/df/d9d/tutorial_py_colorspaces.html
https://medium.com/@abhishekjainindore24/opencv-basic-functionalities-in-c-b172de18b664#:~:text=But%20before%20that%2C%20to%20run%20opencv%20program,compiler%20and%20cmake%20installed%20in%20our%20system.
https://www.opencv-srf.com/2018/03/gaussian-blur.html
https://answers.opencv.org/question/74403/create-mask-to-select-the-black-area/
https://dsp.stackexchange.com/questions/2564/opencv-c-connect-nearby-contours-based-on-distance-between-them
https://answers.opencv.org/question/127368/how-to-determine-the-average-bgr-values-of-all-the-pixels-in-a-closed-contour/
https://docs.opencv.org/4.x/d2/de8/group__core__array.html#ga191389f8a0e58180bb13a727782cd461

STEPS:

1. read frames
2. apply green mask (to remove green) and dark mask to extract dark pixels only
   Tracking:
3. get frames where values have changed using abs difference of prev frame and current frame
4. dilate the resultant to make the difference more apparent
5. get contours of the resultant
6. keep only those contours whose average pixel intensities are very dark (since bees are dark)
7. get bounding boxes of each
8. merge bounding boxes which overlap with each other
9. merge bounding boxes which are very close to eachother (becasue the contours of the bees could be split during findcontours)
10. draw the final bounding boxes on the video frames
11. download output.
