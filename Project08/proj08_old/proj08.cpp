/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 08 - Picture Painter - proj08.cpp
 * 10.31.18
 */

#include <map>
using std::map;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <fstream>
using std::ifstream;
#include <stdexcept>
using std::exception; using std::out_of_range;
#include <sstream>
using std::istringstream;
#include <cctype>
using std::isdigit; using std::isspace;

using std::pair;
using std::make_pair;
using std::to_string;
using std::stoi;

#include "proj08.h"

Painter::Painter(const string &fname) {
  ifstream ifs(fname, std::ifstream::in);
  int indx = 1;
  while (ifs) {
    string line;
    getline(ifs, line);
    if (!line.empty()) {
      pair<int, string> temp_pair(indx, line);
      lines_.insert(temp_pair);
    }
    ++indx;
  }
}

// this function parses the file, draws the canvas, and logs errors
void Painter::CreateCanvas() {
  for (auto line : lines_) {
    int line_num = line.first;
    Parse(line_num);
  }
}

vector<string> Painter::GetCanvas() const {
  return canvas_;
}

vector<string> Painter::GetErrorLog() const {
  return error_log_;
}

// Parse a single line. (A good place to record a line error to the log)
void Painter::Parse(int line_num) {
  try {
    auto line_ptr = lines_.find(line_num);

    if (line_ptr == lines_.end()) {
      out_of_range oor_exception("Error: Line Error -- line num not in range @ ");
      throw(oor_exception);
    }

    string line = line_ptr->second;
    if (line.front() == '#') {
      //ignore line
    } else if (line.find("SetDim") == 0) {
      SetDim(line_num);
    } else if (line.find("SetPenPosition") == 0) {
      SetPenPosition(line_num);
    } else if (line.find("SetPenSymbol") == 0) {
      SetPenSymbol(line_num);
    } else if (line.find("Draw") == 0) {
      Draw();
    } else if (line.find("Move") == 0) {
      Move(line_num);
    } else if (line.find("Turn") == 0) {
      Turn(line_num);
    } else if (line.find("Repeat") == 0) {
      Repeat(line_num);
    } else {
      error_log_.push_back("error on line " + to_string(line_num) +
        ": unable to parse \"" + line + "\"");
    }
  } catch (out_of_range oor) {
    error_log_.push_back(oor.what() + to_string(line_num));
  }
}

// reset the dimensions of the canvas
void Painter::SetDim(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  vector<int> input = parse_num(line);
  // input of set dim is (rows, columns) according to mimir
  int temp_rows = input.at(0);
  int temp_cols = input.at(1);

  if (temp_cols < 10 || temp_cols > 80) {
    error_log_.push_back("error on line " + to_string(line_num) + 
      ": Column Dimension out of bounds");
  } else if (temp_rows < 10 || temp_rows > 80) { // only log dimension error once
    error_log_.push_back("error on line " + to_string(line_num) +
      ": Row Dimension out of bounds");
  } else {
    columns_ = temp_cols;
    rows_ = temp_rows;
    // recreate canvas with new dimensions
    canvas_ = vector<string>(rows_, string(columns_, ' '));
  }
}

// Set the position of the pen
void Painter::SetPenPosition(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  vector<int> input = parse_num(line);
  // note: according to mimir test cases, input of SetPenPosition takes form of
  // (row, column) or (y, x) -- however, my class is written to represent
  // pen_position at (x, y)
  int y = input.at(0);
  int x = input.at(1);
  if (x < 0 || x >= columns_ || y < 0 || y >= rows_) {
    error_log_.push_back("error on line " + to_string(line_num) + 
      ": Pen cannot be placed on canvas at position " + to_string(y) + " " +
      to_string(x));
  } else {
    pen_position_ = make_pair(x, y);
  }
}

// overloaded - for use internally by move
// OBSOLETE - DELETE
/*
void Painter::SetPenPosition(int x, int y) {
  if (x < 0 || x > columns_) {
    error_log_.push_back(string("Error: SetPenPosition X out of range @ ") + to_string(line_num));
  } else if (y < 0 || y > rows_) {
    error_log_.push_back(string("Error: SetPenPosition Y out of range @ ") + to_string(line_num));
  } else {
    pen_position_ = make_pair(x, y);
  }
}
*/

// Set the pen symbol
void Painter::SetPenSymbol(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  int symbol = parse_num(line).at(0);

  if (symbol < 32 || symbol > 126) {
    error_log_.push_back("error on line " + to_string(line_num) + 
      ": Invalid Pen Symbol " + to_string(symbol));
  } else {
    pen_symbol_ = static_cast<char>(symbol);
  }
}

// Draw the current symbol at the current position
void Painter::Draw() {
  int x = pen_position_.first;
  int y = pen_position_.second;
  string &row = canvas_.at(y); //ref lets us modify
  row.at(x) = pen_symbol_; //change ordered pair of canvas to symbol
}

// Move the pen in the direction it is facing n steps
void Painter::Move(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  int num = parse_num(line).at(0);
  int current_x = pen_position_.first;
  int current_y = pen_position_.second;

  // note: (0,0) is at top left of canvas -- position ordered pair has (x, y)
  // with the positive x axis extending right and the positive y axis
  // extending downwards

  switch (dir_) {
    case direction::right:
      pen_position_.first = (current_x + num) % columns_;
      break;
    case direction::left:
      if (current_x >= num) {
        pen_position_.first = current_x - num;
      } else {
        pen_position_.first = columns_ - ((current_x - num) % columns_);
      }
      break;
    case direction::up:
      if (current_y >= num) {
        pen_position_.second = current_y - num;
      } else {
        pen_position_.second = rows_ - ((current_y - num) % rows_);
      }    
      break;
    case direction::down:
      pen_position_.second = (current_y + num) % rows_;
      break;
  }
}

// Turn the pen 90 degrees clockwise n times
void Painter::Turn(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  int num = parse_num(line).at(0);

  int temp_dir = (((static_cast<int>(dir_)) + num - 1) % 4) + 1;
  dir_ = static_cast<direction>(temp_dir);
}

// Repeat a range of commands the specified number of times
void Painter::Repeat(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  vector<int> input = parse_num(line);
  int n = input.at(0);
  int start_line = input.at(1);
  int end_line = input.at(2);
  bool safe_to_repeat = true;

  if (lines_.find(start_line) == lines_.end()) {
    error_log_.push_back("error on line " + to_string(line_num) +
      ": Line " + to_string(start_line) + " is not in the file"); 
    safe_to_repeat = false;
  }
  if (lines_.find(end_line) == lines_.end()) {
    error_log_.push_back("error on line " + to_string(line_num) +
      ": Line " + to_string(end_line) + " is not in the file");
    safe_to_repeat = false;
  }
  if (start_line >= end_line) {
    error_log_.push_back("error on line " + to_string(line_num) +
      ": Repeat range must be ascending");
    safe_to_repeat = false;
  }
  if (line_num >= start_line) {
    error_log_.push_back("error on line " + to_string(line_num) +
      ": Repeat command contains itself or refers to previous commands; " +
      "infinite loop!");
    safe_to_repeat = false;
  } 
  
  if (safe_to_repeat) {
    //good to go
    for (int i = 0; i < n; ++i) { //call all lines n times
      for (int j = start_line; j <= end_line; ++j) { //call start to end lines
        Parse(j);
      }
    }
    //comment out lines called by loop to avoid secondary execution
    /*
    for (int j = start_line; j <= end_line; ++j) {
          auto line_ptr = lines_.find(j);
          string line = line_ptr->second;
          string commented_line = "#" + line;
          line_ptr->second = commented_line;
    }
    */
  }
}


// helper method used in parsing process
vector<int> parse_num(const string &s) {
  vector<int> result;
  string temp_result;
  for (char c : s) {
    if (isdigit(c)) {
      temp_result.push_back(c);
    } else if ((isspace(c) || c == '-') && (!temp_result.empty())) { //delimiters
      result.push_back(stoi(temp_result));
      temp_result.clear();
    }
  }
  if (!temp_result.empty()) {
    result.push_back(stoi(temp_result));
  }
  return result;
}
