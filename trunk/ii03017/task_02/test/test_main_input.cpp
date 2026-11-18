#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

// Include the implementation in this translation unit so its static input
// helpers can be tested without making them part of the public API.
#define OTIS_TESTING
#include "../src/main.cpp"
#undef OTIS_TESTING

namespace {
class CinRedirect {
public:
    explicit CinRedirect(const std::string& input)
        : input_(input), old_(std::cin.rdbuf(input_.rdbuf())), oldState_(std::cin.rdstate())
    {
        std::cin.clear();
    }
    ~CinRedirect() {
        std::cin.rdbuf(old_);
        std::cin.clear(oldState_);
    }
private:
    std::istringstream input_;
    std::streambuf* old_;
    std::ios::iostate oldState_;
};

class CoutRedirect {
public:
    CoutRedirect() : old_(std::cout.rdbuf(output_.rdbuf())) {}
    ~CoutRedirect() { std::cout.rdbuf(old_); }
private:
    std::ostringstream output_;
    std::streambuf* old_;
};
}

TEST(ReadDoubleTest, ReadsValidFiniteNumber)
{
    CinRedirect input("3.125\n");
    CoutRedirect output;
    EXPECT_DOUBLE_EQ(readDouble("value: "), 3.125);
}

TEST(ReadDoubleTest, RetriesAfterInvalidText)
{
    CinRedirect input("abc\n2.5\n");
    CoutRedirect output;
    EXPECT_DOUBLE_EQ(readDouble("value: "), 2.5);
}

TEST(ReadDoubleTest, RetriesAfterNonFiniteInput)
{
    CinRedirect input("1e9999\n4.25\n");
    CoutRedirect output;
    EXPECT_DOUBLE_EQ(readDouble("value: "), 4.25);
}

TEST(ReadIntTest, ReadsValueAtMinimumBoundary)
{
    CinRedirect input("1\n");
    CoutRedirect output;
    EXPECT_EQ(readInt("value: ", 1), 1);
}

TEST(ReadIntTest, RejectsValueBelowMinimumAndRetries)
{
    CinRedirect input("0\n5\n");
    CoutRedirect output;
    EXPECT_EQ(readInt("value: ", 1), 5);
}

TEST(ReadIntTest, RetriesAfterInvalidText)
{
    CinRedirect input("oops\n7\n");
    CoutRedirect output;
    EXPECT_EQ(readInt("value: ", 1), 7);
}
