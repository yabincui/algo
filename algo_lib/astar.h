#ifndef _ASTAR_H
#define _ASTAR_H

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*
This is a experiment implementation of A* algorithm.
*/
namespace algo_lib {

using namespace std;

enum DIR {
    DIR_UP = 0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
};

static int dr[4] = {-1, 0, 0, 1};
static int dc[4] = {0, 1, -1, 0};

struct AstarPoint {
    int r;
    int c;
    AstarPoint(int r=0, int c=0) : r(r), c(c) {}

    bool operator==(const AstarPoint& p) const {
        return r == p.r && c == p.c;
    }

    bool operator!=(const AstarPoint& p) const {
        return !(*this == p);
    }

    static size_t hash(const AstarPoint& p) {
        return ((size_t)p.r << (sizeof(size_t) * 4)) | p.c;
    }

    AstarPoint move(int dir) {
        return move(static_cast<DIR>(dir));        
    }

    AstarPoint move(DIR dir) {
        return AstarPoint(r + dr[dir], c + dc[dir]);
    }
};

// Find the min path from start point to end point in a 2D board using A* algo.
class Astar {
private:
    struct Node {
        int guessTotalDist;
        int distToStart;
        AstarPoint parent;
        Node() {}
        Node(int guessDistToEnd, int distToStart, const AstarPoint& parent)
            : guessTotalDist(guessDistToEnd + distToStart), distToStart(distToStart), parent(parent) {
        }
    };

    struct QueueNode {
        AstarPoint p;
        int guessTotalDist;
        QueueNode(const AstarPoint& p, int guessTotalDist) : p(p), guessTotalDist(guessTotalDist) {}
    };

    int getGuessDistToEnd(const AstarPoint& p) {
        return abs(p.r - end.r) + abs(p.c - end.c);
    }

public:
    vector<AstarPoint> getPath(const vector<vector<bool>>& board,
                             const AstarPoint& start, const AstarPoint& end) {
        pboard = &board;
        int rows = board.size();
        int cols = board[0].size();
        std::unordered_map<AstarPoint, Node, decltype(&AstarPoint::hash)> state_map(100, AstarPoint::hash);
        std::unordered_set<AstarPoint, decltype(&AstarPoint::hash)> closed_set(100, AstarPoint::hash);
        auto compareQueueNode = [](const QueueNode& n1, const QueueNode& n2) {
            return n1.guessTotalDist > n2.guessTotalDist;
        };
        priority_queue<QueueNode, vector<QueueNode>, decltype(compareQueueNode)> q(compareQueueNode);
        state_map[start] = Node(getGuessDistToEnd(start), 0, AstarPoint(-1, -1));
        q.push(QueueNode(start, getGuessDistToEnd(start)));
        while (!q.empty()) {
            QueueNode cur = q.top(); q.pop();
            Node& node = state_map[cur.p];
            if (cur.guessTotalDist > node.guessTotalDist) {
                continue;
            }
            if (cur.p == end) {
                break;
            }
            for (int d = 0; d < 4; ++d) {
                AstarPoint np = cur.p.move(d);
                if (np.r < 0 || np.r >= rows || np.c < 0 || np.c >= cols || !board[np.r][np.c]) {
                    continue;
                }
                auto it = state_map.find(np);
                if (it == state_map.end()) {
                    Node n = Node(getGuessDistToEnd(np), node.distToStart + 1, cur.p);
                    state_map[np] = n;
                    q.push(QueueNode(np, n.guessTotalDist));
                } else if (it->second.distToStart > node.distToStart + 1) {
                    it->second = Node(getGuessDistToEnd(np), node.distToStart + 1, cur.p);
                    q.push(QueueNode(np, it->second.guessTotalDist));
                }
            }
        }
        auto it = state_map.find(end);
        if (it == state_map.end()) {
            return vector<AstarPoint>();
        }
        vector<AstarPoint> result;
        AstarPoint cur = end;
        while (cur != start) {
            result.push_back(cur);
            cur = state_map[cur].parent;
        }
        result.push_back(start);
        std::reverse(result.begin(), result.end());
        return result;
    }

private:
    const vector<vector<bool>>* pboard;
    AstarPoint end;
};


}  // namespace algo_lib

#endif // _ASTAR_H