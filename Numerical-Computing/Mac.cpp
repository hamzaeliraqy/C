


#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip> 

using namespace std;


//declaring functon to be used 
double mSeries(int terms, double t);


int main() {

	//local variables for operation
	int choice;
	int terms;
	double range;
	double mResult = 0.0;
	double mResultE = 0.0;
	double exactError = 0.0;
	double truncError = 0.0;
	double t;
	const char w = 15;
	const char sep = ' ';
	int a = 0;


	//Start of menu
	cout << "Evaluate the Maclaurin Series approximation to D(t) = exp(t) * cos(t)" << endl;
	do {
		choice = 0;
		terms = 0;
		range = 0.0;

		cout << "1: Evaluate the series" << endl;
		cout << "2: quit" << endl;
		cin >> choice;

		//after asking for user input switch based on it 
		switch (choice) {
		case 1:

			//user defined variables 
			cout << "Please enter the number of (non-zero) terms in the series (1, 2, 3, 4, 5 or 6):" << endl;
			cin >> terms;
			//Check if the number of terms is whithin the acceptable values
			if (terms > 6 || terms < 0) {
				cout << "Invalid term" << endl;
				cout << "Please enter the number of (non-zero) terms in the series (1, 2, 3, 4, 5 or 6):" << endl;
				cin >> terms;
			}

			cout << "Please enter the range of t to evaluate in 10 increments (0.0 < t < +4.0):" << endl;
			cin >> range;
			//Check if range is within acceptable values
			if (range > 4.0 || range < 0.0) {
				cout << "Invalid range" << endl;
				cout << "Please enter the range of t to evaluate in 10 increments (0.0 < t < +4.0):" << endl;
				cin >> range;
			}
			
			cout << " " << endl;

			cout << "MACLAURIN SERIES" << endl;
			cout << left << setw(w) << "t     " << "D(t) Series   " << "  D(t) Exact    " << "%RExactE   " << "   %RSerE" << endl;

			cout << " " << endl;
			
			//for loop to get t 
			//Calls function mSeries to calculate Maclaurin series
			for (t = 0.0; t <= range; t += (range / 10.0)) {
				mResult = mSeries(terms, t);
				mResultE = pow(2.716, t) * cos(t);
				exactError = ((mResultE - mResult) / mResultE) * 100;

				if (a >= terms + 1) {
					truncError = (exactError / mResult) * 100;
				}
				a++;
				cout << left << setw(w) << setfill(sep) << t << setw(w) << setfill(sep) << mResult << setw(w) << setfill(sep) << mResultE << setw(w) << setfill(sep) << exactError << setw(w) << setfill(sep) << truncError << scientific << endl;
			}
			cout << " " << endl;
		case 2:

			cout << "Exiting.." << endl;
		}

	} while (choice != 2);
	return 0;
}

//Function to calculate Maclaurin series takes as parameteres int and double and returns the result
	double mSeries(int terms, double t) {

		double result = 0.0;

		if (terms == 1) {
			result = 1.0;
		}
		if (terms == 2) {
			result = 1.0 + t;
		}
		if (terms == 3) {
			result = 1.0 + t + (-1) * ((t * t * t) / 3.0);
		}
		if (terms == 4) {
			result = 1.0 + t + (-1) * ((t * t * t) / 3.0) + (-1) * ((t * t * t * t) / 60.0);
		}
		if (terms == 5) {
			result = 1.0 + t + (-1) * ((t * t * t) / 3.0) + (-1) * ((t * t * t * t) / 6.0) + (-1) * ((t * t * t * t * t) / 30.0);
		}
		if (terms == 6) {
			result = 1.0 + t + (-1) * ((t * t * t) / 3.0) + (-1) * ((t * t * t * t) / 6.0) + (-1) * ((t * t * t * t * t) / 30.0) + ((t * t * t * t * t * t * t) / 630.0);
		}
		if (terms == 7) {
			result = 1.0 + t + (-1) * ((t * t * t) / 3.0) + (-1) * ((t * t * t * t) / 6.0) + (-1) * ((t * t * t * t * t) / 30.0) + ((t * t * t * t * t * t * t) / 630.0) + ((t * t * t * t * t * t * t * t) / 2520.0);
		}

		return result;


	}

