#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <string>

typedef std::string ItemType;

struct Node {
	ItemType value;
	Node *next;
};

class LinkedList {
private:
	Node *head;

public:
	// default constructor
	LinkedList() : head(nullptr) { }

	// copy constructor
	LinkedList(const LinkedList& rhs);

	//copy function
	void copy(const LinkedList& rhs);	//used by assignment operator

	// Destroys all the dynamically allocated memory
	// in the list.
	~LinkedList();

	//deleteList function
	void deleteList(Node *temp);

	// assignment operator
	const LinkedList& operator=(const LinkedList& rhs);

	// Inserts val at the front of the list
	void insertToFront(const ItemType &val);

	// Prints the LinkedList
	void printList() const;

	// Sets item to the value at position i in this
	// LinkedList and return true, returns false if 
	// there is no element i
	bool get(int i, ItemType& item) const;

	// Reverses the LinkedList
	void reverseList();

	// Prints the LinkedList in reverse order
	void printReverse() const;

	void printReverseAux(Node *p) const;

	// Appends the values of other onto the end of this
	// LinkedList.
	void append(const LinkedList &other);

	// Exchange the contents of this LinkedList with the other
	// one.
	void swap(LinkedList &other);

	// Returns the number of items in the Linked List.
	int size() const;
};

#endif
