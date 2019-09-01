/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 08 - Picture Painter - proj08.h
 * 10.31.18
 */

#include <map>
using std::map;
#include <string>
using std::string;
#include <vector>
using std::vector;

#ifndef PAINTER
#define PAINTER

// the type that you must provide
class Painter {

public:
  // You must porovide these functions with exactly this signature
  // Painter Constructor
  Painter(const string &fname);

  // this function parses the file, draws the canvas, and logs errors
  void CreateCanvas();

  // return the canvas
  // return canvas_;
  vector<string> GetCanvas() const;

  // return the error log
  //  return error_log_;
  vector<string> GetErrorLog() const;

private:
  // the private stuff that we won't be testing directly
  // All of these are optional, but are guides to how you can build the class

  // directions (can be represented by ints, but an enum class is cleaner)
  enum direction { right = 1, down = 2, left = 3, up = 4 };
  // store the direction
  direction dir_  = direction::right;

  // number of rows in the canvas
  int rows_ {10};

  // number of columns in the canvas
  int columns_{10};

  // the current pen_symbol
  char pen_symbol_{'#'};

  // pen position can be represented by a pair or 2 different ints, etc
  // pen position is at x, y
  std::pair<int,int> pen_position_{0,0};

  // the actual canvas
  vector<string> canvas_ = vector<string>(rows_,string(columns_,' '));

  // the error log
  vector<string> error_log_;

  // the lines of the .pf file (with line numbers). A vector of strings, where
  // the position represents the line number is also reasonable
  map<int, string> lines_;

  // These are functions that you will probably find useful to write
  // Note that these functions are up to you; change the names, signature, etc.
  // add functions if you want, of course
  // Whatever works for you

  // Parse a single line. (A good place to record a line error to the log
  void Parse(int line_num);

  // reset the dimensions of the canvas
  void SetDim(int line_num);

  // Set the position of the pen from a direct script call
  void SetPenPosition(int line_num);

  // Set the position of the pen from an internal call
  void SetPenPosition(int x, int y);

  // Set the pen symbol
  void SetPenSymbol(int line_num);

  // Draw the current symbol at the current position
  void Draw();

  // Move the pen in the direction it is facing n steps
  void Move(int line_num);

  // Turn the pen 90 degrees clockwise n times
  void Turn(int line_num);

  // Repeat a range of commands the specified number of times
  void Repeat(int line_num);
};

// additional helper method for parsing
vector<int> parse_num(const string &s);


#endif