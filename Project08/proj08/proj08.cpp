/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 08 - Picture Painter - proj08.cpp
 * 11.9.18
 */

#include <map>
using std::map;
#include <string>
using std::string; using std::to_string; using std::stoi;
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

#include "proj08.h"

//Painter::Painter(const string &fname) constructor
//purpose: constructs a painter object given a script file to parse
//args:    "fname": name of a valid script file to be parsed in the creation of 
//         a painted canvas
//return:  n/a (constructs Painter object)
Painter::Painter(const string &fname) {
  ifstream ifs(fname, std::ifstream::in);
  int indx = 1;
  while (ifs) {
    string line;
    getline(ifs, line);
    if (!line.empty()) {
      //add all non-blank lines of file to lines_ (map<int, string>) member
      pair<int, string> temp_pair(indx, line);
      lines_.insert(temp_pair);
    }
    ++indx;
  }
}

//Painter::CreateCanvas()
//purpose: calls Painter::Parse on each line of script file to generate painted
//         canvas
//args:    none
//return:  void - all functionality is carried out by successive calls to
//         Painter::Parse method
void Painter::CreateCanvas() {
  for (auto line : lines_) {
    int line_num = line.first;
    Parse(line_num);
  }
}

//Painter::GetCanvas() const
//purpose: accessor method to retrieve canvas_ (vector<string>) member
//args:    none
//return:  vector<string> representing 2D character-based canvas
vector<string> Painter::GetCanvas() const {
  return canvas_;
}

//Painter::GetErrorLog() const
//purpose: accessor method to retrieve error_log_ (vector<string>) member
//args:    none
//return:  vector<string> containing all error messages generated in the
//         creation of the painted canvas from the given script file 
vector<string> Painter::GetErrorLog() const {
  return error_log_;
}

//Painter::Parse(int line_num)
//purpose: serves as the intermediate processing step in canvas creation,
//         analyzing each line in a given script file, determining whether or
//         not each line is valid, and calling the corresponding private Painter
//         method matching each valid line of input
//args:    "line_num": the line number (read from the given script file, stored
//         in the map<int, string> lines_ member) to be parsed
//return:  void (all functionalities are carried out internally by other Painter
//         methods)
void Painter::Parse(int line_num) {
  try {
    auto line_ptr = lines_.find(line_num);

    if (line_ptr == lines_.end()) {
      out_of_range oor_exception("error: line number not in range: ");
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

//Painter::SetDim(int line_num)
//purpose: carries out functionality of a "SetDim X Y" call within the script
//         file; checks to ensure that desired dimensions of canvas are in range
//         [10, 80] in each dimension and changes canvas_ member accordingly, or
//         writes an error to error_log_ if dimensions are invalid
//args:    "line_num": the line number (read from the given script file, stored
//         in the map<int, string> lines_ member) containing a "SetDim X Y" call
//         to be executed
//return:  void (all functionalities are carried out internally by manipulating
//         Painter data members (canvas_ and error_log_)
void Painter::SetDim(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  vector<int> input = parse_num(line);
  //note: input of set dim is (rows, columns) according to Mimir tests,
  //despite description saying "SetDim" took parameters "X Y"; implementation
  //has been adapted accordingly:
  int temp_rows = input.at(0);
  int temp_cols = input.at(1);

  if (temp_cols < 10 || temp_cols > 80) {
    error_log_.push_back("error on line " + to_string(line_num) + 
      ": Column Dimension out of bounds");
  } else if (temp_rows < 10 || temp_rows > 80) { 
    //only log dimension error once; hence else if
    error_log_.push_back("error on line " + to_string(line_num) +
      ": Row Dimension out of bounds");
  } else {
    //input is valid: recreate canvas with new dimensions
    columns_ = temp_cols;
    rows_ = temp_rows;
    canvas_ = vector<string>(rows_, string(columns_, ' '));
  }
}

//Painter::SetPenPosition(int line_num)
//purpose: carries out functionality of a "SetPenPosition X Y" call within the 
//         script file; checks to ensure that desired pen position on canvas is
//         valid (i.e., is contained within dimensions of canvas) and changes
//         pen_position_ member accordingly, or writes an error to error_log_ if
//         dimensions are invalid
//args:    "line_num": the line number (read from the given script file, stored
//         in the map<int, string> lines_ member) containing a 
//         "SetPenPosition X Y" call to be executed
//return:  void (all functionalities are carried out internally by manipulating
//         Painter data members (pen_position_ and error_log_)
void Painter::SetPenPosition(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  vector<int> input = parse_num(line);
  //note: according to Mimir test cases, input of SetPenPosition takes form of
  //(row, column) or (y, x); however, my class is written to represent
  //pen_position_ at (x, y) since the description notes that the parameters of 
  //"SetPenPosition" are "X Y"; implementation has been adapted accordingly:
  int y = input.at(0);
  int x = input.at(1);
  if (x < 0 || x >= columns_ || y < 0 || y >= rows_) {
    error_log_.push_back("error on line " + to_string(line_num) + 
      ": Pen cannot be placed on canvas at position " + to_string(y) + " " +
      to_string(x));
  } else {
    //input is valid: reset pen position at new location
    pen_position_ = make_pair(x, y);
  }
}

//Painter::SetPenSymbol(int line_num)
//purpose: carries out functionality of a "SetPenSymbol S" call within the 
//         script file; checks to ensure that desired pen symbol character is
//         valid (i.e., is within range [32, 126] of printable ASCII chars)
//         and changes pen_symbol_ member accordingly, or writes an error 
//         to error_log_ if pen symbol character is invalid
//args:    "line_num": the line number (read from the given script file, stored
//         in the map<int, string> lines_ member) containing a "SetPenSymbol S"
//         call to be executed
//return:  void (all functionalities are carried out internally by manipulating
//         Painter data members (pen_symbol_ and error_log_)
void Painter::SetPenSymbol(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  int symbol = parse_num(line).at(0);

  if (symbol < 32 || symbol > 126) {
    error_log_.push_back("error on line " + to_string(line_num) + 
      ": Invalid Pen Symbol " + to_string(symbol));
  } else {
    //input is valid: reset pen symbol to char corresponding to given int
    pen_symbol_ = static_cast<char>(symbol);
  }
}

//Painter::Draw()
//purpose: carries out functionality of a "Draw" call within the script file; 
//         draws the current pen_symbol_ character at the current
//         pen_position_ member onto the canvas_ member
//args:    none (uses data members pen_position_, pen_symbol_, and canvas_ to
//         carry out functionality)
//return:  void (all functionality is carried out internally by manipulating
//         canvas_ data member)
void Painter::Draw() {
  int x = pen_position_.first;
  int y = pen_position_.second;
  string &row = canvas_.at(y); //ref lets us modify
  row.at(x) = pen_symbol_;
}

//Painter::Move(int line_num)
//purpose: carries out functionality of a "Move N" call within the 
//         script file by advancing the pen_position_ member N-steps in the 
//         current direction given by the dir_ member;
//         does not need to check N for validity since the canvas_
//         member is represented toroidally; (i.e. canvas_ supports wraparound)
//args:    "line_num": the line number (read from the given script file, stored
//         in the map<int, string> lines_ member) containing a "Move N"
//         call to be executed
//return:  void (all functionality is carried out internally by manipulating
//         Painter pen_position_ data member
void Painter::Move(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  int num = parse_num(line).at(0);
  int current_x = pen_position_.first;
  int current_y = pen_position_.second;

  //note: I have chosen to represent the canvas_ with (0,0) at top left, 
  //and with the positive x axis extending right and the positive y axis
  //extending downwards

  //note: complexity of switch arises from toroidial (wrap-around) 
  //representation of canvas_
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

//Painter::Turn(int line_num)
//purpose: carries out functionality of a "Turn N" call within the 
//         script file by adjusting the dir_ member by 90 degrees in the 
//         clockwise direction N-times; does not need to check N for validity 
//         since the dir_ member wraps around after 4 90-degree rotations
//args:    "line_num": the line number (read from the given script file, stored
//         in the map<int, string> lines_ member) containing a "Turn N"
//         call to be executed
//return:  void (all functionality is carried out internally by manipulating
//         Painter dir_ data member
void Painter::Turn(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  int num = parse_num(line).at(0);

  int temp_dir = (((static_cast<int>(dir_)) + num - 1) % 4) + 1;
  dir_ = static_cast<direction>(temp_dir);
}

//Painter::Repeat(int line_num)
//purpose: carries out functionality of a "Repeat N B-E" call within the 
//         script file; first, checks to ensure that B and E are valid line
//         numbers in the script file, then, checks to ensure B comes before E,
//         finally, checks that the Repeat command at a line before B to avoid
//         infinite looping; if these conditions all hold, then the method calls
//         lines B-E using Parse() a total of N times
//args:    "line_num": the line number (read from the given script file, stored
//         in the map<int, string> lines_ member) containing a "Repeat N B-E"
//         call to be executed
//return:  void (all functionalities are carried out internally by successively
//         calling Parse() method)
void Painter::Repeat(int line_num) {
  auto line_ptr = lines_.find(line_num);
  string line = line_ptr->second;
  vector<int> input = parse_num(line);
  int n = input.at(0);
  int start_line = input.at(1);
  int end_line = input.at(2);
  bool safe_to_repeat = true; //serves as a flag to prevent potentially fatal
                              //repeat call from being executed

  //note: no else ifs because error_log_ is to contain all errors if more than
  //one exists; implemented with strict ifs instead
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
    //input is valid: execute Repeat N B-E call
    for (int i = 0; i < n; ++i) { //call range of lines N times
      for (int j = start_line; j <= end_line; ++j) { //call lines B through E
        Parse(j);
      }
    }
  }
}


//parse_num(const string &s)
//purpose: serves as a helper method in parsing numerical inputs following
//         script commands; acts as a sort of "split" function by finding all
//         delimited numbers in a script line string and returning them inside
//         of a vector<int>
//args:    "s": the string from which space- and dash-delimited numerical
//         entries are to be parsed; is generally a line from script file
//return:  vector<int> containing all space- and dash-delimited numerical
//         entries in s (note: return vector has one element for SetPenSymbol, 
//         Move, and Turn commands, two elements for SetDim and SetPenPosition
//         commands, and three elements for the Repeat command)
vector<int> parse_num(const string &s) {
  vector<int> result;
  string temp_result;
  for (char c : s) {
    if (isdigit(c)) {
      temp_result.push_back(c);
    } else if ((isspace(c) || c == '-') && (!temp_result.empty())) { //delimiter
      result.push_back(stoi(temp_result));
      temp_result.clear();
    }
  }
  if (!temp_result.empty()) { //push back numerical elements at end of string
    result.push_back(stoi(temp_result));
  }
  return result;
}
