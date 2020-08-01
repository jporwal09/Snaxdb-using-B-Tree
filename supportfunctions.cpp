#include<bits/stdc++.h>
#include "Bplus Tree.h"
using namespace std;

Node* parent = NULL;

BPTree::BPTree() {
    this->maxChildLimit = 3;
    this->maxLeafLimit = 2;
    this->root = NULL;
}

BPTree::BPTree(int degreeOfInternal, int degreeOfLeaf) {
    this->maxChildLimit = degreeOfInternal;
    this->maxLeafLimit = degreeOfLeaf;
    this->root = NULL;
}

int BPTree::getMaxChildLimit() {
    return maxChildLimit;
}

int BPTree::getMaxLeafNode() {
    return maxLeafLimit;
}

Node* BPTree::getRoot() {
    return this->root;
}

void BPTree::setRoot(Node *ptr) {
    this->root = ptr;
}

Node* BPTree::firstLeftNode(Node* current) {
    if (current->leaf)
        return current;
    for (int i = 0; i < current->next.nxtChilds.size(); i++)
        if (current->next.nxtChilds[i] != NULL)
            return firstLeftNode(current->next.nxtChilds[i]);

    return NULL;
}

Node** BPTree::findParent(Node* current, Node* child) {
   
    if (current->leaf || current->next.nxtChilds[0]->leaf)
        return NULL;

    for (int i = 0; i < current->next.nxtChilds.size(); i++) {
        if (current->next.nxtChilds[i] == child) {
            parent = current;
        } else {
            Node* tmp = current->next.nxtChilds[i];
            findParent(tmp, child);
        }
    }

    return &parent;
}

Node::ptr::ptr() {
}

Node::ptr::~ptr() {
}

Node::Node() {
    this->leaf = false;
    this->nextnode = NULL;
}
