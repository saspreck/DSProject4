#pragma once
#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <stack>
#include "Enumerator.h"
#include "AVLTreeOrder.h"

template <typename S>
class AVLTree;

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
private:
	AVLTreeOrder order;
	std::stack<const AVLTree<T>*> traversalStack;
	void buildTraversalStack(const AVLTree<T>* current);
	void buildInOrder(const AVLTree<T>* current);
	void buildPreOrder(const AVLTree<T>* current);
	void buildPostOrder(const AVLTree<T>* current);

public:
	AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order = AVLTreeOrder::inorder);
	virtual ~AVLTreeEnumerator();
	bool hasNext() const;
	T next();                        // throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;                  // throws ExceptionEnumerationBeyondEnd if no next item is available
	AVLTreeOrder getOrder();         // returns the order of this enumerator (preorder, inorder, or postorder)
};

// Add your implementation below this line.

/*
Constructor for an AVLTreeEnumerator, takes a root node and the order
*/
template <typename T>
AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order) {

	//if root contains nothing throw exception
	if (root->empty) {
		throw new ExceptionAVLTreeAccess;
	}
	
	//set order and build stack in that order
	this->order = order;
	buildTraversalStack(root);
}

/*
Destructor for the enumerator
*/
template <typename T>
AVLTreeEnumerator<T>::~AVLTreeEnumerator() {
	
}


/*
Method to see if enumerator has next element
*/
template <typename T>
bool AVLTreeEnumerator<T>::hasNext() const {
	
	//checks if stack is empty and returns appropriate bool
	if (traversalStack.empty()) {
		return false;
	}
	else {
		return true;
	}
}


/*
Method to get the next element in the stack
*/
template <typename T>
T AVLTreeEnumerator<T>::next() {

	//if stack empty throw exception
	if (traversalStack.empty()) {
		throw new ExceptionEnumerationBeyondEnd;
	}

	//else pop element off stack
	else {
		const AVLTree<T>* tempRecord = traversalStack.top();
		traversalStack.pop();
		return tempRecord->data;
	}
}


/*
Method to get next element with out removing it
*/
template <typename T>
T AVLTreeEnumerator<T>::peek() const {

	//if stack empty throw exception
	if (traversalStack.empty()) {
		throw new ExceptionEnumerationBeyondEnd;
	}

	//else return top element without removing
	else {
		const AVLTree<T>* tempRecord = traversalStack.top();
		return tempRecord->data;
	}
}


/*
Method to return the current order of the enumerator
*/
template <typename T>
AVLTreeOrder AVLTreeEnumerator<T>::getOrder() {
	return order;
}


/*
Method to build the traversal stack for the tree
*/
template <typename T>
void AVLTreeEnumerator<T>::buildTraversalStack(const AVLTree<T>* current) {

	//branch for a preorder traversal
	if (order == AVLTreeOrder::preorder) {
		buildPreOrder(current);
	}

	//branch for an in order traversal
	else if (order == AVLTreeOrder::inorder) {
		buildInOrder(current);
	}

	//branch for a post order traversal
	else if (order == AVLTreeOrder::postorder) {
		buildPostOrder(current);
	}
}


/*
Method to build the traversal stack in order
*/
template <typename T>
void AVLTreeEnumerator<T>::buildInOrder(const AVLTree<T>* current) {
	
	//if right child exists, recursion onto that child
	if (current->right != nullptr) {
		buildInOrder(current->right);
	}

	//insert when to farthest right child
	if (!current->empty) {
		traversalStack.push(current);
	}

	//if left child exists, recursion onto that child
	if (current->left != nullptr) {
		buildInOrder(current->left);
	}
}


/*
Method to build the traversal stack pre order
*/
template<typename T>
void AVLTreeEnumerator<T>::buildPreOrder(const AVLTree<T>* current) {

	//if right child exists, call function on it
	if (current->right != nullptr) {
		buildPreOrder(current->right);
	}

	//if left child exists, call function on it
	if (current->left != nullptr) {
		buildPreOrder(current->left);
	}

	//if no children exist, push current object
	if (!current->empty) {
		traversalStack.push(current);
	}
}


/*
Method to build traversal stack in post order
*/
template <typename T>
void AVLTreeEnumerator<T>::buildPostOrder(const AVLTree<T>* current) {

	//push current node
	if (!current->empty) {
		traversalStack.push(current);
	}

	//if right child exists, call function on it
	if (current->right != nullptr) {
		buildPostOrder(current->right);
	}

	//if left child exists, call function on it
	if (current->left != nullptr) {
		buildPostOrder(current->left);
	}
}


#endif // !AVL_TREE_ENUMERATOR