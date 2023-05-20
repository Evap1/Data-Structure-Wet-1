//
// Created by Eva Poluliakhov on 13/05/2023.
//

#ifndef WET1_AVL_H
#define WET1_AVL_H

#include<iostream>
using namespace std;

//TODO: test this whole thing
//TODO: operator = not really because it's pointers

// AVL Tree
template <class T>
class TreeNode
{
public:
    ~TreeNode();

    // Getters
    int getHeight();
    TreeNode* getLeft();
    TreeNode* getRight();
    T getKey();

    // Only visible operations:
    void insert(const T& value);
    void remove(const T& value);

private:
    TreeNode();
    T key;
    TreeNode *left;
    TreeNode *right;
    TreeNode *root;
    int height;
    int weight;

    // Rotation methods
    int get_balance_factor(TreeNode *current);
    void updateHeight();
    TreeNode* right_rotate(TreeNode* B);
    TreeNode* left_rotate(TreeNode* A);
    TreeNode* rotate_LL(TreeNode *v);
    TreeNode* rotate_RL(TreeNode *v);
    TreeNode* rotate_LR(TreeNode *v);
    TreeNode* rotate_RR(TreeNode *v);

    // additional methods
    TreeNode* insert(TreeNode* node, const T& value);
    TreeNode* find(const T& value);
    TreeNode* findMin(TreeNode* v);
    TreeNode* remove(const T& value, TreeNode *v);
};

// PRIVATE METHODS:

// private constructor to be accessed once: defines our root to be empty.
template <class T>
TreeNode<T>::TreeNode() :
        key(NULL),
        left(NULL),
        right(NULL),
        root(NULL),
        height(0),
        weight(0)
{}

// Destructor
// ATTENTION may not work, make sure valngrid
// maybe void recursion
template <class T>
TreeNode<T>::~TreeNode(){
    if (left != NULL){
        delete left;
    }
    if (right != NULL){
        delete right;
    }
}

// dealing with possibility that we are too low in the tree
// works as a setter
template<class T>
void TreeNode<T>::updateHeight() {
    if (left == NULL){
        if (right == NULL){
            height = 0;
        }
        height = right->height + 1;
    }
    else if (right == NULL){
        height = left->height + 1;
    }
    height = 1 + max(left->height, right->height);
}

// Get the balance factor of a node
template<class T>
int TreeNode<T>::get_balance_factor(TreeNode *current)
{
    if (current == NULL)
        return 0;
    int l, r;
    l = current->left->getHeight();
    r = current->right->getHeight();
    return l-r;
}

// Method to determine whether some value already exist
template<class T>
TreeNode<T>* TreeNode<T>::find(const T &value) {
    if (this == NULL) return NULL;

    if (this->key == value) return this;

    if (this->key < value) {
        this = this->right;
    }

    this = this->left;

    return find(value);;
}

// Insert new node as a leaf and make rotations if needed to remain balanced tree- all the way to the root.
template<class T>
TreeNode<T> *TreeNode<T>::insert(TreeNode* node, const T& value) {
    if (node == NULL){
        node = new TreeNode(value);
    }

    if (node->key < value){
        node->right = insert(node->right, value);
        if (get_balance_factor(node) == 2){
            if (get_balance_factor(node->left) == -1){
                node = rotate_LR(node);
            }
            else node = rotate_LL(node);
        }
    }

    if (node->key > value){
        node->left = insert(node->left, value);
        if (get_balance_factor(node) == -2){
            if (get_balance_factor(node->right) == 1){
                node = rotate_RL(node);
            }
            else node = rotate_RR(node);
        }
    }

    updateHeight(node);
    return node;
}
// Find min value and return it's node.
template<class T>
TreeNode<T> *TreeNode<T>::findMin(TreeNode *v) {
    if ( v == NULL){
        return NULL;
    }
        //min is only on sub left tree
    else if (v->left == NULL){
        return v;
    }
        //min will be found on subtree on the left only
    else return findMin(v->left);
}

// Delete a node and re-balance the tree - all the way to the root.
template<class T>
TreeNode<T>* TreeNode<T>::remove(const T& value, TreeNode *v) {
    TreeNode* temp;

    // if value not found, remove returns NULL.
    if (v == NULL){
        return NULL;
    }

        // binary search for the element. ALL CASES ONLY FOR FOUND ELEMENT
        // case1: value to be found is smaller than current value
    else if ( value < v->key){
        v->left = remove(value, v->left);
    }
        // case2: value to be found is greater than current value
    else if ( value > v->key){
        v->right = remove(value, v->right);
    }
        // case3.1: element is found with two subtrees
    else if (v->left != NULL && v->right != NULL){
        //find the new head of the right subtree
        temp = findMin(v->right);
        //copy the min element in right subtree
        v->key = temp->key;
        //delete other(!) element which we have just copied.
        v->right = remove(v->key, v->right);
    }
        // case3.2: element is found with one or none subtrees
    else {
        temp = v;
        if (v->left == NULL){
            v = v->right;
        }
        else if (v->right == NULL){
            v = v->left;
        }
        delete temp;
    }
    //in case v is the last on any subtree;
    if ( v == NULL){
        return v;
    }
    //update new heights:
    v->updateHeight();

    // recursively rotations if needed, from bottom -> up:
    // case1: if left node is deleted, right is heavier, rotation to left
    if (get_balance_factor(v) == 2){
        if (get_balance_factor(v->left) == -1){
            return rotate_LR(v);
        }
        else return rotate_LL(v);
    }
        // case2: if right node is deleted, left is heavier, rotation to right
    else if (get_balance_factor(v) == -2){
        if (get_balance_factor(v->right) == 1){
            return rotate_RL(v);
        }
        else return rotate_RR(v);
    }
    return v;
}

// When lowest node is on the LL path from v.
template<class T>
TreeNode<T> *TreeNode<T>::rotate_LL(TreeNode *v) {
    return right_rotate(v) ;
}

// When lowest node is on the RR path from v.
template<class T>
TreeNode<T> *TreeNode<T>::rotate_RR(TreeNode *v) {
    return left_rotate(v) ;
}

// When lowest node is on the RL path from v.
template<class T>
TreeNode<T> *TreeNode<T>::rotate_RL(TreeNode *v) {
    TreeNode* vr = v->right;
    v->right = right_rotate(vr);
    return left_rotate(v) ;
}

// When lowest node is on the LR path from v.
template<class T>
TreeNode<T> *TreeNode<T>::rotate_LR(TreeNode *v) {
    TreeNode* vl = v->left;
    v->left = left_rotate(vl);
    return right_rotate(v) ;
}

// performing left rotation when A was the src root and then B is the final root.
template<class T>
TreeNode<T> *TreeNode<T>::left_rotate(TreeNode *A) {
    //Sub-trees and nodes to be modified
    TreeNode* B= A->right;
    TreeNode* Bl = B->left;

    //make rotation
    B->left = A;
    A->right= Bl;

    //update height
    A->updateHeight();
    B->updateHeight();

    //return new root
    return B;
}

// performing right rotation when B was the src root and then A is the final root.
template<class T>
TreeNode<T> *TreeNode<T>::right_rotate(TreeNode *B) {
    //Sub-trees and nodes to be modified
    TreeNode* A = B->left;
    TreeNode* Ar = A->right;

    //make rotation
    A->right = B;
    B->left = Ar;

    //update height
    A->updateHeight();
    B->updateHeight();

    //return new root
    return A;
}

// Function to get the maximum of two integers.
int max(int a, int b){
    return a > b ? a : b ;
}


// PUBLIC METHODS:
template<class T>
void TreeNode<T>::remove(const T &value) {
    root = remove(value,root);
}

template<class T>
void TreeNode<T>::insert(const T &value) {
    root = insert(value, root);
}


// Getters:
template<class T>
int TreeNode<T>::getHeight() {
    if (this == NULL) return -1;
    return height;
}

template<class T>
TreeNode<T> *TreeNode<T>::getLeft() {
    return left;
}

template<class T>
TreeNode<T> *TreeNode<T>::getRight() {
    return right;
}

template<class T>
T TreeNode<T>::getKey() {
    return key;
}

#endif //WET1_AVL_H
