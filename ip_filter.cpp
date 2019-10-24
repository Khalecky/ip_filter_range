#include "ip_filter.h"


IP::IP(const std::string &_ip_str) : ip_str(_ip_str)
{
    stringList str_list = ip_str | views::split('.');

    for (const auto&str: str_list)
    {
        unsigned char byte = std::stoi(str);
        ip = ip << 8;
        ip = ip | byte;
        bytes.push_back(byte);
    }
}

IP::IP(const int *p_bytes, size_t bytes_count)
{
    for (size_t i = 0; i < 4; ++i)
    {
        bytes.push_back( static_cast<unsigned char>(bytes_count > i ? (p_bytes[i]) : 0) );
        //
        ip = ip << 8;
        ip = ip | bytes.back();
        //
        if (i > 0)
            ip_str.append(".");
        ip_str.append(std::to_string(bytes.back()) );
    }
}

bool IP::contains(int val) const
{
    return find(bytes, val) != end(bytes);
}


void IP::print() const
{
    using namespace std;
    print<ostream>(cout);
}


IP::operator int() const
{
    return ip;
}

int IP::operator[](size_t i) const
{
    //TODO check i < 4
    return bytes[i];
}
