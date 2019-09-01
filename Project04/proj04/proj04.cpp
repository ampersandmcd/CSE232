/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 04 - Run-length encoding
 * 9.27.18
 */

#include<iostream>
using std::cout; using std::cin; using std::endl;
#include<iomanip>
using std::setprecision;
#include<string>
using std::string;
// any other includes you think you might need here

// global variable for count -> char code
const string code = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

//
// your functions here
//
string encode_sequence(string sequence, char encoder) {
	string result = "";
	if (sequence.length() <= 3) {
		//don't encode
		return sequence;
	}
	else {
		result.push_back(encoder);
		result.push_back(code.at(sequence.length() - 4));
			result.push_back(sequence.at(0));
	}
	return result;
}


string encode(string to_encode, char encoder) {
	string sequence = "";
	string result = "";
	for (char c : to_encode) {
		if (sequence.empty() || c == sequence.back()) {
			//add this char to current sequence
			sequence.push_back(c);
		} 
		else {
			//send sequence to encode_sequence fcn and append to result, reset sequence
			result += encode_sequence(sequence, encoder);
			sequence.clear();
			sequence.push_back(c);
		}
	}
	result += encode_sequence(sequence, encoder); //ensures final sequence gets added to result string
	return result;
}

string decode_sequence(string sequence, char encoder) {
	string result = "";
	long num = 4 + code.find(sequence.at(1));
	char c = sequence.at(2);
	result.assign(num, c);
	return result;
}

string decode(string to_decode, char encoder) {
	string sequence = "";
	string result = "";
	for (decltype(to_decode.length()) i = 0; i < to_decode.length(); ++i) {
		char c = to_decode.at(i);
		if (c == encoder) {
			//decode 3-char sequence, and skip second two chars of encoded sequence in to_decode string
			result += decode_sequence(to_decode.substr(i, 3), encoder);
			i += 2; 
		}
		else {
			//add non-encoded char to result
			result.push_back(c);
		}
	}
	return result;
}

double reduction(string original, string encoded) {
	double ratio = static_cast<double>(encoded.length()) / static_cast<double>(original.length());
	return ratio;
}

int main (){
  long test_num;
  cin >> test_num;

  switch(test_num) {

  case 1: {
    string input;
    char sep;
    cin >> input;
    cin >> sep;
    cout << encode_sequence(input, sep);
    break;
  }

  case 2:{
    string input;
    char sep;
    cin >> input;
    cin >> sep;
    cout << encode(input, sep);
    break;
  }

  case 3:{
    string input;
    char sep;
    cin >> input;
    cin >> sep;
    cout << decode_sequence(input, sep);
    break;
  }

  case 4:{
    string input;
    char sep;
    cin >> input;
    cin >> sep;
    cout << decode(input, sep);
    break;
  }

  case 5:{
    string s1,s2;
    cin >> s1;
    cin >> s2;
    cout << setprecision(2);
    cout << reduction(s1,s2);
    break;
  }
    
  } // of switch
}  // of main
  
