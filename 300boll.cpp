/**
  ******************************************************************************
  * @file    B03/Take-off matrix
  * @author  Zhang weizhi
  * @version V1.0.0
  * @date    3-9-2024
  * @brief   这个算法是GPT给出的 已知底面和球的半径 计算最小堆积高度的算法
  ******************************************************************************
*/

// #include <iostream>
// #include <cmath>
// #include <iomanip>  // for std::setprecision

// // 计算六方最密堆积（HCP）或面心立方堆积（FCC）方式的单元格体积
// double hcp_fcc_packing_volume(double radius) {
//     double ball_volume = 4.0/3.0 * M_PI * std::pow(radius, 3);      // 单个球的体积
//     double unit_cell_volume = 2 * ball_volume;                      // HCP 或 FCC 堆积中一个单元格包含 2 个球
//     return unit_cell_volume;
// }

// // 计算在给定底面积情况下，至少需要的高度以容纳给定数量的球
// double calculate_minimum_height(double base_area/*底面积*/, int num_balls/*球数*/, double radius/*半径*/) {

//     double diameter = 2 * radius;    // 球的直径
//     auto tt = (base_area / (std::sqrt(3) * std::pow(diameter, 2) / 2));
//     int balls_per_layer = (3 * std::sqrt(3) / 2) * std::pow(radius, 2);
//     // int balls_per_layer = static_cast<int>(base_area / (std::sqrt(3) * std::pow(diameter, 2) / 2));         // 计算单层球的最大数量（假设为紧密排列）

//     int num_layers = std::ceil(static_cast<double>(num_balls) / balls_per_layer);                           // 计算需要的层数
//     double min_height = num_layers * std::sqrt(2.0/3.0) * diameter;                                         // 计算最小高度

//     return min_height;
// }

// int main() {

//     double base_area = 25.0;        // 盒子的底面积
//     double radius = 2.5;            // 球的半径
//     int num_balls = 300;            // 要放置的球的数量

//     // 计算最小所需高度
//     double min_height = calculate_minimum_height(base_area, num_balls, radius);

//     std::cout << "为装下 " << num_balls << " 个半径为 " << radius << " 的球，至少需要的高度是: " 
//               << std::setprecision(2) << std::fixed << min_height << std::endl;

//     return 0;
// }


#include <iostream>
#include <cmath>
#include <vector>

struct Point {
    double x, y, z;
};

double calculate_hexagon_area(double radius) {
    return (3 * std::sqrt(3) / 2) * std::pow(radius, 2);
}

int calculate_balls_per_layer(double base_area, double radius) {
    double hexagon_area = calculate_hexagon_area(radius);
    return static_cast<int>(base_area / hexagon_area);
}

double calculate_minimum_height(double base_area, int num_balls, double radius) {

    double diameter = 2 * radius;       // 球的直径
    int balls_per_layer = calculate_balls_per_layer(base_area, radius);    // 计算单层球的最大数量

    if (balls_per_layer < 1) {    // 如果计算出的层数小于1，说明底面积不够放下一个完整的单层
        std::cerr << "底面积不足以放下任何一个完整的单层。" << std::endl;
        return 0;
    }


    int num_layers = static_cast<int>(std::ceil(static_cast<double>(num_balls) / balls_per_layer)); // 计算需要的层数
    double layer_height = std::sqrt(2.0 / 3.0) * diameter;  // 计算每层球的高度，HCP 堆积的高度比是 sqrt(2/3)     
    double min_height = num_layers * layer_height;          // 计算最小高度
    return min_height;
}

int main() {
    // double base_area = 4*12;  // 盒子的底面积
    // double radius = 0.9;      // 球的半径
    // int num_balls = 300;      // 球的数量
    double base_area = 25;  // 盒子的底面积
    double radius = 0.75;      // 球的半径
    int num_balls = 1;      // 球的数量

    // 计算最小高度
    double min_height = calculate_minimum_height(base_area, num_balls, radius);

    if (min_height > 0) {
        std::cout << "为装下 " << num_balls << " 个半径为 " << radius 
                  << " 的球，至少需要的高度是: " << min_height << std::endl;
    }

    return 0;
}
