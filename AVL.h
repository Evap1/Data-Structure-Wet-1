//
// Created by Eva Poluliakhov on 13/05/2023.
//

#ifndef WET1_AVL_H
#define WET1_AVL_H

#include<iostream>
using namespace std;

//TODO: test this whole thing
//TODO: DELETE PRINT FUNCTIONS

// Implementation later on
template <class T>
class TreeNode;

// Node
template <class T>
class Node{
public:
    friend class TreeNode<T>;
    explicit Node(const T& value) : key(value), left(NULL), right(NULL), height(0) {}
    Node() : key(NULL), left(NULL), right(NULL), height(0) {}
    ~Node() = default;
    Node<T>& operator = (const Node<T>&other);

    const T& get_key() {return key;};
    const T& get_key() const{return key;};

    const Node<T>* get_left() {return left;};
    const Node<T>* get_left() const {return left;};

    const Node<T>* get_right() {return right;};
    const Node<T>* get_right() const {return right;};




private:
    T key;
    Node<T>* left;
    Node<T>* right;
    int height;
};

// AVL Tree
template <class T>
class TreeNode
{
private:
    Node<T> *root;
    int elementsCount;

    // Rotation methods
    int get_balance_factor(Node<T> *v);
    void updateHeight(Node<T>* v);
    Node<T>* right_rotate(Node<T>* B);
    Node<T>* left_rotate(Node<T>* A);
    Node<T>* rotate_LL(Node<T> *v);
    Node<T>*  rotate_RL(Node<T>  *v);
    Node<T>*  rotate_LR(Node<T> *v);
    Node<T>*  rotate_RR(Node<T> *v);
    Node<T>*  balance_Tree(Node<T>* node);

    // additional methods
    Node<T>* insert(Node<T>* node, const T& value);
    Node<T>* find(Node<T>* v, const T &value);
    Node<T>* findMin(Node<T>* v);
    bool find_by_id(int value);
    Node<T>* remove(const T& value, Node<T> *v);
    void delete_tree(Node<T>* v);
    void inorder(Node<T>* v);
    int list_inorder_aux( Node<T>* v, Node<T> **arr, int index);
    void printBT(const std::string& prefix, const Node<T>* node, bool isLeft);

public:
    TreeNode();
    ~TreeNode();

    // Getters
    int get_height();
    int get_counter();
    Node<T>* get_root();

    // Only visible operations:
    StatusType insert(const T& value);
    StatusType remove(const T& value);
    Node<T>* find(const T& value);
    void list_inorder(Node<T>** arr);
    void print_tree();
    void printBT(const TreeNode& tree);
};

// -----------------------------------------PRIVATE METHODS----------------------------------------

/// @brief Node operator= overload
template<class T>
Node<T> &Node<T>::operator=(const Node<T> &other) {
    if (this == other) return *this;
    key = other.key;
    left = other.left;
    right = other.right;
    height = other.height;
    return *this;
}

// -----------------------------------------Constructor----------------------------------------
// @brief private constructor to be accessed once: defines our root to be empty.
template <class T>
TreeNode<T>::TreeNode() :
        root(NULL),
        elementsCount(0)
        {}


// -----------------------------------------Destructor----------------------------------------
// TODO: make sure valngrid
template <class T>
TreeNode<T>::~TreeNode(){
    delete_tree(root);
}

/// @brief Additional helper method for destructor. Recursive method since we have no parent pointers and
/// @tparam T
/// @param v
//TODO: is it ok to be void? i think yes but make sure.
template<class T>
void TreeNode<T>::delete_tree(Node<T> *v) {
    if (v == NULL){
        return;
    }
    delete_tree(v->left);
    delete_tree(v->right);
    delete v;
}


/// @brief updating the height of node v. dealing with possibility that we are too low in the tree
/// @tparam T - class type of the node.
/// @param v - node to update
template<class T>
void TreeNode<T>::updateHeight(Node<T>* v) {
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

/// @brief Get the balance factor of a node
/// @tparam T
/// @param v
/// @return integer in the range of [-2,-1,0,1,2]
template<class T>
int TreeNode<T>::get_balance_factor(Node<T> *v) {
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

/// @brief Method to determine whether some value already exist
/// @tparam T - the class type
/// @param v - usually called with root with public func find.
/// @param value - to find.
/// @return  - NULL if not found , the ptr to element if found.
template<class T>
Node<T>* TreeNode<T>::find(Node<T>* v, const T &value) {
    if (v == NULL) return NULL;

    if (v->key == value){
        return v;
    }
    else if (v->key < value){
        return find(v->right, value);
    }
    else{
        return find(v->left, value);
    }
}
/// @brief Insert new node as a leaf and make rotations if needed to remain balanced tree- all the way to the root.
/// @tparam T - the class type
/// @param node - usually called with root with public func find.
/// @param value - to insert
/// @return - ptr to the original node.
template<class T>
Node<T> *TreeNode<T>::insert(Node<T>* node, const T& value) {
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

/// @brief check if the current node is balance and rotate if needed
/// @tparam T
/// @param node
/// @return balanced node
template <class T>
Node<T>* TreeNode<T>::balance_Tree(Node<T>* node)
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

/// @brief Find min value and return it's node.
/// @tparam T
/// @param v
/// @return min node in the subtrees of v
template<class T>
Node<T>* TreeNode<T>::findMin(Node<T> *v) {
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
Node<T>* TreeNode<T>::remove(const T& value, Node<T> *v) {
    Node<T>* temp;

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
    // case3.1: element is found with two subtrees, we make copies and "roll" until
    //  we reach the level of a leaf. and make deletion the the last else.
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
        //TODO: is there a way to determine if delete didnt work?
        delete temp;
    }

    //in case v is the last on any subtree;
    if ( v == NULL ){
        return v;
    }
    //update new heights:
    updateHeight(v);

    //rotations if needed, from bottom -> up:
    v = balance_Tree(v);
    return v;
}


// ---------------------------------------------ROTATION METHODS-------------------------------

// When lowest node is on the LL path from v.
template<class T>
Node<T>* TreeNode<T>::rotate_LL(Node<T> *v) {
    return right_rotate(v) ;
}

// When lowest node is on the RR path from v.
template<class T>
Node<T>* TreeNode<T>::rotate_RR(Node<T> *v) {
    return left_rotate(v) ;
}

// When lowest node is on the RL path from v.
template<class T>
Node<T>* TreeNode<T>::rotate_RL(Node<T> *v) {
    Node<T>* vr = v->right;
    v->right = right_rotate(vr);
    return left_rotate(v) ;
}

// When lowest node is on the LR path from v.
template<class T>
Node<T>* TreeNode<T>::rotate_LR(Node<T> *v) {
    Node<T>* vl = v->left;
    v->left = left_rotate(vl);
    return right_rotate(v) ;
}

// performing left rotation when A was the src root and then B is the final root.
template<class T>
Node<T>* TreeNode<T>::left_rotate(Node<T> *A) {
    //Sub-trees and nodes to be modified
    Node<T>* B= A->right;
    Node<T>* Bl = B->left;

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
Node<T>* TreeNode<T>::right_rotate(Node<T> *B) {
    //Sub-trees and nodes to be modified
    Node<T>* A = B->left;
    Node<T>* Ar = A->right;

    //make rotation
    A->right = B;
    B->left = Ar;

    //update height
    updateHeight(A);
    updateHeight(B);

    //return new root
    return A;
}

/// @param a
/// @param b
/// @return the greater integer
int max(int a, int b){
    return a > b ? a : b ;
}

/// @brief recursive helper for the outward method. PRIVATE
/// @tparam T
/// @param v
/// @param arr
/// @param index (!) - always need be called with 0
/// @return the current index to fill
template<class T>
int TreeNode<T>::list_inorder_aux(Node<T>* v, Node<T> **arr, int index) {
    if (v ==NULL) return index;

    index = list_inorder_aux(v->left, arr, index);
    arr[index++] = v;
    list_inorder_aux(v->right, arr, index);
    return index;
}

// TODO: check memory free
// -----------------------------------------PUBLIC METHODS-----------------------------------------

/// @brief Store inordered tree into a given array. PUBLIC
/// @tparam T - type of the elements
/// @param arr - arr of the elements
template<class T>
void TreeNode<T>::list_inorder(Node<T> **arr) {
    list_inorder_aux(root, arr, 0);
}

/// @brief remove one element with a value.
/// @tparam T -class type
/// @param value - to be removed
/// @return Status of the operation.
template<class T>
StatusType TreeNode<T>::remove(const T &value) {
    // removing one element only if exists
    if (find(root, value) != NULL){
        try {
            root = remove(value,root);
        }
        catch(...){
            return StatusType::ALLOCATION_ERROR;
        }
        elementsCount--;
        return StatusType::SUCCESS;
    }
    // value not found:
    return StatusType::FAILURE;
}

/// @brief insert one element with a value.
/// @tparam T -class type
/// @param value - to be added.
/// @return Status of the operation.
template<class T>
StatusType TreeNode<T>::insert(const T& value) {
    // inserting one element only if NOT exists
    if (find(root, value) == NULL){
        try {
            root = insert(root, value);
        }
        catch (const std::bad_alloc&){
            return StatusType::ALLOCATION_ERROR;
        }
        return StatusType::SUCCESS;
    }
    // already exists
    return StatusType::FAILURE;
}

/// @brief search for an element
/// @tparam T - class
/// @param value - to search
/// @return - NULL if not found , the ptr to element if found.
template<class T>
Node<T> *TreeNode<T>::find(const T &value) {
    return find(root,value);
}

/// @brief search for an element BY ID ONLY
/// @tparam T - class
/// @param value - to search
/// @return - NULL if not found , the ptr to element if found.
template<class T>
bool TreeNode<T>::find_by_id(int value) {
    if (find(root,value) == NULL) return false;
    return true;
}

// -----------------------------------------Getters-----------------------------------------
template<class T>
int TreeNode<T>::get_height() {
    if (this == NULL) return -1;
    return this->height;
}

template<class T>
int TreeNode<T>::get_counter() {
    return this->elementsCount;
}

template<class T>
Node<T> *TreeNode<T>::get_root() {
    return root;
}
// -----------------------------------------PRINT METHODS-----------------------------------------
// TODO: dont forget to delete
template<class T>
void TreeNode<T>::printBT(const std::string& prefix, const Node<T>* node, bool isLeft)
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
void TreeNode<T>::print_tree() {
    inorder(root);
    cout << endl;
}

template<class T>
void TreeNode<T>::printBT(const TreeNode& tree)
{
    printBT("", tree.root, false);    
}

// Print function to show the current tree. left, root, right.
template<class T>
void TreeNode<T>::inorder(Node<T> *v) {
    if (v == NULL) return;
    inorder(v->left);
    cout << v->key << " , ";
    inorder(v->right);
}

#endif //WET1_AVL_H