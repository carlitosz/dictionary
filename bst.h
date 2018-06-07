#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>
using namespace std;

#include "node.h"
#include "Colors.h"

// ============================================================================
// The BST class is a self-balancing binary search tree.
// ============================================================================
class BST {
        node* root;

        void makeEmpty(node* t);
        void printLevelOrder(node* t);
        void printGivenLevel(node* t, int level);
        node* insert(string x, node* t);
        node* singleRightRotate(node* &t);
        node* singleLeftRotate(node* &t);
        node* doubleLeftRotate(node* &t);
        node* doubleRightRotate(node* &t);
        node* findMin(node* t);
        node* findMax(node* t);
        node* remove(string x, node* t);
        int height(node* t);
        int getBalance(node* t);
        void inorder(node* t);
        bool find(node *t, string value);
    public:
        BST() { root = NULL; }
        bool find(string value) { return find(root, value); }
        void insert(string x) { root = insert(x, root); }
        void remove(string x) { root = remove(x, root); }
        void display() { inorder(root); cout << endl; }
        void printLevelOrder() { printLevelOrder(root); }
};

// ============================================================================
// Empties the tree.
// ============================================================================
void BST::makeEmpty(node* t) {
    if(t == NULL)
        return;
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;
}

// ============================================================================
// Inserts into the tree.
// ============================================================================
node* BST::insert(string x, node* t) {
    if(t == NULL) {
        t = new node;
        t->data = x;
        t->height = 0;
        t->left = t->right = NULL;
    } else if(x < t->data) {
        t->left = insert(x, t->left);
        if(height(t->left) - height(t->right) == 2) {
            if(x < t->left->data)
                t = singleRightRotate(t);
            else
                t = doubleRightRotate(t);
        }
    } else if(x > t->data) {
        t->right = insert(x, t->right);
        if(height(t->right) - height(t->left) == 2) {
            if(x > t->right->data)
                t = singleLeftRotate(t);
            else
                t = doubleLeftRotate(t);
        }
    }

    t->height = max(height(t->left), height(t->right))+1;
    return t;
}

// ============================================================================
// Rotates single right.
// ============================================================================
node* BST::singleRightRotate(node* &t) {
    node* u = t->left;
    t->left = u->right;
    u->right = t;
    t->height = max(height(t->left), height(t->right))+1;
    u->height = max(height(u->left), t->height)+1;
    return u;
}

// ============================================================================
// Rotates single left.
// ============================================================================
node* BST::singleLeftRotate(node* &t) {
    node* u = t->right;
    t->right = u->left;
    u->left = t;
    t->height = max(height(t->left), height(t->right))+1;
    u->height = max(height(t->right), t->height)+1 ;
    return u;
}

// ============================================================================
// Executes a double left rotate.
// ============================================================================
node* BST::doubleLeftRotate(node* &t) {
    t->right = singleRightRotate(t->right);
    return singleLeftRotate(t);
}

// ============================================================================
// Executes a double right rotate.
// ============================================================================
node* BST::doubleRightRotate(node* &t) {
    t->left = singleLeftRotate(t->left);
    return singleRightRotate(t);
}

// ============================================================================
// Finds the node with the minimum value.
// ============================================================================
node* BST::findMin(node* t) {
    if(t == NULL)
        return NULL;
    else if(t->left == NULL)
        return t;
    else
        return findMin(t->left);
}

// ============================================================================
// Finds the node with the maximum value.
// ============================================================================
node* BST::findMax(node* t) {
    if(t == NULL)
        return NULL;
    else if(t->right == NULL)
        return t;
    else
        return findMax(t->right);
}

// ============================================================================
// Removes a node from the tree.
// ============================================================================
node* BST::remove(string x, node* t) {
    node* temp;

    // Element not found
    if(t == NULL)
        return NULL;

    // Searching for element
    else if(x < t->data)
        t->left = remove(x, t->left);
    else if(x > t->data)
        t->right = remove(x, t->right);

    // Element found
    // With 2 children
    else if(t->left && t->right) {
        temp = findMin(t->right);
        t->data = temp->data;
        t->right = remove(t->data, t->right);
    }
    // With one or zero child
    else {
        temp = t;
        if(t->left == NULL)
            t = t->right;
        else if(t->right == NULL)
            t = t->left;
        delete temp;
    }
    if(t == NULL)
        return t;

    t->height = max(height(t->left), height(t->right))+1;

    // If node is unbalanced
    // If left node is deleted, right case
    if(height(t->left) - height(t->right) == 2) {
        // right right case
        if(height(t->left->left) - height(t->left->right) == 1)
            return singleLeftRotate(t);
        // right left case
        else
            return doubleLeftRotate(t);
    }
    // If right node is deleted, left case
    else if(height(t->right) - height(t->left) == 2) {
        // left left case
        if(height(t->right->right) - height(t->right->left) == 1)
            return singleRightRotate(t);
        // left right case
        else
            return doubleRightRotate(t);
    }
    return t;
}

// ============================================================================
// Returns the height at a node.
// ============================================================================
int BST::height(node* t) {
    return (t == NULL ? -1 : t->height);
}

// ============================================================================
// Gets the current balance of a tree.
// ============================================================================
int BST::getBalance(node* t) {
    if(t == NULL)
        return 0;
    else
        return height(t->left) - height(t->right);
}

// ============================================================================
// Prints the tree inorder style.
// ============================================================================
void BST::inorder(node* t) {
    if(t == NULL)
        return;
    inorder(t->left);
    cout << t->data << " ";
    inorder(t->right);
}

// ============================================================================
// Finds the desired value.
// ============================================================================
bool BST::find(node* t, string value) {
      if (value == t->data)
        return true;
      else if (value < t->data) {
        if (t->left == NULL) return false;
        else return find(t->left, value);
      } else if (value > t->data) {
        if (t->right == NULL) return false;
        else return find(t->right, value);
      }

      return false;
}

// ============================================================================
// Prints the tree level-order style.
// ============================================================================
void BST::printLevelOrder(node* t)
{
    int h = height(t) + 1;
    int i;
    for (i = 1; i <= h; i++) {
        printGivenLevel(t, i);
        printf("\n");
    }
}

// ============================================================================
// Prints the node at a given level.
// ============================================================================
void BST::printGivenLevel(node* t, int level)
{
    if (t == NULL)
        return;
    if (level == 1)
        cout << t->data << " ";
    else if (level > 1) {
        printGivenLevel(t->left, level - 1);
        printGivenLevel(t->right, level - 1);
    }
}
#endif /* BST_H */
