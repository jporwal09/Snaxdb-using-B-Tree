#include<bits/stdc++.h>
#include "Bplus Tree.h"
using namespace std;

void insertionMethod(BPTree** bPTree) {
    int rollNo;
    int age, marks;
    string name;
    string Email;

    cout << "Please provide the rollNo: ";
    cin >> rollNo;

    cout << "\nWhat's the Name, Age and Email?: ";
    cin >> name >> age >> Email;

    string fileName = "DB_bin/";
    fileName += to_string(rollNo) + ".txt";
    FILE* filePtr = fopen(fileName.c_str(), "w");
    string userTuple = to_string(rollNo)+" "+ name + " " + to_string(age) + " " + Email + "\n";
    fprintf(filePtr, userTuple.c_str());
    fclose(filePtr);

    (*bPTree)->insert(rollNo, filePtr);
    //fclose(filePtr);
}

void searchMethod(BPTree* bPTree) {
    int rollNo;
    cout << "What's the RollNo to Search? ";
    cin >> rollNo;

    bPTree->search(rollNo);
}

void printMethod(BPTree* bPTree) {
    int opt;
    cout << "Press \n\t1.Hierarical-Display \n\t2.Sequential-Display\n";
    cin >> opt;

    cout << "\nHere is your File Structure" << endl;
    if (opt == 1)
        bPTree->show(bPTree->getRoot());
    else
        bPTree->seqshow(bPTree->getRoot());
}

void deleteMethod(BPTree* bPTree) {

    int tmp;
    cout << "Enter a key to delete: " << endl;
    cin >> tmp;
    bPTree->deleteKey(tmp);

}

void  printHelpMess (){
    cout << " ***************************************************************************************** " << endl<<endl
        << "                                 Welcome to the Snaxdb \n \n                        Database file locates in \" DB_bin \"  \n \n                                   2020-08-02 " <<endl<<endl
        << " ****************************************************************************************** " <<endl
        << "   Press 1 : For Insertion"<<endl
        << "   Press 2 : For Search in Database" <<endl
        << "   Press 3 : For Display of nodes in Database" <<endl
        << "   Press 4 : For Deletion" <<endl
        << "   Press 5 : For Help" <<endl
        << "   Press 6 : For Exit " <<endl
        << " ****************************************************************************************** " <<endl
        <<endl;
}
void helpop(){

    cout  << " **************************************************************************************** " <<endl
        << "   Press 1 : For Insertion"<<endl
        << "   Press 2 : For Search in Database" <<endl
        << "   Press 3 : For Display of nodes in Database" <<endl
        << "   Press 4 : For Deletion" <<endl
        << "   Press 5 : For Help" <<endl
        << "   Press 6 : For Exit " <<endl<<endl
        << " ****************************************************************************************** " <<endl
        <<endl;
}
int main() {
    printHelpMess ();

    bool flag = true;
    int option;

    BPTree* bPTree = new BPTree();

    do {
        cout<<"Snaxdb>";
        cin >> option;

        switch (option) {
            case 1:
                insertionMethod(&bPTree);
                break;
            case 2:
                searchMethod(bPTree);
                break;
            case 3:
                printMethod(bPTree);
                break;
            case 4:
                deleteMethod(bPTree);
                break;
            case 5 : helpop();
                break;
            default:
                flag = false;
                break;
        }
    }while (flag);

    return 0;
}
