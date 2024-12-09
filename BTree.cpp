#include <iostream>
#include "Vector.cpp"
using namespace std;

template <class T, int Order>
class BTreeNode
{
private:
    Vector<T> elements;
    Vector<BTreeNode<T, Order> *> nodes;
    bool leaf;
    int realSize;
public:
    BTreeNode(){
        leaf = true;
        realSize = 0;
    }
    void setLeaf(bool newLeaf){
        leaf = newLeaf;
    }
    bool isLeaf(){
        return leaf;
    }
    int keysNum(){
        return realSize;
    }
    Vector<T> &getElements(){
        return elements;
    }

    Vector<BTreeNode<T, Order> *> &getNodes(){
        return nodes;
    }

    void insertIntoElements(T element)
    {
        int left = 0, right = elements.Size() - 1, mid, place = elements.Size();
        while (left <= right)
        {
            mid = (left + right) / 2;
            if (elements[mid] >= element)
            {
                place = mid, right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }
        elements.insert(place, element);
        realSize++;
    }

    void split(int mid, BTreeNode<T, Order> *&newSibling, T &pushedUpElement)
    {
        newSibling = new BTreeNode<T, Order>(); // Allocate a new node
        pushedUpElement = elements[mid];

        // Move elements and child pointers
        newSibling->elements.assign(elements.begin() + mid + 1, elements.end());
        elements.resize(mid); // Resize elements after split

        if (!leaf)
        {
            newSibling->nodes.assign(nodes.begin() + mid + 1, nodes.end());
            nodes.resize(mid + 1); // Resize nodes after split
        }

        newSibling->realSize = newSibling->elements.Size();
        realSize = elements.Size();
        newSibling->setLeaf(leaf);
    }

    void printNode()
    {
        for (int i = 0; i < realSize - 1; i++)
        {
            cout << elements[i] << ",";
        }
        if (realSize > 0)
            cout << elements[realSize - 1];
        cout << endl;
    }

    void addNode(BTreeNode<T, Order> *node)
    {
        nodes.push_back(node);
    }
};

template <class T, int Order>
class BTree
{
private:
    BTreeNode<T, Order> *root;

    BTreeNode<T, Order> *insertHandler(T element, BTreeNode<T, Order> *currentNode)
    {
        if (currentNode->isLeaf())
        {
            currentNode->insertIntoElements(element);
            if (currentNode->keysNum() >= Order)
                return currentNode;
            return nullptr;
        }

        Vector<T> &elements = currentNode->getElements();
        Vector<BTreeNode<T, Order> *> &nodes = currentNode->getNodes();
        // Manual lower_bound implementation
        int index = 0;
        int low = 0, high = elements.Size();
        while (low < high)
        {
            int mid = low + (high - low) / 2;
            if (elements[mid] < element)
            {
                low = mid + 1;
            }
            else
            {
                high = mid;
            }
        }
        index = low;
        BTreeNode<T, Order> *nextNode = nodes[index];
        BTreeNode<T, Order> *needSplit = insertHandler(element, nextNode);
        if (needSplit)
        {
            BTreeNode<T, Order> *newSibling;
            T pushedUpElement;
            needSplit->split(Order / 2, newSibling, pushedUpElement);
            currentNode->insertIntoElements(pushedUpElement);
            currentNode->getNodes().insert(index + 1, newSibling);
            if (currentNode->keysNum() >= Order)
            {
                return currentNode;
            }
        }
        return nullptr;
    }

    void deleteTree(BTreeNode<T, Order> *node)
    {
        if (!node)
            return;

        Vector<BTreeNode<T, Order> *> children = node->getNodes();
        for (int i = 0; i < children.Size(); i++)
        {
            if (children[i])
            {
                deleteTree(children[i]);
            }
        }

        delete node; // Delete the node
    }

    void printHandler(BTreeNode<T, Order> *currentNode, string s)
    {
        if (!currentNode)
            return;
        currentNode->printNode();
        Vector<BTreeNode<T, Order> *> children = currentNode->getNodes();

        if (!currentNode->isLeaf())
        {
            s += " ";
        }
        int j = currentNode->keysNum();
        for (int i = 0; i < min(j + 1, (int)children.Size()); i++)
        {
            cout << s;
            printHandler(children[i], s);
        }
    }

public:
    BTree()
    {
        root = new BTreeNode<T, Order>();
    }

    ~BTree()
    {
        deleteTree(root);
    }

    void Insert(T element)
    {
        BTreeNode<T, Order> *currentRoot = insertHandler(element, root);
        if (currentRoot)
        {
            T newRootelement;
            BTreeNode<T, Order> *sibling;
            currentRoot->split(Order / 2, sibling, newRootelement);
            BTreeNode<T, Order> *newRoot = new BTreeNode<T, Order>();
            newRoot->insertIntoElements(newRootelement);
            newRoot->addNode(currentRoot);
            newRoot->addNode(sibling);
            newRoot->setLeaf(false);
            root = newRoot;
        }
    }

    void Print()
    {
        printHandler(root, "");
    }
};


int main()
{
    // Construct a BTree of order 3, which stores int data
    BTree<int, 3> t1;
    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);
    t1.Print(); // Expected Output:
    /*
    1,4
     0
     2,3
     5
    */

    BTree<char, 5> t; // Construct a BTree of order 5, which stores char data
    t.Insert('G');
    t.Insert('I');
    t.Insert('B');
    t.Insert('J');
    t.Insert('C');
    t.Insert('A');
    t.Insert('K');
    t.Insert('E');
    t.Insert('D');
    t.Insert('S');
    t.Insert('T');
    t.Insert('R');
    t.Insert('L');
    t.Insert('F');
    t.Insert('H');
    t.Insert('M');
    t.Insert('N');
    t.Insert('P');
    t.Insert('Q');
    t.Print(); // Expected Output:
    /*
    K
     C,G
      A,B
      D,E,F
      H,I,J
     N,R
      L,M
      P,Q
      S,T
    */
    return 0;
}

