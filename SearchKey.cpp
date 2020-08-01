#include<bits/stdc++.h>
#include "Bplus Tree.h"
using namespace std;

void BPTree::search(int key) {
    if (root == NULL) {
        cout << "NO Entry in the Table" << endl;
        return;
    } else {
        Node* current = root;
        while (current->leaf == false) {
    
            int ind = upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
            current = current->next.nxtChilds[ind];  
  		}
        int ind = lower_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();  

        if (ind == current->keys.size() || current->keys[ind] != key) {
            cout << "Key NOT FOUND" << endl;
            return;
        }

        string fName = "DB_bin/";
        string data;
        fName += to_string(key) + ".txt";
        FILE* filePtr = fopen(fName.c_str(), "r");
        cout << "Key FOUND" << endl;
        cout << "Corresponding Tuple Data is: ";
        char ch = fgetc(filePtr);
        while (ch != EOF) {
            printf("%c", ch);
            ch = fgetc(filePtr);
        }
        fclose(filePtr);
        cout << endl;
    }
}