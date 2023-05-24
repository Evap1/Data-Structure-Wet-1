//
// Created by Eva Poluliakhov on 13/05/2023.
//

#ifndef WET1_AVL_H
#define WET1_AVL_H

#include<iostream>
using namespace std;

//TODO: test this whole thing
//TODO: operator = not really because it's pointers
//TODO: DELETE PRINT FUNCTIONS

// AVL Tree
template <class T>
class TreeNode
{
public:

    TreeNode();
    ~TreeNode();

    // Getters
    int getHeight();
    int getCounter();

    // Only visible operations:
    void insert(T value);
    void remove(const T& value);
    void print_tree();
    void printBT(const TreeNode& tree);


private:
    class Node{
    public:
        explicit Node(T value) : key(value), left(NULL), right(NULL), height(0) {}
        T key;
        Node* left;
        Node* right;
        int height;
    };

    Node *root;
    int elementsCount;

    // Rotation methods
    int get_balance_factor(Node *v);
    void updateHeight(Node* v);
    Node* right_rotate(Node* B);
    Node* left_rotate(Node* A);
    Node* rotate_LL(Node *v);
    Node* rotate_RL(Node *v);
    Node* rotate_LR(Node *v);
    Node* rotate_RR(Node *v);
    Node* balance_Tree(Node* node);


    // additional methods
    Node* insert(Node* node, const T& value);
    Node* find(const T& value);
    Node* findMin(Node* v);
    Node* remove(const T& value, Node *v);
    void delete_tree(Node* v);
    void inorder(Node* v);
    void printBT(const std::string& prefix, const Node* node, bool isLeft);
};

// PRIVATE METHODS:

// private constructor to be accessed once: defines our root to be empty.
template <class T>
TreeNode<T>::TreeNode() :
        root(NULL),
        elementsCount(0)
        {}

// Destructor
// ATTENTION may not work, make sure valngrid
// maybe void recursion
template <class T>
TreeNode<T>::~TreeNode(){
    delete_tree(root);
}

// Additional helper method for destructor. Recursive method since we have no parent pointers and
//  destructor doesn't have return value
template<class T>
void TreeNode<T>::delete_tree(Node *v) {
    if (v == NULL){
        return;
    }
    delete_tree(v->left);
    delete_tree(v->right);
    delete v;
}
// dealing with possibility that we are too low in the tree
// works as a setter
template<class T>
void TreeNode<T>::updateHeight(Node* v) {
    if (v->left == NULL){
        if (v->right == NULL){
        }
        else v->height = v->right->height + 1;
    }
    else if (v->right == NULL){
        v->height = v->left->height + 1;
    }
    else v->height = 1 + max(v->left->height, v->right->height);
}

// Get the balance factor of a node
template<class T>
int TreeNode<T>::get_balance_factor(Node *v) {
    if (v == NULL)
        return 0;
    int l = 0, r = 0;

    if (v->left == NULL)
    {
        l = -1;
        if (v->right == NULL) return 0;
        else r = v->right->height;
    }
    else if (v->right == NULL)
    {
        r = -1;
        l = v->left->height;
    }
    else
    {
        l = v->left->height;
        r = v->right->height;
    }

    return l - r;
}

// Method to determine whether some value already exist
template<class T>
typename TreeNode<T>::Node* TreeNode<T>::find(const T &value) {
    if (this == NULL) return NULL;

    if (this->key == value) return this;

    if (this->key < value) {
        this = this->right;
    }

    this = this->left;

    return find(value);
}

// Insert new node as a leaf and make rotations if needed to remain balanced tree- all the way to the root.
template<class T>
typename TreeNode<T>::Node *TreeNode<T>::insert(Node* node, const T& value) {
    if (node == NULL){
        node = new Node(value);
        elementsCount++;
    }

    if (node->key < value){
        node->right = insert(node->right, value);
    }

    if (node->key > value){
        node->left = insert(node->left, value);
    }

    node = balance_Tree(node);
    updateHeight(node);
    return node;
}


template <class T>
typename TreeNode<T>::Node* TreeNode<T>::balance_Tree(Node* node)
{
    if (get_balance_factor(node) == -2){
            if (get_balance_factor(node->right) == 1){
                node = rotate_RL(node);
            }
            else node = rotate_RR(node);
        }

        if (get_balance_factor(node) == 2){
            if (get_balance_factor(node->left) == -1){
                node = rotate_LR(node);
            }
            else node = rotate_LL(node);
        }
     return node;
}


// Find min value and return it's node.
template<class T>
typename TreeNode<T>::Node* TreeNode<T>::findMin(Node *v) {
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
typename TreeNode<T>::Node* TreeNode<T>::remove(const T& value, Node *v) {
    Node* temp;

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
    updateHeight(v);

    //rotations if needed, from bottom -> up:
    v = balance_Tree(v);
    return v;
}

// When lowest node is on the LL path from v.
template<class T>
typename TreeNode<T>::Node* TreeNode<T>::rotate_LL(Node *v) {
    return right_rotate(v) ;
}

// When lowest node is on the RR path from v.
template<class T>
typename TreeNode<T>::Node* TreeNode<T>::rotate_RR(Node *v) {
    return left_rotate(v) ;
}

// When lowest node is on the RL path from v.
template<class T>
typename TreeNode<T>::Node* TreeNode<T>::rotate_RL(Node *v) {
    Node* vr = v->right;
    v->right = right_rotate(vr);
    return left_rotate(v) ;
}

// When lowest node is on the LR path from v.
template<class T>
typename TreeNode<T>::Node* TreeNode<T>::rotate_LR(Node *v) {
    Node* vl = v->left;
    v->left = left_rotate(vl);
    return right_rotate(v) ;
}

// performing left rotation when A was the src root and then B is the final root.
template<class T>
typename TreeNode<T>::Node* TreeNode<T>::left_rotate(Node *A) {
    //Sub-trees and nodes to be modified
    Node* B= A->right;
    Node* Bl = B->left;

    //make rotation
    B->left = A;
    A->right= Bl;

    //update height
    updateHeight(A);
    updateHeight(B);

    //return new root
    return B;
}

// performing right rotation when B was the src root and then A is the final root.
template<class T>
typename TreeNode<T>::Node* TreeNode<T>::right_rotate(Node *B) {
    //Sub-trees and nodes to be modified
    Node* A = B->left;
    Node* Ar = A->right;

    //make rotation
    A->right = B;
    B->left = Ar;

    //update height
    updateHeight(A);
    updateHeight(B);

    //return new root
    return A;
}

// Function to get the maximum of two integers.
int max(int a, int b){
    return a > b ? a : b ;
}

// Print function to show the current tree. left, root, right.
template<class T>
void TreeNode<T>::inorder(Node *v) {
    if (v == NULL) return;
    inorder(v->left);
    cout << v->key << " , ";
    inorder(v->right);
}

// PUBLIC METHODS:
template<class T>
void TreeNode<T>::remove(const T &value) {
    // removing one element only if exists
    if (find(value)!= NULL){
        elementsCount--;
    }
    root = remove(value,root);
}

template<class T>
void TreeNode<T>::insert(T value) {
    root = insert(root, value);
}

template<class T>
void TreeNode<T>::print_tree() {
    inorder(root);
    cout << endl;
}

// Getters:
template<class T>
int TreeNode<T>::getHeight() {
    if (this == NULL) return -1;
    return this->height;
}

template<class T>
int TreeNode<T>::getCounter() {
    return this->elementsCount;
}

template<class T>
void TreeNode<T>::printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->key << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->left, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

template<class T>
void TreeNode<T>::printBT(const TreeNode& tree)
{
    printBT("", tree.root, false);    
}




#endif //WET1_AVL_H
