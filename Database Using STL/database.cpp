#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
//#include "Martian.h"
//#include "MartianDatabase.h"
using namespace std;

enum class Criteria { first, last, ssn };

struct Martian {
	string fname_;
	string lname_;
	string SSN_;

	Martian(string fname, string lname, string MSSN) {
		fname_ = fname;
		lname_ = lname;
		SSN_ = MSSN;
	}
};

class MartianDatabase {
private:
	vector<Martian*> mDatabase_;
	vector<string*> fNamePointers_;
	vector<string*> lNamePointers_;
	vector<string*> ssnPointers_;

public:
	// creates a new empty Martian Database initialized with the
	// contents of filename.
	MartianDatabase(istream &infile);

	// add item m to database, return true if successfully added.
	bool addMartian(const Martian &m);

	// delete item with the ssn in database, return true if successfully 
	// deleted.
	bool deleteMartian(string ssn);

	// return true if the database does not contain any items, false
	// otherwise.
	bool isEmpty();

	// return true if the database is at its maximum storage, false
	// otherwise.
	bool isFull();

	// prints the list of Martians, unsorted, sorted by last name, sorted
	// by first name and by MSSN.
	void printMartians(Criteria type);

	string findMatchingFirst(string s);
	string findMatchingLast(string s);
	string findMatchingSSN(string s);

	void sort();
	//erases matching 
	void eraseMatchingFirst(string fName);
	void eraseMatchingLast(string lName);
	void eraseMatchingSSN(string ssn);

	~MartianDatabase();
};

void printHeader();
void Quicksort(vector<string*> &a, int firstPos, int lastPos);
int partition(vector<string*> &a, int lowPos, int highPos);
void swap(string *&left, string *&right);

const int MAXSIZE = 100;

MartianDatabase::MartianDatabase(istream &infile) {
	string s;
	string fname, lname, MSSN;

	while (infile >> s) {
		if(mDatabase_.size() <= MAXSIZE) {
			fname = s;
			infile >> lname;
			infile >> MSSN;

			//creates a pointer to a martian and adds it to the database
			//creates pointers to the martian's first name, last name,
			//and ssn, and adds them to their corersponding vectors
			Martian *Marvin_p = new Martian(fname, lname, MSSN);
			string *fp, *lp, *ssn_p;

			fp = &(Marvin_p->fname_);
			lp = &(Marvin_p->lname_);
			ssn_p = &(Marvin_p->SSN_);

			mDatabase_.push_back(Marvin_p);
			fNamePointers_.push_back(fp);
			lNamePointers_.push_back(lp);
			ssnPointers_.push_back(ssn_p);
		}
	}
}

bool MartianDatabase::addMartian(const Martian &m) {
	if (mDatabase_.size() >= MAXSIZE)
		return false;
	//creates a marshin
	Martian *Marvin_p = new Martian(m.fname_, m.lname_, m.SSN_);
	string *fp, *lp, *ssn_p;

	//create its fname, lname, and ssn pointers
	fp = &(Marvin_p->fname_);
	lp = &(Marvin_p->lname_);
	ssn_p = &(Marvin_p->SSN_);

	//push the marshin and its matching pointers onto their 
	//corresponding vectors
	mDatabase_.push_back(Marvin_p);	
	fNamePointers_.push_back(fp);	
	lNamePointers_.push_back(lp);	
	ssnPointers_.push_back(ssn_p);

	return true;
}

bool MartianDatabase::deleteMartian(string ssn) {
	if (isEmpty())	//if the database is empty, return false
		return false;

	vector<Martian*>::iterator it;
	it = mDatabase_.begin();
	//if ssn is found, delete the marshin matching the ssn along with its
	//corresponding first name pointer, last name pointer, and ssn pointer
	while (it != mDatabase_.end()) {
		if ((*it)->SSN_ == ssn) {
			eraseMatchingFirst((*it)->fname_);
			eraseMatchingLast((*it)->lname_);
			eraseMatchingSSN(ssn);
			delete(*it);
			it = mDatabase_.erase(it);
			return true;
		}
		it++;
	}
	//if ssn isn't found return false
	cout << "SSN not found" << endl << endl;
	return false;
}
//deletes pointer pointing to fName from fNameVector
void MartianDatabase::eraseMatchingFirst(string fName) {
	vector<string*>::iterator it;
	it = fNamePointers_.begin();
	while (it != fNamePointers_.end()) {
		if (*(*it) == fName) {
			it = fNamePointers_.erase(it);
			return;
		}
		it++;
	}
}
//deletes pointer pointing to lName from lNamePointers vector
void MartianDatabase::eraseMatchingLast(string lName) {
	vector<string*>::iterator it;
	it = lNamePointers_.begin();
	while (it != lNamePointers_.end()) {
		if (*(*it) == lName) {
			it = lNamePointers_.erase(it);
			return;
		}
		it++;
	}
}
//deletes pointer pointing to ssnPointers vector
void MartianDatabase::eraseMatchingSSN(string ssn) {
	vector<string*>::iterator it;
	it = ssnPointers_.begin();
	while (it != ssnPointers_.end()) {
		if (*(*it) == ssn) {
			it = ssnPointers_.erase(it);
			return;
		}
		it++;
	}
}

bool MartianDatabase::isEmpty() {
	return mDatabase_.empty();
}

bool MartianDatabase::isFull() {
	if (mDatabase_.size() == MAXSIZE)
		return true;
	else
		return false;
}

//finds matching first name
string MartianDatabase::findMatchingFirst(string s) {
	vector<Martian*>::iterator it;
	it = mDatabase_.begin();
	while (it != mDatabase_.end()) {
		if ((*it)->lname_ == s || (*it)->SSN_ == s) {
			return ((*it)->fname_);
		}
		it++;
	}
}
//finds matching last name
string MartianDatabase :: findMatchingLast(string s) {
	vector<Martian*>::iterator it;
	it = mDatabase_.begin();
	while (it != mDatabase_.end()) {
		if ((*it)->fname_ == s || (*it)->SSN_ == s) {
			return ((*it)->lname_);
		}
		it++;
	}
}
//finds matching ssn
string MartianDatabase :: findMatchingSSN(string s) {
	vector<Martian*>::iterator it;
	it = mDatabase_.begin();
	while (it != mDatabase_.end()) {
		if ((*it)->fname_ == s || (*it)->lname_ == s) {
			return ((*it)->SSN_);
		}
		it++;
	}
}

void MartianDatabase::printMartians(Criteria type) {
	//sorts vectors
	sort();

	printHeader();
	int numM = 0;

	//prints in ascending order with respect to first names
	if (type == Criteria::first) {
		while (numM < mDatabase_.size()) {
			cout << numM + 1 << " ";
			cout << setw(10) << right << *(fNamePointers_[numM]);
			cout << setw(12) << right << findMatchingLast(*(fNamePointers_[numM]));
			cout << setw(10) << right << findMatchingSSN(*(fNamePointers_[numM])) << endl;
			numM++;
		}
	}
	//prints in ascending order with respect to last names
	if (type == Criteria::last) {
		while (numM < mDatabase_.size()) {
			cout << numM + 1 << " ";
			cout << setw(10) << right << findMatchingFirst(*(lNamePointers_[numM]));
			cout << setw(12) << right << *(lNamePointers_[numM]);
			cout << setw(10) << right << findMatchingSSN(*(lNamePointers_[numM])) << endl;
			numM++;
		}
	}
	//prints in ascending order with respect to ssn
	if (type == Criteria::ssn) {
		while (numM < mDatabase_.size()) {
			cout << numM + 1 << " ";
			cout << setw(10) << right << findMatchingFirst(*(ssnPointers_[numM]));
			cout << setw(12) << right << findMatchingLast(*(ssnPointers_[numM]));
			cout << setw(10) << right << *(ssnPointers_[numM]) << endl;
			numM++;
		}
	}
}

void printHeader() {
	cout << endl;
	cout << setw(12) << right << "  First Name";
	cout << setw(12) << right << "Last Name";
	cout << setw(10) << right << "MSSN" << endl;
}

//sorts first name vector, last name vector, and ssn vector
void MartianDatabase::sort(){
	Quicksort(fNamePointers_, 0, fNamePointers_.size() - 1);
	Quicksort(lNamePointers_, 0, lNamePointers_.size() - 1);
	Quicksort(ssnPointers_, 0, ssnPointers_.size() - 1);
}

void Quicksort(vector<string*> &a, int firstPos, int lastPos) {
	if (lastPos - firstPos >= 1) {
		int pivotIndex;
		pivotIndex = partition(a, firstPos, lastPos);
		Quicksort(a, firstPos, pivotIndex - 1); //sorts pointers to the left of the pivot
		Quicksort(a, pivotIndex + 1, lastPos); //sorts pointers to the right of the pivot
	}
}

int partition(vector<string*> &a, int lowPos, int highPos) {
	string pivot = *(a[lowPos]); //set the first string in the vector as the pivot
	int pivIndex = lowPos; //pivot index = first pos in vector

	while (lowPos < highPos) {
		while (lowPos <= highPos && *(a[lowPos]) <= pivot)
			lowPos++;
		while (*(a[highPos]) > pivot)
			highPos--;
		if (lowPos < highPos)
			swap(a[lowPos], a[highPos]);
	}
	swap(a[pivIndex], a[highPos]);
	pivIndex = highPos;
	return pivIndex;
}

void swap(string *&left, string *&right) {
	//swaps pointers
	string *temp = left;
	left = right;
	right = temp;
}

//destructor
MartianDatabase::~MartianDatabase() {
	for (int pos = 0; pos < mDatabase_.size(); pos++) {
		delete mDatabase_[pos];
	}
}

