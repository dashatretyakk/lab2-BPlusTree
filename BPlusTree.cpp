#include "BPlusTree.h"

using namespace std;

BPlusTree::Node::Node(bool isLeaf) : isLeaf(isLeaf) {}

BPlusTree::Node::~Node() {
    if (isLeaf) {
        for (auto child : children) {
            delete child;
        }
    }
}

BPlusTree::BPlusTree() {
    root = nullptr;
}

BPlusTree::~BPlusTree() {
    delete root;
}

void BPlusTree::insert(double key) {
    if (root == nullptr) {
        root = new Node(true);
        root->keys.push_back(key);
    } else {
        if (root->keys.size() == 2 * t - 1) {
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }
        insertNonFull(root, key);
    }
}

void BPlusTree::insertNonFull(Node* node, double key) {
    int i = node->keys.size() - 1;
    if (node->isLeaf) {
        node->keys.push_back(0);
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->keys.size() == 2 * t - 1) {
            splitChild(node, i);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

void BPlusTree::splitChild(Node* parent, int index) {
    Node* originalChild = parent->children[index];
    Node* newChild = new Node(originalChild->isLeaf);

    parent->keys.insert(parent->keys.begin() + index, originalChild->keys[t - 1]);

    newChild->keys = vector<double>(originalChild->keys.begin() + t, originalChild->keys.end());
    originalChild->keys.resize(t - 1);

    if (!originalChild->isLeaf) {
        newChild->children = vector<Node*>(originalChild->children.begin() + t, originalChild->children.end());
        originalChild->children.resize(t);
    }

    parent->children.insert(parent->children.begin() + index + 1, newChild);
}

void BPlusTree::remove(double key) {
    if (root == nullptr) {
        return;
    }

    removeNonLeaf(root, key);

    if (root->keys.empty() && !root->isLeaf) {
        Node* oldRoot = root;
        root = root->children[0];
        oldRoot->children.clear();
        delete oldRoot;
    }
}

void BPlusTree::removeNonLeaf(Node* node, double key) {
    int index = 0;
    while (index < node->keys.size() && key > node->keys[index]) {
        index++;
    }

    if (node->isLeaf) {
        if (index < node->keys.size() && node->keys[index] == key) {
            node->keys.erase(node->keys.begin() + index);
        }
        return;
    }

    if (index < node->keys.size() && node->keys[index] == key) {
        Node* predecessorChild = node->children[index];
        if (predecessorChild->keys.size() >= t) {
            double predecessor = getPredecessor(predecessorChild);
            node->keys[index] = predecessor;
            removeNonLeaf(predecessorChild, predecessor);
            return;
        }

        Node* successorChild = node->children[index + 1];
        if (successorChild->keys.size() >= t) {
            double successor = getSuccessor(successorChild);
            node->keys[index] = successor;
            removeNonLeaf(successorChild, successor);
            return;
        }

        mergeChildren(node, index);
        removeNonLeaf(predecessorChild, key);
    } else {
        Node* child = node->children[index];
        if (child->keys.size() == t - 1) {
            if (index > 0 && node->children[index - 1]->keys.size() >= t) {
                borrowFromPrev(node, index);
            } else if (index < node->keys.size() && node->children[index + 1]->keys.size() >= t) {
                borrowFromNext(node, index);
            } else {
                if (index < node->keys.size()) {
                    mergeChildren(node, index);
                } else {
                    mergeChildren(node, index - 1);
                }
            }
        }
        removeNonLeaf(child, key);
    }
}

double BPlusTree::getPredecessor(Node* node) const {
    while (!node->isLeaf) {
        node = node->children[node->children.size() - 1];
    }
    return node->keys[node->keys.size() - 1];
}

double BPlusTree::getSuccessor(Node* node) const {
    while (!node->isLeaf) {
        node = node->children[0];
    }
    return node->keys[0];
}

void BPlusTree::borrowFromPrev(Node* parent, int index) {
    Node* child = parent->children[index];
    Node* sibling = parent->children[index - 1];

    child->keys.insert(child->keys.begin(), parent->keys[index - 1]);
    parent->keys[index - 1] = sibling->keys.back();
    sibling->keys.pop_back();

    if (!sibling->isLeaf) {
        child->children.insert(child->children.begin(), sibling->children.back());
        sibling->children.pop_back();
    }
}

void BPlusTree::borrowFromNext(Node* parent, int index) {
    Node* child = parent->children[index];
    Node* sibling = parent->children[index + 1];

    child->keys.push_back(parent->keys[index]);
    parent->keys[index] = sibling->keys.front();
    sibling->keys.erase(sibling->keys.begin());

    if (!sibling->isLeaf) {
        child->children.push_back(sibling->children.front());
        sibling->children.erase(sibling->children.begin());
    }
}

void BPlusTree::mergeChildren(Node* parent, int index) {
    Node* child = parent->children[index];
    Node* sibling = parent->children[index + 1];

    child->keys.push_back(parent->keys[index]);
    child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());

    if (!child->isLeaf) {
        child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());
    }

    parent->keys.erase(parent->keys.begin() + index);
    parent->children.erase(parent->children.begin() + index + 1);

    delete sibling;
}

bool BPlusTree::search(double key) const {
    Node* node = root;
    while (node != nullptr) {
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }
        if (i < node->keys.size() && key == node->keys[i]) {
            return true;
        }
        if (node->isLeaf) {
            break;
        }
        node = node->children[i];
    }
    return false;
}

void BPlusTree::printTree() const {
    printTreeHelper(root, 0);
}

void BPlusTree::printTreeHelper(Node* node, int indent) const {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < indent; i++) {
        cout << "  ";
    }
    for (auto key : node->keys) {
        cout << key << " ";
    }
    cout << endl;
    if (!node->isLeaf) {
        for (auto child : node->children) {
            printTreeHelper(child, indent + 1);
        }
    }
}

void BPlusTree::printKeys() const {
    printKeysHelper(root);
}

void BPlusTree::printKeysHelper(Node* node) const {
    if (node == nullptr) {
        return;
    }
    if (node->isLeaf) {
        for (auto key : node->keys) {
            cout << key << " ";
        }
    } else {
        for (int i = 0; i < node->keys.size(); i++) {
            printKeysHelper(node->children[i]);
            cout << node->keys[i] << " ";
        }
        printKeysHelper(node->children[node->children.size() - 1]);
    }
}


