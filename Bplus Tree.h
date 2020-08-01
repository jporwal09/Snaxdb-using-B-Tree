#include<bits/stdc++.h>
using namespace std;

#ifndef NODE_H
#define NODE_H

class Node{

	public:
			bool leaf;
			vector<int>keys;
			Node* nextnode;

			struct ptr{
				vector<Node*> nxtChilds;
				vector<FILE*> datafiles; 
			    ptr();
			    ~ptr();
			}next;

	friend class BPTree;
		public:
			Node();		



};

class BPTree{
	private:
		int maxChildLimit;
		int maxLeafLimit;
		Node* root;
		 void insertInt(int val, Node** current, Node** child);  //Insert x from child in cursor(parent)
    Node** findParent(Node* current, Node* child);
    Node* firstLeftNode(Node* current);

   public:
    BPTree();
    BPTree(int degreeOfInternal, int degreeOfLeaf);
    Node* getRoot();
    int getMaxChildLimit();
    int getMaxLeafNode();
    void setRoot(Node *);
    void show(Node* current);
    void seqshow(Node* current);
    void search(int key);
    void insert(int key, FILE* fPtr);
    void deleteKey(int key);
    void deleteInt(int val, Node* current, Node* child);
};
#endif
