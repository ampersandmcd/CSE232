/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 06 - Steganography - proj06_functions.cpp
 * 10.18.18
 */

#include<vector>
using std::vector;
#include<string>
using std::string;
#include <fstream>
using std::ifstream;
#include <exception>
using std::runtime_error;
#include <iostream>
using std::cout; using std::endl;
#include <iomanip>
using std::setw; using std::setfill; using std::right; using std::boolalpha;
#include <sstream>
using std::ostringstream;
#include <cmath>
using std::pow;

#include "proj06_functions.h"
using matrix = vector<vector<int>>;

//start: spec functions

//read_file
//purpose: read a text file of 0's and 1's and store in an int matrix 
//         representing a printer's steganographic signature
//input:   filename (as a string) of text file to be read
//output:  int matrix representing the steganographic signature of a printer
matrix read_file(const string &fname) {
  matrix result_mat;
  ifstream ifs(fname);

  if (ifs.fail()) {
    throw runtime_error("Cannot open specified file; invalid filename.");
  }

  while (ifs) { //while input filestream has not hit EOF
    string temp_row;
    vector<int> result_row;
    getline(ifs, temp_row);
    for (char c : temp_row) {
      int i = static_cast<int>(c - '0'); //convert string of 1's and 0's to ints
      result_row.push_back(i);
    }
    if (!result_row.empty()) { //prevents adding empty last row
      result_mat.push_back(result_row);
    }
  }
  return result_mat;
}

//get_row
//purpose: return a selected row of the int matrix representing a printer's
//         steganographic signature
//input:   int matrix representing steganographic signature, int row index
//output:  int vector representing the selected row of the int matrix
vector<int> get_row(const matrix &v, int row) {
  vector<int> result = v.at(row);
  return result;
}

//get_col
//purpose: return a selected column of the int matrix representing a printer's
//         steganographic signature
//input:   int matrix representing steganographic signature, int column index
//output:  int vector representing the selected column of the int matrix
vector<int> get_column(const matrix &v, int col) {
  vector<int> result;
  for (vector<int> row : v) {
    result.push_back(row.at(col));
  }
  return result;
}

//col_to_int
//purpose: return an integer from a column vector of 1's and 0's (from printer's
//         steganographic signature matrix) that represents a number in binary
//input:   int matrix representing steganographic signature, int column index
//output:  int represented by the selected column of the steganographic matrix
int col_to_int(const matrix &v, size_t col) {
  int sum = 0;
  vector<int> current_column = get_column(v, col);
  size_t col_size = current_column.size();

  for (size_t i = 1; i < col_size; ++i) { //start at 1 to skip parity row
    if (current_column.at(i) == 1) {
      sum += static_cast<int>(pow(2, (col_size - i - 1))); //convert bin to int
    }
  }
  return sum;
}

//get_time
//purpose: return the time encoded in a printer's steganographic signature
//input:   int matrix representing steganographic signature
//output:  string of the time represented by the steganographic signature
string get_time(const matrix &v) {
  ostringstream oss;
  const size_t HRS_COL = 4;
  const size_t MINS_COL = 1;
  int hours = col_to_int(v, HRS_COL);
  int mins = col_to_int(v, MINS_COL);

  oss << setfill('0') << setw(2) << right << hours << ":";
  oss << setfill('0') << setw(2) << right << mins;
  return oss.str();
}

//get_date
//purpose: return the date encoded in a printer's steganographic signature
//input:   int matrix representing steganographic signature
//output:  string of the date represented by the steganographic signature
string get_date(const matrix &v) {
  ostringstream oss;
  const size_t MON_COL = 6;
  const size_t DAY_COL = 5;
  const size_t YR_COL = 7;
  int month = col_to_int(v, MON_COL);
  int day = col_to_int(v, DAY_COL);
  int year = col_to_int(v, YR_COL);

  oss << setfill('0') << setw(2) << right << month << '/';
  oss << setfill('0') << setw(2) << right << day << '/';
  oss << "20" << setfill('0') << setw(2) << right << year;
  return oss.str();
}

//get_serial
//purpose: return the serial # encoded in a printer's steganographic signature
//input:   int matrix representing steganographic signature
//output:  string of the serial # represented by the steganographic signature
string get_serial(const matrix &v) {
  ostringstream oss;
  const size_t SERIAL_FIRST_COL = 10;
  const size_t SERIAL_LAST_COL = 13;

  for (size_t col = SERIAL_LAST_COL; col >= SERIAL_FIRST_COL; --col) {
    int num = col_to_int(v, col);
    oss << setfill('0') << setw(2) << right << num;
  }
  return oss.str();
}

//check_column_parity
//purpose: check that the parity bit of a given column in a printer's
//         steganographic signature matrix is properly set
//input:   int matrix representing steganographic signature, int column index
//output:  string of the form "x:y:bool" where x is the state of the parity bit
//         (1 or 0), y is the count of 1's in the selected column, and bool is 
//         true or false representing if the parity bit is set properly
string check_column_parity(const matrix &v, int col) {
  vector<int> current_col = get_column(v, col);
  const size_t PARITY_INDX = 0;
  ostringstream oss;

  int num_ones = 0;
  for (size_t row = 1; row < current_col.size(); ++row) { //skip parity row when
                                                          //counting ones
    if (current_col.at(row) == 1) {
      ++num_ones;
    }
  }
  oss << current_col.at(PARITY_INDX) << ":";
  oss << num_ones << ":";
  oss << boolalpha;
  if (current_col.at(PARITY_INDX) == parity(num_ones)) {
    oss << true;
  } else {
    oss << false;
  }
  return oss.str();
}

//check_column_parity
//purpose: check that the parity bit of a given row in a printer's
//         steganographic signature matrix is properly set
//input:   int matrix representing steganographic signature, int row index
//output:  string of the form "x:y:bool" where x is the state of the parity bit
//         (1 or 0), y is the count of 1's in the selected row, and bool is 
//         true or false representing if the parity bit is set properly
string check_row_parity(const matrix &v, int r) {
  vector<int> current_row = get_row(v, r);
  const size_t PARITY_INDX = 0;
  ostringstream oss;

  int num_ones = 0;
  for (size_t col = 1; col < current_row.size(); ++col) { //skip parity col when
                                                          //counting ones
    if (current_row.at(col) == 1) {
      ++num_ones;
    }
  }
  oss << current_row.at(PARITY_INDX) << ":";
  oss << num_ones << ":";
  oss << boolalpha;
  if (current_row.at(PARITY_INDX) == parity(num_ones)) {
    oss << true;
  } else {
    oss << false;
  }
  return oss.str();
}

//end: spec functions
//start: helper functions

//parity
//helper function - returns 1 if num is even; returns 0 if num is odd
int parity(int num) {
  if (num % 2 == 0) {
    return 1;
  } else {
    return 0;
  }
}

//print_vector
//helper function - creates a print-friendly string from a given vector input
string print_vector(const vector<int> &v) {
  ostringstream oss;
  int width = 5;

  for (auto ele : v) {
    oss << setw(width) << ele;
  }
  oss << endl;
  return oss.str();
}

//print_matrix
//helper function - creates a print-friendly string from a given matrix input
string print_matrix(const matrix &v) {
  ostringstream oss;
  int width = 5;

  for (auto row : v) {
    for (auto ele : row) {
      oss << setw(width) << ele;
    }
    oss << endl;
  }
  return oss.str();
}

//end: helper functions