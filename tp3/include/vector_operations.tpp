#ifndef VECTOR_OPS_TPP
#define VECTOR_OPS_TPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <ostream>
#include <vector>

template<typename T>
void operator+=(std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());
    std::transform(a.cbegin(),a.cend(),b.cbegin(),a.begin(),std::plus<T>());
};

template<typename T>
void operator+=(std::vector<T>& a, const T& b)
{
    std::transform(a.cbegin(),a.cend(),a.begin(),[&b](T a_i){return a_i+b;});
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
void operator/=(std::vector<T>& a, T k)
{
    for(auto it=a.begin();it!=a.end();++it)
        *it/=k;
};

template<typename T>
std::vector<T> operator/(const std::vector<T>& a, T k)
{
    std::vector<T> result(a.size(),k);
    std::transform(a.cbegin(),a.cend(),result.cbegin(),result.begin(),std::divides<T>());

    return result;
};

// Es un bardo imprimirlo asi linea por linea para debug. Dejemos este asi y usemos el especial para el output de la catedra
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    if(!v.empty())
    {
        auto it=v.cbegin();
        os << "[" << *it;
        ++it;
        while(it!=v.cend())
        {
            os << "," << *it;
            ++it;
        };
        os << "]";
    }

    return os;
}

template<typename T>
std::ostream& imprimir_en_linea(std::ostream& os, const std::vector<T>& v)
{
    for(auto it=v.cbegin();it!=v.cend();++it) os << *it << std::endl;

    return os;
}

#endif
