#ifndef LIST_H
#define LIST_H

#include <iostream>

#include "ListNode.h"

template <typename T>
class List //linked list of ListNode objects
{
public:
	List();
	void insert(T); //fucntion used to insert new node in order in the list
	void print(); //prints the contents of the linked list
	ListNode<T> *startPtr; //stores the pointer of first object in the linked list
	ListNode<T>* search(T); // search values in the list
	void sort(); // sort the list by key
	void pop(T); // remove an item from the list
	void clear(); // clear the contents of the list
private:
	ListNode<T> *endPtr; //stored the pointer of the last object in the linked list
	bool isEmpty(); //utility functions used to see if the list contains no elements
};

template <typename T>
List<T>::List() //creates list with start and end as NULL
{
	startPtr = NULL;
	endPtr = NULL;
}

template <typename T>
void List<T>::clear()
{
	if (!isEmpty()) // List is not empty
	{
		ListNode<T> *currentPtr = startPtr;
		ListNode<T> *tempPtr;

		while (currentPtr != 0) // delete remaining nodes
		{
			tempPtr = currentPtr;
			currentPtr = currentPtr->nextPtr;
			delete tempPtr;
		}
	}
}

template <typename T>
bool List<T>::isEmpty()
{
	if (startPtr == NULL && endPtr == NULL) //if the start pointer and end pointer are NULL then the list is empty
		return 1;
	else
		return 0;
}

template <typename T>
void List<T>::insert(T dataIn)
{
	if (isEmpty()) //if the list is empty create first element of the list (same as first case in insert at begin)
	{
		ListNode<T> * newPtr = new ListNode<T>(dataIn);
		startPtr = newPtr;
		endPtr = newPtr;
	}
	else //if node(s) exist in the list then insert new node at the end of the list
	{
		ListNode<T> * newPtr = new ListNode<T>(dataIn);
		endPtr->nextPtr = newPtr; //the current last node's next pointer points to the new node
		endPtr = newPtr; //the new node is now the last node in the list
	}
}

template <typename T>
void List<T>::print()
{
	if (isEmpty())
	{
		std::cout << "The list is empty" << std::endl;

	}
	else
	{
		ListNode<T> * currentPtr = startPtr;

		std::cout << "The contents of the list is: ";
		while (currentPtr != NULL) //prints until the end of the list is reached
		{
			std::cout << currentPtr->data << ' ';
			currentPtr = currentPtr->nextPtr; //moves to next node in list
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

template <typename T>
ListNode<T>* List<T>::search(T key) //search functions that searches for node that contains data equal to the key
{
	ListNode<T>* nodePtr;
	bool found = false;

	nodePtr = startPtr;

	while ((!found) && (nodePtr != NULL)) //runs through list until data is found within a node or end of list is reached
	{
		if (nodePtr->data == key) //if the node's data equals the key then the node has been found
			found = true;
		else
			nodePtr = nodePtr->nextPtr; //moves to next node in list
	}
	return nodePtr; //returns pointer to the node that contains data equal to key (NULL if not found)
}

template <typename T>
void List<T>::sort()
{
	if (!isEmpty()) // check if the list is empty
	{
		ListNode<T> * temp = startPtr;
		ListNode<T> * curr;
		for (bool didSwap = true; didSwap; ) {
			didSwap = false;
			for (curr = startPtr; curr->nextPtr != NULL; curr = curr->nextPtr) {
				if (curr->data > curr->nextPtr->data) {
					temp->data = curr->data;
					curr->data = curr->nextPtr->data;
					curr->nextPtr->data = temp->data;
					didSwap = true;
				}
			}
		}
	}
}

template <typename T>
void List<T>::pop(T key) 
{
	ListNode<T>* nodePtr;
	ListNode<T>* nextPtr;
	nodePtr = startPtr;

	if (nodePtr != NULL) // check if the list is empty
	{
		nextPtr = nodePtr->nextPtr;
	}
	else
	{
		return; // else exit the function
	}

	while (nextPtr != NULL) // iterate through the list
	{
		if (nextPtr->data == key) // find the item with the matching key
		{
			ListNode<T> *tempNode = nextPtr->nextPtr; // create temporary node
			if (tempNode != NULL)
			{
				nodePtr->nextPtr = tempNode; // set the next node
				delete nextPtr; // delete the node with the key
				return;
			}
			else //if the next item is NULL delete this item
			{
				delete nextPtr;
				nodePtr->nextPtr = NULL;
				return;
			}
		}
		else // iterate
		{
			nodePtr = nodePtr->nextPtr;
			nextPtr = nextPtr->nextPtr;
		}
			
	}
}
#endif 