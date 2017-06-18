#ifndef _DS_BINARY_INDEX_TREE_H
#define _DS_BINARY_INDEX_TREE_H


struct BinaryIndexTree;

// Create a binary index tree of array size [size].
struct BinaryIndexTree* createBinaryIndexTree(int size);

// Set the value of one element in the binary index tree.
// index is [0 - tree->size-1].
void binaryIndexTreeSetValue(struct BinaryIndexTree* tree, int index, int value);

// Get the value of one element in the binary index tree.
// index is [0 - tree->size-1].
int binaryIndexTreeGetValue(struct BinaryIndexTree* tree, int index);

// Find the index when the value sum up to [value].
// Return -1 if [value] > total sum, otherwise return [0 - tree->size-1].
int binaryIndexTreeFindIndexForSum(struct BinaryIndexTree* tree, int sum);

// Find the sum of adding array[0:index].
// index is [0 - tree->size-1].
int binaryIndexTreeFindSumToIndex(struct BinaryIndexTree* tree, int index);

#endif _DS_BINARY_INDEX_TREE_H