#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <stdlib.h>

#include <string>

namespace algo_lib {

// Similar to Balanced Tree.
template <typename KeyT, typename ValueT>
class SkipList {
public:
    SkipList(int maxSize) {
        int size = 1;
        maxLevels = 1;
        while (size < maxSize) {
            size <<= 1;
            maxLevels++;
        }
        curLevels = 1;
        head = newNode(maxLevels);
    }

    ~SkipList() {
        Node* cur = head;
        while (cur != nullptr) {
            Node* tmp = cur->next[0];
            delete cur;
            cur = tmp;
        }
    }
    
    ValueT* find(const KeyT& key) {
        Node* cur = head;
        for (int i = curLevels - 1; i >= 0; --i) {
            while (cur->next[i] && cur->next[i]->key < key) {
               cur = cur->next[i];
            }
        }
        if (cur->next[0] && cur->next[0]->key == key) {
            return &(cur->next[0]->value);
        }
        return nullptr;
    }

    void insert(const KeyT& key, const ValueT& value) {
        Node* updates[maxLevels];
        for (int i = 0; i < maxLevels; ++i) {
            updates[i] = head;
        }
        Node* cur = head;
        for (int i = curLevels - 1; i >= 0; --i) {
            while (cur->next[i] && cur->next[i]->key < key) {
                cur = cur->next[i];
            }
            updates[i] = cur;
        }
        if (cur->next[0] && cur->next[0]->key == key) {
            cur->next[0]->value = value;
            return;
        }
        int levels = randomLevels();
        Node* node = newNode(levels);
        node->key = key;
        node->value = value;
        for (int i = 0; i < levels; ++i) {
            node->next[i] = updates[i]->next[i];
            updates[i]->next[i] = node;
        }
    }

    void erase(KeyT key) {
        Node* updates[maxLevels];
        for (int i = 0; i < maxLevels; ++i) {
            updates[i] = head;
        }
        Node* cur = head;
        for (int i = curLevels - 1; i >= 0; --i) {
            while (cur->next[i] && cur->next[i]->key < key) {
                cur = cur->next[i];
            }
            updates[i] = cur;
        }
        if (cur->next[0] && cur->next[0]->key == key) {
            cur = cur->next[0];
            for (int i = 0; i < maxLevels && updates[i]->next[i] == cur; ++i) {
                updates[i]->next[i] = cur->next[i];
            }
            while (curLevels > 1 && !head->next[curLevels - 1]) {
                curLevels--;
            }
            delete cur;
        }
    }

    std::string toString() {
        std::string result;
        Node* cur = head;
        char s[100];
        while (cur != nullptr) {
            if (cur == head) {
                result += "head";
            } else {
                snprintf(s, sizeof(s), "(key %d, value %d)", cur->key, cur->value);
                result += s;
            }
            result += " next ";
            for (int i = 0; i < cur->levels; ++i) {
                snprintf(s, sizeof(s), "[%d] = %d, ", i, (cur->next[i] ? cur->next[i]->key : -1));
                result += s;
            }
            result += "\n";
            cur = cur->next[0];
        }
        return result;
    }

private:
    struct Node {
        KeyT key;
        ValueT value;
        int levels;
        Node* next[];
    };

    int randomLevels() {
        int levels = 1;
        while (levels < maxLevels && random() % 2 == 1) {
            levels++;
        }
        return levels;
    }

    Node* newNode(int levels) {
        Node* node = (Node*)calloc(1, sizeof(Node) + sizeof(Node*) * levels);
        node->levels = levels;
        return node;
    }

    void deleteNode(Node* node) {
        free(node);
    }

    int maxLevels;
    int curLevels;
    Node* head;
};

} // namespace algo_lib

#endif  // _SKIPLIST_H