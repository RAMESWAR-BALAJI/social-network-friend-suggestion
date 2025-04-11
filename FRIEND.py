from collections import defaultdict, deque

# Graph: Adjacency List Representation
class Graph:
    def __init__(self):
        self.adj_list = defaultdict(set)

    def add_edge(self, u, v):
        self.adj_list[u].add(v)
        self.adj_list[v].add(u)

    def get_friends(self, user):
        return list(self.adj_list.get(user, []))


# BST Node
class BSTNode:
    def __init__(self, friend_id, mutual_count):
        self.friend_id = friend_id
        self.mutual_count = mutual_count
        self.left = None
        self.right = None


# BST for sorting recommendations based on mutual friends
class BST:
    def __init__(self):
        self.root = None

    def insert(self, node, friend_id, mutual_count):
        if node is None:
            return BSTNode(friend_id, mutual_count)
        if mutual_count < node.mutual_count:
            node.left = self.insert(node.left, friend_id, mutual_count)
        else:
            node.right = self.insert(node.right, friend_id, mutual_count)
        return node

    def insert_node(self, friend_id, mutual_count):
        self.root = self.insert(self.root, friend_id, mutual_count)

    def inorder_traversal(self, node):
        if not node:
            return
        self.inorder_traversal(node.right)
        print(f"Friend ID: {node.friend_id} | Mutual Friends: {node.mutual_count}")
        self.inorder_traversal(node.left)

    def display_sorted_recommendations(self):
        self.inorder_traversal(self.root)


# Friend Suggestion System
class FriendSuggestion:
    def __init__(self):
        self.graph = Graph()
        self.bst = BST()

    def add_friendship(self, u, v):
        self.graph.add_edge(u, v)

    def suggest_friends(self, user):
        suggested_friends = set()
        friends = self.graph.get_friends(user)
        user_friends = set(friends)
        q = deque(friends)
        mutual_count = defaultdict(int)

        while q:
            curr_friend = q.popleft()
            for mutual in self.graph.get_friends(curr_friend):
                if mutual != user and mutual not in user_friends:
                    suggested_friends.add(mutual)
                    mutual_count[mutual] += 1

        print(f"\nFriend Suggestions for User {user}:\n")
        for suggested in suggested_friends:
            self.bst.insert_node(suggested, mutual_count[suggested])
        self.bst.display_sorted_recommendations()


# Driver Code
if __name__ == "__main__":
    fs = FriendSuggestion()

    fs.add_friendship(1, 2)
    fs.add_friendship(1, 3)
    fs.add_friendship(1, 4)
    fs.add_friendship(2, 4)
    fs.add_friendship(3, 5)
    fs.add_friendship(4, 6)
    fs.add_friendship(5, 6)
    fs.add_friendship(4, 9)
    fs.add_friendship(2, 9)
    fs.add_friendship(8, 2)
    fs.add_friendship(8, 3)
    fs.add_friendship(3, 6)

    fs.suggest_friends(1)
