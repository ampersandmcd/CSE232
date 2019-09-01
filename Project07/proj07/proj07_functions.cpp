/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 07 - Spell Checker & Word Completion - proj07_functions.cpp
 * 10.26.18
 */


#include "proj07_functions.h"

#include <algorithm>
using std::set_intersection;
#include <fstream>
using std::ifstream;
#include <set>
using std::set;
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <iterator>
using std::ostream_iterator; using std::inserter;
#include <cctype>
using std::isalpha; using std::isupper; using std::tolower;


//deletes
//purpose: creates all possible single-letter deletes in a given word
//args:    "word": root string to find single-letter deletes from;
//         "result": reference to string-type set where all single-letter
//         deletes will be placed
//return:  void (all single-letter deletes are placed into "result" string-type 
//         set passed by reference)
void deletes(const string &word, set<string> &result) {
  for (size_t pos = 0; pos < word.size(); ++pos) {
    result.insert(word.substr(0, pos) + word.substr(pos + 1));
  }
}

//inserts
//purpose: creates all possible single-letter inserts in a given word
//args:    "word": root string to find single-letter inserts from;
//         "result": reference to string-type set where all single-letter
//         inserts will be placed
//return:  void (all single-letter inserts are placed into "result" string-type 
//         set passed by reference)
void inserts(const string &word, set<string> &result) {
  for (size_t pos = 0; pos <= word.size(); ++pos) {
    for (char c : letters) {
      if (pos == 0) {
        result.insert(string(1, c) + word);
      } else if (pos == word.size()) {
        result.insert(word + string(1, c));
      } else {
        result.insert(word.substr(0, pos) + c + word.substr(pos));
      }
    }
  }
}

//transposes
//purpose: creates all possible bi-letter swaps (transposes) in a given word
//args:    "word": root string to find bi-letter swaps (transposes) from;
//         "result": reference to string-type set where all bi-letter
//         swaps (transposes) will be placed
//return:  void (all bi-letter swaps (transposes) are placed into "result"
//         string-type set passed by reference)
void transposes(const string &word, set<string> &result) {
  for (size_t pos = 0; pos < word.size(); ++pos) {
    if (pos == 0) {
      result.insert(word.substr(1, 1) + word.at(0) + word.substr(2));
    } else if (pos == word.size() - 1) {
      result.insert(word.substr(0, pos - 1) + word.at(pos) + word.at(pos - 1));
    } else {
      result.insert(word.substr(0, pos) + word.at(pos + 1) + word.at(pos) + 
                                                          word.substr(pos + 2));
    }
   }
}

//replaces
//purpose: creates all possible single-letter replacements in a given word
//args:    "word": root string to find single-letter replacements from;
//         "result": reference to string-type set where all single-letter
//         replacements will be placed
//return:  void (all single-letter replacements are placed into "result"
//         string-type set passed by reference)
void replaces(const string &word, set<string> &result) {
  for (size_t pos = 0; pos <= word.size(); ++pos) {
    for (char c : letters) {
      if (pos == 0) {
        result.insert(string(1, c) + word.substr(pos + 1));
      } else if (pos == word.size()) {
        result.insert(word.substr(0, pos - 1) + c);
      } else {
        result.insert(word.substr(0, pos - 1) + c + word.substr(pos));
      }
    }
  }
}

//read_words
//purpose: reads a set of words from a text file into a string-type set;
//         converts all words to lowercase and removes any non-alpha characters
//         prior to placing them in set
//args:    "fname": name of file from which words are to be read;
//         "result": reference to string-type set where all words read from file
//         will be placed
//return:  void (all words read from file are placed into "result" string-type 
//         set passed by reference)
void read_words(string fname, set<string> &result) {
  ifstream ifs(fname, std::ifstream::in);
  while (ifs) {
    string temp_word;
    ifs >> temp_word;
    size_t pos = 0;
    while (pos < temp_word.size()) {
      char c = temp_word.at(pos);
      if (!isalpha(c)) {
        temp_word.erase(pos, 1);
      } else if (isupper(c)) {
        temp_word.erase(pos, 1);
        temp_word.insert(pos, string(1, tolower(c)));
        ++pos;
      } else {
        ++pos;
      }
    }
    result.insert(temp_word);
  }
  //get rid of empty string element, if it exists:
  if (result.count("") > 0) {
    auto itr = result.find("");
    result.erase(itr);
  }
}

//find_completions
//purpose: finds all words in "word_list" beginning with "word" and places them
//         into a string-type set  "result"
//args:    "word": string which the function searches "word_list" for potential
//         completions of
//         "word_list": reference to string-type set where all candidates for 
//         potential completions of "word" exist
//         "result": reference to string-type set where all potential
//         completions of "word" from "word_list" are placed
//return:  void (all candidate strings in "word_list" which are potential
//         completions of "word" are placed into "result" set passed by ref)
void find_completions(const string &word, const set<string> &word_list,
                                                          set<string> &result) {
  for (auto itr = word_list.begin(); itr != word_list.end(); ++itr) {
    if ((*itr).find(word) == 0) { //entry is the beginning of an existing word
      result.insert(*itr);
    }
  }
}

//find_corrections
//purpose: finds all potential corrections for argument "word" by finding all
//         single-letter deletes, replacements, transposes and inserts of "word"
//args:    "word": string which the function finds all potential corrections of
//         "result": reference to string-type set where all potential
//         single-letter corrections of "word" are placed
//return:  void (all potential correction strings of "word" are placed into 
//         "result" set passed by ref)
void find_corrections(const string &word, set<string> &result) {
  deletes(word, result);
  inserts(word, result);
  transposes(word, result);
  replaces(word, result);
}

//find_2step_corrections
//purpose: finds all potential 2-step corrections for argument "word" by first
//         finding all potential single-letter corrections of "word", then for
//         each single-letter correction candidate, finding all potential
//         second-level-single-letter corrections
//args:    "word": string which the function finds all potential 2-step 
//         corrections of
//         "result": reference to string-type set where all potential
//         2-step corrections of "word" are placed
//return:  void (all potential 2-step correction strings of "word" are placed 
//         into "result" set passed by ref)
void find_2step_corrections(const string &word, set<string> &result) {
  find_corrections(word, result);
  set<string> one_level_corr = result;
  for (const string &s : one_level_corr) {
    find_corrections(s, result);
  }
}

//find_reasonable_corrections
//purpose: finds all corrections of some word in "possibles" which exist in
//         a dictionary set of known words called "word_list" and places all
//         reasonable corrections in string-type set "result"
//args:    "possibles": string-type set containing all corrections of some word
//         "word_list": string-type set acting as dictionary of known words
//         "result": reference to string-type set where all reasonable
//         corrections are stored
//return:  void (all reasonable corrections in "possibles" are placed into 
//         "result" set passed by ref)
void find_reasonable_corrections(const set<string> &possibles,
                          const set<string> &word_list, set<string> &result) {

  set_intersection(possibles.begin(), possibles.end(), word_list.begin(),
                              word_list.end(), inserter(result, result.end()));

  //get rid of empty string element, if it exists
  if (result.count("") > 0) {
    auto itr = result.find("");
    result.erase(itr);
  }
}

//print_set
//purpose: prints a string-type set for debugging purposes
//args:    "s": string-type set to be printed
//return:  string containing all elements of "s", delimited with " | "
string print_set(const set<string> &s) {
  ostringstream oss;
  ostream_iterator<string> osi(oss, " | ");
  copy(s.begin(), s.end(), osi);
  return oss.str();
}