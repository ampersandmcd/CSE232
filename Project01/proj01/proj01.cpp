/*
* Andrew McDonald
* CSE 232.006 FS 18
* Project 01 - Model of Global Warming
*/

#include <iostream>
#include <iomanip>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;
using std::fixed;
using std::setprecision;
using std::round;

int main()
{
	/* Variables:
	*  background_slope: slope given from linear model in background section of project description
	*  intercept: intercept given from linear model in background section of project description
	*  input_year: year input by the user used to calculate output 1 (the predicted temperature in that year)
	*  input_slope: custom input slope used to predict what year a temperature 7 degrees greater than the calculated temperature would occur on output 3
	*  temperature: temperature calculated with the background_slope and intercept for the input_year
	*  output_year1: year predicted using background_slope and intercept in which the temperature will be 7 degrees greater than the calculated temperature variable
	*  output_year2: year predicted using input_slope and intercept in which the temperature will be 7 degrees greater than the calculated temperature variable
	*/

	const double background_slope = 0.01173;
	const double intercept = 34.3491;
	long input_year;
	double input_slope;
	
	cout << fixed << setprecision(2);
	cin >> input_year >> input_slope;

	double temperature = background_slope * input_year + intercept;
	long output_year1 = round((temperature + 7 - intercept) / background_slope);
	long output_year2 = round((temperature + 7 - intercept) / input_slope);

	cout << temperature << endl;
	cout << output_year1 << endl;
	cout << output_year2 << endl;

	return 0;
}