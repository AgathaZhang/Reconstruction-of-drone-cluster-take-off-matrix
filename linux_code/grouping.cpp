#include "matplotlibcpp.h"
#include <vector>
#include <iostream>
#include <cmath>

namespace plt = matplotlibcpp;

// 定义一个结构体来存储坐标
struct Point {
    double x, y, z;
    Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

// 生成飞机摆放的矩阵
std::vector<Point> generatePlaneCoordinates(int totalPlanes = 300, 
                                            int planesPerRow = 10, 
                                            double xIntervalOne = 0.4, 
                                            double xIntervalTwo = 0.8, 
                                            double yInterval = 0.4, 
                                            double minSpacing = 0.0, 
                                            double layerSpacing = 0.0, 
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

// 对飞机进行分组
void grouping(std::vector<Point>& coordinates, double minSpacing, double layerSpacing, double height) {
    int group = 0;
    for (size_t i = 0; i < coordinates.size(); ++i) {
        // 跳过已经分组的点
        if (coordinates[i].z != height) continue;

        coordinates[i].z = height + group * layerSpacing;

        for (size_t j = i + 1; j < coordinates.size(); ++j) {
            // 检查 x 和 y 轴距离是否大于等于 minSpacing
            if (std::abs(coordinates[j].x - coordinates[i].x) >= minSpacing &&
                std::abs(coordinates[j].y - coordinates[i].y) >= minSpacing) {
                // 将符合条件的点分为同一组
                coordinates[j].z = height + group * layerSpacing;
            }
        }
        group++;  // 增加组数，下一组赋新的z值
    }
}

int main() {
    // 调用函数生成飞机坐标
    std::vector<Point> planeCoordinates = generatePlaneCoordinates();

    // 对飞机进行分组，分组时使用最小间距和层间距
    double minSpacing = 1.8;  // 示例值
    double layerSpacing = 2.0; // 层间距
    double height = 2.0;      // 高度
    grouping(planeCoordinates, minSpacing, layerSpacing, height);
    
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
