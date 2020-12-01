#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <list>
#include <string>
#include <functional>

using namespace std;

const int TABLE_SIZE = 10007;
const int STRING_SIZE = 13;

struct Node {
	string str;
	vector<int> offsets;

	Node(string s, int off) {
		str = s;
		offsets.push_back(off); // adds first offset
	}

	void addOffset(int off) {
		offsets.push_back(off); //adds additional offsets
	}
};

class HashTable {
public:
	HashTable() {
		for (int i = 0; i < TABLE_SIZE; i++) { // create linked list of Nodes for each entry
			table[i] = list<Node>();
		}
	}

	void insert(string str, int offset) {
		hash<string> hashstr;
		int hash = hashstr(str) % TABLE_SIZE;
		
		list<Node>& lists = table[hash];
		for (list<Node>::iterator it = lists.begin(); it != lists.end(); ++it) {
			if ((*it).str == str) {
				if ((*it).offsets.size() < 100) {//test number of offsets in vector
					(*it).offsets.push_back(offset);
					return;
				}
				
			}
		}
		table[hash].push_back(Node(str, offset));
		return;
	}

	bool searchString(string str) {
		hash<string> hashstr;
		int hash = hashstr(str) % TABLE_SIZE;
		if (table[hash].empty()) { // if list is empty nothing is there
			return false;
		}
		else {
			list<Node>& lists = table[hash];
			for (list<Node>::iterator it = lists.begin(); it != lists.end(); ++it) {
				if ((*it).str == str) {
					return true;
				}
			}
			return false;
		}
	}

	vector<int> getOffsets(string str) {
		hash<string> hashstr;
		int hash = hashstr(str) % TABLE_SIZE;
		list<Node>& lists = table[hash];
		for (list<Node>::iterator it = lists.begin(); it != lists.end(); ++it) {
			if ((*it).str == str) {
				return (*it).offsets;
			}
		}
		return vector<int>();
	}

private:
	list<Node> table[TABLE_SIZE];
	
};
