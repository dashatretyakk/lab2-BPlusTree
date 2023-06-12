#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <vector>

class BPlusTree {
private:
    struct Node {
        bool isLeaf;
        std::vector<double> keys;
        std::vector<Node*> children;

        Node(bool isLeaf);
        ~Node();
    };

    Node* root;

public:
    BPlusTree();
    ~BPlusTree();

    void insert(double key);
    void remove(double key);

    void printTree() const;
    void printKeys() const;

    bool search(double key) const;

private:
    void insertNonFull(Node* node, double key);
    void splitChild(Node* parent, int index);
    void mergeChildren(Node* parent, int index);
    void borrowFromPrev(Node* parent, int index);
    void borrowFromNext(Node* parent, int index);
    void removeNonLeaf(Node* node, double key);
    double getPredecessor(Node* node) const;
    double getSuccessor(Node* node) const;

    void printTreeHelper(Node* node, int indent) const;
    void printKeysHelper(Node* node) const;
};

#endif  
