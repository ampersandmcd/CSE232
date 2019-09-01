/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 03 - Rock, Paper, Scissors
 * 9.21.18
 */

#include<iostream>
using std::cout; using std::cin; using std::endl; using std::boolalpha;
// add any other includes you need. Up to you

/*
  for this we will use:
  1 - rock
  2 - paper
  3 - scissors
*/

// PUT YOUR FUNCTIONS BELOW THIS LINE

int beat_play(int play) { //helper function I decided to add
	return (play % 3) + 1;
}

int strategy1(long player, long previous_result, long previous_play, long opponent_previous_play) {
	return (previous_play % 3) + 1;
}

int strategy2(long player, long previous_result, long previous_play, long opponent_previous_play) {
	if (player == 1) {
		if (previous_result == 0 || previous_result == 1) { //player 1 won or tied
			return previous_play;
		}
		else { //player 1 lost
			return opponent_previous_play;
		}
	}
	else {
		if (previous_result == 0 || previous_result == 2) { //player 2 won or tied
			return previous_play;
		}
		else { //player 2 lost
			return opponent_previous_play;
		}
	}
}

int strategy3(long player, long previous_result, long previous_play, long opponent_previous_play) {
	if (player == 1) {
		if (previous_result == 0 || previous_result == 1) { //player 1 won or tied
			return previous_play;
		}
		else { //player 1 lost
			return beat_play(opponent_previous_play);
		}
	}
	else {
		if (previous_result == 0 || previous_result == 2) { //player 2 won or tied
			return previous_play;
		}
		else { //player 2 lost
			return beat_play(opponent_previous_play);
		}
	}
}

int score_round(int p1, int p2) {
	if (((p1 % 3) + 1) == p2) { //p2 wins
		return 2;
	}
	else if (((p2 % 3) + 1) == p1) { //p1 wins
		return 1;
	}
	else { //tie
		return 0;
	}
}

// MAIN IS BELOW, DON'T CHANGE

int main() {
  cout << boolalpha;   // print true or false for bools
  int test;
  cin >> test;
  
  switch (test) {
  case 1 : {   // strategy 1
    long player, previous_result, previous_play, opponent_play;
    cin >> player >> previous_result >> previous_play >> opponent_play;
    cout << strategy1(player, previous_result, previous_play, opponent_play) << endl;
    break;
  } // of case 1

  case 2:{    // strategy 2
    long player, previous_result, previous_play, opponent_play;
    cin >> player >> previous_result >> previous_play >> opponent_play;
    cout << strategy2(player, previous_result, previous_play, opponent_play) << endl;
    break;
  } // of case 2

  case 3: {    // strategy 3
    long player, previous_result, previous_play, opponent_play;
    cin >> player >> previous_result >> previous_play >> opponent_play;
    cout << strategy3(player, previous_result, previous_play, opponent_play) << endl;
    break;
  } // of case 3

  case 4: {       // score result
    long p1, p2;
    cin >> p1 >> p2;
    cout << score_round(p1, p2) << endl;
    break;
  } // of case 4
    
  } // of switch
} // of main


	
