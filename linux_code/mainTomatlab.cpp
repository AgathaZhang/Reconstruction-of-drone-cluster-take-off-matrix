#include "matplotlibcpp.h"
#include <vector>
#include <iostream>
#include <map>

namespace plt = matplotlibcpp;

// 定义一个结构体来存储坐标
struct Point {
    double x, y, z;
    int times_increase_x, times_increase_y;
    Point(double x_, double y_, double z_, int id_x ,int id_y) : x(x_), y(y_), z(z_) ,times_increase_x(id_x), times_increase_y(id_y) {}
};

// 生成飞机摆放的矩阵
std::vector<Point> generatePlaneCoordinates(int totalPlanes = 300, 
                                            int planesPerRow = 10, 
                                            double xIntervalOne = 0.4, 
                                            double xIntervalTwo = 0.8, 
                                            double yInterval = 0.4, 
                                            double minSpacing = 1.8, 
                                            double layerSpacing = 1.8, 
                                            double height = 2.0) {
    // 如果输入参数为 0，使用默认参数
    totalPlanes = (totalPlanes == 0) ? 300 : totalPlanes;
    planesPerRow = (planesPerRow == 0) ? 10 : planesPerRow;
    xIntervalOne = (xIntervalOne == 0) ? 0.4 : xIntervalOne;
    xIntervalTwo = (xIntervalTwo == 0) ? 0.8 : xIntervalTwo;
    yInterval = (yInterval == 0) ? 0.4 : yInterval;
    height = (height == 0) ? 2.0 : height;

    std::vector<std::vector<Point>> groups;
    std::vector<Point> coordinates;
    
    double x = -0.4, y = 0.0, z = 0.0;
    bool useXIntervalOne = true;

    // 记录 x 和 y 总增量
    double xTotalIncrement = 0.0;
    double yTotalIncrement = 0.0;
    int times_increase_x = -1;
    int times_increase_y = 0;
    int times_increase_y_true = 0;

    for (int i = 0; i < totalPlanes; ++i) {
        // 计算x坐标，交替使用xIntervalOne和xIntervalTwo
        if (useXIntervalOne) {
            x += xIntervalOne;
            if (i == 0){xTotalIncrement = 0;}
            else {xTotalIncrement += xIntervalOne;}
            times_increase_x ++;
        } else {
            x += xIntervalTwo;
            xTotalIncrement += xIntervalTwo;
            times_increase_x ++;
        }
        useXIntervalOne = !useXIntervalOne; // 交替切换间隔
        if (i == 0) {useXIntervalOne = true;}

        // 如果x的总增量超过minSpacing，则重置x并重置增量
        if (xTotalIncrement >= minSpacing) {
            xTotalIncrement = 0.0;
            times_increase_x = 0;
        }

        // 计算y坐标，每排放 planesPerRow 个后换行
        if (i > 0 && i % planesPerRow == 0) {
            y += yInterval; // 换行后 y 轴坐标增加
            yTotalIncrement += yInterval;
            times_increase_y ++;

        // 如果y的总增量超过minSpacing，则重置y并重置增量
        if (yTotalIncrement >= minSpacing) {
            yTotalIncrement = 0.0;
            times_increase_y = 0;
        }

            x = 0.0;        // 换行后 x 轴坐标重置为 0
            useXIntervalOne = true; // 新的一行从xIntervalOne开始
            
        }
            /** z轴的增量函数 */

        // 添加当前坐标点，z值为高度
        coordinates.push_back(Point(x, y, 0, times_increase_x, times_increase_y));
    }
    
    /** 开始处理分组 */
    std::vector<std::vector<Point>> result;  // 用于存储分组后的点集

    // 遍历每个点
    for (const auto& point : coordinates) {
        bool foundGroup = false;

        // 遍历当前已有的分组，寻找是否已经有相同 (id_x, id_y) 的组
        for (auto& group : result) {
            if (!group.empty() && group[0].times_increase_x == point.times_increase_x && group[0].times_increase_y == point.times_increase_y) {
                // 找到相同 id_x 和 id_y 的组，将点添加到该组中
                group.push_back(point);
                foundGroup = true;
                break;
            }
        }

        // 如果没有找到相同 id_x 和 id_y 的组，则创建新的组
        if (!foundGroup) {
            result.push_back({point});
        }
    }

    std::vector<Point> result2;
        // 遍历所有分组
    for (auto& group : result) {
        // 为每组的所有点赋值 z，并将点添加到结果中
        for (auto& point : group) {
            point.z = height;  // 赋 z 值
            result2.push_back(point);  // 将点添加到结果中
        }
        // 每组之后增加 layerSpacing
        height += layerSpacing;
    }

    // for (size_t i = 0; i < result.size(); i++)
    // {
    //     for (size_t j = 0; j < result[i].size(); j++)
    //     {
    //         if (j >= result[i].size()) {continue;}
    //         else {result2.push_back(result[i][j]);}
    //     }
    // }

    return result2;
}


/** 计算两点间的欧氏距离*/
double calculateDistance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + 
                     std::pow(p2.y - p1.y, 2) + 
                     std::pow(p2.z - p1.z, 2));
}

// /** 检查是否存在欧氏距离小于1.8米的点对*/
// bool checkMinDistance(const std::vector<Point>& points, double minDistance) {
//     for (size_t i = 0; i < points.size(); ++i) {
//         for (size_t j = i + 1; j < points.size(); ++j) {
//             if (calculateDistance(points[i], points[j]) < minDistance) {
//                 return true;
//             }
//         }
//     }
//     return false;
// }

/** 返回最小欧氏距离 */
double checkMinDistance(const std::vector<Point>& points, double minDistance) {
    double minDist = std::numeric_limits<double>::max();  // 初始化为一个很大的值

    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            double dist = calculateDistance(points[i], points[j]);
            // printf("dist: %f \n",dist);
            if (dist < minDist) {
                minDist = dist;  // 记录当前最小距离
            }
        }
    }

    return minDist;  // 返回找到的最小距离
}

// 重新赋值 z 值的函数，并返回取消分组后的点集合
std::vector<Point> assignZValues(std::vector<std::vector<Point>>& groupedPoints, double height, double layerSpacing) {
    std::vector<Point> result;  // 用于存储取消分组后的结果
    double currentHeight = height;  // 初始 z 值

    // 遍历所有分组
    for (auto& group : groupedPoints) {
        // 为每组的所有点赋值 z，并将点添加到结果中
        for (auto& point : group) {
            point.z = currentHeight;  // 赋 z 值
            result.push_back(point);  // 将点添加到结果中
        }
        // 每组之后增加 layerSpacing
        currentHeight += layerSpacing;
    }

    return result;  // 返回取消分组后的点集合
}

int main() {

    // 调用函数并输出结果
    std::vector<Point> result = generatePlaneCoordinates();
    // for (const auto& point : result) {
    //     std::cout << "x: " << point.x << ", y: " << point.y << ", z: " << point.z << ", id_x: " << point.times_increase_x << ", id_y: " << point.times_increase_y << std::endl;
    // }

    // 计算最小欧氏距离
    double minDistance = checkMinDistance(result, 1.8);

    std::cout << "The minimum Euclidean distance between any two points is: " << minDistance << " meters." << std::endl;

    // 提取 x, y, z 坐标

    std::vector<double> x, y, z;
    for (const auto& point : result) {
        x.push_back(point.x);
        y.push_back(point.y);
        z.push_back(point.z);
    }
    
    // 使用 matplotlibcpp 绘制三维散点图
    plt::scatter(x, y, z);
    plt::xlabel("X Axis");
    plt::ylabel("Y Axis");
    plt::set_zlabel("Z Axis");
    // plt::set_aspect("equal");
    plt::show();  // 显示图形

    
    return 0;
}
