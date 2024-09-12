#include "matplotlibcpp.h"
#include <vector>
#include <iostream>

namespace plt = matplotlibcpp;

// 定义一个结构体来存储坐标
struct Point {
    double x, y, z;
    int times_increase_x, times_increase_y;
    Point(double x_, double y_, double z_, int ix_, int iy_) : x(x_), y(y_), z(z_), times_increase_x(ix_), times_increase_y(iy_) {}
};

// 生成飞机摆放的矩阵
std::vector<Point> generatePlaneCoordinates(int totalPlanes = 300,
                                            int planesPerRow = 10,
                                            double xIntervalOne = 0.4,
                                            double xIntervalTwo = 0.8,
                                            double yInterval = 0.4,
                                            double minSpacing = 1.8,
                                            double layerSpacing = 2.0,
                                            double height = 2.0) {
    std::vector<Point> coordinates;
    double x = -0.4, y = 0.0, z = 0.0;
    bool useXIntervalOne = true;
    double xTotalIncrement = 0.0;
    double yTotalIncrement = 0.0;
    int times_increase_x = 0;
    int times_increase_y = 0;
    int times_increase_y_true = 0;

    for (int i = 0; i < totalPlanes; ++i) {
        if (useXIntervalOne) {
            x += xIntervalOne;
            xTotalIncrement += xIntervalOne;
            times_increase_x++;
        } else {
            x += xIntervalTwo;
            xTotalIncrement += xIntervalTwo;
            times_increase_x++;
        }
        useXIntervalOne = !useXIntervalOne;
        if (i == 0) { useXIntervalOne = true; }

        if (xTotalIncrement >= minSpacing) {
            xTotalIncrement = 0.0;
            times_increase_x = 0;
        }

        if (i > 0 && i % planesPerRow == 0) {
            y += yInterval;
            yTotalIncrement += yInterval;
            times_increase_y++;

            if (yTotalIncrement >= minSpacing) {
                yTotalIncrement = 0.0;
                times_increase_y = 0;
            }

            x = 0.0;
            useXIntervalOne = true;
            times_increase_y_true = times_increase_y + times_increase_x;
        }

        z = (times_increase_x + times_increase_y_true) * minSpacing + height;
        coordinates.push_back(Point(x, y, z, times_increase_x, times_increase_y));
    }

    return coordinates;
}

/** 计算两点间的欧氏距离*/
double calculateDistance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) +
                     std::pow(p2.y - p1.y, 2) +
                     std::pow(p2.z - p1.z, 2));
}

/** 返回最小欧氏距离 */
double checkMinDistance(const std::vector<Point>& points, double minDistance) {
    double minDist = std::numeric_limits<double>::max();

    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            double dist = calculateDistance(points[i], points[j]);
            if (dist < minDist) {
                minDist = dist;
            }
        }
    }

    return minDist;
}

int main() {
    // 调用函数并输出结果
    std::vector<Point> planeCoordinates = generatePlaneCoordinates();

    // 分离 ix_ 或 iy_ 等于 0 的点
    std::vector<double> x_default, y_default, z_default;
    std::vector<double> x_highlight, y_highlight, z_highlight;

    for (const auto& point : planeCoordinates) {
        if (point.times_increase_x == 0 || point.times_increase_y == 0) {
            x_highlight.push_back(point.x);
            y_highlight.push_back(point.y);
            z_highlight.push_back(point.z);
        } else {
            x_default.push_back(point.x);
            y_default.push_back(point.y);
            z_default.push_back(point.z);
        }
    }

    // 计算最小欧氏距离
    double minDistance = checkMinDistance(planeCoordinates, 1.8);
    std::cout << "The minimum Euclidean distance between any two points is: " << minDistance << " meters." << std::endl;

    // 使用 matplotlibcpp 绘制三维散点图，两个数据集绘制在同一张图上
    plt::scatter(x_default, y_default, z_default);  // 默认颜色的点
    plt::scatter(x_highlight, y_highlight, z_highlight, 50.0, {{"color", "yellow"}});  // 黄色标出的点
    plt::xlabel("X Axis");
    plt::ylabel("Y Axis");
    plt::set_zlabel("Z Axis");
    plt::show();  // 显示图形

    return 0;
}
