#define BOOST_TEST_MODULE app_tests

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <fstream>

#include "../version.h"
#include "../ip_filter.h"

using boost::test_tools::output_test_stream;

ip_pool_t ip_pool;

void fill_ip_pool()
{
    if (!ip_pool.empty())
        return;

    std::ifstream ifs ("ip_filter.tst");

    for(std::string line; std::getline(ifs, line);)
    {
        //std::cout << line << std::endl;
        ip_pool.push_back(IP(line));
    }

}


BOOST_AUTO_TEST_SUITE(app_tests)

BOOST_AUTO_TEST_CASE(test_version)
{
    BOOST_CHECK(PROJECT_VERSION_PATCH > 0);
}

BOOST_AUTO_TEST_CASE(test_filter_first_byte)
{
    output_test_stream output( "ip_filter_first_byte.tst", true );
    fill_ip_pool();

    int first_byte = 46;

    rangeIP range = filter(ip_pool, first_byte);
    BOOST_REQUIRE_EQUAL(range.size(), 2);

    print<output_test_stream>(range, output);

    BOOST_REQUIRE (!output.is_empty(false));
    BOOST_CHECK( output.match_pattern() );
}

BOOST_AUTO_TEST_CASE(test_filter_two_bytes)
{
    output_test_stream output( "ip_filter_two_bytes.tst", true );
    fill_ip_pool();

    int first_byte = 46;
    int second_byte = 70;

    rangeIP range = filter(ip_pool, first_byte, second_byte);
    BOOST_REQUIRE_EQUAL(range.size(), 2);

    //output << __PRETTY_FUNCTION__<< std::endl;
    print<output_test_stream>(range, output);

    BOOST_REQUIRE (!output.is_empty(false));
    BOOST_CHECK( output.match_pattern() );
}


BOOST_AUTO_TEST_SUITE_END()
