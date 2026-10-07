#include <cmath>
#include "Approximator.h"

const double eps = 1e-15;

/* поиск узла, выдаст -1, если индекс не найден */
int Approximator::findIndex(double x) const {
    for (unsigned int i = 0; i < n; ++i)
        if (fabs(xs[i] - x) < eps) return i;
    return -1;
}

/* очищение памяти */
void Approximator::freeMemory(){
    if (xs) { delete[] xs; xs = 0; }
    if (ys) { delete[] ys; ys = 0; }

    if (dd) {
        for (unsigned int i = 0; i < n; ++i) delete[] dd[i];
        delete[] dd; dd = 0;
        }
    }

/* пересчет разделенных разностей */
void Approximator::recompute() {
    /* освобождаем старую таблицу */
    if (dd) {
        for (unsigned int i = 0; i < n; ++i) delete[] dd[i];
        delete[] dd; dd = 0; }

    /* если точек нет, то и таблица не нужна */
    if (n == 0) { dd = 0; return; }

    /* составляем новую таблицу, заполняем ее нулями */
    dd = new double*[n];
    for (unsigned int i = 0; i < n; ++i)
        dd[i] = new double[n];
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < n; ++j)
            dd[i][j] = 0.0;
    /* нулевой столбец - это ys */
    for (unsigned int i = 0; i < n; ++i)
        dd[i][0] = ys[i];
    /* все остальное считаем по формуле */
    for (unsigned int j = 1; j < n; ++j)
        for (unsigned int i = 0; i + j < n; ++i)
            dd[i][j] = (dd[i + 1][j - 1] - dd[i][j - 1]) / (xs[i + j] - xs[i]);
}

/* конструктор по умолчанию */
Approximator::Approximator() : xs(0), ys(0), dd(0), n(0) {}

/* конструктор копирования */
Approximator::Approximator(const Approximator& other) :
    xs(0), ys(0), dd(0), n(0) {
    n = other.n;
    if (n == 0) return;
    xs = new double[n];
    ys = new double[n];
    for (int unsigned i = 0; i < n; ++i) {
        xs[i] = other.xs[i];
        ys[i] = other.ys[i];
    }
    recompute();
}

/* деструктор */
Approximator::~Approximator() { freeMemory(); }

/* переопределение присваивания */
const Approximator& Approximator::operator=(const Approximator& other) {
    if (this == &other) return *this;
    freeMemory();
    n = other.n;
    if (n == 0) return *this;
    xs = new double[n];
    ys = new double[n];
    for (unsigned int i = 0; i < n; ++i) {
        xs[i] = other.xs[i];
        ys[i] = other.ys[i];
    }
    recompute();
    return *this;
}

/* сравнение */
bool Approximator::operator==(const Approximator& other) const {
    if (n != other.n) return false;
    for (unsigned int i = 0; i < n; ++i) {
        if (fabs(xs[i] - other.xs[i]) > eps) return false;
        if (fabs(ys[i] - other.ys[i]) > eps) return false;
    }
    return true;
}

/* несравнение */
bool Approximator::operator!=(const Approximator& other) const{
    return !(*this == other);
}

/* добавление approximator[x] = y */
double& Approximator::operator[] (double x) {
    int idx = findIndex(x);
    /* если точки нет - добавляем */
    if (idx < 0) {
        double *nx = new double[n + 1];
        double *ny = new double[n + 1];
        for (unsigned int i = 0; i < n; ++i) { nx[i] = xs[i]; ny[i] = ys[i]; }
        nx[n] = x;
        ny[n] = 0.0;
        delete[] xs; delete[] ys; xs = nx; ys = ny; idx = n; ++n;
    }
    if (dd) {
        for (unsigned int i = 0; i < n; ++i) delete[] dd[i];
        delete[] dd; dd = 0; }

    recompute();
    return ys[idx];
}

/* добавление approximator(x, y) */
void Approximator::operator()(double x, double y) {
    int idx = findIndex(x);
    /* если точки нет - добавляем */
    if (idx < 0) {
        double *nx = new double[n + 1];
        double *ny = new double[n + 1];
        for (unsigned int i = 0; i < n; ++i) { nx[i] = xs[i]; ny[i] = ys[i]; }
        nx[n] = x;
        ny[n] = y;
        delete[] xs; delete[] ys; xs = nx; ys = ny; idx = n; ++n;
    }
    else { ys[idx] = y; }

    recompute();
}

/* удаление точки */
void Approximator::removePoint(unsigned int idx) {
    if (idx >= n) throw Error("Approximation Error: index outside the field");
    double *nx = new double[n - 1];
    double *ny = new double[n - 1];
    int k = 0;
    for (unsigned int i = 0; i < n; ++i) {
        if (i == idx) continue;
        nx[k] = xs[i]; ny[k] = ys[i]; ++k; }
    delete[] xs; delete[] ys;
    xs = nx; ys = ny;
    if (dd) {
        for (unsigned int i = 0; i < n; ++i) delete[] dd[i];
        delete[] dd; dd = 0; }
    --n; recompute();
}

/* вычисление значения, формула Ньютона, схема Горнера */
double Approximator::operator()(double x) const {
    if (n == 0) throw Error("Approximation Error: no data to interpolate");
    double res = dd[0][0];
    double prod = 1.0;
    for (unsigned int i = 1; i < n; ++i) {
        prod *= (x - xs[i - 1]);
        res += dd[0][i] * prod;
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, const Approximator& a) {
    for (unsigned int i = 0; i < a.n; ++i) {
        os << "x[" << i << "] = " << a.xs[i] << " : ";
        for (unsigned int j = 0; j + i < a.n; ++j)
            os << a.dd[i][j] << " ";
        os << "\n";
    }
    return os;
}
