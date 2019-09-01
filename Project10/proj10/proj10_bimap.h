/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 10 - Dynamic Memory BiMap
 * proj10_bimap.h
 * 11.25.18
 */

#pragma once

#include<iostream>
using std::ostream;
#include<string>
using std::string;
#include<vector>
using std::vector;
#include<utility>
using std::pair;
#include<initializer_list>
using std::initializer_list;
#include<algorithm>
using std::lower_bound; using std::swap; using std::copy;
#include<sstream>
using std::ostringstream;



template<typename K, typename V>
struct Node {
public:
  //data members
  K first;
  V second;
  //constructors
  Node() = default;
  Node(K, V);
  //methods
  bool operator<(const Node&) const;
  bool operator==(const Node&) const;
  //friend functions
  friend ostream& operator<<(ostream &out, const Node &node) {
    out << node.first << ":" << node.second;
    return out;
  }
};

//Node method definitions follow in order of declaration above

//Node<K, V>::Node(K key, V value)
//purpose: construct a Node object from a key of type K and a
//         value of type V
//args:    "key": a key of type K which is to be the "first" data member of the 
//         node
//         "value": a value of type V which is to be the "second" data member of
//         the node
//return:  n/a
template <typename K, typename V>
Node<K, V>::Node(K key, V value) {
  first = key;
  second = value;
}

//bool Node<K, V>::operator<(const Node& other) const
//purpose: overload the < operator to enable less-than comparison of two nodes
//args:    "other": the node to be compared with the calling node
//return:  true if the calling node's "first" member is less than the argument  
//         node's "first" member (comparison is based on type K of "first"
//         member); else, false
template <typename K, typename V>
bool Node<K, V>::operator<(const Node& other) const {
  if (first < other.first) {
    return true;
  } else {
    return false;
  }
}

//bool Node<K, V>::operator==(const Node& other) const
//purpose: overload the == operator to enable equality comparison of two nodes
//args:    "other": the node to be compared with the calling node
//return:  true if the calling node's "first" member is equal to the argument  
//         node's "first" member (comparison is based on type K of "first"
//         member); else, false
template <typename K, typename V>
bool Node<K, V>::operator==(const Node& other) const {
  if (first == other.first) {
    return true;
  } else {
    return false;
  }
}



template<typename K, typename V>
class BiMap {
private:
  //private data members
  Node<K, V>* ordered_by_keys_;
  Node<K, V>* ordered_by_vals_;
  size_t last_;
  size_t capacity_;
  //private required methods
  Node<K, V>* find_key(K key);
  Node<K, V>* find_value(V value);
  void grow();
  //private additional helper methods
  void insert(Node<K, V> node);
  void erase(K key, V value);

public:
  //constructors
  BiMap(int sz = 2);
  BiMap(initializer_list<Node<K, V>>);
  //rule of three stuff
  BiMap(const BiMap&);
  BiMap operator=(BiMap);
  ~BiMap();
  //public methods
  size_t size();
  V value_from_key(K);
  K key_from_value(V);
  bool update(K, V);
  K remove_val(V value); 
  V remove_key(K key); 
  bool add(K, V);
  int compare(BiMap &);
  //friend functions
  friend ostream& operator<<(ostream &out, const BiMap &bm) {
    ostringstream oss;
    for (size_t i = 0; i < bm.last_; ++i) {
      oss << bm.ordered_by_keys_[i] << ", ";
    }
    string to_print = oss.str();
    out << to_print.substr(0, to_print.length() - 2);
    return out;
  }
};

//BiMap method definitions follow in order of declaration above

//Node<K, V>* BiMap<K, V>::find_key(K key)
//purpose: return a pointer to the first Node within ordered_by_keys_ with a
//         key which is greater than or equal to the argument key
//args:    "key": the key to be searched for / compared with keys within the
//         ordered_by_keys_ array
//return:  a Node<K, V>* pointer which points to the first Node within 
//         ordered_by_keys_ with a key which is greater than or equal to
//         the argument key
//note:    no use of linear-complexity algorithms
template<typename K, typename V>
Node<K, V>* BiMap<K, V>::find_key(K key) {
  return lower_bound(ordered_by_keys_, (ordered_by_keys_ + last_), key, 
    [](Node<K, V> node, K key)->bool {
      return (node.first < key);
    });
}

//Node<K, V>* BiMap<K, V>::find_value(V value)
//purpose: return a pointer to the first Node within ordered_by_vals_ with a
//         value which is greater than or equal to the argument value
//args:    "value": the value to be searched for / compared with values within
//         the ordered_by_vals_ array
//return:  a Node<K, V>* pointer which points to the first Node within 
//         ordered_by_vals_ with a value which is greater than or equal to
//         the argument value
//note:    no use of linear-complexity algorithms
template<typename K, typename V>
Node<K, V>* BiMap<K, V>::find_value(V value) {
  return lower_bound(ordered_by_vals_, (ordered_by_vals_ + last_), value,
    [](Node<K, V> node, V value)->bool {
      return (node.second < value);
    });
}

//void BiMap<K, V>::grow()
//purpose: double the size of the ordered_by_keys_ and ordered_by_vals_ array
//         data members
//args:    n/a
//return:  n/a
template<typename K, typename V>
void BiMap<K, V>::grow() {
  Node<K, V>* new_key_ary = new Node<K, V>[capacity_ * 2]{};
  Node<K, V>* new_val_ary = new Node<K, V>[capacity_ * 2]{};
  copy(ordered_by_keys_, (ordered_by_keys_ + last_), new_key_ary);
  copy(ordered_by_vals_, (ordered_by_vals_ + last_), new_val_ary);
  swap(new_key_ary, ordered_by_keys_);
  swap(new_val_ary, ordered_by_vals_);
  delete[] new_key_ary;
  delete[] new_val_ary;
  capacity_ *= 2;
}


//void BiMap<K, V>::insert(Node<K, V> node)
//purpose: called by add method if a node is unique and is to be added; inserts
//         the node into both the ordered_by_keys_ and ordered_by_vals_ members
//         by using the find_key and find_value methods to find appropriate
//         location for new node; maintains ordering of ordered_by_keys_ and 
//         ordered_by_vals_ members
//args:    "node": the unique node passed from the add method which is to be
//         inserted into the ordered_by_keys_ and ordered_by_vals_ members
//return:  n/a
//note:    no use of linear-complexity algorithms; called by add method; uses
//         find_key and find_value
template<typename K, typename V>
void BiMap<K, V>::insert(Node<K, V> node) {
  if (last_ == capacity_) {
    grow();
  }

  Node<K, V>* key_ptr = find_key(node.first);
  Node<K, V>* val_ptr = find_value(node.second);

  //add to ordered_by_keys_
  if (key_ptr == ordered_by_keys_ + last_) {
    //add directly to end
    *key_ptr = node;
  } else {
    //need to insert in middle
    size_t indx = key_ptr - ordered_by_keys_;
    Node<K, V>* new_key_ary = new Node<K, V>[capacity_]{};
    copy(ordered_by_keys_, key_ptr, new_key_ary);
    *(new_key_ary + indx) = node;
    copy(key_ptr, ordered_by_keys_ + last_, new_key_ary + indx + 1);
    swap(new_key_ary, ordered_by_keys_);
    delete[] new_key_ary;
  }

  //add to ordered_by_vals_
  if (val_ptr == ordered_by_vals_ + last_) {
    //add directly to end
    *val_ptr = node;
  } else {
    //need to insert in middle
    size_t indx = val_ptr - ordered_by_vals_;
    Node<K, V>* new_val_ary = new Node<K, V>[capacity_]{};
    copy(ordered_by_vals_, val_ptr, new_val_ary);
    *(new_val_ary + indx) = node;
    copy(val_ptr, ordered_by_vals_ + last_, new_val_ary + indx + 1);
    swap(new_val_ary, ordered_by_vals_);
    delete[] new_val_ary;
  }

  ++last_;
}

//void BiMap<K, V>::erase(K key, V value)
//purpose: called by remove_key and remove_val if a matching key/value (resp.)
//         is found within the BiMap's Nodes; copies all Nodes except the Node
//         to be removed from ordered_by_keys_ and ordered_by_vals_ over to 
//         new arrays, shrinking the capacity of both arrays if possible
//args:    "key": the key of the Node to be removed, passed from remove_key or
//         remove_val
//         "value": the value of the Node to be removed, passed from remove_key
//         or remove_val
//return:  n/a
//note:    no use of linear-complexity algorithms aside from the necessary
//         copy when moving non-deleted Nodes to new arrays; uses find_key and
//         find_value which are sub-linear
//note:    takes care of memory leakage by deleting new arrays created
template<typename K, typename V>
void BiMap<K, V>::erase(K key, V value) {
  Node<K, V>* key_ptr = find_key(key);
  Node<K, V>* val_ptr = find_value(value);
  size_t key_indx = key_ptr - ordered_by_keys_;
  size_t val_indx = val_ptr - ordered_by_vals_;
  Node<K, V>* new_key_ary;
  Node<K, V>* new_val_ary;
  if (last_ - 1 <= capacity_ / 2) {
    new_key_ary = new Node<K, V>[capacity_ / 2]{};
    new_val_ary = new Node<K, V>[capacity_ / 2]{};
    capacity_ /= 2;
  } else {
    new_key_ary = new Node<K, V>[capacity_] {};
    new_val_ary = new Node<K, V>[capacity_] {};
  }
  copy(ordered_by_keys_, ordered_by_keys_ + key_indx, new_key_ary);
  copy(ordered_by_keys_ + key_indx + 1, ordered_by_keys_ + last_, new_key_ary + key_indx);
  swap(ordered_by_keys_, new_key_ary);
  delete[] new_key_ary;
  copy(ordered_by_vals_, ordered_by_vals_ + val_indx, new_val_ary);
  copy(ordered_by_vals_ + val_indx + 1, ordered_by_vals_ + last_, new_val_ary + val_indx);
  swap(ordered_by_vals_, new_val_ary);
  delete[] new_val_ary;
  --last_;
}

//BiMap<K, V>::BiMap(int sz)
//purpose: constructs an empty BiMap with capacity_ of sz / arrays of size sz 
//         (default sz = 2)
//args:    "sz": the capacity_ of the BiMap and size of the ordered_by_keys_ and 
//         ordered_by_vals_ arrays
//return:  n/a
template<typename K, typename V>
BiMap<K, V>::BiMap(int sz) {
  capacity_ = sz;
  last_ = 0;
  ordered_by_keys_ = new Node<K, V>[capacity_] {};
  ordered_by_vals_ = new Node<K, V>[capacity_] {};
}

//BiMap<K, V>::BiMap(initializer_list<Node<K, V>> init_list)
//purpose: constructs a BiMap filled with Nodes from an initializer list
//         & capacity of size init_list.size() / arrays of size init_list.size()
//args:    "init_list": an initializer list of Nodes to be placed in the BiMap
//return:  n/a
//note:    uses add method to ensure ordering of ordered_by_keys_ and
//         ordered_by_vals_ are maintained regardless of ordering of init_list
template<typename K, typename V>
BiMap<K, V>::BiMap(initializer_list<Node<K, V>> init_list) {
  capacity_ = init_list.size();
  last_ = 0;
  ordered_by_keys_ = new Node<K, V>[capacity_] {};
  ordered_by_vals_ = new Node<K, V>[capacity_] {};
  for (Node<K, V> node : init_list) {
    add(node.first, node.second);
  }
}

//BiMap<K, V>::BiMap(const BiMap& other)
//purpose: constructs a copy of an existing BiMap
//args:    "other": the BiMap to be copied
//return:  n/a
//note:    defined as part of "rule of 3"
template<typename K, typename V>
BiMap<K, V>::BiMap(const BiMap& other) {
  capacity_ = other.capacity_;
  last_ = other.last_;
  ordered_by_keys_ = new Node<K, V>[capacity_] {};
  ordered_by_vals_ = new Node<K, V>[capacity_] {};
  copy(other.ordered_by_keys_, other.ordered_by_keys_ + last_, ordered_by_keys_);
  copy(other.ordered_by_vals_, other.ordered_by_vals_ + last_, ordered_by_vals_);
}

//BiMap<K, V> BiMap<K, V>::operator=(BiMap other)
//purpose: assigns an Rvalue BiMap ("other") to an Lvalue BiMap (caller)
//args:    "other": the BiMap to be assigned to the calling BiMap (will be
//         constructed as a local copy)
//return:  n/a
//note:    defined as part of "rule of 3"
template<typename K, typename V>
BiMap<K, V> BiMap<K, V>::operator=(BiMap other) {
  //note: other is a local copy created with copy constructor; thus, underlying
  //data members of Rvalue will not be disturbed
  //note: destructor will properly dispose of local copy
  swap(capacity_, other.capacity_);
  swap(last_, other.last_);
  swap(ordered_by_keys_, other.ordered_by_keys_);
  swap(ordered_by_vals_, other.ordered_by_vals_);
  return *this;
}

//BiMap<K, V>::~BiMap()
//purpose: destructs a BiMap object by de-allocating memory of ordered_by_keys_
//         and ordered_by_vals_ array members
//args:    n/a
//return:  n/a
//note:    defined as part of "rule of 3"
template<typename K, typename V>
BiMap<K, V>::~BiMap() {
  delete[] ordered_by_keys_;
  delete[] ordered_by_vals_;
}

//size_t BiMap<K, V>::size()
//purpose: returns the current number of elements in the BiMap
//args:    n/a
//return:  a size_t equal to the current number of elements in the BiMap
template<typename K, typename V>
size_t BiMap<K, V>::size() {
  return last_;
}

//V BiMap<K, V>::value_from_key(K key)
//purpose: returns the value associated with a key in a BiMap if the argument
//         key exists -- otherwise, returns a default type V
//args:    "key": the key to search for a match within the BiMap's Nodes
//return:  the value associated with the argument "key" in a BiMap if the
//         argument key exists -- otherwise, a default type V
//note:    no use of linear-complexity algorithms; uses
//         find_key which is sub-linear
template<typename K, typename V>
V BiMap<K, V>::value_from_key(K key) {
  Node<K, V>* key_ptr = find_key(key);
  if (key_ptr != ordered_by_keys_ + last_ && key_ptr->first == key) {
    //found a match
    return key_ptr->second;
  } else {
    //no match found
    return V();
  }
}

//K BiMap<K, V>::key_from_value(V value)
//purpose: returns the key associated with a vvalue in a BiMap if the argument
//         value exists -- otherwise, returns a default type K
//args:    "value": the value to search for a match within the BiMap's Nodes
//return:  the key associated with the argument "value" in a BiMap if the
//         argument value exists -- otherwise, a default type K
//note:    no use of linear-complexity algorithms; uses
//         find_value which is sub-linear
template<typename K, typename V>
K BiMap<K, V>::key_from_value(V value) {
  Node<K, V>* val_ptr = find_value(value);
  if (val_ptr != ordered_by_vals_ + last_ && val_ptr->second == value) {
    //found a match
    return val_ptr->first;
  } else {
    //no match found
    return K();
  }
}

//bool BiMap<K, V>::update(K key, V value)
//purpose: updates a Node with key matching argument key to have the argument
//         value and returns true if such a Node exists within the BiMap; else,
//         does nothing and returns false
//args:    "key": the key of the Node to be updated (if a Node with a
//         matching key exists)
//         "value": the value which the Node with key matching argument key
//         is to be updated to (if a Node with a matching key exists)
//return:  true if a Node with key matching argument key was found and
//         successfully updated; else, false
//note:    no use of linear-complexity algorithms aside from the necessary
//         copy within erase; uses find_key, add, and erase which are otherwise
//         sub-linear
//note:    calls erase then add to maximize code reuse and ensure ordering of
//         both arrays is preserved
template<typename K, typename V>
bool BiMap<K, V>::update(K key, V value) {
  Node<K, V>* key_ptr = find_key(key);
  if (key_ptr != ordered_by_keys_ + last_ && key_ptr->first == key) {
    //found a match
    V old_value = key_ptr->second;
    erase(key, old_value);
    add(key, value);
    return true;
  } else {
    //no match found
    return false;
  }
}

//K BiMap<K, V>::remove_val(V value)
//purpose: removes a Node with the argument value from the BiMap and returns its
//         associated key if a Node with the argument value exists by calling
//         the erase method; else, does nothing and returns a default type K
//args:    "value": the value of the Node to be removed (if a Node with a
//         matching value exists)
//return:  the key associated with the Node removed if a Node with a value
//         matching the argument value existed in the BiMap; else, a default
//         type K
//note:    no use of linear-complexity algorithms aside from the necessary
//         copy within erase; uses find_value and key_from_value which are
//         sub-linear
//note:    calls erase to carry out functionality
template<typename K, typename V>
K BiMap<K, V>::remove_val(V value) {
  Node<K, V>* value_ptr = find_value(value);
  if (value_ptr != ordered_by_vals_ + last_ && value_ptr->second == value) {
    //found match: remove
    K key = key_from_value(value);
    //takes care of erasure from both arrays & decrements last_:
    erase(key, value); 
    return key;
  } else {
    //no match found
    return K();
  }
}

//V BiMap<K, V>::remove_key(K key)
//purpose: removes a Node with the argument key from the BiMap and returns its
//         associated value if a Node with the argument key exists by calling
//         the erase method; else, does nothing and returns a default type V
//args:    "key": the key of the Node to be removed (if a Node with a
//         matching key exists)
//return:  the value associated with the Node removed if a Node with a key
//         matching the argument key existed in the BiMap; else, a default
//         type V
//note:    no use of linear-complexity algorithms aside from the necessary
//         copy within erase; uses find_key and value_from_key which are
//         sub-linear
//note:    calls erase to carry out functionality
template<typename K, typename V>
V BiMap<K, V>::remove_key(K key) {
  Node<K, V>* key_ptr = find_key(key);
  if (key_ptr != ordered_by_keys_ + last_ && key_ptr->first == key) {
    //found match: remove
    V value = value_from_key(key);
    erase(key, value); //takes care of erasure from both arrays & decrements last_
    return value;
  } else {
    //no match found
    return V();
  }
}

//bool BiMap<K, V>::add(K key, V value)
//purpose: utilizes insert method to add a unique node to the BiMap if and only
//         if both the key and value do not yet exist in the BiMap
//args:    "key": the "first" member of the node to be added to the BiMap
//         "value": the "second" member of the node to be added to the BiMap
//return:  true if both key and value are unique and new node is successfully
//         added to the BiMap; else, false
//note:    no use of linear-complexity algorithms; uses
//         find_key, find_value, and insert methods, all of which are sub-linear
template<typename K, typename V>
bool BiMap<K, V>::add(K key, V value) {
  Node<K, V>* key_ptr = find_key(key);
  Node<K, V>* val_ptr = find_value(value);
  if (key_ptr == (ordered_by_keys_ + last_) || key_ptr->first != key) {
    if (val_ptr == (ordered_by_vals_ + last_) || val_ptr->second != value) {
      //then, no matching pair exists -- add
      insert(Node<K, V>(key, value));
      return true;
    }
  }
  //catch-all else
  return false;
}

//int BiMap<K, V>::compare(BiMap& other)
//purpose: lexicographically compares the keys of the Nodes of the calling
//         BiMap to those of the argument BiMap to return (as an int) which
//         BiMap is "greater"
//args:    "other": the BiMap to which the keys of the Nodes of calling BiMap
//         are to be compared to
//return:  1 if the calling BiMap is "greater", -1 if the argument BiMap is 
//         "greater", or 0 if the two BiMaps have exactly equal keys
//note:    compares keys of Nodes of BiMaps one-by-one by looking at the
//         ordered_by_keys_ array in each
//note:    "greater" means that the first non-equal key of the "greater"
//         BiMap compared lexicographically greater than the corresponding
//         key in the other BiMap--or, if the BiMaps have all-equal elements,
//         but one is longer, then the longer BiMap is "greater"
template<typename K, typename V>
int BiMap<K, V>::compare(BiMap& other) {
  size_t sz = 0;
  if (size() < other.size()) {
    //calling BiMap is smaller
    sz = size();
  } else {
    //calling BiMap is larger or BiMaps are same size
    sz = other.size();
  }

  for (size_t i = 0; i < sz; ++i) {
    Node<K, V> calling_node = *(ordered_by_keys_ + i);
    Node<K, V> arg_node = *(other.ordered_by_keys_ + i);
    if (calling_node < arg_node) {  //uses operator< defined in node struct
      return -1;
    } else if (arg_node < calling_node) {
      return 1;
    }
  }

  //if execution reaches here, first sz elements of each BiMap are equivalent;
  //compare sizes
  if (size() < other.size()) {
    return -1;
  } else if (other.size() < size()) {
    return 1;
  } else {
    return 0;
  }
}
