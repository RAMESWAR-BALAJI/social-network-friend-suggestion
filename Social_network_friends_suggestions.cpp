#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
using namespace std;

// Graph: Adjacency List Representation
class Graph {
public:
    unordered_map<int, set<int>> adjList; // Stores user connections

    void addEdge(int u, int v) {
        adjList[u].insert(v);
        adjList[v].insert(u); // Undirected Graph
    }

    vector<int> getFriends(int user) {
        if (adjList.find(user) != adjList.end())
            return vector<int>(adjList[user].begin(), adjList[user].end());
        return {};
    }
};

// BST Node
class BSTNode {
public:
    int friendID;
    int mutualCount;
    BSTNode *left, *right;
    BSTNode(int id, int count) : friendID(id), mutualCount(count), left(NULL), right(NULL) {}
};

// BST for sorting recommendations based on mutual friends
class BST {
public:
    BSTNode *root;
    BST() : root(NULL) {}

    BSTNode* insert(BSTNode* node, int id, int count) {
        if (!node) return new BSTNode(id, count);
        if (count < node->mutualCount)
            node->left = insert(node->left, id, count);
        else
            node->right = insert(node->right, id, count);
        return node;
    }

    void insert(int id, int count) { root = insert(root, id, count); }

    void inorderTraversal(BSTNode* node) {
        if (!node) return;
        inorderTraversal(node->right);
        cout << "Friend ID: " << node->friendID << " | Mutual Friends: " << node->mutualCount << endl;
        inorderTraversal(node->left);
    }
    
    void displaySortedRecommendations() {
        inorderTraversal(root);
    }
};

// Friend Suggestion System using BFS and BST
class FriendSuggestion {
public:
    Graph graph;
    BST bst;

    void addFriendship(int u, int v) {
        graph.addEdge(u, v);
    }

    void suggestFriends(int user) {
        set<int> suggestedFriends;
        vector<int> friends = graph.getFriends(user);
        set<int> userFriends(friends.begin(), friends.end());
        queue<int> q;
        unordered_map<int, int> mutualCount;

        // Perform BFS from user's friends to find second-degree connections
        for (int friendID : friends) {
            q.push(friendID);
        }
        
        while (!q.empty()) {
            int currFriend = q.front();
            q.pop();
            
            vector<int> mutuals = graph.getFriends(currFriend);
            for (int mutual : mutuals) {
                if (mutual != user && userFriends.find(mutual) == userFriends.end()) {
                    suggestedFriends.insert(mutual);
                    mutualCount[mutual]++;
                }
            }
        }

        cout << "\nFriend Suggestions for User " << user << ":\n";
        for (auto& suggested : suggestedFriends) {
            bst.insert(suggested, mutualCount[suggested]);
        }
        bst.displaySortedRecommendations();
    }
};

int main() {
    FriendSuggestion fs;
    
    fs.addFriendship(1, 2);
    fs.addFriendship(1, 3);
    fs.addFriendship(1, 4);
    fs.addFriendship(2, 4);
    fs.addFriendship(3, 5);
    fs.addFriendship(4, 6);
    fs.addFriendship(5, 6);
    fs.addFriendship(4, 9);
    fs.addFriendship(2, 9);
    fs.addFriendship(8, 2);
    fs.addFriendship(8, 3);
    fs.addFriendship(3, 6);

    fs.suggestFriends(1);

    return 0;
}