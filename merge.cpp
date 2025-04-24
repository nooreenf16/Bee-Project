#include "merge.h"

// 1. merge overlapping bounding boxes
// 2. merge bounding boxes which are close to eachother

using namespace cv;
using namespace std;

vector<Rect> mergeOverlappingBoxes(const vector<Rect> &boxes)
{
    vector<Rect> toProcess = boxes;
    vector<Rect> merged;

    while (!toProcess.empty())
    {
        Rect base = toProcess.back();
        toProcess.pop_back();

        bool hasMerged = false;
        for (auto it = toProcess.begin(); it != toProcess.end();)
        {
            if ((base & *it).area() > 0)
            {
                base = base | *it;
                it = toProcess.erase(it);
                hasMerged = true;
            }
            else
            {
                ++it;
            }
        }

        merged.push_back(base);
    }

    return merged;
}
bool rectsAreClose(const Rect &r1, const Rect &r2, int threshold = 100)
{
    // Expand r1 by `threshold` in all directions
    Rect inflated = r1;
    inflated.x -= threshold;
    inflated.y -= threshold;
    inflated.width += 2 * threshold;
    inflated.height += 2 * threshold;

    return (inflated & r2).area() > 0;
}

vector<Rect> mergeCloseBoxes(const vector<Rect> &boxes, int threshold = 100)
{
    vector<bool> visited(boxes.size(), false);
    vector<Rect> merged;

    for (size_t i = 0; i < boxes.size(); ++i)
    {
        if (visited[i])
            continue;

        Rect current = boxes[i];
        // Rect current;

        for (size_t j = i + 1; j < boxes.size(); ++j)
        {
            if (visited[j])
                continue;

            if (rectsAreClose(current, boxes[j], threshold))
            {
                current = current | boxes[j]; // union of both boxes
                visited[j] = true;
            }
        }

        merged.push_back(current);
        visited[i] = true;
    }

    return merged;
}