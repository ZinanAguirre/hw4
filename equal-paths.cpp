#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

/* This recursive helper function will help find the tree height of any Node
by looking at its left and right subtree heights. It will take the maximum height
of the two and take it as its height.
*/
int treeHeight(Node* root){
    if (root == NULL){
        return 0;
    }
    // Recursive left and right subtree height
    int leftSub =  1 + treeHeight(root->left);
    int rightSub = 1 + treeHeight(root->right);
    if(leftSub >= rightSub){
        return leftSub;
    }
    else{
        return rightSub;
    }
}

/* This function will get the root of tree and find its left and right
subtree height with the helper function. It will determine if 
the path is the same.
*/
bool equalPaths(Node * root)
{
    if(root == NULL){
      return true;
    }
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    int heightDiff = leftHeight - rightHeight;

    // This if will show that if the two subtrees' height are not equal then the path are not the same
    if((leftHeight == 0 && rightHeight == 1) || (rightHeight == 0 && leftHeight == 1)){
      return true;
    }
    else if(heightDiff != 0){
        return false;
    }

    return true;
}

