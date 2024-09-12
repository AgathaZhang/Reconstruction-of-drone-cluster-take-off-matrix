#include "matplotlibcpp.h"
#include <vector>
#include <iostream>
#include <limits>

namespace plt = matplotlibcpp;

// 定义一个结构体来存储坐标
struct Point {
    double x, y, z;
    int times_increase_x, times_increase_y;
    Point(double x_, double y_, double z_, int ix_ ,int iy_) : x(x_), y(y_), z(z_) ,times_increase_x(ix_), times_increase_y(iy_) {}
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
    std::vector<Point> cache_region; // 用来保存距离不满足的点
    std::vector<Point> extrusion;
    double x = -0.4, y = 0.0, z = 0.0;
    bool useXIntervalOne = true;

    double xTotalIncrement = 0.0;
    double yTotalIncrement = 0.0;
    int times_increase_x = 0;
    int times_increase_y = 0;
    int times_increase_y_true = 0;

    // 欧氏距离计算函数
    auto calculateDistance = [](const Point& p1, const Point& p2) {
        return std::sqrt(std::pow(p2.x - p1.x, 2) + 
                         std::pow(p2.y - p1.y, 2) + 
                         std::pow(p2.z - p1.z, 2));
    };

    for (int i = 0; i < totalPlanes; ++i) {
        // 计算x坐标
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

            // times_increase_y_true = times_increase_y + times_increase_x;
            times_increase_y_true = times_increase_y;
        }

        z = (times_increase_x + times_increase_y_true) * minSpacing + height;

        // 创建当前点
        Point newPoint(x, y, z, times_increase_x, times_increase_y);

        // 检查是否与已有点的欧氏距离小于1.8米
        bool tooClose = false;
        for (const auto& existingPoint : coordinates) {
            if (calculateDistance(newPoint, existingPoint) < minSpacing) {
                tooClose = true;
                break;
            }
        }

        // 如果距离太近，设置z值为0，并将点加入cache_region
        if (tooClose) {
            newPoint.z = 0;
            cache_region.push_back(newPoint);
        } else {
            // 添加点到坐标列表
            coordinates.push_back(newPoint);
        }
    }
    auto cache_regionSIZE = cache_region.size();
    auto coordinatesSIZE = coordinates.size();
    return coordinates;
}

int main() {
    // 调用函数并输出结果
    std::vector<Point> planeCoordinates = generatePlaneCoordinates();
    
    for (const auto& point : planeCoordinates) {
        std::cout << "x: " << point.x << ", y: " << point.y << ", z: " << point.z << std::endl;
    }

    // 提取 x, y, z 坐标
    std::vector<double> x, y, z;
    for (const auto& point : planeCoordinates) {
        x.push_back(point.x);
        y.push_back(point.y);
        z.push_back(point.z);
    }

    // 使用 matplotlibcpp 绘制三维散点图
    plt::scatter(x, y, z);
    plt::xlabel("X Axis");
    plt::ylabel("Y Axis");
    plt::set_zlabel("Z Axis");
    plt::show();  // 显示图形

    return 0;
}
