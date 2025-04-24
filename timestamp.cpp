#include "timestamp.h"
using namespace std;

pair<int, int> timestamp(const int frame, const double fps)
{
    double timeInSeconds = frame / fps;

    int minutes = static_cast<int>(timeInSeconds) / 60;
    int seconds = static_cast<int>(timeInSeconds) % 60;

    return make_pair(minutes, seconds);
}
