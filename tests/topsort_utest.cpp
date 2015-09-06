#include "topsort.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include <gtest/gtest.h>

typedef std::unordered_map<int, std::set<int>> int_type;
typedef std::unordered_map<std::string, std::set<std::string>> str_type;

TEST(TopsortTest, SimpleRoot) {
    int_type m = {{3,{1}}, {2,{1}}};

    domidimi::Topsort<int_type> t(m);

    std::vector<int> &sorted = t.sort();
    ASSERT_EQ(1, sorted[0]);
}

TEST(TopsortTest, SimpleRoot2) {
    int_type m = {{3,{1}}, {2,{1}}, {4,{2}}, {5,{4}}};

    domidimi::Topsort<int_type> t(std::move(m));

    std::vector<int> &sorted = t.sort();
    ASSERT_EQ(1, sorted[0]);
}

TEST(TopsortTest, SimpleLoop) {
    int_type m = {{3,{1}}, {2,{1}}, {4,{2}}, {1,{2}}};

    domidimi::Topsort<int_type> t(m);
    ASSERT_THROW(t.sort(), std::invalid_argument);
}

TEST(TopsortTest, BigGraph) {
    int_type m;
    std::vector<int> expected;

    for (int i = 1; i < 10000; ++i)
        m.insert({i,{i+1}});

    for (int i = 10000; i > 0; --i)
        expected.push_back(i);

    domidimi::Topsort<int_type> t(move(m));
    std::vector<int> &sorted = t.sort();
    EXPECT_EQ(expected, sorted);
}

TEST(TopsortTest, MoveTopsort) {
    int_type m;
    std::vector<int> expected;

    for (int i = 1; i < 100; ++i)
        m.insert({i,{i+1}});

    for (int i = 100; i > 0; --i)
        expected.push_back(i);

    auto t = domidimi::make_topsort(move(m));
    std::vector<int> &sorted = t->sort();

    EXPECT_EQ(expected, sorted);
}

TEST(TopsortTest, MakeTopsort) {
    int_type m;
    std::vector<int> expected;

    for (int i = 1; i < 100; ++i)
        m.insert({i,{i+1}});

    for (int i = 100; i > 0; --i)
        expected.push_back(i);

    auto t = domidimi::make_topsort<int_type>(m);
    std::vector<int> &sorted = t->sort();

    EXPECT_EQ(expected, sorted);
}

TEST(TopsortTest, SimpleString) {
    str_type m = {{"c",{"a"}}, {"c",{"a"}}, {"d",{"b"}}, {"e",{"d"}}};

    domidimi::Topsort<str_type> t(std::move(m));

    std::vector<std::string> &sorted = t.sort();
    ASSERT_EQ("a", sorted[0]);
}
