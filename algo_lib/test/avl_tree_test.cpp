#include "avl_tree.h"

#include <stdio.h>

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include <gtest/gtest.h>

using namespace algo_lib;

TEST(avl_tree, smoke) {
	AVLTree<int> tree;
	for (int i = 0; i < 10; ++i) {
		tree.insert(i);
		ASSERT_TRUE(tree.checkValid());
		ASSERT_NE(tree.find(i), nullptr) << i;
	}
	for (int i = 0; i < 10; ++i) {
		auto node = tree.findNode(i);
		ASSERT_TRUE(node != nullptr) << i;
		ASSERT_EQ(node->value, i);
	}
	for (int i = 10; i <= 20; ++i) {
		ASSERT_EQ(nullptr, tree.find(i));
	}
	for (int i = 0; i < 10; ++i) {
		for (int j = i; j < 10; ++j) {
			auto begin = tree.findNode(i);
			auto end = tree.findNode(j);
			ASSERT_EQ(tree.findSizeInBetween(begin, end), j - i + 1) << "i = " << i << ", j = " << j;
		}
	}
	for (int i = 9; i >= 0; --i) {
		tree.erase(i);
		ASSERT_TRUE(tree.checkValid());
		ASSERT_EQ(tree.findNode(i), nullptr);
		if (i > 0) {
			ASSERT_TRUE(tree.findNode(i-1) != nullptr);
		}
	}
	ASSERT_TRUE(tree.empty());
}

TEST(avl_tree, random_test) {
	int TEST_COUNT = 100;
	int TEST_NUMS = 100;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	for (int test_count = 0; test_count < TEST_COUNT; ++test_count) {
		std::vector<int> values;
		for (int i = 0; i < TEST_NUMS; ++i) {
			values.push_back(i);
		}
		std::shuffle(values.begin(), values.end(), std::default_random_engine(seed++));

		AVLTree<int> tree;
		for (int i = 0; i < values.size(); ++i) {
			tree.insert(values[i]);
			ASSERT_TRUE(tree.checkValid());
			ASSERT_NE(tree.find(values[i]), nullptr) << i;
		}
		for (int i = 0; i < values.size(); ++i) {
			auto node = tree.findNode(values[i]);
			ASSERT_TRUE(node != nullptr) << i;
			ASSERT_EQ(node->value, values[i]);
		}
		for (int i = values.size(); i <= values.size() + 10; ++i) {
			ASSERT_EQ(nullptr, tree.find(i));
		}
		for (int i = values.size() - 1; i >= 0; --i) {
			tree.erase(values[i]);
			ASSERT_TRUE(tree.checkValid());
			ASSERT_EQ(tree.findNode(values[i]), nullptr);
			if (i > 0) {
				ASSERT_TRUE(tree.findNode(values[i-1]) != nullptr);
			}
		}
		ASSERT_TRUE(tree.empty());
	}
}