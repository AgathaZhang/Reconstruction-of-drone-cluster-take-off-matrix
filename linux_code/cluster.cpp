#include <vector>
#include <iostream>
#include <cmath>  // 用于数学计算
#include "matplotlibcpp.h"

// 定义点结构体
struct Point {
    double x, y, z;
    Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

// 判断两个点之间的距离是否大于 1.44m
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

int main() {
    // 示例点数据
    std::vector<Point> points = { Point(0, 0, 0), Point(2, 2, 0), Point(3, 3, 0), Point(5, 5, 0) };
    
    // 找到集群并输出
    std::vector<std::vector<Point>> clusters = findClusters(points);
    
    for (size_t i = 0; i < clusters.size(); ++i) {
        std::cout << "Cluster " << i + 1 << ":\n";
        for (const auto& point : clusters[i]) {
            std::cout << "(" << point.x << ", " << point.y << ", " << point.z << ")\n";
        }
    }

    return 0;
}
