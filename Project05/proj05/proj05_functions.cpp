/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 05 - Playfair Cipher - proj05_functions.cpp
 * 10.10.18
 */


#include <string>
using std::string;
#include <cctype>
#include "proj05_functions.h"

//start: helper functions

//helper function - checks if two characters are in the same column of playfair
//cipher key
bool same_col(const string &key, const char &c1, const char &c2) {
  size_t c1_indx = key.find(c1);
  size_t c2_indx = key.find(c2);
  return (c1_indx % dim == c2_indx % dim) ? 1 : 0;
}

//helper function - checks if two characters are in the same row of playfair
//cipher key
bool same_row(const string &key, const char &c1, const char &c2) {
  size_t c1_indx = key.find(c1);
  size_t c2_indx = key.find(c2);
  return (c1_indx / dim == c2_indx / dim) ? 1 : 0;
}

//helper function - checks if a character is in the rightmost column of playfair
//cipher key
bool on_right_edge(const string &key, const char &c) {
  size_t c_indx = key.find(c);
  return ((c_indx + 1) % dim == 0) ? 1 : 0;
}

//helper function - checks if a character is in the leftmost column of playfair
//cipher key
bool on_left_edge(const string &key, const char &c) {
  size_t c_indx = key.find(c);
  return (c_indx % dim == 0) ? 1 : 0;
}

//helper function - checks if a character is in the bottom row of playfair
//cipher key
bool on_bottom_row(const string &key, const char &c) {
  size_t c_indx = key.find(c);
  return (c_indx / dim == (dim - 1)) ? 1 : 0;
}

//helper function - checks if a character is in the top row of playfair
//cipher key
bool on_top_row(const string &key, const char &c) {
  size_t c_indx = key.find(c);
  return (c_indx / dim == 0) ? 1 : 0;
}

//helper function - returns column index of a given character in playfair 
//cipher key
long get_col_indx(const string &key, const char &c) {
  size_t c_indx = key.find(c);
  return c_indx % dim;
}

//helper function - returns row index of a given character in playfair 
//cipher key
long get_row_indx(const string &key, const char &c) {
  size_t c_indx = key.find(c);
  return c_indx / dim;
}

//helper function - returns character at a given row and column of a playfair
//cipher key
char at_row_col(const string &key, const long &row, const long &col) {
  return key.at(row*dim + col);
}

//end: helper functions
//start: required functions

//prepare_plaintext
//purpose: returns string with no non a-i, k-x, or uppercase characters;
//         also replaces any second successively repeated character with 'x';
//         also ensures returned string is even in length by appending 'x'
//         if necessary in order to prepare a text string to be encoded using
//         playfair cipher encoding scheme
//input:   string to be prepared for playfair cipher encoding
//output:  string prepared for playfair cipher encoding
string prepare_plaintext(const string &s) {
  string result;
  char last = '\n';
  for (char c : s) {
    if (isalpha(c)) {
      if (c == toupper(last) || c == tolower(last)) {
        result.push_back('x');
      } else if (c != 'j' && c != 'J') {
        result.push_back(tolower(c));
      }
    }
    last = c;
  }

  if (result.size() % 2 != 0) {
    result.push_back('x');
  }

  return result;
}

//create_encoding
//purpose: returns a "playfair cipher key square" string of length 25 by
//         successively appending non-repeat characters from a keyword, then 
//         filling the remaining characters with non-repeat, alphabetically 
//         ordered characters (excluding J) until length 25 is reached
//input:   keyword string to be used for playfair cipher key square generation
//output:  25-character playfair cipher key square string
string create_encoding(const string &key) {
  string square;
  string used;
  size_t square_indx = 0;
  size_t key_indx = 0;
  size_t alpha_indx = 0;
  while (square_indx < (dim*dim)) {
    if (key_indx < key.size()) {
      char current = key.at(key_indx);
      if (used.find(current) == string::npos) {
        //key char (from keyword) has not yet been used
        square.push_back(current);
        used.push_back(current);
        square_indx++;
      }
      ++key_indx;
    } else {
      char current = alphabet_minus_j.at(alpha_indx);
      if (used.find(current) == string::npos) {
        //key char (from remaining alphabet) has not yet been used
        square.push_back(current);
        used.push_back(current);
        ++square_indx;
      }
      ++alpha_indx;
    }
  }
  return square;
}


//encode_pair
//purpose: encodes a pair of characters following playfair cipher rules, using
//         the supplied playfair cipher key square string
//input:   2-character string to be encoded, playfair cipher key square string
//output:  2-character encoded string
string encode_pair(const string &pr, const string &key) {
  char c1 = pr.at(0);
  char c2 = pr.at(1);
  size_t c1_indx = key.find(c1);
  size_t c2_indx = key.find(c2);
  string result;

  if (same_col(key, c1, c2)) {
    //use same col "shift" encoding method
    if (on_bottom_row(key, c1)) {
      result.push_back(at_row_col(key, 0, get_col_indx(key, c1)));
    } else {
      result.push_back(at_row_col(key, (get_row_indx(key, c1) + 1),
                                  get_col_indx(key, c1)));
    }
    if (on_bottom_row(key, c2)) {
      result.push_back(at_row_col(key, 0, get_col_indx(key, c2)));
    } else {
      result.push_back(at_row_col(key, (get_row_indx(key, c2) + 1),
                                  get_col_indx(key, c2)));
    }
  } else if (same_row(key, c1, c2)) {
    //use same row "shift" encoding method
    if (on_right_edge(key, c1)) {
      result.push_back(at_row_col(key, get_row_indx(key, c1), 0));
    } else {
      result.push_back(at_row_col(key, get_row_indx(key, c1),
                                  (get_col_indx(key, c1) + 1)));
    }

    if (on_right_edge(key, c2)) {
      result.push_back(at_row_col(key, get_row_indx(key, c2), 0));
    } else {
      result.push_back(at_row_col(key, get_row_indx(key, c2), 
                                  (get_col_indx(key, c2) + 1)));
    }
  } else {
    //use "box" encoding method
    long c1_col = get_col_indx(key, c1);
    long c2_col = get_col_indx(key, c2);
    long c1_row = get_row_indx(key, c1);
    long c2_row = get_row_indx(key, c2);
    result.push_back(at_row_col(key, c1_row, c2_col));
    result.push_back(at_row_col(key, c2_row, c1_col));
  }
  return result;
}

//decode_pair
//purpose: decodes a pair of characters following playfair cipher rules, using
//         the supplied playfair cipher key square string
//input:   2-character (encoded) string to be decoded, playfair cipher key 
//         square string
//output:  2-character decoded string
string decode_pair(const string &pr, const string &key) {
  char c1 = pr.at(0);
  char c2 = pr.at(1);
  size_t c1_indx = key.find(c1);
  size_t c2_indx = key.find(c2);
  string result;

  if (same_col(key, c1, c2)) {
    //use same col "shift" decoding method
    if (on_top_row(key, c1)) {
      result.push_back(at_row_col(key, (dim - 1), get_col_indx(key, c1)));
    } else {
      result.push_back(at_row_col(key, (get_row_indx(key, c1) - 1),
                                  get_col_indx(key, c1)));
    }
    if (on_top_row(key, c2)) {
      result.push_back(at_row_col(key, (dim - 1), get_col_indx(key, c2)));
    } else {
      result.push_back(at_row_col(key, (get_row_indx(key, c2) - 1),
                                  get_col_indx(key, c2)));
    }
  } else if (same_row(key, c1, c2)) {
    //use same row "shift" decoding method
    if (on_left_edge(key, c1)) {
      result.push_back(at_row_col(key, get_row_indx(key, c1), (dim - 1)));
    } else {
      result.push_back(at_row_col(key, get_row_indx(key, c1),
                                  (get_col_indx(key, c1) - 1)));
    }
    if (on_left_edge(key, c2)) {
      result.push_back(at_row_col(key, get_row_indx(key, c2), (dim - 1)));
    } else {
      result.push_back(at_row_col(key, get_row_indx(key, c2),
                                  (get_col_indx(key, c2) - 1)));
    }
  } else {
    //use "box" decoding method
    long c1_col = get_col_indx(key, c1);
    long c2_col = get_col_indx(key, c2);
    long c1_row = get_row_indx(key, c1);
    long c2_row = get_row_indx(key, c2);
    result.push_back(at_row_col(key, c1_row, c2_col));
    result.push_back(at_row_col(key, c2_row, c1_col));
  }
  return result;
}

//encode
//purpose: encodes a string using a given playfair cipher key square by breaking
//         string into 2-character strings and using encode_pair
//input:   string to be encoded, playfair cipher key square string
//output:  encoded string
string encode(const string &plaintxt, const string &key) {
  string prepared = prepare_plaintext(plaintxt);
  string result;
  for (size_t i = 0; i < prepared.size(); i += 2) {
    string pair = encode_pair(prepared.substr(i, 2), key);
    result += pair;
  }
  return result;
}

//decode
//purpose: decodes a string using a given playfair cipher key square by breaking
//         string into 2-character strings and using decode_pair
//input:   (encoded) string to be decoded, playfair cipher key square string
//output:  decoded string
string decode(const string &encodedtxt, const string &key) {
  string result;
  for (size_t i = 0; i < encodedtxt.size(); i += 2) {
    string pair = decode_pair(encodedtxt.substr(i, 2), key);
    result += pair;
  }
  return result;
}
