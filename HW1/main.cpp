#include <iostream>
#include <cmath>
#include <vector>

using interval = std::pair<double, double>;
using method = std::tuple<double, int, interval>(double, double, double);

/// Целевая функция
double f(double x) {
    return sqrt(4 * x + 7) - 3 * cos(x);
}

/// Производная функции
double derivativeF(double x) {
    return 3 * sin(x) + 2 / sqrt(4 * x + 7) ;
}

/// Метод бисекции
auto bisection(double a, double b, double epsilon) -> std::tuple<double, int, interval> {
    int steps = 0;

    while (b - a > 2 * epsilon) {
        steps++;
        double c = (a + b) / 2;
        if (f(a) * f(c) <= 0) {
            b = c;
        } else {
            a = c;
        }
    }

    return {a + b / 2, steps, {a, b}};
}

/// Метод Ньютона
auto newton(double a, double b, double epsilon) -> std::tuple<double, int, interval> {
    int steps = 0;
    double x_k_1 = (a + b) / 2;
    double x_k = x_k_1 - f(x_k_1) / derivativeF(x_k_1);

    while (fabs(x_k - x_k_1) >= epsilon) {
        steps++;
        x_k_1 = x_k;
        x_k = x_k_1 - f(x_k_1) / derivativeF(x_k_1);
    }

    return {x_k, steps, {x_k_1, x_k}};
}

/// Модифицированный метод Ньютона
auto modNewton(double a, double b, double epsilon) -> std::tuple<double, int, interval> {
    int steps = 0;
    double x_0 = (a + b) / 2;
    double x_k_1 = x_0;
    double x_k = x_k_1 - f(x_k_1) / derivativeF(x_0);

    while (fabs(x_k - x_k_1) >= epsilon) {
        steps++;
        x_k_1 = x_k;
        x_k = x_k_1 - f(x_k_1) / derivativeF(x_0);
    }

    return {x_k, steps, {x_k_1, x_k}};
}


/// Метод секущих
auto secant(double a, double b, double epsilon) -> std::tuple<double, int, interval> {
    int steps = 0;
    double x_k_2 = (a + b) / 2;
    double x_k_1 = (x_k_2 + b) / 2;
    double x_k = x_k_1 - f(x_k_1) / (f(x_k_1) - f(x_k_2)) * (x_k_1 - x_k_2);

    while (fabs(x_k - x_k_1) >= epsilon) {
        steps++;
        x_k_2 = x_k_1;
        x_k_1 = x_k;
        x_k = x_k_1 - f(x_k_1) / (f(x_k_1) - f(x_k_2)) * (x_k_1 - x_k_2);
    }

    return {x_k, steps, {x_k_1, x_k}};
}

auto getIntervals(double A, double B, int N, double epsilon) -> std::vector<interval> {
    std::vector<interval> intervals;
    double h = (B - A) / N;
    double x_1 = A;
    double x_2 = x_1 + h;
    double y_1 = f(x_1);
    while (x_2 < B || fabs(B - x_2) < epsilon) {
        double y_2 = f(x_2);
        if (y_1 * y_2 <= 0) {
            intervals.emplace_back(x_1, x_2);
        }
        x_1 = x_2;
        x_2 = x_1 + h;
        y_1 = y_2;
    }
    return std::move(intervals);
}

void printer(double a, double b, double epsilon, method function) {
    auto [result, steps, interval] = function(a, b, epsilon);
    std::cout << "[" << a << ", " << b << "]\n";
    std::cout << "Количество шагов: " << steps << std::endl;
    std::cout << "Приближенное решение: " << result << std::endl;
    std::cout << "|x_m - x_(m - 1)| = " << fabs(interval.first - interval.second) << std::endl;
    std::cout << "|f(x_m) - 0| = " << fabs(f(result)) << std::endl;
    std::cout << std::endl;
}
int main() {
    double A, B, epsilon;
    int N;

    std::cout << "Введите A: ";
    std::cin >> A;
    std::cout << "Введите B: ";
    std::cin >> B;

    do {
        std::cout << "Введите N (N >= 2): ";
        std::cin >> N;
    } while (N < 2);

    std::cout << "Введите точность вычислений (ε): ";
    std::cin >> epsilon;


    std::vector<interval> intervals = getIntervals(A, B, N, epsilon);
    std::cout << "Количество отрезков перемены знака: " << intervals.size() << std::endl;
    std::cout << "Найденные отрезки перемены знака:\n";
    for (const auto& interval : intervals) {
        std::cout << "[" << interval.first << ", " << interval.second << "]\n";
    }
    std::cout << std::endl;

    for (const auto& interval : intervals) {
        std::cout << "Метод бисекции:\n";
        printer(interval.first, interval.second, epsilon, bisection);

        std::cout << "Метод Ньютона:\n";
        printer(interval.first, interval.second, epsilon, newton);

        std::cout << "Модифицированный метод Ньютона:\n";
        printer(interval.first, interval.second, epsilon, modNewton);

        std::cout << "Метод секущих:\n";
        printer(interval.first, interval.second, epsilon, secant);
    }

    return 0;
}