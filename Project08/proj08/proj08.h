/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 08 - Picture Painter - proj08.h
 * 11.9.18
 */

#include <map>
using std::map;
#include <string>
using std::string;
#include <vector>
using std::vector;

#ifndef PAINTER
#define PAINTER


class Painter {

public:
  //mandatory methods: see proj08.cpp for complete function descriptions
  Painter(const string &fname);
  void CreateCanvas();
  vector<string> GetCanvas() const;
  vector<string> GetErrorLog() const;

private:
  //data members:

  //directions pen may face
  enum direction { right = 1, down = 2, left = 3, up = 4 };

  //current direction pen is facing
  direction dir_  = direction::right;

  //number of rows in the canvas
  int rows_ {10};

  //number of columns in the canvas
  int columns_{10};

  //current pen_symbol
  char pen_symbol_{'#'};

  //pen position on canvas, represented as (x,y) where (0,0) is at top left of
  //canvas, positive x-axis extends rightwards, and positive y-axis extends
  //downwards
  std::pair<int,int> pen_position_{0,0};

  //canvas on which the 'painting' is created
  vector<string> canvas_ = vector<string>(rows_,string(columns_,' '));

  //log containing all errors encountered in the interpretation of the script
  //file / creation of the 'painting'
  vector<string> error_log_;

  //lines of the script file from which the Painter object is constructed
  map<int, string> lines_;



  //non-mandatory methods: see proj08.cpp for complete function descriptions
  void Parse(int line_num);
  void SetDim(int line_num);
  void SetPenPosition(int line_num);
  void SetPenSymbol(int line_num);
  void Draw();
  void Move(int line_num);
  void Turn(int line_num);
  void Repeat(int line_num);
};

//additional helper method: see proj08.cpp for complete function description
vector<int> parse_num(const string &s);

#endif