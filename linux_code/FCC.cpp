#include "matplotlibcpp.h"
#include <vector>
#include <iostream>

namespace plt = matplotlibcpp;

// 定义一个结构体来存储坐标
struct Point {
    double x, y, z;
    Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

// 生成飞机摆放的矩阵
std::vector<Point> generateHCPCoordinates(int totalPlanes = 300, 
                                          int planesPerRow = 10, 
                                          double xIntervalOne = 0.4, 
                                          double xIntervalTwo = 0.8, 
                                          double yInterval = 0.4, 
                                          double layerSpacing = 2.0) {
    std::vector<Point> coordinates;
    double x = -0.4, y = 0.0;
    bool useXIntervalOne = true;
    int layerCount = 0;

    for (int i = 0; i < totalPlanes; ++i) {
        // 计算x坐标，交替使用xIntervalOne和xIntervalTwo
        if (useXIntervalOne) {
            x += xIntervalOne;
        } else {
            x += xIntervalTwo;
        }
        useXIntervalOne = !useXIntervalOne; // 交替切换间隔
        if (i == 0) { useXIntervalOne = true; }

        // 计算y坐标，每排放 planesPerRow 个后换行
        if (i > 0 && i % planesPerRow == 0) {
            y += yInterval; // 换行后 y 轴坐标增加
            x = 0.0;        // 换行后 x 轴坐标重置为 0
            useXIntervalOne = true; // 新的一行从xIntervalOne开始
        }

        // 根据层数决定 z 坐标
        double z = layerCount % 3 == 0 ? 0.0 : (layerCount % 3 == 1 ? layerSpacing / 2 : layerSpacing);
        
        // 如果是第二层（layerCount % 3 == 1），x 和 y 要进行偏移
        if (layerCount % 3 == 1) {
            x += xIntervalOne / 2;
            y += yInterval / 2;
        }

        // 添加当前坐标点
        coordinates.push_back(Point(x, y, z));

        // 每 planesPerRow * 2 个点构成一层
        if (i > 0 && i % (planesPerRow * 2) == 0) {
            layerCount++;
            y = 0.0; // 层切换后，y 轴重新开始
        }
    }

    return coordinates;
}

int main() {
    // 调用函数并输出结果
    std::vector<Point> hcpCoordinates = generateHCPCoordinates();
    
    for (const auto& point : hcpCoordinates) {
        std::cout << "x: " << point.x << ", y: " << point.y << ", z: " << point.z << std::endl;
    }

    // 提取 x, y, z 坐标
    std::vector<double> x, y, z;
    for (const auto& point : hcpCoordinates) {
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
