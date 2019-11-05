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
    std::vector<unsigned char> bytes; //192.168.0.1 => bytes[0] = 192, bytes[1] = 168, ...

    explicit IP(const std::string &ip_str)
    {
        const StringList& bytes_str = ip_str | split('.') | to<StringList>;
        for_each (bytes_str, [&](const auto& byte_str) {
            bytes.push_back(static_cast<unsigned char>(std::stoi(byte_str)));
        });
    }

    void print() const
    {
        size_t i = 0;
        for_each(bytes,[&](auto byte){ std::cout << (i != 0 ? ".": "") << static_cast<int>(byte); ++i; });
        std::cout << std::endl;
    }

    bool operator>(const IP &r) const { return bytes > r.bytes; }
};

using PoolIP = std::vector<IP>;

inline void print_pool(const PoolIP &ip_pool)
{
    for_each( ip_pool, [](const IP& ip){ ip.print();}  );
}


inline void filter_any(const PoolIP &ip_pool, int ip_part)
{
    auto grep = views::filter([&](const IP& ip) { return find(ip.bytes, ip_part) != ip.bytes.end(); });
    print_pool(ip_pool | grep);
}


template<typename ...Args>
void filter(const PoolIP &ip_pool, Args... ip_parts)
{
    const int count_args = sizeof...(ip_parts);
    int arr[count_args] = {ip_parts...};

    PoolIP pool;

    for (int i = 0; i < count_args; ++i)
    {
        auto grep = views::filter([&](const IP& ip) { return ip.bytes[i] == arr[i]; });
        if (i == 0)
            pool = ip_pool | grep;
        else
            pool = pool | grep;
    }

    print_pool(pool);
}

#endif // IP_FILTER_H_INCLUDED
