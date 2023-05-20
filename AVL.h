//
// Created by Eva Poluliakhov on 13/05/2023.
//

#ifndef WET1_AVL_H
#define WET1_AVL_H

#include<iostream>
using namespace std;

//TODO: test this whole thing
//TBD operator = not really because it's pointers
// AVL TreeNode
template <class T>
class TreeNode
{
public:
    explicit TreeNode(const T& value);
    ~TreeNode();

    //getters
    int getHeight();
    TreeNode* getLeft();
    TreeNode* getRight();
    T getKey();
    int get_balance_factor(TreeNode *current);

    //setters
    void setLeft(TreeNode* other);
    void setRight(TreeNode* other);
    void updateHeight();

    //Rotation methods
    TreeNode* right_rotate(TreeNode* B);
    TreeNode* left_rotate(TreeNode* A);
    TreeNode* rotate_LL(TreeNode *v);
    TreeNode* rotate_RL(TreeNode *v);
    TreeNode* rotate_LR(TreeNode *v);
    TreeNode* rotate_RR(TreeNode *v);

    //additional methods
    TreeNode* insert(TreeNode* node, const T& value);
    TreeNode* find(const T& value);
    TreeNode* findMin(TreeNode* v);
    TreeNode* remove(const T& value, TreeNode *v);

private:

    T key;
    TreeNode *left;
    TreeNode *right;
    int height;
    int weight;
};

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

template<class T>
TreeNode<T>* TreeNode<T>::remove(const T& value, TreeNode *v) {
    TreeNode* temp;

    //if value not found, remove returns NULL.
    if (v == NULL){
        return NULL;
    }

    //binary search for the element. ALL CASES ONLY FOR FOUND ELEMENT
    //case1: value to be found is smaller than current value
    else if ( value < v->key){
        v->left = remove(value, v->left);
    }
    //case2: value to be found is greater than current value
    else if ( value > v->key){
        v->right = remove(value, v->right);
    }
    //case3.1: element is found with two subtrees
    else if (v->left != NULL && v->right != NULL){
        //find the new head of the right subtree
        temp = findMin(v->right);
        //copy the min element in right subtree
        v->key = temp->key;
        //delete other(!) element which we have just copied.
        v->right = remove(v->key, v->right);
    }
    //case3.2: element is found with one or none subtrees
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

    //recursively rotations if needed, from bottom -> up:
    //case1: if left node is deleted, right is heavier, rotation to left
    if (get_balance_factor(v) == 2){
        if (get_balance_factor(v->left) == -1){
            return rotate_LR(v);
        }
        else return rotate_LL(v);
    }
    //case2: if right node is deleted, left is heavier, rotation to right
    else if (get_balance_factor(v) == -2){
        if (get_balance_factor(v->right) == 1){
            return rotate_RL(v);
        }
        else return rotate_RR(v);
    }
    return v;
}

//constructor
template <class T>
TreeNode<T>::TreeNode(const T& value) :
        key(value),
        left(NULL),
        right(NULL),
        height(0),
        weight(1)
{}

//destructor
//ATTENTION may not work, make sure valngrid
//maybe void recursion
template <class T>
TreeNode<T>::~TreeNode(){
    if (left != NULL){
        delete left;
    }
    if (right != NULL){
        delete right;
    }
}

template<class T>
void TreeNode<T>::setRight(TreeNode *other) {
    right = other;
}

template<class T>
void TreeNode<T>::setLeft(TreeNode *other) {
    left = other;
}

//dealing with possibility that we are too low in the tree
//works as a setter
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

//Method to determine whether some value already exist
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

//Insert new node as a leaf and make rotations if needed to remain balanced tree
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

//When lowest node is on the LL path form v
template<class T>
TreeNode<T> *TreeNode<T>::rotate_LL(TreeNode *v) {
    return right_rotate(v) ;
}

//When lowest node is on the RR path form v
template<class T>
TreeNode<T> *TreeNode<T>::rotate_RR(TreeNode *v) {
    return left_rotate(v) ;
}

//When lowest node is on the RL path form v
template<class T>
TreeNode<T> *TreeNode<T>::rotate_RL(TreeNode *v) {
    TreeNode* vr = v->right;
    v->right = right_rotate(vr);
    return left_rotate(v) ;
}

//When lowest node is on the LR path form v
template<class T>
TreeNode<T> *TreeNode<T>::rotate_LR(TreeNode *v) {
    TreeNode* vl = v->left;
    v->left = left_rotate(vl);
    return right_rotate(v) ;
}

//performing left rotation when A was the src root and then B is the final root.
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

//performing right rotation when B was the src root and then A is the final root.
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

// Function to get the maximum of two integers
int max(int a, int b){
    return a > b ? a : b ;
}


#endif //WET1_AVL_H
