#include<bits/stdc++.h>
#include "Bplus Tree.h"
using namespace std;

void BPTree::deleteKey(int x) {
	Node* root = getRoot();

	// If tree is empty
	if (root == NULL) {
		cout << "B+ Tree is Empty" << endl;
		return;
	}

	Node* current = root;
	Node* parent;
	int leftchild, rightchild;

	// Going to the Leaf Node, Which may contain the *key*
	// TO-DO : Use Binary Search to find the val
	while (current->leaf != true) {
		for (int i = 0; i < current->keys.size(); i++) {
			parent = current;
			leftchild = i - 1;//left side of the parent node
			rightchild = i + 1;// right side of the parent node

			if (x < current->keys[i]) {
				current = current->next.nxtChilds[i];
				break;
			}
			if (i == current->keys.size() - 1) {
				leftchild = i;
				rightchild = i + 2;// CHECK here , might need to make it negative
				current = current->next.nxtChilds[i+1];
				break;
			}
		}
	}

	// Check if the value exists in this leaf node
	int pos = 0;
	bool ok = false;
	for (pos = 0; pos < current->keys.size(); pos++) {
		if (current->keys[pos] == x) {
			ok = true;
			break;
		}
	}

	auto it = lower_bound(current->keys.begin(), current->keys.end(), x);

	if (it == current->keys.end()) {
		cout << "Key Not Found in the Tree" << endl;
		return;
	}
	
	// Delete the respective File and FILE*
	string fileName = "DB_bin/" + to_string(x) + ".txt";
	char fPtr[256];
	strcpy(fPtr, fileName.c_str());

	//delete current->ptr2TreeOrData.dataPtr[pos];//avoid memory leaks
	if (remove(fPtr) == 0)
		cout << "Record SuccessFully Deleted  " << fileName << endl;
	else
		cout << "Unable to delete the Record: " << fileName << endl;

	// Shifting the keys and dataPtr for the leaf Node
	for (int i = pos; i < current->keys.size()-1; i++) {
		current->keys[i] = current->keys[i+1];
		current->next.datafiles[i] = current->next.datafiles[i + 1];
	}
	int prev_size = current->keys.size();
	current->keys.resize(prev_size - 1);
	current->next.datafiles.resize(prev_size - 1);

	// If it is leaf as well as the root node
	if (current == root) {
		if (current->keys.size() == 0) {
			// Tree becomes empty
			setRoot(NULL);
			cout << " Tree is Empty Now " << endl;
		}
	}
	
	cout << "Deleted " << x << " From Leaf Node successfully" << endl;
	if (current->keys.size() >= (getMaxLeafNode()+1) / 2) {
		//Sufficient Node available for invariant to hold
		return;
	}

	cout << "UnderFlow in the leaf Node Happended" << endl;
	cout << "Starting Redistribution..." << endl;

	//1. Try to borrow a key from leftchild
	if (leftchild >= 0) {
		Node* leftNode = parent->next.nxtChilds[leftchild];

		//Check if leftchild has extra Key to transfer
		if (leftNode->keys.size() >= (getMaxLeafNode()+1) / 2 +1) {

			//Transfer the maximum key from the left Sibling
			int maxIdx = leftNode->keys.size()-1;
			current->keys.insert(current->keys.begin(), leftNode->keys[maxIdx]);
			current->next.datafiles.insert(current->next.datafiles.begin(), leftNode->next.datafiles[maxIdx]);

			//resize the left Sibling Node After Tranfer
			leftNode->keys.resize(maxIdx);
			leftNode->next.datafiles.resize(maxIdx);

			//Update Parent
			parent->keys[leftchild] = current->keys[0];
			printf("Transferred from left sibling of leaf node\n");
			return;
		}
	}

	//2. Try to borrow a key from rightchild
	if (rightchild < parent->next.nxtChilds.size()) {
		Node* rightNode = parent->next.nxtChilds[rightchild];

		//Check if rightchild has extra Key to transfer
		if (rightNode->keys.size() >= (getMaxLeafNode() + 1) / 2 + 1) {

			//Transfer the minimum key from the right Sibling
			int minIdx = 0;
			current->keys.push_back(rightNode->keys[minIdx]);
			current->next.datafiles.push_back(rightNode->next.datafiles[minIdx]);

			//resize the right Sibling Node After Tranfer
			rightNode->keys.erase(rightNode->keys.begin());
			rightNode->next.datafiles.erase(rightNode->next.datafiles.begin());

			//Update Parent
			parent->keys[rightchild-1] = rightNode->keys[0];
			printf("Transferred from right sibling of leaf node\n");
			return;
		}
	}

	// Merge and Delete Node
	if (leftchild >= 0) {// If left sibling exists
		Node* leftNode = parent->next.nxtChilds[leftchild];
		//Transfer Key and dataPtr to leftchild and connect ptr2next
		for (int i = 0; i < current->keys.size(); i++) {
			leftNode->keys.push_back(current->keys[i]);
			leftNode->next.datafiles
				.push_back(current->next.datafiles[i]);
		}
		leftNode->nextnode = current->nextnode;
		cout << "Merging two leaf Nodes" << endl;
		deleteInt(parent->keys[leftchild], parent, current);//delete parent Node Key
		//delete current;
	}
	else if (rightchild <= parent->keys.size()) {
		Node* rightNode = parent->next.nxtChilds[rightchild];
		//Transfer Key and dataPtr to rightchild and connect ptr2next
		for (int i = 0; i < rightNode->keys.size(); i++) {
			current->keys.push_back(rightNode->keys[i]);
			current->next.datafiles
				.push_back(rightNode->next.datafiles[i]);
		}
		current->nextnode = rightNode->nextnode;
		cout << "Merging two leaf Nodes" << endl;
		deleteInt(parent->keys[rightchild-1], parent, rightNode);//delete parent Node Key
		//delete rightNode;
	}

}

void BPTree::deleteInt(int x, Node* current, Node* child) {
	Node* root = getRoot();

	// Check if key from root is to deleted
	if (current == root) {
		if (current->keys.size() == 1) {
			// If only one key is left and matches with one of the
			// child Pointers
			if (current->next.nxtChilds[1] == child) {
				setRoot(current->next.nxtChilds[0]);
				//delete current;
				cout << " New Changed Root" <<endl;
				return;
			}
			else if (current->next.nxtChilds[0] == child) {
				setRoot(current->next.nxtChilds[1]);
				//delete current;
				cout << "New Changed Root" << endl;
				return;
			}
		}
	}

	// Deleting key x from the parent
	int pos;
	for (pos = 0; pos < current->keys.size(); pos++) {
		if (current->keys[pos] == x) {
			break;
		}
	}
	for (int i = pos; i < current->keys.size()-1; i++) {
		current->keys[i] = current->keys[i + 1];
	}
	current->keys.resize(current->keys.size() - 1);

	// Now deleting the ptr2tree
	for (pos = 0; pos < current->next.nxtChilds.size(); pos++) {
		if (current->next.nxtChilds[pos] == child) {
			break;
		}
	}

	for (int i = pos; i < current->next.nxtChilds.size() - 1; i++) {
		current->next.nxtChilds[i] = current->next.nxtChilds[i + 1];
	}
	current->next.nxtChilds.resize(current->next.nxtChilds.size()-1);

	// If there is No underflow. Phew!!
	if (current->keys.size() >= (getMaxChildLimit() + 1) / 2 - 1) {
		cout << "Deleted " << x << " from internal node successfully\n";
		return;
	}

	cout << "UnderFlow in internal Node! What did you do :/" << endl;

	if (current == root) {
		return;
	}

	Node** p1 = findParent(root, current);
	Node* parent = *p1;

	int leftchild, rightchild;

	// Finding Left and Right Siblings as we did earlier
	for (pos = 0; pos < parent->next.nxtChilds.size(); pos++) {
		if (parent->next.nxtChilds[pos] == current) {
			leftchild = pos - 1;
			rightchild = pos + 1;
			break;
		}
	}

	// If possible transfer to leftchild
	if (leftchild >= 0) {
		Node* leftNode = parent->next.nxtChilds[leftchild];

		//Check if leftchild has extra Key to transfer
		if (leftNode->keys.size() >= (getMaxChildLimit() + 1) / 2 ) {

			//transfer key from left sibling through parent
			int maxIdxKey = leftNode->keys.size() - 1;
			current->keys.insert(current->keys.begin(), parent->keys[leftchild]);
			parent->keys[leftchild] = leftNode->keys[maxIdxKey];

			int maxIdxPtr = leftNode->next.nxtChilds.size()-1;
			current->next.nxtChilds
				.insert(current->next.nxtChilds.begin(), leftNode->next.nxtChilds[maxIdxPtr]);

			//resize the left Sibling Node After Tranfer
			leftNode->keys.resize(maxIdxKey);
			leftNode->next.datafiles.resize(maxIdxPtr);

			return;
		}
	}

	// If possible transfer to rightchild
	if (rightchild < parent->next.nxtChilds.size()) {
		Node* rightNode = parent->next.nxtChilds[rightchild];

		//Check if leftchild has extra Key to transfer
		if (rightNode->keys.size() >= (getMaxChildLimit() + 1) / 2) {

			//transfer key from right sibling through parent
			int maxIdxKey = rightNode->keys.size() - 1;
			current->keys.push_back(parent->keys[pos]);
			parent->keys[pos] = rightNode->keys[0];
			rightNode->keys.erase(rightNode->keys.begin());

			//transfer the pointer from rightchild to current
			current->next.nxtChilds.push_back(rightNode->next.nxtChilds[0]);
			current->next.nxtChilds.erase(current->next.nxtChilds.begin());
			 
			return;
		}
	}

	//Start to Merge Now, if None of the above cases applied
	if (leftchild >= 0) {
		//leftNode + parent key + current
		Node* leftNode = parent->next.nxtChilds[leftchild];
		leftNode->keys.push_back(parent->keys[leftchild]);

		for (int val : current->keys) {
			leftNode->keys.push_back(val);
		}

		for (int i = 0; i < current->next.nxtChilds.size(); i++) {
			leftNode->next.nxtChilds.push_back(current->next.nxtChilds[i]);
			current->next.nxtChilds[i] = NULL;
		}

		current->next.nxtChilds.resize(0);
		current->keys.resize(0);

		deleteInt(parent->keys[leftchild], parent, current);
		cout << "Merged with left sibling"<<endl;
	}
	else if (rightchild < parent->next.nxtChilds.size()) {
		//current + parentkey +rightNode
		Node* rightNode = parent->next.nxtChilds[rightchild];
		current->keys.push_back(parent->keys[rightchild - 1]);

		for (int val : rightNode->keys) {
			current->keys.push_back(val);
		}

		for (int i = 0; i < rightNode->next.nxtChilds.size(); i++) {
			current->next.nxtChilds
				.push_back(rightNode->next.nxtChilds[i]);
			rightNode->next.nxtChilds[i] = NULL;
		}

		rightNode->next.nxtChilds.resize(0);
		rightNode->keys.resize(0);

		deleteInt(parent->keys[rightchild - 1], parent, rightNode);
		cout << "Merged with right sibling\n";
	}
}