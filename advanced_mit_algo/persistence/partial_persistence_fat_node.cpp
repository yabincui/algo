/*
From "Make Data Structures Persistent" 1989
James R. Driscoll
Neil Sarnak
Daniel D. Sleator
Robert E. Tarjan

*/

#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

template <typename T>
class FatField {
private:
    vector<pair<int, T>> values;  // pair of (version, value).

    static int compareVersion(int version, const pair<int, T>& value) {
        return version < value.first;
    }

public:
    void addValue(int version, const T& value) {
        values.push_back(make_pair(version, value));
    }

    // Find value <= version.
    T* findValue(int version) {
        auto it = upper_bound(values.begin(), values.end(), version, compareVersion);
        if (it != values.begin()) {
            --it;
            return &it->second;
        }
        return nullptr;
    }
};

template <typename T>
struct FatNode {
    typedef FatNode<T>* pointer_type;
    FatField<T> value;
    FatField<pointer_type> left;
    FatField<pointer_type> right;
};

// Partial persistent binary search tree.
template <typename T>
class BinarySearchTree {
public:
    BinarySearchTree() : curVersion(0) {}

    // Add a value in the BST, return the new version.
    int addValue(const T& value) {
        FatField<pointer_type>* pointer = &root;
        while (true) {
            pointer_type cur = getPointer(pointer, curVersion);
            if (cur == nullptr) {
                break;
            }
            T* curValue = cur->value.findValue(curVersion);
            assert(curValue != nullptr);
            if (*curValue < value) {
                pointer = &cur->right;
            } else {
                pointer = &cur->left;
            }
        }
        pointer_type p = new FatNode<T>();
        p->value.addValue(++curVersion, value);
        pointer->addValue(curVersion, p);
        return curVersion;
    }

    T* findValue(const T& value, int version) {
        pointer_type cur = getPointer(&root, version);
        while (cur != nullptr) {
            T* curValue = cur->value.findValue(version);
            if (curValue == nullptr) {
                return nullptr;
            }
            if (*curValue == value) {
                return curValue;                
            }
            if (*curValue < value) {
                cur = getPointer(&cur->right, version);
            } else {
                cur = getPointer(&cur->left, version);
            }
        }
        return nullptr;
    }

    // Remove a value in the BST, return the new version.
    int delValue(const T& value) {
        FatField<pointer_type>* pointer = &root;
        while (true) {
            pointer_type cur = getPointer(pointer, curVersion);
            if (cur == nullptr) {
                break;
            }
            T* curValue = cur->value.findValue(curVersion);
            assert(curValue != nullptr);
            if (*curValue == value) {
                pointer_type leftNode = getPointer(&cur->left, curVersion);
                pointer_type rightNode = getPointer(&cur->right, curVersion);
                if (leftNode == nullptr) {
                    pointer->addValue(++curVersion, rightNode);
                    return curVersion;
                }
                if (rightNode == nullptr) {
                    pointer->addValue(++curVersion, leftNode);
                    return curVersion;
                }
                pointer_type newCur = delAndGetLeftestNode(&cur->right);
                newCur->left.addValue(curVersion, leftNode);
                rightNode = getPointer(&cur->right, curVersion);
                newCur->right.addValue(curVersion, rightNode);
                pointer->addValue(curVersion, newCur);
                break;
            }
            if (*curValue < value) {
                pointer = &cur->right;
            } else {
                pointer = &cur->left;
            }
        }
        return curVersion;
    }

    // Update a value in the BST, return the new version.
    int updateValue(const T& value) {
        FatField<pointer_type>* cur = root->findValue(curVersion);
        while (cur != nullptr) {
            T* curValue = cur->value.findValue(curVersion);
            assert(curValue != nullptr);
            if (*curValue == value) {
                cur->value.addValue(++curVersion, value);
                return curVersion;
            }
            if (*curValue < value) {
                cur = getPointer(&cur->right, curVersion);
            } else {
                cur = getPointer(&cur->left, curVersion);
            }
        }
        // Unreachable.
        return curVersion;
    }

private:
    typedef FatNode<T>* pointer_type;

    pointer_type delAndGetLeftestNode(FatField<pointer_type>* pointer) {
        pointer_type cur = getPointer(pointer, curVersion);
        assert(cur != nullptr);
        while (true) {
            pointer_type leftNode = getPointer(&cur->left, curVersion);
            if (leftNode == nullptr) {
                pointer_type rightNode = getPointer(&cur->right, curVersion);
                pointer->addValue(++curVersion, rightNode);
                return cur;
            } else {
                pointer = &cur->left;
                cur = leftNode;
            }
        }
    }

    pointer_type getPointer(FatField<pointer_type>* field, int version) {
        pointer_type* p = field->findValue(version);
        return p == nullptr ? nullptr : *p;
    }

    FatField<pointer_type> root;
    int curVersion;
};



TEST(bst, smoke) {
    BinarySearchTree<int> tree;
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(tree.addValue(i), i + 1);
        for (int version = 0; version <= i; ++version) {
            ASSERT_EQ(tree.findValue(-1, version), nullptr);
            for (int j = 0; j < version; ++j) {
                ASSERT_NE(tree.findValue(j, version), nullptr) << "j = " << j << ", version = " << version;
            }
        }
        for (int j = 0; j <= i; ++j) {
            ASSERT_NE(tree.findValue(j, i + 1), nullptr) << "i = " << i << ", j = " << j;
        }
    }
    int base_version = 10;
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(tree.delValue(i), base_version + i + 1);
        for (int version = base_version; version < base_version + i + 1; ++version) {
            int start_number = version - base_version;
            for (int j = 0; j < start_number; ++j) {
                ASSERT_EQ(tree.findValue(j, version), nullptr);
            }
            for (int j = start_number; j < 10; ++j) {
                ASSERT_NE(tree.findValue(j, version), nullptr);
            }
        }
        ASSERT_EQ(tree.findValue(i, base_version + i + 1), nullptr);
    }

}
