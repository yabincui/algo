#ifndef _AVL_TREE_H
#define _AVL_TREE_H

#include <algorithm>
#include <functional>
#include <unordered_set>

namespace algo_lib {

template <typename T>
class AVLTree {
private:
	struct AVLNode {
		T value;
		AVLNode* left;
		AVLNode* right;
		AVLNode* parent;
		int height;
		int size;
	};
	
public:

	typedef AVLNode node_type;

	AVLTree() {
		compareFunction_ = [](const T& t1, const T& t2) {
			if (t1 < t2) return -1;
			if (t2 < t1) return 1;
			return 0;
		};
		root_ = NULL;
	}

	AVLTree(const std::function<int(const T&, const T&)>& compareFunction) {
		compareFunction_ = compareFunction;
		root_ = NULL;
	}
	
	~AVLTree() {
		eraseTree(root_);
	}
	
	bool empty() const {
		return root_ == NULL;
	}

	int size() const {
		return getSize(root_);
	}

	void insert(const T& value) {
		//printf("insert %d\n", value);
		AVLNode* parent = NULL;
		AVLNode** cur = &root_;
		while (*cur != NULL) {
			parent = *cur;
			int ret = compareFunction_(value, (*cur)->value);
			if (ret <= 0) {
				cur = &(*cur)->left;
			} else {
				cur = &(*cur)->right;
			}
		}
		*cur = newNode(value, parent);
		makeBalance(*cur);
		//checkValid();
	}
	
	const AVLNode* getRoot() const {
		return root_;
	}
	
	AVLNode* findNode(const T& value) {
		//printf("find %d\n", value);
		AVLNode* cur = root_;
		while (cur) {
			int ret = compareFunction_(value, cur->value);
			if (ret == 0) {
				return cur;
			} else if (ret < 0) {
				cur = cur->left;
			} else {
				cur = cur->right;
			}
		}
		return cur;
	}

	// Find lowest node >= value.
	AVLNode* lowerBoundNode(const T& value) {
		AVLNode* result = NULL;
		AVLNode* cur = root_;
		while (cur) {
			int ret = compareFunction_(value, cur->value);
			if (ret <= 0) {
				result = cur;
				cur = cur->left;
			} else {
				cur = cur->right;
			}
		}
		return result;
	}

	// Find lowest node > value.
	AVLNode* upperBoundNode(const T& value) {
		AVLNode* result = NULL;
		AVLNode* cur = root_;
		while (cur) {
			int ret = compareFunction_(value, cur->value);
			if (ret < 0) {
				result = cur;
				cur = cur->left;
			} else {
				cur = cur->right;
			}
		}
		return result;
	}
	
	T* find(const T& value) {
		AVLNode* node = findNode(value);
		if (node) {
			return &node->value;
		}
		return NULL;
	}
	
	void erase(const T& value) {
		AVLNode* node = findNode(value);
		if (node) {
			erase(node);
		}
	}
	
	void erase(AVLNode* node) {
		//printf("erase %d\n", node->value);
		if (node->left == NULL)	{
			if (node->right == NULL) {
				AVLNode* parent = node->parent;
				replaceChild(parent, node, NULL);
				delete node;
				makeBalance(parent);
			} else {
				AVLNode* leafNode = findMin(node->right);
				node->value = leafNode->value;
				erase(leafNode);
			}
		} else {
			AVLNode* leafNode = findMax(node->left);
			node->value = leafNode->value;
			erase(leafNode);
		}
		//checkValid();
	}

	int findSizeInBetween(AVLNode* begin, AVLNode* end) {
		//printf("begin = %d, end = %d\n", begin->value, end->value);
		if (begin == NULL) {
			return 0;
		}
		if (end == NULL) {
			end = findMax(root_);
		}
		if (compareFunction_(begin->value, end->value) > 0) {
			return 0;
		}
		std::unordered_set<AVLNode*> set;
		for (AVLNode* i = begin; i; i = i->parent) {
			set.insert(i);
		}
		AVLNode* root = NULL;
		for (AVLNode* i = end; i; i = i->parent) {
			if (set.find(i) != set.end()) {
				root = i;
				break;
			}
		}
		if (root == NULL) {
			return 0;
		}
		int result = 1;
		AVLNode* prev = begin->left;
		for (AVLNode* i = begin; i != root; i = i->parent) {
			if (i->left == prev) {
				//printf("at node %d, add %d\n", i->value, 1 + getSize(i->right));
				result += 1 + getSize(i->right);	
			}
			prev = i;
		}
		prev = end->right;
		for (AVLNode* i = end; i != root; i = i->parent) {
			if (i->right == prev) {
				//printf("2, at node %d, add %d\n", i->value, 1 + getSize(i->left));
				result += 1 + getSize(i->left);
			}
			prev = i;
		}
		return result;
	}

	bool checkValid() {
		bool ret = true;
		if (root_) {
			AVLNode* node = findMin(root_);
			bool ret = checkValid(root_, node);
			if (!ret) {
				dumpTree([](const int& value) {
					printf("%d", value);
				});
				abort();
			}
		}
		return ret;
	}
	
	void dumpTree(const std::function<void(const T&)>& dumpValue) {
		dumpTree(root_, dumpValue, 0);	
	}

private:

	AVLNode* newNode(const T& value, AVLNode* parent) {
		AVLNode* node = new AVLNode;
		node->value = value;
		node->height = 0;
		node->size = 1;
		node->left = node->right = NULL;
		node->parent = parent;
		return node;
	}
	
	void makeBalance(AVLNode* cur) {
		//printf("makeBalance\n");
		AVLNode* prev = NULL;
		while (cur != NULL) {
			//printf("before move, cur = %d\n", cur->value);
			fixHeight(cur);
			fixSize(cur);
			int leftHeight = getHeight(cur->left);	
			int rightHeight = getHeight(cur->right);
			if (leftHeight - rightHeight == 2) {
				int leftLeftHeight = getHeight(cur->left->left);
				int leftRightHeight = getHeight(cur->left->right);
				if (leftLeftHeight >= leftRightHeight) {
					cur = cur->left;
					//printf("before up, cur = %d\n", cur);
					RotateUp(cur);
				} else {
					cur = cur->left->right;
					RotateUp(cur);
					RotateUp(cur);
				}
			} else if (rightHeight - leftHeight == 2) {
				int rightLeftHeight = getHeight(cur->right->left);
				int rightRightHeight = getHeight(cur->right->right);
				if (rightRightHeight >= rightLeftHeight) {
					cur = cur->right;
					//printf("before up, cur = %d\n", cur->value);
					RotateUp(cur);
				} else {
					cur = cur->right->left;
					RotateUp(cur);
					RotateUp(cur);
				}
			}
			//printf("cur is %d\n", cur->value);
			prev = cur;
			cur = cur->parent;
		}
		root_ = prev;
		//printf("makeBalance end\n");
	}
	
	int getHeight(AVLNode* node) {
		return node ? node->height : -1;
	}
	
	void RotateUp(AVLNode* cur) {
		AVLNode* k = cur->parent;
		if (k == NULL) {
			return;
		}
		AVLNode* parent = k->parent;
		if (k->left == cur) {
			k->left = cur->right;
			if (k->left) {
				k->left->parent = k;
			}
			cur->right = k;
			k->parent = cur;
		} else {
			k->right = cur->left;
			if (k->right) {
				k->right->parent = k;
			}
			cur->left = k;
			k->parent = cur;
		}
		replaceChild(parent, k, cur);
		fixHeight(k);
		fixHeight(cur);
		fixHeight(parent);
		fixSize(k);
		fixSize(cur);
		fixSize(parent);
	}
	
	void replaceChild(AVLNode* parent, AVLNode* old_child, AVLNode* new_child) {
		if (new_child) {
			new_child->parent = parent;
		}
		if (parent) {
			if (parent->left == old_child) {
				parent->left = new_child;
			} else {
				parent->right = new_child;
			}
		}
	}
	
	void fixHeight(AVLNode* node) {
		if (node) {
			node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
		}
	}
	
	int getSize(AVLNode* node) {
		return node ? node->size : 0;
	}
	
	void fixSize(AVLNode* node) {
		if (node) {
			node->size = getSize(node->left) + getSize(node->right) + 1;
		}
	}
	
	AVLNode* findMin(AVLNode* node) {
		if (node) {
			while (node->left) {
				node = node->left;
			}
		}
		return node;
	}

	AVLNode* findMax(AVLNode* node) {
		if (node) {
			while (node->right) {
				node = node->right;
			}
		}
		return node;
	}
	
	void eraseTree(AVLNode* node) {
		if (node) {
			eraseTree(node->left);
			eraseTree(node->right);
			delete node;
		}
	}
	
	bool checkValid(AVLNode* cur, AVLNode*& prev_node) {
		if (cur) {
			if (cur->left == cur || cur->right == cur || (cur->left != NULL && cur->left == cur->right)) {
				printf("t6\n");
				return false;
			}
			if (cur->size != getSize(cur->left) + getSize(cur->right) + 1) {
				printf("t7\n");
				return false;
			}
			if (cur->left && cur->left->parent != cur) {
				printf("t5");
				return false;
			}
			if (!checkValid(cur->left, prev_node)) {
				return false;
			}
			if (compareFunction_(prev_node->value, cur->value) >= 0 && cur != prev_node) {
				printf("t1\n");
				return false;
			}
			prev_node = cur;
			if (cur->right && cur->right->parent != cur) {
				printf("t4\n");
				return false;
			}
			if (!checkValid(cur->right, prev_node)) {
				return false;
			}
			int leftHeight = getHeight(cur->left);
			int rightHeight = getHeight(cur->right);
			if (abs(leftHeight - rightHeight) > 1) {
				//printf("t2, cur->value = %d\n", cur->value);
				return false;
			}
			if (cur->height != std::max(leftHeight, rightHeight) + 1) {
				printf("t3\n");
				return false;
			}
		}
		return true;
	}

	void dumpTree(AVLNode* cur, const std::function<void(const T&)>& dumpValue, int indent) {
		if (cur) {
			printf("%*s", indent * 2, "");
			dumpValue(cur->value);
			printf(", height = %d, size = %d\n", cur->height, cur->size);
			printf("\n");
			dumpTree(cur->left, dumpValue, indent + 1);
			dumpTree(cur->right, dumpValue, indent + 1);
		}
	}
	
	std::function<int(const T&, const T&)> compareFunction_;
	AVLNode* root_;
};
	
}  // namespace algo_lib

#endif // _AVL_TREE_H