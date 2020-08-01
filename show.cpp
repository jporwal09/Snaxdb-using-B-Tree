#include<bits/stdc++.h>
#include "Bplus Tree.h"
using namespace std;


void BPTree::show(Node* current) {
   
    if (current == NULL) return;
    queue<Node*> q;
    q.push(current);

    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node* u = q.front(); 
            q.pop();

            for (int val : u->keys)
                cout << val << " ";

            cout << "|| ";
            
            if (u->leaf != true) {
                for (Node* v : u->next.nxtChilds) {
                    q.push(v);
                }
            }
        }
        cout << endl;
    }
}

void BPTree::seqshow(Node* current) {
    Node* firstLeft = firstLeftNode(current);

    if (firstLeft == NULL) {
        cout << "Empty Table!" << endl;
        return;
    }
    cout <<"--------------------------------------------------------------------------------------------\n";
    cout <<" RollNo        Name             Age              Email"<<endl;
   
    while (firstLeft != NULL) {
        for (int i = 0; i < firstLeft->keys.size(); i++) {
            /*cout << firstLeft->keys[i] << " ";*/
         cout <<"---------------------------------------------------------------------------------------\n";  
        string fName = "DB_bin/";
        string data;
        fName += to_string(firstLeft->keys[i]) + ".txt";
        FILE* filePtr = fopen(fName.c_str(), "r");
        char ch = fgetc(filePtr);
        while (ch != EOF) {
            printf("%c", ch);
            ch = fgetc(filePtr);
        }
        fclose(filePtr);
        cout << endl;
        }

        firstLeft = firstLeft->nextnode;
    }
    cout<<"--------------------------------------------------------------------------------------------\n"; 
}

