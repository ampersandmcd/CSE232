/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 09 - BiMap - proj09_bimap.cpp
 * 11.17.18
 */


#include <algorithm>
using std::lower_bound; using std::copy;
#include <utility>
using std::make_pair;
#include <cassert>
#include <sstream>
using std::ostringstream;
#include <iterator>
using std::ostream_iterator;

#include "proj09_bimap.h"


//vector<pair<string, string>>::iterator BiMap::find_key(string key)
//purpose: return an iterator to a pair<string, string> in ordered_by_keys_
//         that has a key equal to the argument key or is the first pair in
//         ordered_by_keys_ with a key greater than the argument key
//args:    "key": the argument key to search for in ordered_by_keys_
//return:  a vector<pair<string, string>>::iterator pointing to the first pair  
//         in ordered_by_keys_ with a key greater than or equal to the
//         argument key
//note:    must be private to prevent return of iterator on private member
//note:    no use of linear-complexity algorithms
vector<pair<string, string>>::iterator BiMap::find_key(string key) {
  return lower_bound(ordered_by_keys_.begin(), ordered_by_keys_.end(), key, 
                                   [](pair<string, string> pr, string s)->bool {
                                     return pr.first < s;
                                   });
}

//vector<pair<string, string>>::iterator BiMap::find_value(string value)
//purpose: return an iterator to a pair<string, string> in ordered_by_vals_
//         that has a value equal to the argument value or is the first pair in
//         ordered_by_vals_ with a value greater than the argument value
//args:    "value": the argument value to search for in ordered_by_vals_
//return:  a vector<pair<string, string>>::iterator pointing to the first pair  
//         in ordered_by_vals_ with a value greater than or equal to the
//         argument value
//note:    must be private to prevent return of iterator on private member
//note:    no use of linear-complexity algorithms
vector<pair<string, string>>::iterator BiMap::find_value(string value) {
  return lower_bound(ordered_by_vals_.begin(), ordered_by_vals_.end(), value,
                                   [](pair<string, string> pr, string s)->bool {
                                    return pr.second < s;
                                   });
}

//BiMap::BiMap(initializer_list<pair<string, string>> init_list)
//purpose: construct a BiMap object with properly sorted ordered_by_keys_ and 
//         ordered_by_vals_ members from an init list of pair<string, string>
//args:    "init_list": an unordered initializer list of pair<string, string>
//         from which to construct the BiMap
//return:  n/a (constructor)
//note:    uses add method to ensure proper ordering of ordered_by_keys_ and 
//         ordered_by_vals_ members
BiMap::BiMap(initializer_list<pair<string, string>> init_list) {
  for (pair<string, string> pr : init_list) {
    add(pr.first, pr.second);
  }
}

//BiMap BiMap::Merge(BiMap &bm_other)
//purpose: construct a new BiMap object by merging a calling BiMap and an
//         argument BiMap
//args:    "bm_other": the argument BiMap to be merged with calling BiMap
//return:  a new BiMap object with properly sorted, non-duplicate
//         ordered_by_keys_ and ordered_by_vals_ members
//note:    in the case of duplicates, precedence goes to calling BiMap
BiMap BiMap::merge(BiMap &bm_other) {
  BiMap bm_new;
  //copy keys, vals from calling object
  bm_new.ordered_by_keys_ = ordered_by_keys_;
  bm_new.ordered_by_vals_ = ordered_by_vals_;
  //copy keys, vals from argument object, deferring duplicates to be those of
  //the calling BiMap object (hence use of the add method)
  //note: only need to copy from one vector -- add method will sort out ordering
  for (pair<string, string> pr : bm_other.ordered_by_keys_) {
    bm_new.add(pr.first, pr.second);
  }
  return bm_new;
}


//size_t BiMap::size()
//purpose: return the number of pairs held by the BiMap in either vector member
//args:    n/a
//return:  the the number of pairs held by the BiMap in either vector member as
//         an unsigned int
//note:    uses assert as a sanity check that the two vector members of the
//         BiMap have the same number of elements (which they always should)
size_t BiMap::size() {
  assert(ordered_by_keys_.size() == ordered_by_vals_.size());
  return ordered_by_keys_.size();
}

//string BiMap::value_from_key(string key)
//purpose: find and return the value corresponding to a key in the BiMap if the 
//         key exists; else, return an empty string
//args:    "key": the argument key to search for in the BiMap
//return:  a string containing the value corresponding to the argument key in
//         the BiMap, or the empty string if the argument key DNE in BiMap
//note:    no use of linear-complexity algorithms (find_key is sub-linear)
string BiMap::value_from_key(string key) {
  auto ptr = find_key(key);
  if (ptr != ordered_by_keys_.end() && ptr->first == key) {
    //found match
    return ptr->second;
  } else {
    return string("");
  }
}

//string BiMap::key_from_value(string value)
//purpose: find and return the key corresponding to a value in the BiMap if the
//         value exists; else, return an empty string
//args:    "value": the argument value to search for in the BiMap
//return:  a string containing the key corresponding to the argument value in
//         the BiMap, or the empty string if the argument value DNE in BiMap
//note:    no use of linear-complexity algorithms (find_value is sub-linear)
string BiMap::key_from_value(string value) {
  auto ptr = find_value(value);
  if (ptr != ordered_by_vals_.end() && ptr->second == value) {
    //found match
    return ptr->first;
  } else {
    return string("");
  }
}

//bool BiMap::update(string key, string value)
//purpose: update the pair in the BiMap with key matching argument key to have
//         a value of the argument value and return true if such a pair
//         exists; else, return false and do nothing
//args:    "key": the argument key to search for a match within the BiMap
//         "value": the value to which the pair within the BiMap with key
//         matching argument key's value will be set
//return:  bool indicating whether an update to the BiMap occurred or not
//note:    no use of linear-complexity algorithms (find_key is sub-linear)
bool BiMap::update(string key, string value) {
  auto ptr = find_key(key);
  if (ptr != ordered_by_keys_.end() && ptr->first == key) {
    //we have a pre-existing key
    ptr->second = value;
    return true;
  } else {
    return false;
  }
}

//pair<string, string> BiMap::remove(string key_or_val)
//purpose: remove any pair within the BiMap with a key or value matching
//         the argument key_or_val, and return a pair<string, string> with the
//         strings being the key and value removed, respectively
//args:    "key_or_val": the argument string for which any pair with a matching
//         key or value (or both) will be removed         
//return:  pair<string, string> containing key, value removed (respectively), or
//         containing two empty strings if no matches were found
//note:    if two pairs exist within the BiMap such that the key of one and the 
//         value of the other both match key_or_val, remove both and return a
//         pair with key and value opposite the string removed
//note:    no use of linear-complexity algorithms (find_key and find_value are
//         sub-linear)
pair<string, string> BiMap::remove(string key_or_val) {
  auto key_ptr = find_key(key_or_val);
  auto val_ptr = find_value(key_or_val);
  if (key_ptr != ordered_by_keys_.end() && val_ptr != ordered_by_vals_.end() &&
                key_ptr->first == key_or_val && val_ptr->second == key_or_val) {
    //string is key and value
    //return pair with associated key & value strings opposite the term queried
    pair<string, string> pr = make_pair(val_ptr->first, key_ptr->second);
    //find opposite key, val matches to remove all (2 entries) * (2 pairs) = 
    //4 pairs
    auto opp_key_ptr = find_key(val_ptr->first);
    auto opp_val_ptr = find_value(key_ptr->second);
    //remove pairs in key, val lists with key, val (respective) match
    ordered_by_keys_.erase(key_ptr);
    ordered_by_vals_.erase(val_ptr);
    //remove pairs in key, val lists with val, key (respective) match
    ordered_by_keys_.erase(opp_key_ptr);
    ordered_by_vals_.erase(opp_val_ptr);
    return pr;
  } else if (key_ptr != ordered_by_keys_.end() && key_ptr->first == key_or_val) {
    //string is key only
    pair<string, string> pr = make_pair(key_ptr->first, key_ptr->second);
    val_ptr = find_value(key_ptr->second);
    ordered_by_keys_.erase(key_ptr);
    ordered_by_vals_.erase(val_ptr);
    return pr;
  } else if (val_ptr != ordered_by_vals_.end() && val_ptr->second == key_or_val) {
    //string is value only
    pair<string, string> pr = make_pair(val_ptr->first, val_ptr->second);
    key_ptr = find_key(val_ptr->first);
    ordered_by_keys_.erase(key_ptr);
    ordered_by_vals_.erase(val_ptr);
    return pr;
  } else {
    //no match found
    return(make_pair("", ""));
  }
}

//bool BiMap::add(string key, string value)
//purpose: add a new key, value pair to a BiMap into the ordered_by_keys_ and 
//         ordered_by_vals data members such that proper sorting is maintained;
//         do not add if there already exists some pair in the BiMap with an
//         equivalent key or value string
//args:    "key": the first (key) string of the pair to be added
//         "value": the second (value) string of the pair to be added
//return:  bool indicating whether or not the pair was successfully added
//note:    no use of linear-complexity algorithms (find_key and find_value are
//         sub-linear)
bool BiMap::add(string key, string value) {
  auto key_ptr = find_key(key);
  auto val_ptr = find_value(value);
  if (key_ptr == ordered_by_keys_.end() || key_ptr->first != key) {
    //then, no equivalent key exists yet
    if (val_ptr == ordered_by_vals_.end() || val_ptr->second != value) {
      //then, no equivalent value exists yet; add
      pair<string, string> pr = make_pair(key, value);
      ordered_by_keys_.insert(key_ptr, pr);
      ordered_by_vals_.insert(val_ptr, pr);
      return true;
    }
  }
  //if execution flows here, key or value already exists in BiMap; don't add
  return false;
}

//int BiMap::compare(BiMap &bm_other)
//purpose: lexicographically compare two BiMaps by their keys; return -1 if
//         calling BiMap is smaller, 1 if calling BiMap is greater, and 0 if 
//         calling and argument BiMaps are equivalent; in the case that two
//         BiMaps are of different length but compare equal for all elements of
//         the shorter BiMap, the longer BiMap is considered greater
//args:    "bm_other": the argument BiMap to which the calling BiMap will be
//         lexicographically compared by key
//return:  int in range [-1, 1] indicating whether calling BiMap is
//         lexicographically less (-1), lexicographically greater (1), or equal
//         (0) to the argument BiMap
int BiMap::compare(BiMap &bm_other) {
  //find smaller BiMap's size
  size_t sz = (size() < bm_other.size()) ? size() : bm_other.size();
  for (size_t i = 0; i < sz; ++i) {
    if ((ordered_by_keys_.at(i)).first < 
                                      (bm_other.ordered_by_keys_.at(i)).first) {
      //calling BiMap is lexicographically "smaller"
      return -1;
    } else if ((ordered_by_keys_.at(i)).first >
                                      (bm_other.ordered_by_keys_.at(i)).first) {
      //calling BiMap is lexicographically "greater"
      return 1;
    }
  }
  //if execution flows here, first "sz" elements of both BiMaps are equivalent

  if (size() < bm_other.size()) {
    //calling BiMap is "smaller" by size
    return -1;
  } else if (size() > bm_other.size()) {
    //calling BiMap is "greater" by size
    return 1;
  } else {
    //BiMaps must be equal
    return 0;
  }
}

//ostream& operator<<(ostream& out, BiMap& bm)
//purpose: prints a BiMap to the stream with formatting 
//         "key : value, key : value, ..." ordered by keys and returns the
//         ostream by reference
//args:    "out": the ostream to which the BiMap will be printed
//         "bm": the BiMap to be printed
//return:  reference to the ostream passed as an argument to enable chaining of
//         extraction operator
ostream& operator<<(ostream& out, BiMap& bm) {
  vector<string> to_print_vec;
  for (auto pr : bm.ordered_by_keys_) {
    to_print_vec.push_back(pr.first + " : " + pr.second);
  }
  ostringstream oss;
  copy(to_print_vec.begin(), to_print_vec.end(), 
                                           ostream_iterator<string>(oss, ", "));
  string to_print_str = oss.str();
  out << to_print_str.substr(0, to_print_str.size() - 2);
  return out;
}