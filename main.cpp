#include "Approximator.h"
#include <iostream>
#include <cmath>

const double eps = 1e-15;

bool close(double a, double b) {
    return std::fabs(a - b) < eps;
}

bool expect(bool cond, const char* msg) {
    if (!cond) { std::cerr << "FAIL: " << msg << "\n"; return false; }
    return true;
}

int main() {
    bool ok = true;

    Approximator a;
    a[0.0] = 1.0;
    a(1.0, 2.0);
    a(2.0, 3.0);
    std::cout << "Таблица разделенных разностей:\n" << a << "\n";
    ok &= expect(close(a(0.0), 1.0), "a(0.0) == 1.0");
    ok &= expect(close(a(1.0), 2.0), "a(1.0) == 2.0");
    ok &= expect(close(a(2.0), 3.0), "a(2.0) == 3.0");
    
    Approximator par;
    par(0.0, 0.0);
    par(1.0, 1.0);
    par(2.0, 4.0);
    
    ok &= expect(close(par(3.0), 9.0), "par(3.0) == 9.0 (экстраполяция)");
    ok &= expect(close(par(1.5), 2.25), "par(1.5) == 2.25 (интерполяция)");
    
    Approximator b = par;
    ok &= expect(b == par, "b == par (после копирования)");
    ok &= expect(fabs(b(1.5) - par(1.5)) < eps, "b(1.5) == par(1.5)");
    
    Approximator c;
    c = par;
    ok &= expect(c == par, "b == par (после присваивания)");
    
    Approximator d;
    d(0.0, 0.0);
    d(1.0, 1.0);
    d(2.0, 4.0);
    d.removePoint(1);
    ok &= expect(close(d(0.0), 0.0), "d(0.0) == 0.0 (после удаления)");
    ok &= expect(close(d(2.0), 4.), "a(0.0) == 2.0 (интерполяция");

    Approximator empty;
    bool thrown = false;
    try {
        empty(0.0);
    } catch (const Approximator::Error&) {
        thrown = true; }
    ok &= expect(thrown, "empty должен бросить Approximator::Error");

    if (!ok) {
        std::cerr << "Где-то ошибка!\n"; return 1;
    }
    std::cout << "Победа!\n";
    return 0;

}
