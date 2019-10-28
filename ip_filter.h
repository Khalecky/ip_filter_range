#ifndef IP_FILTER_H_INCLUDED
#define IP_FILTER_H_INCLUDED

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
//#include <algorithm>

#include <range/v3/all.hpp>

using namespace ranges;
using views::split;


using StringList = std::vector<std::string>;

struct IP
{
    std::string ip_str;
    std::vector<unsigned char> bytes; //192.168.0.1 => bytes[0] = 192, bytes[1] = 168, ...

    explicit IP(const std::string &_ip_str) : ip_str(_ip_str)
    {
        for_each(ip_str | split('.'), [&](const std::string& byte_str) {
            bytes.push_back(static_cast<unsigned char>(std::stoi(byte_str)));
        });
    }

    template<class T>
    void print(T& stream) const {
        stream << ip_str << std::endl;
    }

    inline void print() const { print<std::ostream>(std::cout);}
    inline bool operator>(const IP &r) const { return bytes > r.bytes;}
    //inline bool operator<(const IP &r) const { return ! ( (*this) > r);}
    inline bool contains(int val) const { return std::find(bytes.cbegin(), bytes.cend(), val) != bytes.cend();}
};

using PoolIP = std::vector<IP>;
using RangeIP = std::vector<PoolIP::const_iterator>;


template<class T>
void print(const RangeIP &range, T& stream)
{
    if (range.size() != 2)
        return;

    for(auto iter_ip = range.front(); iter_ip != range.back(); ++iter_ip)
        iter_ip->print<T>(stream);
}

inline void print(const RangeIP &range)
{
    print<std::ostream>(range, std::cout);
}


inline void filter_any(const PoolIP &ip_pool, int ip_part)
{
    for_each(ip_pool, [=](const IP& ip) {
            if(ip.contains(ip_part))
                ip.print();
    });
}


/*
    return existing range of IP in vector<IP>
    cases:
    1) empty range - nothing found
    2) range with two iterators: iter_from, iter_to
*/
template<typename ...Args>
RangeIP filter(const PoolIP &ip_pool, Args... ip_parts)
{
    const int count_args = sizeof...(ip_parts);
    int arr[count_args] = {ip_parts...};

    RangeIP range;

    auto iter_from = ip_pool.cbegin();
    auto iter_to = ip_pool.cend();

    for (int i = 0; i < count_args; ++i)
    {
        iter_from = std::lower_bound( iter_from, iter_to, arr[i], [&](const IP &ip, int val) { return ip.bytes[i] > val;});

        if (iter_from == iter_to)
            return range;

        iter_to = std::upper_bound( iter_from, iter_to, arr[i], [&](int val, const IP &ip) { return ip.bytes[i] < val;});
    }

    range.push_back(iter_from);
    range.push_back(iter_to);

    return range;
}

#endif // IP_FILTER_H_INCLUDED
