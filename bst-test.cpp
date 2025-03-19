#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;



int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('A',1));
    bt.print();
    bt.insert(std::make_pair('a',2));
    bt.print();
    bt.insert(std::make_pair('B',3));
    bt.insert(std::make_pair('b',4));
    bt.insert(std::make_pair('C',5));
    bt.insert(std::make_pair('c',6));
    bt.print();
  
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('b');
    bt.print();

    
    
    return 0;
}
