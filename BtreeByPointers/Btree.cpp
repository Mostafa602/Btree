#include <iostream>
using namespace std;

template <typename KeyType, int MaxKeys>
class BTree {
    struct Node {
        KeyType* keys; // Array of keys in the node
        Node** children; // Array of pointers to child nodes
        int currentKeyCount; // Number of keys currently in the node
        bool isLeaf; // Indicates if the node is a leaf

        Node(bool isLeafNode)
                : isLeaf(isLeafNode), currentKeyCount(0) {
            keys = new KeyType[MaxKeys];
            children = new Node*[MaxKeys + 1]{nullptr};
        }
    };

    Node* root; // Root node of the B-tree

    int FindInsertPosition(KeyType* keysArray, int size, const KeyType& value) {
        int low = 0, high = size;
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (keysArray[mid] < value) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
        return low;
    }

    void InsertRecursive(Node* currentNode, const KeyType& value, Node*& newChild, KeyType& promotedKey) {
        if (currentNode->isLeaf) {
            int insertPos = FindInsertPosition(currentNode->keys, currentNode->currentKeyCount, value);

            for (int i = currentNode->currentKeyCount; i > insertPos; --i)
                currentNode->keys[i] = currentNode->keys[i - 1];

            currentNode->keys[insertPos] = value;
            currentNode->currentKeyCount++;

            if (currentNode->currentKeyCount == MaxKeys)
                SplitNode(currentNode, newChild, promotedKey);
        } else {
            int childIndex = FindInsertPosition(currentNode->keys, currentNode->currentKeyCount, value);
            InsertRecursive(currentNode->children[childIndex], value, newChild, promotedKey);

            if (newChild) {
                for (int i = currentNode->currentKeyCount; i > childIndex; --i) {
                    currentNode->keys[i] = currentNode->keys[i - 1];
                    currentNode->children[i + 1] = currentNode->children[i];
                }
                currentNode->keys[childIndex] = promotedKey;
                currentNode->children[childIndex + 1] = newChild;
                currentNode->currentKeyCount++;

                if (currentNode->currentKeyCount == MaxKeys)
                    SplitNode(currentNode, newChild, promotedKey);
                else
                    newChild = nullptr;
            }
        }
    }

    void SplitNode(Node* currentNode, Node*& newChild, KeyType& promotedKey) {
        int middleIndex = MaxKeys / 2;
        promotedKey = currentNode->keys[middleIndex];

        newChild = new Node(currentNode->isLeaf);
        newChild->currentKeyCount = currentNode->currentKeyCount - middleIndex - 1;

        for (int i = 0; i < newChild->currentKeyCount; ++i)
            newChild->keys[i] = currentNode->keys[middleIndex + 1 + i];

        if (!currentNode->isLeaf) {
            for (int i = 0; i <= newChild->currentKeyCount; ++i)
                newChild->children[i] = currentNode->children[middleIndex + 1 + i];
        }

        currentNode->currentKeyCount = middleIndex;
    }

    void PrintTree(Node* currentNode, int level) {
        if (currentNode) {
            cout << string(level * 2, ' ');
            for (int i = 0; i < currentNode->currentKeyCount; i++) {
                if (i > 0) cout << ",";
                cout << currentNode->keys[i];
            }
            cout << "\n";

            for (int i = 0; i <= currentNode->currentKeyCount; i++)
                PrintTree(currentNode->children[i], level + 1);
        }
    }

public:
    BTree() : root(new Node(true)) {}

    void Insert(const KeyType& value) {
        Node* newChild = nullptr;
        KeyType promotedKey;

        InsertRecursive(root, value, newChild, promotedKey);

        if (newChild) {
            Node* newRoot = new Node(false);
            newRoot->keys[0] = promotedKey;
            newRoot->children[0] = root;
            newRoot->children[1] = newChild;
            newRoot->currentKeyCount = 1;
            root = newRoot;
        }
    }

    void Print() {
        PrintTree(root, 0);
    }
};

int main() {
    BTree<int, 3> intTree;

    intTree.Insert(1);
    intTree.Insert(5);
    intTree.Insert(0);
    intTree.Insert(4);
    intTree.Insert(3);
    intTree.Insert(2);

    intTree.Print();

    BTree<char, 5> charTree;

    charTree.Insert('G');
    charTree.Insert('I');
    charTree.Insert('B');
    charTree.Insert('J');
    charTree.Insert('C');
    charTree.Insert('A');
    charTree.Insert('K');
    charTree.Insert('E');
    charTree.Insert('D');
    charTree.Insert('S');
    charTree.Insert('T');
    charTree.Insert('R');
    charTree.Insert('L');
    charTree.Insert('F');
    charTree.Insert('H');
    charTree.Insert('M');
    charTree.Insert('N');
    charTree.Insert('P');
    charTree.Insert('Q');

    charTree.Print();
}
