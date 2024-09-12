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

    for (int i = 0; i < totalPlanes; ++i) {
        if (useXIntervalOne) {
            x += xIntervalOne;
        } else {
            x += xIntervalTwo;
        }
        useXIntervalOne = !useXIntervalOne;

        if (i > 0 && i % planesPerRow == 0) {
            y += yInterval;
            x = 0.0;
            useXIntervalOne = true;
        }

        coordinates.push_back(Point(x, y, z, 0, 0));
    }

    return coordinates;
}

// 计算两点间的欧氏距离
double calculateDistance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + 
                     std::pow(p2.y - p1.y, 2) + 
                     std::pow(p2.z - p1.z, 2));
}

// 对点进行分组，按照欧氏距离大于1.8m
std::vector<std::vector<Point>> grouping(std::vector<Point> coordinates) {
    std::vector<std::vector<Point>> groups;
    auto not_deal_set = coordinates;

    while (!not_deal_set.empty()) {
        std::vector<Point> group;
        Point referencePoint = not_deal_set[0];
        group.push_back(referencePoint);
        not_deal_set.erase(not_deal_set.begin());

        for (auto it = not_deal_set.begin(); it != not_deal_set.end();) {
            double distance = calculateDistance(referencePoint, *it);
            if (distance > 1.8) {
                group.push_back(*it);
                it = not_deal_set.erase(it);
            } else {
                ++it;
            }
        }
        groups.push_back(group);
    }
    return groups;
}

// 对分组后的点赋 z 值，依次递增 2
std::vector<Point> assignZValues(const std::vector<std::vector<Point>>& groups) {
    std::vector<Point> finish_set;
    double z_value = 0.0;

    for (const auto& group : groups) {
        for (const auto& point : group) {
            Point new_point = point;
            new_point.z = z_value;  // 赋 z 值
            finish_set.push_back(new_point);
        }
        z_value += 2.0;  // 每组 z 递增 2
    }

    return finish_set;
}

int main() {
    // 生成飞机坐标
    std::vector<Point> planeCoordinates = generatePlaneCoordinates();

    // 分组
    auto gr = grouping(planeCoordinates);

    // 赋 z 值并返回新的集合
    std::vector<Point> finish_set = assignZValues(gr);

    // 提取 x, y, z 坐标用于绘图
    std::vector<double> x, y, z;
    for (const auto& point : finish_set) {
        x.push_back(point.x);
        y.push_back(point.y);
        z.push_back(point.z);
    }

    // 绘制三维散点图
    plt::scatter(x, y, z);
    plt::xlabel("X Axis");
    plt::ylabel("Y Axis");
    plt::set_zlabel("Z Axis");
    plt::show();  // 显示图形

    return 0;
}
