#include "BinaryIndexTree.h"

#include <stdbool.h>
#include <stdlib.h>

#include "logging.h"

struct BinaryIndexTree {
	int size;
	int s[0];
};

// Create a binary index tree of array size [size].
struct BinaryIndexTree* createBinaryIndexTree(int size) {
	CHECK(size >= 0);
	struct BinaryIndexTree* tree = (struct BinaryIndexTree*)calloc(
			1, sizeof(struct BinaryIndexTree) + sizeof(int) * (size + 1));
	CHECK(tree);
	return tree;
}

static int getLastBitValue(int index) {
	CHECK(index > 0);
	return index - (index & (index-1));
}

static int getParent(struct BinaryIndexTree* tree, int index) {
	CHECK(index > 0);
	index += getLastBitValue(index);
	if (index > tree->size) {
		return -1;
	}
	return index;
}

static int getPrefix(int index) {
	CHECK(index > 0);
	return index - getLastBitValue(index);
}

// Set the value of one element in the binary index tree.
void binaryIndexTreeSetValue(struct BinaryIndexTree* tree, int index, int value) {
	CHECK(index < tree->size && index >= 0);
	index++;
	int adjust_value = value - tree->s[index];
	while (index != -1) {
		tree->s[index] += adjust_value;
		index = getParent(tree, index);
	}
}

// Get the value of one element in the binary index tree.
int binaryIndexTreeGetValue(struct BinaryIndexTree* tree, int index) {
	CHECK(index < tree->size && index >= 0);
	index++;
	int part_num = getLastBitValue(index);
	part_num >>= 1;
	int sub_index = index - part_num;
	int result = tree->s[index];
	while (part_num > 0) {
		result -= tree->s[sub_index];
		part_num >>= 1;
	}
	return result;
}

// Find the index when the value sum up to [value].
// Return -1 if [value] > total sum.
int binaryIndexTreeFindIndexForSum(struct BinaryIndexTree* tree, int sum) {
	int low = 0;
	int high = tree->size - 1;
	while (low < high) {
		int mid = (low + high) / 2;
		int value = binaryIndexTreeFindSumToIndex(tree, mid);
		if (value == sum) {
			return mid;
		} else if (value < sum) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	if (binaryIndexTreeFindSumToIndex(tree, high) >= sum) {
		return high;
	}
	return -1;
}

// Find the sum of adding array[0:index].
int binaryIndexTreeFindSumToIndex(struct BinaryIndexTree* tree, int index) {
	CHECK(index >= 0 && index < tree->size);
	index++;
	int sum = 0;
	while (index != 0) {
		sum = tree->s[index];
		index = getPrefix(index);
	}
	return sum;
}
