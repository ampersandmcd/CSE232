/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 11 - Linked List BiMap
 * proj11_bimap.h
 * 12.5.18
 */

#pragma once

#include<iostream>
using std::ostream; using std::cout; using std::endl;
#include<string>
using std::string;
#include<vector>
using std::vector;
#include<utility>
using std::pair; using std::swap;
#include<initializer_list>
using std::initializer_list;
#include<algorithm>
using std::swap; using std::copy;
#include<sstream>
using std::ostringstream;

////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V>
struct Node {

  // data members
  K first;
  V second;
  Node* next = nullptr;

  // constructors
  Node() = default;
  Node(K, V);
  Node(const Node&);

  // public methods
  bool operator==(const Node&) const;

  //friend functions
  friend ostream& operator<<(ostream &out, const Node &node) {
    out << node.first << ":" << node.second;
    return out;
  }
};

////////////////////////////////////////////////////////////////////////////////

// Node function definitions follow in order of declaration above

// Node<K, V>::Node(K key, V value)
// purpose: construct a Node object from a key of type K and a
//          value of type V (with nullptr to next node)
// args:    "key": a key of type K which is to be the "first" data member of the 
//          node
//          "value": a value of type V which is to be the "second" data member
//          of the node
// return:  n/a
template <typename K, typename V>
Node<K, V>::Node(K key, V value) {
  first = key;
  second = value;
  next = nullptr;
}

// Node<K, V>::Node(const Node& other)
// purpose: construct a Node object as a copy of another Node object
// args:    "other": the Node object to be copied
// return:  n/a
// note: sets this->next equal to nullptr; we don't really want a copied pointer
template <typename K, typename V>
Node<K, V>::Node(const Node& other) {
  first = other.first;
  second = other.second;
  next = nullptr; // don't really want a copied pointer
}

// bool Node<K, V>::operator==(const Node& other) const
// purpose: overload the == operator to enable equality comparison of two nodes
// args:    "other": the node to be compared with the calling (*this) node
// return:  true if the the "first" and "second" of each node are equal
// note:    does not need to check pointer equivalency for intended use cases
template <typename K, typename V>
bool Node<K, V>::operator==(const Node& other) const {
  // context in which equality is tested will not need to depend on whether
  // next pointers are equal, so don't check
  if (first == other.first && second == other.second) {
    return true;
  } else {
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V>
class BiMap {
private:

  // data members
  Node<K, V>* ordered_by_keys_head_ = nullptr;
  Node<K, V>* ordered_by_vals_head_ = nullptr;
  size_t sz_ = 0;

  // private methods - chose pair<previous_node, current_node> return approach
  pair<Node<K, V>*, Node<K, V>*> find_key(K);
  pair<Node<K, V>*, Node<K, V>*> find_value(V);

public:

  // constructors
  BiMap() = default;
  BiMap(initializer_list<Node<K, V>>);

  // rule of 3 declarations
  BiMap(const BiMap&);
  BiMap operator=(BiMap);
  ~BiMap();

  // public methods
  size_t size();
  K remove_val(V value);
  V remove_key(K key);
  bool add(K, V);
  V value_from_key(K);
  K key_from_value(V);
  bool update(K, V);
  int compare(BiMap&);
  BiMap merge(BiMap&);

  // friend functions
  friend ostream& operator<<(ostream &out, const BiMap &bm) {
    ostringstream oss;
    for (Node<K, V>* node = bm.ordered_by_keys_head_; node != nullptr; node = node->next) {
      oss << *node << ", ";
    }
    string to_print = oss.str();
    out << to_print.substr(0, to_print.length() - 2);
    return out;
  }
};

////////////////////////////////////////////////////////////////////////////////

// BiMap function definitions follow in order of declaration above

// pair<Node<K, V>*, Node<K, V>*> BiMap<K, V>::find_key(K key)
// purpose: return a pair of pointers: one to the first Node within 
//          ordered_by_keys_ having a key which is greater than or equal to the
//          argument key and one to the node immediately preceding it
// args:    "key": the key to be searched for / compared with keys within the
//          ordered_by_keys_ linked list
// return:  a pair of Node<K, V>* pointers: the first in the pair points to the
//          node in the ordered_by_keys_ linked list immediately prior to the 
//          second pointer in the pair, which points to the node in 
//          ordered_by_keys_ with a key >= the argument key
// note:    returned pair of pointers in order (prev, found) matches left-to-
// right representation of a linked list
// note:    models upper_bound behavior in a linked list context
template<typename K, typename V>
pair<Node<K, V>*, Node<K, V>*> BiMap<K, V>::find_key(K key) {
  if (ordered_by_keys_head_ == nullptr) {
    // no items in BiMap; not found; return nullptrs;
    return pair<Node<K, V>*, Node<K, V>*>(nullptr, nullptr);
  } else {
    Node<K, V>* prev = nullptr;
    Node<K, V>* found = ordered_by_keys_head_;
    while (found != nullptr) {
      if (found->first >= key) {
        return make_pair(prev, found);
      }
      // not found, advance
      prev = found;
      found = found->next;
    }
    // if execution reaches here, we've reached the end of the list; not found;
    // return nullptr for value; note that prev is tail of list
    return pair<Node<K, V>*, Node<K, V>*>(prev, nullptr);
  }
}

// pair<Node<K, V>*, Node<K, V>*> BiMap<K, V>::find_key(K key)
// purpose: return a pair of pointers: one to the first Node within 
//          ordered_by_vals_ having a value which is greater than or equal to 
//          the argument value and one to the node immediately preceding it
// args:    "vlaue": the value to be searched for / compared with values within 
//          the ordered_by_vals_ linked list
// return:  a pair of Node<K, V>* pointers: the first in the pair points to the
//          node in the ordered_by_vals_ linked list immediately prior to the 
//          second pointer in the pair, which points to the node in 
//          ordered_by_vals_ with a value >= the argument value
// note:    returned pair of pointers in order (prev, found) matches left-to-
// right representation of a linked list
// note:    models upper_bound behavior in a linked list context
template<typename K, typename V>
pair<Node<K, V>*, Node<K, V>*> BiMap<K, V>::find_value(V value) {
  if (ordered_by_vals_head_ == nullptr) {
    // no items in BiMap; not found; return nullptrs;
    return pair<Node<K, V>*, Node<K, V>*>(nullptr, nullptr);
  } else {
    Node<K, V>* prev = nullptr;
    Node<K, V>* found = ordered_by_vals_head_;
    while (found != nullptr) {
      if (found->second >= value) {
        return pair<Node<K, V>*, Node<K, V>*>(prev, found);
      }
      // not found, advance
      prev = found;
      found = found->next;
    }
    // if execution reaches here, we've reached the end of the list; not found;
    // return prev (last element) and nullptr for found
    return pair<Node<K, V>*, Node<K, V>*>(prev, nullptr);
  }
}


// BiMap<K, V>::BiMap(initializer_list<Node<K, V>> init_list)
// purpose: constructs a BiMap filled with Nodes from an initializer list
// args:    "init_list": an initializer list of Nodes to be placed in the BiMap
// return:  n/a
// note:    uses add method to ensure ordering of ordered_by_keys_ and
//          ordered_by_vals_ are maintained regardless of ordering of init_list
template<typename K, typename V>
BiMap<K, V>::BiMap(initializer_list<Node<K, V>> init_list) {
  for (Node<K, V> element : init_list) {
    add(element.first, element.second);
  }
}

// BiMap<K, V>::BiMap(const BiMap& other)
// purpose: constructs a copy of an existing BiMap
// args:    "other": the BiMap to be copied
// return:  n/a
// note:    defined as part of "rule of 3"
template<typename K, typename V>
BiMap<K, V>::BiMap(const BiMap& other) {
  if (other.ordered_by_keys_head_ == nullptr) {
    // note: keys_ and vals_ lists are always same length, so if one is empty, 
    // both are
    ordered_by_keys_head_ = nullptr;
    ordered_by_vals_head_ = nullptr;
  } else {
    // other BiMap is nonempty
    Node<K, V>* keys_ptr = other.ordered_by_keys_head_;
    while (keys_ptr != nullptr) {
      // note: keys_ and vals_ lists are always same length, and keys_ has all
      // necessary information for us to reconstruct a new BiMap with add, so we
      // don't need to worry about the other.ordered_by_vals_ list
      // note: add takes care of ordering and sz_ automatically
      add(keys_ptr->first, keys_ptr->second);
      keys_ptr = keys_ptr->next;
    }
  }
}

// BiMap<K, V> BiMap<K, V>::operator=(BiMap other)
// purpose: assigns an Rvalue BiMap ("other") to an Lvalue BiMap (caller)
// args:    "other": the BiMap to be assigned to the calling BiMap (will be
//          constructed as a local copy)
// return:  n/a
// note:    defined as part of "rule of 3"
template<typename K, typename V>
BiMap<K, V> BiMap<K, V>::operator=(BiMap other) {
  // other is a local copy; won't affect RHS of operator in code
  swap(sz_, other.sz_);
  swap(ordered_by_keys_head_, other.ordered_by_keys_head_);
  swap(ordered_by_vals_head_, other.ordered_by_vals_head_);
  return *this;
}

// BiMap<K, V>::~BiMap()
// purpose: destructs a BiMap object by iteratively de-allocating memory of 
//          ordered_by_keys_ and ordered_by_vals_ linked lists
// args:    n/a
// return:  n/a
// note:    defined as part of "rule of 3"
template<typename K, typename V>
BiMap<K, V>::~BiMap() {
  if (ordered_by_keys_head_ != nullptr && ordered_by_vals_head_ != nullptr) {
    // list is nonempty; destruct its keys list, then its vals list
    Node<K, V>* to_delete = ordered_by_keys_head_;
    while (to_delete != nullptr) {
      ordered_by_keys_head_ = ordered_by_keys_head_->next;
      delete to_delete;
      to_delete = ordered_by_keys_head_;
    }

    to_delete = ordered_by_vals_head_;
    while (to_delete != nullptr) {
      ordered_by_vals_head_ = ordered_by_vals_head_->next;
      delete to_delete;
      to_delete = ordered_by_vals_head_;
    }
    ordered_by_keys_head_ = nullptr;
    ordered_by_vals_head_ = nullptr;
  }
}

// size_t BiMap<K, V>::size()
// purpose: returns the current number of elements in the BiMap
// args:    n/a
// return:  a size_t equal to the current number of elements in the BiMap
template<typename K, typename V>
size_t BiMap<K, V>::size() {
  return sz_;
}

// K BiMap<K, V>::remove_val(V value)
// purpose: removes a Node with the argument value from the BiMap, decrements
//          sz_, and returns its associated key if a Node with the argument 
//          value exists; else, does nothing and returns a default type K
// args:    "value": the value of the Node to be removed (if a Node with a
//          matching value exists)
// return:  the key associated with the Node removed if a Node with a value
//          matching the argument value existed in the BiMap; else, a default
//          type K
template<typename K, typename V>
K BiMap<K, V>::remove_val(V value) {
  K to_return = key_from_value(value);
  Node<K, V>* to_del_key = find_key(to_return).second;
  Node<K, V>* key_prev = find_key(to_return).first;
  Node<K, V>* to_del_val = find_value(value).second;
  Node<K, V>* val_prev = find_value(value).first;

  if (to_del_val == nullptr || to_del_val->second != value) {
    // value DNE in list; return default K() and do nothing else
    return K();
  } else {
    // value exists; remove it from BOTH lists, starting with keys_ then vals_
    if (to_del_key == ordered_by_keys_head_) {
      // to_del_key is head
      ordered_by_keys_head_ = ordered_by_keys_head_->next;
      delete to_del_key;
    } else if (to_del_key->next == nullptr) {
      // to_del_key is tail
      key_prev->next = nullptr;
      delete to_del_key;
    } else {
      // to_del_key is in middle
      key_prev->next = to_del_key->next;
      delete to_del_key;
    }

    if (to_del_val == ordered_by_vals_head_) {
      // to_del_val is head
      ordered_by_vals_head_ = ordered_by_vals_head_->next;
      delete to_del_val;
    } else if (to_del_val->next == nullptr) {
      // to_del_val is tail
      val_prev->next = nullptr;
      delete to_del_val;
    } else {
      // to_del_val is in middle
      val_prev->next = to_del_val->next;
      delete to_del_val;
    }

    --sz_;
    return to_return;
  }
}

// V BiMap<K, V>::remove_key(K key)
// purpose: removes a Node with the argument key from the BiMap, decrements sz_, 
//          and returns its associated value if a Node with the argument key 
//          exists; else, does nothing and returns a default type V
// args:    "key": the key of the Node to be removed (if a Node with a
//          matching key exists)
// return:  the value associated with the Node removed if a Node with a key
//          matching the argument key existed in the BiMap; else, a default
//          type V
template<typename K, typename V>
V BiMap<K, V>::remove_key(K key) {
  V to_return = value_from_key(key);
  Node<K, V>* to_del_key = find_key(key).second;
  Node<K, V>* key_prev = find_key(key).first;
  Node<K, V>* to_del_val = find_value(to_return).second;
  Node<K, V>* val_prev = find_value(to_return).first;

  if (to_del_key == nullptr || to_del_key->first != key) {
    // key DNE in list; return default V() and do nothing else
    return V();
  } else {
    // value exists; remove it from BOTH lists, starting with keys_ then vals_
    if (to_del_key == ordered_by_keys_head_) {
      // to_del_key is head
      ordered_by_keys_head_ = ordered_by_keys_head_->next;
      delete to_del_key;
    } else if (to_del_key->next == nullptr) {
      // to_del_key is tail
      key_prev->next = nullptr;
      delete to_del_key;
    } else {
      // to_del_key is in middle
      key_prev->next = to_del_key->next;
      delete to_del_key;
    }

    if (to_del_val == ordered_by_vals_head_) {
      // to_del_val is head
      ordered_by_vals_head_ = ordered_by_vals_head_->next;
      delete to_del_val;
    } else if (to_del_val->next == nullptr) {
      // to_del_val is tail
      val_prev->next = nullptr;
      delete to_del_val;
    } else {
      // to_del_val is in middle
      val_prev->next = to_del_val->next;
      delete to_del_val;
    }

    --sz_;
    return to_return;
  }
}

// bool BiMap<K, V>::add(K key, V value)
// purpose: add a unique node to the BiMap if and only if both the key and value
//          do not yet exist in the BiMap, increment sz, and return true; 
//          if either exists, do not add and return false
// args:    "key": the "first" member of the node to be added to the BiMap
//          "value": the "second" member of the node to be added to the BiMap
// return:  true if both key and value are unique and new node is successfully
//          added to the BiMap; else, false
template<typename K, typename V>
bool BiMap<K, V>::add(K key, V value) {
  Node<K, V>* to_add_key = new Node<K, V>(key, value);
  Node<K, V>* to_add_val = new Node<K, V>(key, value);

  if ((find_key(key).second != nullptr && find_key(key).second->first == key) ||
    (find_value(value).second != nullptr && find_value(value).second->second == value)) {
    // key or value is not unique; do not add
    delete to_add_key;
    delete to_add_val;
    return false;
  } else if (ordered_by_keys_head_ == nullptr && ordered_by_vals_head_ == nullptr) {
    // list is empty; add to beginning
    ordered_by_keys_head_ = to_add_key;
    ordered_by_vals_head_ = to_add_val;
    ++sz_;
    return true;
  } else {
    // list is not empty; add nodes to keys and vals list one at a time
    Node<K, V>* keys_prev = find_key(key).first;
    Node<K, V>* keys_found = find_key(key).second;

    if (keys_found == ordered_by_keys_head_) {
      // add to beginning of keys list
      to_add_key->next = ordered_by_keys_head_;
      ordered_by_keys_head_ = to_add_key;
    } else if (keys_found == nullptr) {
      // add to end of keys list (recall that find_'s first return value
      // will point to end of list if element is not found in list
      keys_prev->next = to_add_key;
    } else {
      // add to middle of keys list
      keys_prev->next = to_add_key;
      to_add_key->next = keys_found;
    }

    Node<K, V>* vals_prev = find_value(value).first;
    Node<K, V>* vals_found = find_value(value).second;

    if (vals_found == ordered_by_vals_head_) {
      // add to beginning of vals list
      to_add_val->next = ordered_by_vals_head_;
      ordered_by_vals_head_ = to_add_val;
    } else if (vals_found == nullptr) {
      // add to end of vals list (recall that find_'s first return value
      // will point to end of list if element is not found in list
      vals_prev->next = to_add_val;
    } else {
      // add to middle of vals list
      vals_prev->next = to_add_val;
      to_add_val->next = vals_found;
    }

    ++sz_;
    return true;
  }
}

// V BiMap<K, V>::value_from_key(K key)
// purpose: returns the value associated with a key in a BiMap if the argument
//          key exists -- otherwise, returns a default type V
// args:    "key": the key to search for a match within the BiMap's Nodes
// return:  the value associated with the argument "key" in a BiMap if the
//          argument key exists -- otherwise, a default type V
template<typename K, typename V>
V BiMap<K, V>::value_from_key(K key) {
  Node<K, V>* key_ptr = find_key(key).second;
  if (key_ptr != nullptr && key_ptr->first == key) {
    // we have a match; return it
    return key_ptr->second;
  } else {
    // no match
    return V();
  }
}

// K BiMap<K, V>::key_from_value(V value)
// purpose: returns the key associated with a vvalue in a BiMap if the argument
//          value exists -- otherwise, returns a default type K
// args:    "value": the value to search for a match within the BiMap's Nodes
// return:  the key associated with the argument "value" in a BiMap if the
//          argument value exists -- otherwise, a default type K
template<typename K, typename V>
K BiMap<K, V>::key_from_value(V value) {
  Node<K, V>* val_ptr = find_value(value).second;
  if (val_ptr != nullptr && val_ptr->second == value) {
    // we have a match; return it
    return val_ptr->first;
  } else {
    // no match
    return K();
  }
}

// bool BiMap<K, V>::update(K key, V value)
// purpose: updates a Node with key matching argument key to have the argument
//          value and returns true if such a Node exists within the BiMap; else,
//          does nothing and returns false
// args:    "key": the key of the Node to be updated (if a Node with a
//          matching key exists)
//          "value": the value which the Node with key matching argument key
//          is to be updated to (if a Node with a matching key exists)
// return:  true if a Node with key matching argument key was found and
//          successfully updated; else, false
template <typename K, typename V>
bool BiMap<K, V>::update(K key, V value) {
  Node<K, V>* to_update_key = find_key(key).second;
  if (to_update_key != nullptr && to_update_key->first == key) {
    // we have a matching key; remove old entry and add new updated one
    remove_key(key); // note: will remove from both lists
    add(key, value); // note: will add new key to both lists
    return true;
  } else {
    // no matching key exists; do nothing and return false
    return false;
  }
}

// int BiMap<K, V>::compare(BiMap& other)
// purpose: lexicographically compares the keys of the Nodes of the calling
//          BiMap to those of the argument BiMap to return (as an int) which
//          BiMap is "greater"
// args:    "other": the BiMap to which the keys of the Nodes of calling BiMap
//          are to be compared to
// return:  1 if the calling BiMap is "greater", -1 if the argument BiMap is 
//          "greater", or 0 if the two BiMaps have exactly equal keys
// note:    compares keys of Nodes of BiMaps one-by-one by looking at the
//          ordered_by_keys_ array in each
// note:    "greater" means that the first non-equal key of the "greater"
//          BiMap compared lexicographically greater than the corresponding
//          key in the other BiMap--or, if the BiMaps have all-equal elements,
//          but one is longer, then the longer BiMap is "greater"
template <typename K, typename V>
int BiMap<K, V>::compare(BiMap& other) {
  Node<K, V>* calling_node = ordered_by_keys_head_;
  Node<K, V>* arg_node = other.ordered_by_keys_head_;

  while (calling_node != nullptr && arg_node != nullptr) {
    if (calling_node->first > arg_node->first) {
      return 1;
    } else if (arg_node->first > calling_node->first) {
      return -1;
    }
    // else, check the next node
    calling_node = calling_node->next;
    arg_node = arg_node->next;
  }

  // BiMaps are equal through first (smaller.size()) elements; check length
  if (calling_node == nullptr && arg_node != nullptr) {
    // calling BiMap is shorter
    return -1;
  } else if (arg_node == nullptr && calling_node != nullptr) {
    // calling BiMap is longer
    return 1;
  } else {
    // BiMaps are same size and equal
    return 0;
  }
}

// BiMap<K, V> BiMap<K, V>::merge(BiMap& other)
// purpose: construct a new BiMap object by merging a calling BiMap and an
//          argument BiMap
// args:    "other": the argument BiMap to be merged with calling BiMap
// return:  a new BiMap object with properly sorted, non-duplicate
//          ordered_by_keys_ and ordered_by_vals_ members
// note:    in the case of duplicates, precedence goes to calling BiMap
//          as its elements are added in the first loop
template <typename K, typename V>
BiMap<K, V> BiMap<K, V>::merge(BiMap& other) {
  BiMap<K, V> to_return = BiMap<K, V>();
  for (Node<K, V>* node = ordered_by_keys_head_; node != nullptr; node = node->next) {
    to_return.add(node->first, node->second);
  }
  for (Node<K, V>* node = other.ordered_by_keys_head_; node != nullptr; node = node->next) {
    to_return.add(node->first, node->second);
  }
  return to_return;
}