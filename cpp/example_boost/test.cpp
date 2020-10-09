# define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test1)
{
    const int x = 1;
    BOOST_CHECK(x == 1);
    BOOST_CHECK(x == 2);
}

//g++ ./test.cpp