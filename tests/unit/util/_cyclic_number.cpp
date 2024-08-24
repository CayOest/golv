#include <gtest/gtest.h>

#include <golv/games/exception.hpp>
#include <golv/util/cyclic_number.hpp>

using namespace golv;

TEST(cyclic_number, increment)
{
    cyclic_number<size_t, 4> counter;
    ASSERT_EQ(*counter, 0);
    ++counter;
    ASSERT_EQ(*counter, 1);
    ++counter;
    ASSERT_EQ(*counter, 2);
    ++counter;
    ASSERT_EQ(*counter, 3);
    ++counter;
    ASSERT_EQ(*counter, 0);
    auto copy = counter++;
    ASSERT_EQ(*copy, 0);
    ASSERT_EQ(*counter, 1);
}

TEST(cyclic_number, decrement)
{
    cyclic_number<size_t, 4> counter;
    ASSERT_EQ(*counter, 0);
    --counter;
    ASSERT_EQ(*counter, 3);
    --counter;
    ASSERT_EQ(*counter, 2);
    --counter;
    ASSERT_EQ(*counter, 1);
    --counter;
    ASSERT_EQ(*counter, 0);
    auto copy = counter--;
    ASSERT_EQ(*copy, 0);
    ASSERT_EQ(*counter, 3);
}

TEST(cyclic_number, assign)
{
    cyclic_number<size_t, 4> counter;
    counter = 2;
    ASSERT_EQ(*counter, 2);
    ASSERT_THROW((counter = 4), golv::exception);
}