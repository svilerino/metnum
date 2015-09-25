#ifndef VECTOR_OPS_TPP
#define VECTOR_OPS_TPP

#include <iostream>

#include<algorithm>
#include<cassert>
#include<functional>
#include<ostream>
#include<vector>

template<typename T>
void operator+=(std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());
    std::transform(a.cbegin(),a.cend(),b.cbegin(),a.begin(),std::plus<T>());
};

template<typename T>
std::vector<T> operator+(const std::vector<T> a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::vector<T> result(a.size(),0);
    std::transform(a.cbegin(),a.cend(),b.cbegin(),result.begin(),std::plus<T>());

    return result;
};

template<typename T>
void operator-=(std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());
    std::transform(a.begin(),a.end(),b.begin(),a.begin(),std::minus<T>());
};

template<typename T>
std::vector<T> operator-(const std::vector<T> a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::vector<T> result(a.size(),0);
    std::transform(a.cbegin(),a.cend(),b.cbegin(),result.begin(),std::minus<T>());

    return result;
};

template<typename T>
void operator*=(std::vector<T>& a, T k)
{
    for(auto it=a.begin();it!=a.end();++it)
        *it*=k;
};

template<typename T>
std::vector<T> operator*(const std::vector<T>& a, T k)
{
    std::vector<T> result(a.size(),k);
    std::transform(a.cbegin(),a.cend(),result.cbegin(),result.begin(),std::multiplies<T>());

    return result;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "(";
    if(v.size()>0)
    {
        auto it=v.cbegin();
        for(;it!=v.cend()-1;++it)
            os << *it << ", ";
        os << *it;
    };
    os << ")";

    return os;
}

double norma1(std::vector<double>& a, bool asumir_todos_positivos)
{
    double norma = 0.0; 
    size_t size = a.size();
    
    for (int i = 0; i < size; i++)
    {
        norma += (asumir_todos_positivos ? a[i] : std::abs(a[i]));
    }    
    return norma;
};

#endif
