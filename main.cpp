#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#include "version.h"
#include "ip_filter.h"

using ranges::views::split;

int main()
{


    try
    {
        PoolIP ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            const StringList& v = line | split('\t') | to<StringList>;

            ip_pool.push_back( IP(v.at(0)) );
        }

        ranges::sort(ip_pool, std::greater<IP>());

        for(const auto &ip: ip_pool)
            ip.print();

        print(filter(ip_pool, 1));
        print(filter(ip_pool, 46, 70));
        filter_any(ip_pool, 46);

        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
