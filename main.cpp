#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#include "version.h"
#include "ip_filter.h"



int main()
{

    try
    {
        PoolIP ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            const std::string& s = ranges::front( line | ranges::views::split('\t') );
            ip_pool.push_back( IP(s) );
        }

        ranges::sort(ip_pool, std::greater<IP>());

        print_pool(ip_pool);

        filter(ip_pool, 1);
        filter(ip_pool, 46, 70);
        filter_any(ip_pool, 46);

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
