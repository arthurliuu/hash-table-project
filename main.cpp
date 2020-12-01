#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include "hashtable.h"

using namespace std;

void createDiff(istream& fold, istream& fnew, ostream& fdiff) {
	char c;
	string oldstring, newstring;
	while (fold.get(c)) {
		oldstring += c;
	}
	while (fnew.get(c)) {
		newstring += c;
	}
	HashTable table = HashTable();
	for (int i = 0; i < oldstring.length(); i++) { // hashtable of oldstring 
		table.insert(oldstring.substr(i, STRING_SIZE), i);
	}

	for (int i = 0; i < newstring.length(); i++) { // processing the newstring
		int maxL = 0;
		int maxOff = 0;

		if (i > newstring.length() - STRING_SIZE - 1) { // edge case for adding the last part of the string when the last part is not our constant string size
			fdiff << "A" << newstring.substr(i).length() << ":" << newstring.substr(i);
			return;
		}

		vector<int> offsets = table.getOffsets(newstring.substr(i, STRING_SIZE));
		
		//check all the offsets in the table and find the best match
		if (!offsets.empty()) {
			for (int k = 0; k < offsets.size(); k++) {  
				int offset = offsets[k];
				int length = STRING_SIZE;

				do {
					string target = newstring.substr(i, length);
					string match = oldstring.substr(offset, length);
					if (target != match) {
						break;
					}
					length++;
				} while ((i + length) < newstring.length());

				//check if this length is the new max length
				length--;
				if (length > maxL) {
					maxL = length;
					maxOff = offset;
				}

			}
			if (maxL != 0) { 
				fdiff << "C" << maxL << "," << maxOff;
			}
		}
		else { // no offsets found so we must add
			string toAdd = "";
			string target = newstring.substr(i, STRING_SIZE);
			// copy as much as you can until new match is found then we can copy
			while ((i + maxL) < newstring.length() && !table.searchString(target)) {
				toAdd += newstring[i + maxL];
				maxL++;
				target = newstring.substr(i + maxL, STRING_SIZE);
			}
			if (maxL != 0) {
				fdiff << "A" << maxL << ":" << toAdd;
			}
		}
		i += maxL - 1;
	}
}

bool getInt(istream& inf, int& n)
{
	char ch;
	if (!inf.get(ch) || !isascii(ch) || !isdigit(ch))
		return false;
	inf.unget();
	inf >> n;
	return true;
}

bool getCommand(istream& inf, char& cmd, int& length, int& offset)
{
	if (!inf.get(cmd))
	{
		cmd = 'x';  // signals end of file
		return true;
	}
	char ch;
	switch (cmd)
	{
	case 'A':
		return getInt(inf, length) && inf.get(ch) && ch == ':';
	case 'C':
		return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
	case '\r':
	case '\n':
		return true;
	}
	return false;
}

bool applyDiff(istream& fold, istream& fdiff, ostream& fnew) {
	char command = ' ';
	int length = 0;
	int offset = 0;
	char c;
	string oldstring;

	while (fold.get(c)) {
		oldstring += c;
	}
	while (command != 'x') {
		if (getCommand(fdiff, command, length, offset)) {
			if (length != 0) {
				string result;
				switch (command) {
				case 'A':
					result = "";
					char c;
					for (int i = 0; i < length; i++) {
						fdiff.get(c);
						result += c;
					}
					fnew << result;
					break;
				case 'C':
					result = "";
					result = oldstring.substr(offset, length);
					fnew << result;
					break;
				default:
					break;
				}
			}
		}
		else {
			return false;
		}
	}
	return true;
}


void check(string difffile, string newfile) {
	cerr << "The dif file length is ";
	long begin, end;
	ifstream myfile(difffile, ios::binary);
	begin = myfile.tellg();
	myfile.seekg(0, ios::end);
	end = myfile.tellg();
	cerr << (end - begin);
	cerr << endl;
	cerr << "and its text is " << endl;
	string foo;
	myfile.seekg(0);
	char c;
	while (myfile.get(c))
		cerr << c;
	cerr << endl;
}

int main()
{
	//runtest("smallmart1.txt", "smallmart2.txt", "smallmartDiff.txt", "smallmartNew.txt");
	check("smallmartDiff.txt", "smallmartNew.txt");
	//runtest("greeneggs1.txt", "greeneggs2.txt", "greeneggsDiff.txt", "greeneggsNew.txt");
	check("greeneggsDiff.txt", "greeneggsNew.txt");
	//runtest("warandpeace1.txt", "warandpeace2.txt", "warandpeaceDiff.txt", "warandpeaceNew.txt");
	check("warandpeaceDiff.txt", "warandpeaceNew.txt");
	//runtest("strange1.txt", "strange2.txt", "Diff.txt", "strangeNew.txt");
	check("Diff.txt", "strangeNew.txt");
	cerr << "Test PASSED" << endl;
}
