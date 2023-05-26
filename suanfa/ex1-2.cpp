#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <ctime>

using namespace std;

// 定义二维点结构体
struct Point {
    double x, y;
    Point(double x, double y): x(x), y(y) {}
};

// 计算两个点之间的距离
double distance(const Point& p1, const Point& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

// 使用蛮力法求解最近点对
struct ClosestPairResult {
    double distance;
    Point p1, p2;
    ClosestPairResult() : distance(-1), p1(0, 0), p2(0, 0) {}
    ClosestPairResult(double d, const Point& p_1, const Point& p_2) : distance(d), p1(p_1), p2(p_2) {}
};

ClosestPairResult brute_force(const vector<Point>& points) {
    double min_distance = numeric_limits<double>::max();
    ClosestPairResult res;
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            double dist = distance(points[i], points[j]);
            if (dist < min_distance) {
                min_distance = dist;
                res.p1 = points[i];
                res.p2 = points[j];
            }
        }
    }
    res.distance = min_distance;
    return res;
}

// 最近点对分治算法
ClosestPairResult closest_pair(const vector<Point>& points) {
    if (points.size() <= 3) {
        return brute_force(points);
    }
    // 将点按照横坐标排序
    vector<Point> sorted_points(points);
    sort(sorted_points.begin(), sorted_points.end(), [](const Point& p1, const Point& p2) {
        return p1.x < p2.x;
    });
    // 分别在左右两个子集上递归执行分治算法
    size_t mid = points.size() / 2;
    auto left_pair = closest_pair(vector<Point>(sorted_points.begin(), sorted_points.begin() + mid));
    auto right_pair = closest_pair(vector<Point>(sorted_points.begin() + mid, sorted_points.end()));
    ClosestPairResult result;
    if (left_pair.distance < right_pair.distance) {
        result = left_pair;
    } else {
        result = right_pair;
    }
    // 找出中间区域内可能存在更小距离的点对
    vector<Point> middle_points;
    for (size_t i = 0; i < points.size(); ++i) {
        if (abs(points[i].x - sorted_points[mid].x) < result.distance) {
            middle_points.push_back(points[i]);
        }
    }
    sort(middle_points.begin(), middle_points.end(), [](const Point& p1, const Point& p2) {
        return p1.y < p2.y;
    });
    for (size_t i = 0; i < middle_points.size(); ++i) {
        for (size_t j = i + 1; j < middle_points.size() && middle_points[j].y - middle_points[i].y < result.distance; ++j) {
            double dist = distance(middle_points[i], middle_points[j]);
            if (dist < result.distance) {
                result.distance = dist;
                result.p1 = middle_points[i];
                result.p2 = middle_points[j];
        }
    }
}
    return result;
}

int main() {

    srand(time(NULL));
    vector<Point> points;
    for (int i = 0; i < 500; ++i) {
        double x = rand() / (double)RAND_MAX;
        double y = rand() / (double)RAND_MAX;
        points.emplace_back(x, y);
    }
    // 使用蛮力法求解最近点对
    auto bf_result = brute_force(points);
    cout << "暴力: distance = " << bf_result.distance << ", points = (" << bf_result.p1.x << "," << bf_result.p1.y << "), (" << bf_result.p2.x << "," << bf_result.p2.y << ")" << endl;
    // 使用分治算法求解最近点对
    auto dc_result = closest_pair(points);
    cout << "分治: distance = " << dc_result.distance << ", points = (" << dc_result.p1.x << "," << dc_result.p1.y << "), (" << dc_result.p2.x << "," << dc_result.p2.y << ")" << endl;
    return 0;
}

