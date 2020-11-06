#include <iostream>
#include <string>
using namespace std;

typedef string ItemType;

struct WordNode {
	ItemType m_data;
	WordNode *m_left;
	WordNode *m_right;
	int m_dataCount; //keeps track of the data's number of occurences 

	//default constructor with no data parameter
	WordNode() :m_left(nullptr), m_right(nullptr), m_dataCount(1) {};
	//default csontructor with data parameter
	WordNode(ItemType d) :
		m_data(d), m_left(nullptr), m_right(nullptr), m_dataCount(1) {};
	
	int getWordCount() {
		return m_dataCount;
	}
};

class WordTree {
private:
	WordNode *root;

	//takes a WordNode pointer and copies the tree it's pointing to. 
	//Returns a WordNode pointer to the root of the copied tree
	WordNode* copyTree(const WordNode *copy_p);

	//takes in a WordNode pointer and deletes the tree its pointing to
	void deleteTree(WordNode *&node_p);

	//creates a new WordNode and inserts it into the tree *node_p
	//is pointing to
	void insert(WordNode *&node_p, ItemType data);

	//incrament's the node's dataCount if the data already exists
	bool isDuplicateWord(WordNode *&node_p, ItemType data);

	//returns the total number of Itemtypes inserted, including dublicates
	int countTotalData(WordNode *node_p) const;

	//returns the number of nodes in the tree
	int countNodes(WordNode *node_p) const;

	//prints the node's data and dataCount using in-order traversal
	void printTree(ostream &o, const WordNode *node_p);

public:
	// default constructor            
	WordTree() : root(nullptr) { };

	// copy constructor
	WordTree(const WordTree& rhs);

	// assignment operator
	const WordTree& operator=(const WordTree& rhs);

	// Inserts v into the WordTree    
	void add(ItemType v);

	// Returns the number of distince words / nodes   
	int distinctWords() const;

	// Returns the total number of words inserted, including duplicate
	// values    
	int totalWords() const;

	// Prints the LinkedList 
	friend ostream& operator<<(ostream &out, const WordTree& rhs);

	// Destroys all the dynamically allocated memory
	// in the tree.
	~WordTree();
};

//copy constructor
WordTree::WordTree(const WordTree& rhs) {
	root = copyTree(rhs.root);
}

WordNode* WordTree::copyTree(const WordNode *rhs_p) {
	if (rhs_p == nullptr)
		return nullptr;

	//creates new node
	WordNode *node_p = new WordNode();

	//copies data and dataCount from rhs
	node_p->m_data = rhs_p->m_data;
	node_p->m_dataCount = rhs_p->m_dataCount;

	//copies rhs left children and rhs right children
	node_p->m_left = copyTree(rhs_p->m_left);
	node_p->m_right = copyTree(rhs_p->m_right);

	return node_p;
}

const WordTree &WordTree::operator=(const WordTree& rhs) {
	if (this == &rhs)
		return *this;

	//delete what this is already pointing to
	deleteTree(this->root);

	//copy rhs tree
	(*this).root = (*this).copyTree(rhs.root);

	return (*this); //return copied tree
}

void WordTree::deleteTree(WordNode *&node_p) {
	if (node_p != nullptr) {
		deleteTree(node_p->m_left);
		deleteTree(node_p->m_right);
		delete node_p;
		node_p = nullptr;
	}
}

void WordTree::add(ItemType v) {
	insert(root, v);
}

void WordTree::insert(WordNode *&node_p, ItemType data) {
	if (node_p == nullptr) {
		node_p = new WordNode(data);
		return;
	}

	//if the data already exists in the tree, the node 
	//is not added and the data's occurences increases
	if (isDuplicateWord(node_p, data))
		return;

	//inserts to the left if data being inserted is less 
	//than node's data, and inserts to the right if it's greater
	else if (data < node_p->m_data)
		insert(node_p->m_left, data);
	else
		insert(node_p->m_right, data);
}

bool WordTree::isDuplicateWord(WordNode *&node_p, ItemType data) {
	if (node_p->m_data == data) {
		node_p->m_dataCount++; //incrament the data's occurences
		return true;
	}
	else {
		return false;
	}
}

int WordTree::totalWords() const {
	return countTotalData(root);
}

int WordTree::countTotalData(WordNode *node_p) const{
	if (node_p == nullptr)
		return 0;
	int n = node_p->m_dataCount;
	n += countTotalData(node_p->m_left);
	n += countTotalData(node_p->m_right);
	
	return n;
}

int WordTree::distinctWords() const {
	return countNodes(root);
}

int WordTree::countNodes(WordNode *node_p) const {
	if (node_p == nullptr)
		return 0; 
	int numNodes = 1;
	numNodes += countNodes(node_p->m_left);
	numNodes += countNodes(node_p->m_right);
	return numNodes;
}

WordTree::~WordTree() {
	deleteTree(root);
}

ostream& operator<<(ostream &out, const WordTree& rhs){
	WordTree rhs_copy;
	rhs_copy = rhs;
	rhs_copy.printTree(out, rhs.root);
	return out;
}

void WordTree::printTree(ostream &o, const WordNode *node_p) {
	if (node_p == nullptr)
		return;
	//print left nodes
	printTree(o, node_p->m_left);
	//print current node
	o << node_p->m_data << " " << node_p->m_dataCount << endl;
	//print right nodes
	printTree(o, node_p->m_right);
}

int main() {
	WordTree t;

	t.add("Marley");
	t.add("Jack");
	t.add("Krys");
	t.add("Abby");
	t.add("Neil");
	t.add("Peter");
	t.add("Jack");
	t.add("Peter");

	WordTree s = t;
	
	cout << t << endl;
	cout << "distinct: " << t.distinctWords() << endl;
	cout << "total: " << t.totalWords() << endl << endl;

	s.add("Abby");
	s.add("Zuko");
	s.add("Megs");
	cout << s << endl;
	cout << "distinct: " << s.distinctWords() << endl;
	cout << "total: " << s.totalWords() << endl << endl;

	WordTree u;
	u = s;
	cout << s << endl;
	cout << "distinct: " << u.distinctWords() << endl;
	cout << "total: " << u.totalWords() << endl << endl;
	
}