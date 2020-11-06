#include <iostream>
#include <string>
#include "linkedlist.h"
using namespace std;

//copy constructor
LinkedList::LinkedList(const LinkedList& rhs) {
	if (rhs.head == nullptr) {
		head = nullptr;
		return;
	}

	Node *rhs_p = rhs.head;

	head = new Node;
	Node *copy_p = head;

	while (rhs_p->next != nullptr) {

		copy_p->value = rhs_p->value;
		copy_p->next = new Node;

		copy_p = copy_p->next;
		rhs_p = rhs_p->next;
	}

	if (rhs_p->next == nullptr) {
		copy_p->value = rhs_p->value;
		copy_p->next = nullptr;
	}
}

//copy function
void LinkedList::copy(const LinkedList& rhs) {
	if (rhs.head == nullptr) {
		head = nullptr;
		return;
	}

	Node *rhs_p = rhs.head;

	head = new Node;
	Node *copy_p = head;

	while (rhs_p->next != nullptr) {

		copy_p->value = rhs_p->value;
		copy_p->next = new Node;

		copy_p = copy_p->next;
		rhs_p = rhs_p->next;
	}

	if (rhs_p->next == nullptr) {
		copy_p->value = rhs_p->value;
		copy_p->next = nullptr;
	}
}

//destructor
LinkedList::~LinkedList() {
	Node *p = head;
	while (p != nullptr) {
		head = p;
		p = p->next;
		delete head;
	}
}

//delete function
void LinkedList::deleteList(Node *temp) {
	Node *p = head;
	while (p != nullptr) {
		head = p;
		p = p->next;
		delete head;
	}
}

//assignment operator
const LinkedList &LinkedList :: operator=(const LinkedList  &rhs) {
	if (this == &rhs)
		return *this;

	deleteList((this)->head); //deletes what this currently points to
	(*this).copy(rhs);

	return(*this);
}

void LinkedList::insertToFront(const ItemType &val) {
	Node *p = new Node;
	p->value = val;
	p->next = head;
	head = p;
}

void LinkedList::printList() const {
	Node *p = head;
	while (p != nullptr) {
		cout << p->value << " ";
		p = p->next;
	}
	cout << endl;
}

bool LinkedList::get(int i, ItemType& item) const {
	int position = 0;
	Node *p = head;

	while (p != nullptr) {
		if (position == i) {
			item = p->value;
			return true;
		}
		position++;
		p = p->next;
	}
	return false;
}

void LinkedList::reverseList() {
	if (head == nullptr || head->next == nullptr)
		return;

	Node *previous = head;
	Node *current = head->next;
	Node *forward = nullptr;

	while (current != nullptr) {
		forward = current->next;
		current->next = previous;
		previous = current;
		current = forward;
	}
	head->next = nullptr;
	head = previous;
}

void LinkedList::printReverse() const {
	if (head == nullptr)
		return;
	printReverseAux(head);
	cout << endl;
}

void LinkedList::printReverseAux(Node *p) const{
	if (p == nullptr)
		return;
	printReverseAux(p->next);
	cout << p->value << " ";
}

void LinkedList::append(const LinkedList &other) {
	if (other.head == nullptr)
		return;

	LinkedList temp = other;

	if (head == nullptr) {
		head = temp.head;
		temp.head = nullptr;
		return;
	}

	Node *p = head;
	//traverse through LL until p points to last node
	for (; p->next != nullptr; p = p->next);
	p->next = temp.head;
	temp.head = nullptr;
}

void LinkedList::swap(LinkedList &other) {
	LinkedList temp = (*this);
	(*this) = other;
	other = temp;
}

int LinkedList::size() const {
	Node *p = head;
	int count = 0;

	while (p != nullptr) {
		count++;
		p = p->next;
	}
	return count;
}
