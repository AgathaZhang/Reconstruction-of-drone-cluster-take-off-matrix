#include <vector>
#include <iostream>
#include <cmath>  // 用于数学计算
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// 定义点结构体
struct Point {
    double x, y, z;
    Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

// 判断两个点之间的距离是否大于等于 1.8m
bool isDistanceGreaterThan1_8m(const Point& p1, const Point& p2) {
    double distance = std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
    return distance >= 1.8;
}

// 查找并分组集群
std::vector<std::vector<Point>> findClusters(const std::vector<Point>& points) {
    std::vector<std::vector<Point>> clusters;
    std::vector<Point> remainingPoints = points;

    while (!remainingPoints.empty()) {
        std::vector<Point> cluster;
        cluster.push_back(remainingPoints.front());
        remainingPoints.erase(remainingPoints.begin());

        for (int j = remainingPoints.size() - 1; j >= 0; --j) {
            bool tooShort = false;
            for (size_t i = 0; i < cluster.size(); ++i) {
                if (isDistanceGreaterThan1_8m(cluster[i], remainingPoints[j])) {
                    tooShort = true;
                    break;
                }
            }
            if (!tooShort) {
                cluster.push_back(remainingPoints[j]);
                remainingPoints.erase(remainingPoints.begin() + j);
            }
        }
        clusters.push_back(cluster);
    }
    return clusters;
}

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
    double x = -0.4, y = 0.0;
    bool useXIntervalOne = true;

    for (int i = 0; i < totalPlanes; ++i) {
        // 计算x坐标，交替使用xIntervalOne和xIntervalTwo
        if (useXIntervalOne) {
            x += xIntervalOne;
        } else {
            x += xIntervalTwo;
        }
        useXIntervalOne = !useXIntervalOne; // 交替切换间隔
        if (i == 0) {useXIntervalOne = true;}

        // 计算y坐标，每排放 planesPerRow 个后换行
        if (i > 0 && i % planesPerRow == 0) {
            y += yInterval; // 换行后 y 轴坐标增加
            x = 0.0;        // 换行后 x 轴坐标重置为 0
            useXIntervalOne = true; // 新的一行从xIntervalOne开始
        }

        // 添加当前坐标点，z值为高度
        coordinates.push_back(Point(x, y, height));
    }

    return coordinates;
}

int main() {
    // 生成飞机坐标
    std::vector<Point> planeCoordinates = generatePlaneCoordinates();

    // 对生成的坐标进行分组
    std::vector<std::vector<Point>> clusters = findClusters(planeCoordinates);

    // 为每个分组设置 z 值
    double layerSpacing = 2.0; // 层间距
    double height = 2.0;       // 初始高度
    for (size_t i = 0; i < clusters.size(); ++i) {
        for (auto& point : clusters[i]) {
            point.z = height + i * layerSpacing;  // 根据分组层次设置 z 值
        }
    }

    // 提取所有分组中的 x, y, z 坐标
    std::vector<double> x, y, z;
    for (const auto& cluster : clusters) {
        for (const auto& point : cluster) {
            x.push_back(point.x);
            y.push_back(point.y);
            z.push_back(point.z);
        }
    }

    // 使用 matplotlibcpp 绘制三维散点图
    plt::scatter(x, y, z);
    plt::xlabel("X Axis");
    plt::ylabel("Y Axis");
    plt::set_zlabel("Z Axis");
    plt::show();  // 显示图形

    return 0;
}
