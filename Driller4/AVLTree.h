#pragma once
#ifndef AVL_TREE
#define AVL_TREE

#include "Comparator.h"
#include "AVLTreeEnumerator.h"

template <typename T>
class AVLTree {
	template <typename U>
	friend class AVLTreeEnumerator;
private:
	Comparator<T>* comparator = NULL;               // used to determine tree order and item equality
	unsigned long size = 0;                         // actual number of items currently in tree
	T data{};                                       // data item of any type
	bool empty = true;                              // flag to indicate whether node contains data
	int diff = 0;                                   // height of right minus height of left
	AVLTree<T>* left = NULL;                        // pointer to left subtree
	AVLTree<T>* right = NULL;                       // pointer to right subtree
	void zig();                                     // right rotation
	void zag();                                     // left rotation
	void zigzag();                                  // left rotation on left subtree, followed by right rotation
	void zagzig();                                  // right rotation on right subtree, followed by left rotation
	void rebalance();                               // check for and rebalance this node, if needed
public:
	AVLTree(Comparator<T>* comparator);             // creates empty linked tree with comparator
	virtual ~AVLTree();                             // deletes subtrees

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave tree unchanged and return false
	bool insert(const T item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool replace(const T item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool remove(const T item);

	// if an equivalent item is present, return true
	// if an equivalent item is not present, false
	bool contains(T item) const;

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionLinkedtreeAccess
	T find(const T item) const;

	unsigned long getSize() const;                  // returns the current number of items in the tree

	AVLTreeEnumerator<T> enumerator(AVLTreeOrder order = AVLTreeOrder::inorder) const;    // create an enumerator for this AVL tree
};

// Add your implementation below this line.

#endif // !AVL_TREE

/*
Constructor for a tree, takes a comparator parameter
*/
template <typename T>
AVLTree<T>::AVLTree(Comparator<T>* comparator) {
	this->comparator = comparator;
}


/*
Destructor for a tree, deletes all the subtrees
*/
template <typename T>
AVLTree<T>::~AVLTree() {
	delete left;
	delete right;
	left = nullptr;
	right = nullptr;
}


/*
Method to perform a right rotation
NOTE: This code is copied from the document in Canvas
*/
template <typename T>
void AVLTree<T>::zig() {

	if (left == NULL) {  // no left, no way to zig
		return;
	}
	
	// keep track of diff of node and left for changes
	int gdiff = diff;
	int pdiff = left->diff;
	
	// modify the tree
	AVLTree<T>* olnr = left;  
	
	// olnr is "old left, new right"
	left = olnr->left;
	olnr->left = olnr->right;
	olnr->right = right;
	right = olnr;
	
	// note that the modifications kept the node itself in place, so we need to swap its data with its olnr's
	T* tempData = new T(data);
	data = olnr->data;
	olnr->data = *tempData;
	
	// update the diff fields for node and new right
	if (pdiff < 0) {  // if imbalance was outside left heavy (left-left violation)
		diff = gdiff + 2;
		right->diff = gdiff - pdiff + 1;
	}
	else {  // otherwise imbalance was inside left heavy (left-right violation)
		diff = pdiff + 1;
		right->diff = gdiff + 1;
	}
}


/*
Method to perform a left rotation
NOTE: This code is copied from the documents on Canvas
*/
template<typename T>
void AVLTree<T>::zag(){

	if (right == NULL) {  // no right, no way to zig
		return;
	}
	
	// keep track of diff of node and right for changes
	int gdiff = diff;
	int pdiff = right->diff;
	
	// modify the tree
	AVLTree<T>* ornl = right;  
	
	// ornl is "old right, new left"
	right = ornl->right;
	ornl->right = ornl->left;
	ornl->left = left;
	left = ornl;
	
	// note that the modifications kept the node itself in place, so we need to swap its data with its ornl's
	T* tempData = new T(data);
	data = ornl->data;
	ornl->data = *tempData;
	
	// update the diff fields for node and new left
	if (pdiff > 0) {  // if imbalance was outside right heavy (right-right violation)
		diff = gdiff - 2;
		left->diff = gdiff - pdiff - 1;
	}
	
	else {  // otherwise imbalance was inside right heavy (right-left violation)
		diff = pdiff - 1;
		left->diff = gdiff - 1;
	}
}


/*
Method to perform a left and then a right rotation
*/
template <typename T>
void AVLTree<T>::zigzag() {

	left->zag();
	zig();
}


/*
Method to perform a right rotation and then a left rotation
*/
template <typename T>
void AVLTree<T>::zagzig() {

	right->zig();
	zag();
}


/*
Method to check which rotation needs to be performed
NOTE: This code is largely taken from the code in the lecture slides
*/
template <typename T>
void AVLTree<T>::rebalance() {

	//check if difference is within acceptable range
	if (diff >= -1 && diff <= 1) {
		return;
	}

	//if left-left violation, call zig
	if (diff < 0 && left->diff <= 0 && left != nullptr) {
		zig();
	}

	//if left-right violation, call zigzag
	else if (diff < 0 && left->diff > 0 && left != nullptr) {
		zigzag();
	}

	//if right-right violation, call zag
	else if (diff > 0 && right->diff >= 0 && right != nullptr) {
		zag();
	}

	//if right-left violation, call zagzig
	else if (diff > 0 && right->diff < 0 && right != nullptr) {
		zagzig();
	}
}


/*
Method to insert an item into the tree
*/
template <typename T>
bool AVLTree<T>::insert(T item) {

	//if node empty, put item in node
	if (empty) {
		size++;
		data = item;
		empty = false;
		return true;
	}

	//if item already present, return false
	else if(comparator->compare(item, data) == 0) {
		return false;
	}

	//if item less than node, insert in left subtree
	else if (comparator->compare(item, data) == -1) {
		
		//check if left child exists
		if (left == nullptr) {

			//initialize left subtree
			left = new AVLTree<T>(comparator);

			//insert on left subtree
			left->data = item;
			left->size++;
			size++;
			left->empty = false;
			diff--;
			return true;
		}

		//branch if left child already exists
		if (left != nullptr) {

			int oldDiff = left->diff;

			//if inserted successfully, update diff and rebalance if needed
			if (left->insert(item)) {
				size++;

				//check if left difference has changed and update accordingly
				if (oldDiff != left->diff && oldDiff == 0) {
					diff--;
				}

				//check if tree needs rebalanced
				if (diff < -1) {
					rebalance();
				}
				return true;
			}
		}

	}
	//if item greater than node, insert in right subtree
	else if (comparator->compare(item, data) == 1) {
		
		//check if right child exists
		if (right == nullptr) {

			//initialize right subtree
			right = new AVLTree<T>(comparator);

			//insert on right subtree
			right->data = item;
			right->size++;
			size++;
			right->empty = false;
			diff++;
			return true;

		}

		//branch if right child already exists
		if (right != nullptr) {

			int oldDiff = right->diff;
			//if inserted successfully, update diff and rebalance if needed
			if (right->insert(item)) {
				size++;

				//check if right difference has changed and update accordingly
				if (oldDiff != right->diff && oldDiff == 0) {
					diff++;
				}

				//check if tree needs rebalanced
				if (diff > 1) {
					rebalance();
				}
				return true;
			}
		}
	}

	//return false if all tests fail
	return false;
}


/*
Method to replace an item in the tree
*/
template <typename T>
bool AVLTree<T>::replace(T item) {

	//if node empty, return false
	if (empty) {
		return false;
	}

	//if item equals node, replace and return true
	else if (comparator->compare(item, data) == 0) {
		data = item;
		return true;
	}

	//if item less than node, replace in left subtree
	else if (comparator->compare(item, data) == -1) {

		//if left is null return false
		if (left == nullptr) {
			return false;
		}
		
		return left->replace(item);
	}

	//if item greater than node, replace in right subtree
	else if (comparator->compare(item, data) == 1) {

		//if right is null return false
		if (right == nullptr) {
			return false;
		}

		return right->replace(item);
	}

	return false;
}


/*
Method to remove item from the tree
*/
template <typename T>
bool AVLTree<T>::remove(T item) {

	//if node empty, return false
	if (empty) {
		return false;
	}

	//if item already present, return false
	else if (comparator->compare(item, data) == 0) {

		//if leaf node, set to empty;
		if (left == nullptr && right == nullptr) {
			this->empty = true;
			size = 0;
		}

		//if no right child, replace with left child
		else if (right == nullptr && left != nullptr) {
			this->data = left->data;
			diff++;
			size--;
			left->remove(left->data);
		}

		//else use right child
		else {
			//assign temp tree to right child
			AVLTree<T>* tempTree = right;

			//find the farthest left element in the right subtree
			while (tempTree->left != nullptr) {
				tempTree = tempTree->left;
			}
			//copy its data and remove it
			int oldDiff = right->diff;
			this->data = tempTree->data;
			right->remove(tempTree->data);

			//update difference
			if (oldDiff != right->diff && oldDiff != 0) {
				diff--;
			}
			size--;

			//if right is empty delete it
			if (right->empty) {
				delete right;
				right = nullptr;
			}
		}

		return true;
	}

	//if item less than node, remove from left subtree
	else if (comparator->compare(item, data) == -1) {

		if (left == nullptr) {
			return false;
		}
		//if removed successfully, set left child to empty and rebalance
		int oldDiff = left->diff;
		if (left->remove(item)) {

			//update diff
			if (oldDiff != left->diff && left->diff == 0) {
				diff++;
			}

			//if left empty delete it
			if (left->empty) {
				delete left;
				left = nullptr;
			}
			size--;

			//check for rebalancing
			if (diff > 1) {
				rebalance();
			}

			return true;
		}

	}

	//if item greater than node, remove from right subtree
	else if (comparator->compare(item, data) == 1) {
		
		if (right == nullptr) {
			return false;
		}
		//if removed successfully, set right child to empty and rebalance
		int oldDiff = right->diff;
		if (right->remove(item)) {

			//update diff
			if (oldDiff != right->diff && right->diff == 0) {
				diff--;
			}

			//if right empty delete it
			if (right->empty) {
				delete right;
				right = nullptr;
			}
			size--;

			//check for rebalancing
			if (diff < -1) {
				rebalance();
			}

			return true;
		}
	}
	return false;
}


/*
Method to check if the tree contains an item
*/
template <typename T>
bool AVLTree<T>::contains(T item) const {

	//if node empty, return false
	if (empty) {
		return false;
	}

	//if item matches, return true
	else if (comparator->compare(item, data) == 0) {
		return true;
	}

	//if item less than node, check in left subtree
	else if (comparator->compare(item, data) == -1) {

		//if left null return false
		if (left == nullptr) {
			return false;
		}
		return left->contains(item);
	}

	//if item greater than node, check in right subtree
	else if (comparator->compare(item, data) == 1) {

		//if right null return false
		if (right == nullptr) {
			return false;
		}
		return right->contains(item);
	}

	return false;
}


/*
Method to find an item within the tree
*/
template <typename T>
T AVLTree<T>::find(const T item) const {

	//if node empty, throw exception
	if (empty) {
		throw new ExceptionAVLTreeAccess;
	}

	//if item already present, return item
	else if (comparator->compare(item, data) == 0) {
		return data;
	}

	//if item less than node, check in left subtree
	else if (comparator->compare(item, data) == -1) {

		//if left null throw exception
		if (left == nullptr) {
			throw new ExceptionAVLTreeAccess;
		}
		return left->find(item);
	}

	//if item greater than node, check in in right subtree
	else if (comparator->compare(item, data) == 1) {

		//if right null throw exception
		if (right == nullptr) {
			throw new ExceptionAVLTreeAccess;
		}
		return right->find(item);
	}

	else {
		throw new ExceptionAVLTreeAccess;
	}

}


/*
Method to get the size of the tree
*/
template <typename T>
unsigned long AVLTree<T>::getSize() const {
	return size;
}


/*
Method to return an enumerator for the tree
*/
template <typename T>
AVLTreeEnumerator<T> AVLTree<T>::enumerator(AVLTreeOrder order) const {
	return AVLTreeEnumerator<T>(this, order);
}
