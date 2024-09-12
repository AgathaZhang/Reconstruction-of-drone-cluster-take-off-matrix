#include "matplotlibcpp.h"
#include <vector>

namespace plt = matplotlibcpp;

int main() {
    // 定义点的数据
    std::vector<double> x_coords = {1.6, 3.6, 0, 2.4, 4.8, 1.2, 3.6, 0, 2.4, 4.8, 1.2, 3.6, 0, 2.4, 4.8};
    std::vector<double> y_coords = {0, 0, 2, 2, 2, 4, 4, 6, 6, 6, 8, 8, 10, 10, 10};

    // 绘制散点图
    plt::scatter(x_coords, y_coords);

    // 添加坐标轴标签
    plt::xlabel("X Axis");
    plt::ylabel("Y Axis");
    plt::title("Scatter Plot of Points");

    // 显示网格
    plt::grid(true);

    // 显示图形
    plt::show();

    return 0;
}
