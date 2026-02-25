//
// Created by tomas on 2/25/26.
//

#include "canvas.h"
#include "shapes.h"
#include <vector>
#include <memory>

int main() {
    Canvas canvas(800, 600);
    canvas.show();
    cv::waitKey(0);
    return 0;
}