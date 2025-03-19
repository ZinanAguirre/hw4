#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    void rotateRight(AVLNode<Key,Value>* n1);
    void rotateLeft(AVLNode<Key,Value>* n1);
    void removeFix(AVLNode<Key,Value>* parent, int8_t diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL){
      this->root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
      return;
    }

    // Will similarily insert node such as BST, but now as to update balance.
    if(this->internalFind(new_item.first) == NULL){
        AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
        AVLNode<Key, Value>* before = NULL;
         while(temp != NULL){
            if(temp->getKey() > new_item.first){
                before = temp;
                temp = temp->getLeft();
                
            }
            else{
                before = temp;
                temp = temp->getRight();
                
            }
        }
        AVLNode<Key, Value>* creation = new AVLNode<Key,Value>(new_item.first, new_item.second, before);
        if(before == NULL){
            this->root_ = creation;
        }
        else if(before->getKey() > creation->getKey()){
            before->setLeft(creation);
            //Will need to check if it did not cause any unbalance
            if(before->getBalance() == 0){
                before->setBalance(-1);
                this->insertFix(before, creation);
            }
            else{
                before->setBalance(before->getBalance() - 1);
            }
        }
        else{
            before->setRight(creation);
            //Will need to check if it did not cause any unbalance
            if(before->getBalance() == 0){
                before->setBalance(1);
                this->insertFix(before, creation);
            }
            else{
                before->setBalance(before->getBalance() + 1);
            }
        }
    }
    else {
         AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(new_item.first));
         temp->setValue(new_item.second);
    }

}

// This will help balance the AVL tree if a certain insert caused any unbalance.
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    if(n1 == NULL || n1->getParent() == NULL){
        return;
    }

    AVLNode<Key, Value>* grand = n1->getParent();

    //Assume if parent(n1) is the left child of grand(grandparent).
    if(grand->getLeft() == n1){
        grand->setBalance(grand->getBalance() - 1);
        int8_t grandBalance = grand->getBalance();
        if(grandBalance == 0){
            return;
        }
        //Will now need to check parent of grand if unbalanced
        else if(grandBalance == -1){
            this->insertFix(grand, n1);
        }
        // This means it is unbalanced and need to be fixed.
        else if(grandBalance == -2){
            // if parent has left child then it is a zig-zig.
            if(n1->getBalance() == -1){
                this->rotateRight(grand);
                n1->setBalance(0);
                grand->setBalance(0);
            }
            // if parent has a right child then it is a zig-zag.
            else if(n1->getBalance() == 1){
                this->rotateLeft(n1);
                this->rotateRight(grand);
                if(n2->getBalance() == -1){
                    n1->setBalance(0);
                    grand->setBalance(1);
                    n2->setBalance(0);
                }
                else if(n2->getBalance() == 0){
                    n1->setBalance(0);
                    grand->setBalance(0);
                }
                else if(n2->getBalance() == 1){
                    n1->setBalance(-1);
                    grand->setBalance(0);
                    n2->setBalance(0);
                }

            }
        }
    }
    //Assume if parent(n1) is the right child of grand(grandparent).
    else if(grand->getRight() == n1){
        grand->setBalance(grand->getBalance() + 1);
        int8_t grandBalance = grand->getBalance();
        if(grandBalance == 0){
            return;
        }
        //Will now need to check parent of grand if unbalanced
        else if(grandBalance == 1){
            this->insertFix(grand, n1);
        }
         // This means it is unbalanced and need to be fixed.
        else if(grandBalance == 2){
            // If parent has right child then it is a zig-zig.
            if(n1->getBalance() == 1){
                this->rotateLeft(grand);
                n1->setBalance(0);
                grand->setBalance(0);
            }
            // If parent has a left child then it is a zig-zag.
            else if(n1->getBalance() == -1){
                this->rotateRight(n1);
                this->rotateLeft(grand);
                if(n2->getBalance() == 1){
                    n1->setBalance(0);
                    grand->setBalance(-1);
                    n2->setBalance(0);
                }
                else if(n2->getBalance() == 0){
                    n1->setBalance(0);
                    grand->setBalance(0);
                }
                else if(n2->getBalance() == -1){
                    n1->setBalance(1);
                    grand->setBalance(0);
                    n2->setBalance(0);
                }
            }
        }
    }
}

// Will rotate the node to the right in order to allow AVL balance.
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* n1)
{
    // Special case if the node that is rotating is the root.
    if(n1 == this->root_){
        AVLNode<Key, Value>* temp = n1->getLeft();
        temp->setParent(n1->getParent());
        n1->setLeft(temp->getRight());
        n1->setParent(temp);
        if (temp->getRight() != nullptr) {
          temp->getRight()->setParent(n1);
        }
        temp->setRight(n1);
        this->root_ = temp;
        return;
    }
    
    AVLNode<Key, Value>* temp = n1->getLeft();
    n1->setLeft(temp->getRight());
    temp->setParent(n1->getParent());
    if (temp->getRight() != nullptr) {
        temp->getRight()->setParent(n1);
    }
    temp->setRight(n1);
    n1->setParent(temp);
    if(temp->getParent()->getRight() == n1){
        temp->getParent()->setRight(temp);
        }
    else{
        temp->getParent()->setLeft(temp);
    }
}

// Will rotate the node to the right in order to allow AVL balance.
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* n1)
{
    // Special case if the node that is rotating is the root.
    if(n1 == this->root_){
        AVLNode<Key, Value>* temp = n1->getRight();
        temp->setParent(n1->getParent());
        n1->setRight(temp->getLeft());
        n1->setParent(temp);
        if (temp->getLeft() != nullptr) {
          temp->getLeft()->setParent(n1);
        }
        temp->setLeft(n1);
        this->root_ = temp;
        return;
    }
    
    AVLNode<Key, Value>* temp = n1->getRight();
    n1->setRight(temp->getLeft());
    temp->setParent(n1->getParent());
    if (temp->getLeft() != nullptr) {
        temp->getLeft()->setParent(n1);
    }
    temp->setLeft(n1);
    n1->setParent(temp);
    if(temp->getParent()->getRight() == n1){
        temp->getParent()->setRight(temp);
        }
    else{
        temp->getParent()->setLeft(temp);
    }   
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if(current == NULL){
        return;
    }
    //Will first swap with predecessor if it has 2 children.
    if(current->getLeft() != NULL && current->getRight() != NULL){
      AVLNode<Key, Value>* child = static_cast<AVLNode<Key, Value>*>(this->predecessor(current));
      this->nodeSwap(current,child);
    }
    AVLNode<Key, Value>* realChild = NULL;
    AVLNode<Key, Value>* parent = current->getParent();
    int8_t diff = 0;

    //Will determine how the removal of the node will affect the parents' balance.
    if(parent != NULL){
        if(parent->getLeft() == current){
            diff = 1;
        }
        else if(parent->getRight() == current){
            diff = -1;
        }
    }

    //Will follow the same procedure as BST remove in changing pointers.
    if(current->getLeft() != NULL){
        realChild = current->getLeft();
    }
    else if (current->getRight() != NULL){
        realChild = current->getRight();
    }

    if(realChild != NULL){
      realChild->setParent(current->getParent());
    }

    if(current->getParent() == NULL){
      this->root_ = realChild;
    }
    else if(current->getParent()->getLeft() == current){
      current->getParent()->setLeft(realChild);

    }
    else if(current->getParent()->getRight() == current){
      current->getParent()->setRight(realChild);
    }

    delete current;
    // Now after the removal of node, the balance of parent has to be checked.
    this->removeFix(parent, diff);
}

// Helper function to help balance AVL after removal if needed.
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* parent, int8_t diff)
{
    if(parent == NULL){
        return;
    }
    AVLNode<Key, Value>* newParent = parent->getParent();
    int8_t ndiff = 0;
    // Will compute the diff of grandparent if needed to be fixed as well.
    if(newParent != NULL){
      if(newParent->getLeft() == parent){
        ndiff = 1;
      }
      else if(newParent->getRight() == parent){
        ndiff = -1;
      }
    }

    if(diff == -1){
        if(parent->getBalance() + diff == -2){
            AVLNode<Key, Value>* left = parent->getLeft();
            if(left->getBalance() == -1){
                this->rotateRight(parent);
                parent->setBalance(0);
                left->setBalance(0);
                this->removeFix(newParent,ndiff);
            }
            else if(left->getBalance() == 0){
                this->rotateRight(parent);
                parent->setBalance(-1);
                left->setBalance(1);
            }
            else if(left->getBalance() == 1){
                AVLNode<Key, Value>* grand = left->getRight();
                this->rotateLeft(left);
                this->rotateRight(parent);
                if(grand->getBalance() == 1){
                  parent->setBalance(0);
                  left->setBalance(-1);
                  grand->setBalance(0);
                }
                else if(grand->getBalance() == 0){
                  parent->setBalance(0);
                  left->setBalance(0);
                }
                else if(grand->getBalance() == -1){
                  parent->setBalance(1);
                  left->setBalance(0);
                  grand->setBalance(0);
                }
                this->removeFix(newParent,ndiff);
            }
        }
        else if (parent->getBalance() + diff == -1){
            parent->setBalance(-1);
        }
        else if (parent->getBalance() + diff == 0){
            parent->setBalance(0);
            this->removeFix(newParent,ndiff);
        }
    }
    else if (diff == 1){
        if (parent->getBalance() + diff == 2){
          AVLNode<Key, Value>* right = parent->getRight();
          if (right->getBalance() == 1){
            this->rotateLeft(parent);
            parent->setBalance(0);
            right->setBalance(0);
            this->removeFix(newParent,ndiff);
          }
          else if (right->getBalance() == 0){
            this->rotateLeft(parent);
            parent->setBalance(1);
            right->setBalance(-1);
          }
          else if (right->getBalance() == -1){
            AVLNode<Key, Value>* grand = right->getLeft();
            this->rotateRight(right);
            this->rotateLeft(parent);
            if(grand->getBalance() == -1){
              parent->setBalance(0);
              right->setBalance(1);
              grand->setBalance(0);
            }
            else if(grand->getBalance() == 0) {
              parent->setBalance(0);
              right->setBalance(0);
            }
            else if (grand->getBalance() == 1){
              parent->setBalance(-1);
              right->setBalance(0);
              grand->setBalance(0);
            }
            this->removeFix(newParent, ndiff);
          }
        }
        else if(parent->getBalance() + diff == 1){
            parent->setBalance(1);
        }
        else if(parent->getBalance() + diff == 0){
            parent->setBalance(0);
            this->removeFix(newParent, ndiff);
        }
    }
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
