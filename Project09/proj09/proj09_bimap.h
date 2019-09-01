/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 09 - BiMap - proj09_bimap.h
 * 11.17.18
 */

//see proj09_bimap.cpp for more complete method/function descriptions and 
//comments

#pragma once

#include <vector>
using std::vector; 
#include <string>
using std::string; 
#include <utility>
using std::pair; 
using std::initializer_list; 
#include <ostream>
using std::ostream; 

class BiMap {

private: 
  //holds the pairs ordered by keys;  all keys must be unique
  vector<pair<string, string>> ordered_by_keys_;

  //holds the pairs ordered by values.  all values must be unique
  vector<pair<string, string>> ordered_by_vals_;


  //see proj09_bimap.cpp for full method/function descriptions
  vector<pair <string, string>>::iterator find_key(string key);
  vector<pair <string, string>>::iterator find_value(string value);

public: 
	//default constructor 
	BiMap() = default; 

  //see proj09_bimap.cpp for full method/function descriptions
  BiMap(initializer_list<pair<string, string>>);
  BiMap merge(BiMap&);
	size_t size(); 
	string value_from_key(string);
	string key_from_value(string);
	bool update(string, string);
	pair<string, string> remove(string);
	bool add(string, string);
	int compare(BiMap &);
	friend ostream& operator<<(ostream&, BiMap&);
};