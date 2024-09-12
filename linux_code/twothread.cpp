#include "matplotlibcpp.h"
#include <cmath>
#include <thread>

namespace plt = matplotlibcpp;

void show_plot_1(std::vector<double> x, std::vector<double> y, std::vector<double> z, std::map<std::string, std::string> keywords) {
    plt::plot3(x, y, z, keywords);
    plt::xlabel("x label");
    plt::ylabel("y label");
    plt::set_zlabel("z label");
    plt::legend();
    plt::show();
}

void show_plot_2(std::vector<double> x, std::vector<double> y, std::vector<double> z, std::map<std::string, std::string> keywords) {
    plt::plot3(x, y, z, keywords);
    plt::xlabel("x label");
    plt::ylabel("T label");
    plt::show();
}

int main() {
    std::vector<double> x, y, z;
    double theta, r;
    double z_inc = 4.0/99.0; 
    double theta_inc = (8.0 * M_PI)/99.0;
    
    for (double i = 0; i < 100; i += 1) {
        theta = -4.0 * M_PI + theta_inc*i;
        z.push_back(-2.0 + z_inc*i);
        r = z[i]*z[i] + 1;
        x.push_back(r * sin(theta));
        y.push_back(r * cos(theta));
    }

    std::map<std::string, std::string> keywords;
    keywords.insert(std::pair<std::string, std::string>("label", "parametric curve"));

    // 启动两个线程分别显示不同的plot
    std::thread show1(show_plot_1, x, y, z, keywords);
    // std::thread show2(show_plot_2, x, y, z, keywords);

    // 等待线程结束
    show1.join();
    // show2.join();
    printf("nice\n");

    return 0;
}
