#include<bits/stdc++.h>
#include "Bplus Tree.h"
using namespace std;

void BPTree::insert(int key, FILE* fPtr) {  //in Leaf Node
    /*
		1. If the node has an empty space, insert the key/reference pair into the node.
		2. If the node is already full, split it into two nodes, distributing the keys
		evenly between the two nodes. If the node is a leaf, take a copy of the minimum
		value in the second of these two nodes and repeat this insertion algorithm to
		insert it into the parent node. If the node is a non-leaf, exclude the middle
		value during the split and repeat this insertion algorithm to insert this excluded
		value into the parent node.
	*/

    if (root == NULL) {
        root = new Node;
        root->leaf = true;
        root->keys.push_back(key);
        new (&root->next.datafiles) vector<FILE*>;
        
        root->next.datafiles.push_back(fPtr);

        cout << "Query Inserted successfully" << endl;
        return;
    } else {
        Node* current = root;
        Node* parent = NULL;
        //searching for the possible position for the given key by doing the same procedure we did in search
        while (current->leaf == false) {
            parent = current;
            int ind = upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
            current = current->next.nxtChilds[ind];
        }

        //now current is the leaf node in which we'll insert the new key
        if (current->keys.size() < maxLeafLimit) {
            /*
				If current leaf Node is not FULL, find the correct position for the new key and insert!
			*/
            int i = upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
            current->keys.push_back(key);
            current->next.datafiles.push_back(fPtr);

            if (i != current->keys.size() - 1) {
                for (int j = current->keys.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                    current->keys[j] = current->keys[j - 1];
                    current->next.datafiles[j] = current->next.datafiles[j - 1];
                }

                //since earlier step was just to inc. the size of vectors and making space, now we are simplying inserting
                current->keys[i] = key;
                current->next.datafiles[i] = fPtr;
            }
            cout << " Query Inserted successfully "<< endl;
        } else {
            /*
				DAMN!! Node Overflowed :(
				HAIYYA! Splitting the Node .
			*/
            vector<int> copyNode(current->keys);
            vector<FILE*> copyDataNode(current->next.datafiles);

            //finding the probable place to insert the key
            int i = upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();

            copyNode.push_back(key);          // to create space
            copyDataNode.push_back(fPtr);  // to create space

            if (i != copyNode.size() - 1) {
                for (int j = copyNode.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                    copyNode[j] = copyNode[j - 1];
                    copyDataNode[j] = copyDataNode[j - 1];
                }

                //inserting
                copyNode[i] = key;
                copyDataNode[i] = fPtr;
            }
            /*
				BAZINGA! I have the power to create new Leaf :)
			*/

            Node* newLeaf = new Node;
            newLeaf->leaf = true;
            new (&newLeaf->next.datafiles) vector<FILE*>;
            //// now, newLeaf->ptr2TreeOrData.ptr2Tree is the active member of the union

            //swapping the next ptr
            Node* temp = current->nextnode;
            current->nextnode = newLeaf;
            newLeaf->nextnode = temp;

            //resizing and copying the keys & dataPtr to OldNode
            current->keys.resize((maxLeafLimit) / 2 +1);//check +1 or not while partitioning
            current->next.datafiles.reserve((maxLeafLimit) / 2 +1);
            for (int i = 0; i <= (maxLeafLimit) / 2; i++) {
                current->keys[i] = copyNode[i];
                current->next.datafiles[i] = copyDataNode[i];
            }

            //Pushing new keys & dataPtr to NewNode
            for (int i = (maxLeafLimit) / 2 + 1; i < copyNode.size(); i++) {
                newLeaf->keys.push_back(copyNode[i]);
                newLeaf->next.datafiles.push_back(copyDataNode[i]);
            }

            if (current == root) {
                /*
					If current is root node we create new node
				*/

                Node* newRoot = new Node;
                newRoot->keys.push_back(newLeaf->keys[0]);
                new (&newRoot->next.nxtChilds) vector<Node*>;
                newRoot->next.nxtChilds.push_back(current);
                newRoot->next.nxtChilds.push_back(newLeaf);
                root = newRoot;
                cout << "New Root in B+ Tree!" << endl;
            } else {
                // Insert new key in the parent
                insertInt(newLeaf->keys[0], &parent, &newLeaf);
            }
        }
    }
}

void BPTree::insertInt(int x, Node** current, Node** child) {  //in Internal Nodes
    if ((*current)->keys.size() < maxChildLimit - 1) {
        /*
			If current is not full find the position for the new key.
		*/
        int i = upper_bound((*current)->keys.begin(), (*current)->keys.end(), x) - (*current)->keys.begin();
        (*current)->keys.push_back(x);
        //new (&(*current)->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
        //// now, root->ptr2TreeOrData.ptr2Tree is the active member of the union
        (*current)->next.nxtChilds.push_back(*child);

        if (i != (*current)->keys.size() - 1) {  // if there are more than one element
            // Different loops because size is different for both (i.e. diff of one)

            for (int j = (*current)->keys.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                (*current)->keys[j] = (*current)->keys[j - 1];
            }

            for (int j = (*current)->next.nxtChilds.size() - 1; j > (i + 1); j--) {
                (*current)->next.nxtChilds[j] = (*current)->next.nxtChilds[j - 1];
            }

            (*current)->keys[i] = x;
            (*current)->next.nxtChilds[i + 1] = *child;
        }
        cout << "Inserted key in the internal node :)" << endl;
    } else {  //splitting
        cout << "Inserted Node in internal node successful" << endl;
        cout << "Overflow in internal:( HAIYAA! splitting internal nodes" << endl;

        vector<int> copyKeyNode((*current)->keys);
        vector<Node*> copyTreePtrNode((*current)->next.nxtChilds);

        int i =upper_bound((*current)->keys.begin(), (*current)->keys.end(), x) - (*current)->keys.begin();  //finding the position for x
        copyKeyNode.push_back(x);                                                                   // to create space
        copyTreePtrNode.push_back(*child);                                                           // to create space

        if (i != copyKeyNode.size() - 1) {
            for (int j = copyKeyNode.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                copyKeyNode[j] = copyKeyNode[j - 1];
            }

            for (int j = copyTreePtrNode.size() - 1; j > (i + 1); j--) {
                copyTreePtrNode[j] = copyTreePtrNode[j - 1];
            }

            copyKeyNode[i] = x;
            copyTreePtrNode[i + 1] = *child;
        }

        int partitionKey;                                            //exclude middle element while splitting
        partitionKey = copyKeyNode[(copyKeyNode.size() / 2)];  //right biased
        int pInd = (copyKeyNode.size() / 2);

        //resizing and copying the keys & TreePtr to OldNode
        (*current)->keys.resize(pInd);
        (*current)->next.nxtChilds.resize(pInd + 1);
        (*current)->next.nxtChilds.reserve(pInd + 1);
        for (int i = 0; i < pInd; i++) {
            (*current)->keys[i] = copyKeyNode[i];
        }

        for (int i = 0; i < pInd + 1; i++) {
            (*current)->next.nxtChilds[i] = copyTreePtrNode[i];
        }

        Node* newInternalNode = new Node;
        new (&newInternalNode->next.nxtChilds) vector<Node*>;
        //Pushing new keys & TreePtr to NewNode

        for (int i = pInd + 1; i < copyKeyNode.size(); i++) {
            newInternalNode->keys.push_back(copyKeyNode[i]);
        }

        for (int i = pInd + 1; i < copyTreePtrNode.size(); i++) {  // because only key is excluded not the pointer
            newInternalNode->next.nxtChilds.push_back(copyTreePtrNode[i]);
        }

        if ((*current) == root) {
            /*
				If current is a root we create a new Node
			*/
            Node* newRoot = new Node;
            newRoot->keys.push_back(partitionKey);
            new (&newRoot->next.nxtChilds) vector<Node*>;
            newRoot->next.nxtChilds.push_back(*current);
            //// now, newRoot->ptr2TreeOrData.ptr2Tree is the active member of the union
            newRoot->next.nxtChilds.push_back(newInternalNode);

            root = newRoot;
            cout << "Created new ROOT!" << endl;
        } else {
            /*
				::Recursion::
			*/
            insertInt(partitionKey, findParent(root, *current), &newInternalNode);
        }
    }
}