#include "skiplist.h"

#include <stdio.h>

#include <algorithm>
#include <chrono>
#include <random>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

using namespace algo_lib;

TEST(skiplist, smoke) {
    SkipList<int, int> skiplist(20);

    for (int i = 0; i < 10; ++i) {
        skiplist.insert(i, i);
        //printf("After insert %d:\n%s\n\n", i, skiplist.toString().c_str());
    }
    for (int i = 0; i < 10; ++i) {
        int* value = skiplist.find(i);
        ASSERT_TRUE(value != nullptr) << i;
        ASSERT_EQ(*value, i);
    }
    for (int i = 3; i < 6; ++i) {
        skiplist.erase(i);
        int* value = skiplist.find(i);
        ASSERT_TRUE(value == nullptr);
    }
    for (int i = 0; i < 10; ++i) {
        if (i < 3 || i >= 6) {
            int* value = skiplist.find(i);
            ASSERT_TRUE(value != nullptr);
            ASSERT_EQ(*value, i);
        }
    }
}

TEST(skiplist, random_test) {
    std::unordered_map<int, int> map;
    SkipList<int, int> skiplist(1000);

    for (int i = 0; i < 1000; ++i) {
        int op = random() % 10;
        if (op < 4) {
            int key = random() % 100;
            int value = random();
            map[key] = value;
            skiplist.insert(key, value);
        } else if (op < 8) {
            while (true) {
                int key = random() % 100;
                auto it = map.find(key);
                if (it == map.end()) {
                    ASSERT_TRUE(skiplist.find(key) == nullptr);
                } else {
                    int* value = skiplist.find(key);
                    ASSERT_TRUE(value != nullptr);
                    ASSERT_EQ(*value, it->second);
                    break;
                }
            }
        } else {
            while (true) {
                int key = random() % 100;
                auto it = map.find(key);
                if (it == map.end()) {
                    continue;
                }
                map.erase(it);
                skiplist.erase(key);
                break;
            }
        }
    }

}
