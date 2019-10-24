#ifndef IP_FILTER_H_INCLUDED
#define IP_FILTER_H_INCLUDED

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <range/v3/all.hpp>

using namespace ranges;
using stringList = std::vector<std::string>;

struct IP
{
    std::string ip_str;

    IP(const std::string &ip_str);
    IP(const int *p_bytes, size_t bytes_count);

    void print() const;

    template<class T>
    void print(T& stream) const {
        stream << ip_str << std::endl;
    }

    bool contains(int val) const;

    operator int() const;
    int operator[](size_t i) const;

private:

    uint32_t ip = 0;
    std::vector<unsigned char> bytes; //192.168.0.1 => bytes[0] = 192, bytes[1] = 168, ...
};

using ip_pool_t = std::vector<IP>;
using rangeIP = std::vector<ip_pool_t::const_iterator>;


template<class T>
void print(const rangeIP &range, T& stream)
{
    if (range.size() != 2)
        return;

    for(auto iter_ip = range.front(); iter_ip != range.back(); ++iter_ip)
        iter_ip->print<T>(stream);
}

inline void print(const rangeIP &range)
{
    using namespace std;
    print<ostream>(range, cout);
}


inline void filter_any(const ip_pool_t &ip_pool, int ip_part)
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
rangeIP filter(const ip_pool_t &ip_pool, Args... ip_parts)
{
    const int count_args = sizeof...(ip_parts);
    int arr[count_args] = {ip_parts ... };

    rangeIP range;

    auto iter_from = ip_pool.cbegin();
    auto iter_to = ip_pool.cend();

    for (int i = 0; i < count_args; ++i)
    {
        iter_from = std::lower_bound( iter_from, iter_to, arr[i], [&](const IP &ip, int val) { return ip[i] > val;});

        if (iter_from == iter_to)
            return range;

        iter_to = std::upper_bound( iter_from, iter_to, arr[i], [&](int val, const IP &ip) { return ip[i] < val;});
    }

    range.push_back(iter_from);
    range.push_back(iter_to);

    return range;
}


#endif // IP_FILTER_H_INCLUDED
