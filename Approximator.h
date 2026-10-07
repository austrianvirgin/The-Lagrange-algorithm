#ifndef APPROXIMATOR_H
#define APPROXIMATOR_H

#include <iostream>

class Approximator {
private:
    double *xs, *ys;   /* массив узлов и их значений */
    double **dd;   /* таблица разделенных разностей */
    unsigned int n;   /* количество точек */
    
    int findIndex(double x) const;   /* поиск узла, выдаст -1, если индекс не найден */
    void recompute();   /* пересчет разделенных разностей */
    void freeMemory();    /* очищение памяти */
    
public:
    Approximator();   /* конструктор по умолчанию */
    Approximator(const Approximator& other);   /* конструктор копирования */
    ~Approximator();   /* деструктор */
    
    const Approximator& operator=(const Approximator& other);   /* присваивание */
    bool operator==(const Approximator& other) const;
    bool operator!=(const Approximator& other) const;
    
    double& operator[] (double);   /* approximator[x] = y */
    void operator()(double x, double y);   /* approximator(x, y) */
    double operator()(double x) const;   /* вычисление значения */
    void removePoint(unsigned int idx);   /* удаление точки */
    friend std::ostream& operator<<(std::ostream& os, const Approximator& a);
    
    class Error {   /* отладка ошибок */
    private: const char* msg;
    public: Error(const char* m) : msg(m) {}   /* конструктор */
    const char *what() const { return msg; }
    };
    
};

#endif
