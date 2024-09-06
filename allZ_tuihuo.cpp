/**
  ******************************************************************************
  * @file    B03/Take-off matrix
  * @author  Zhang weizhi
  * @version V1.0.0
  * @date    3-9-2024
  * @brief   1大致计算所需的FCC密铺高度 不改变xy坐标，在高度区间内对所有点的z赋随机初值 
  *          2模拟退火算法 优化到满足避碰半径约束目标函数是Σz 即对z的和进行优化
  ******************************************************************************
*/

#include <iostream>  // 引入输入输出流库
#include <vector>    // 引入向量容器库
#include <cmath>     // 引入数学函数库
#include <limits>    // 引入用于处理极值的库
#include <algorithm> // 引入算法库
#include <random>    // 引入随机数生成库
#include <iostream>
#include <cmath>
#include <vector>


 #include <mgl2\mgl.h>
 
 int main()
 {
     mglGraph gr;
     gr.Title("MathGL Demo");
     gr.SetOrigin(0, 0);
     gr.SetRanges(0, 10, -2.5, 2.5);
      gr.FPlot("sin(1.7*2*pi*x) + sin(1.9*2*pi*x)", "r-2");
     gr.Axis();
     gr.Grid();
     gr.WriteFrame("mgl_example.png");
}


// double calculate_hexagon_area(double radius) {
//     return (3 * std::sqrt(3) / 2) * std::pow(radius, 2);
// }

// int calculate_balls_per_layer(double base_area, double radius) {
//     double hexagon_area = calculate_hexagon_area(radius);
//     return static_cast<int>(base_area / hexagon_area);
// }

// double calculate_minimum_height(double base_area, int num_balls, double radius) {

//     double diameter = 2 * radius;       // 球的直径
//     int balls_per_layer = calculate_balls_per_layer(base_area, radius);    // 计算单层球的最大数量

//     if (balls_per_layer < 1) {    // 如果计算出的层数小于1，说明底面积不够放下一个完整的单层
//         std::cerr << "底面积不足以放下任何一个完整的单层。" << std::endl;
//         return 0;
//     }


//     int num_layers = static_cast<int>(std::ceil(static_cast<double>(num_balls) / balls_per_layer)); // 计算需要的层数
//     double layer_height = std::sqrt(2.0 / 3.0) * diameter;  // 计算每层球的高度，HCP 堆积的高度比是 sqrt(2/3)     
//     double min_height = num_layers * layer_height;          // 计算最小高度
//     return min_height;
// }


// // 定义点结构体，包含x, y, z三个坐标
// struct Point {
//     double x, y, z;

//     // 构造函数，初始化x和y，z初始化为0到min_height之间的随机数
//     Point(double x, double y, double min_height) : x(x), y(y) {
//         // 使用静态的随机数生成器和分布器，保证每次生成的随机数不同
//         static std::random_device rd;
//         static std::mt19937 gen(rd());
//         std::uniform_real_distribution<> dis(0, min_height);
        
//         // 生成随机的z值
//         z = dis(gen);
//     }
// };

// // 计算两点之间的欧氏距离
// double distance(const Point& p1, const Point& p2) {
//     return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + 
//                      (p1.y - p2.y) * (p1.y - p2.y) + 
//                      (p1.z - p2.z) * (p1.z - p2.z));
// }

// // 检查点集中的所有点是否满足约束条件：两点之间的距离必须大于1.8
// bool satisfies_constraints(const std::vector<Point>& points) {
//     for (size_t i = 0; i < points.size(); ++i) {
//         for (size_t j = i + 1; j < points.size(); ++j) {
//             if (distance(points[i], points[j]) < 1.8) {  // 如果发现有两点之间的距离小于1.8
//                 return false;  // 返回false，不满足约束
//             }
//         }
//     }
//     return true;  // 所有点都满足约束，返回true
// }

// // 模拟退火算法用于优化点集的z值总和
// void optimize_z(std::vector<Point>& points) {
//     const double initial_temp = 100.0;  // 初始温度
//     const double final_temp = 1e-3;  // 最终温度
//     const double cooling_rate = 0.9;  // 降温速率
//     // const double step_size = 0.1;  // z值调整的步长
//     const double initial_step_size = 8;             // TODO 改为锯齿形或者梳状高低生成器
//     const double min_step_size = 0.5;
    
//     std::random_device rd;
//     std::mt19937 gen(rd());

//     double temperature = initial_temp; 
//     double step_size = initial_step_size;
//     std::uniform_real_distribution<> dis(-step_size, step_size);  // 在[-step_size, step_size]范围内生成随机数
//     std::normal_distribution<> dis_check(0, 1);  // 用于判断是否接受新状态
//      // 设置初始温度
//     double current_energy = std::accumulate(points.begin(), points.end(), 0.0, [](double sum, const Point& p) { return sum + p.z; });  // 计算当前能量（z值的总和）
    
//     while (temperature > final_temp) {  // 当温度未降到最终温度时继续迭代
//         for (auto& point : points) {    // 遍历每个点
//             double old_z = point.z;     // 保存当前的z值
//             point.z = point.z + dis(gen);        // 随机调整z值
//             printf(" dis:%f  \n",dis(gen));
//             // std::cout << " new z: " << point.z << std::endl;

//             point.z = std::max(0.0, point.z);  // 保证z值不为负
            
//             if (!satisfies_constraints(points)) {  // 如果不满足约束条件
//                 point.z = old_z;  // 恢复原来的z值
//             } else {
//                 double new_energy = std::accumulate(points.begin(), points.end(), 0.0, [](double sum, const Point& p) { return sum + p.z; });  // 计算新的能量（z值的总和）
//                 double delta_energy = new_energy - current_energy;  // 计算能量变化
//                 // printf(" delta_energy:%f  \n",delta_energy);
//                 printf(" dis_check(gen):%f  \n",dis_check(gen));
//                 if (delta_energy < 0 || std::exp(-delta_energy / temperature) > dis_check(gen)) {
//                     current_energy = new_energy;  // 接受新状态
//                 } else {
//                     point.z = old_z;  // 否则回退到旧状态
//                 }
//             }
//         }
//         temperature *= cooling_rate;  // 按照冷却速率降温
//         step_size = initial_step_size * temperature / initial_temp;
//         step_size += 0.1;
//         // step_size = std::max(initial_step_size * temperature / initial_temp, min_step_size);
//         // std::uniform_real_distribution<> dis(-step_size, step_size);
//         dis.param(std::uniform_real_distribution<>::param_type(-step_size, step_size));
//         // printf("step_size:%f   ,",step_size);
//         // printf("points[1].z:%f\n",points[1].z);
//     }
// } 

// int main() {

//     double base_area = 4*12;     // 盒子的底面积
//     double radius = 0.9;         // 球的半径
//     int num_balls = 300;         // 球的数量
//     double min_height = calculate_minimum_height(base_area, num_balls, radius);     // 计算最小高度

//     std::vector<Point> points = {
//         Point(0, 0, min_height),  // 初始化点集
//         Point(2, 2, min_height),
//         Point(3, 1, min_height),
//         Point(4, 1, min_height),  // 初始化点集
//         Point(2.3, 2, min_height),
//         Point(3, 2, min_height),
//         Point(0, 0, min_height),
//         Point(0, 0.4, min_height),
//         Point(0.4, 0.4, min_height)
//         // 其他点...
//     };

//     optimize_z(points);  // 对点集进行优化

//     for (const auto& point : points) {  // 输出优化结果
//         std::cout << "Point (" << point.x << ", " << point.y << ", " << point.z << ")\n";
//     }

//     return 0;
// }
