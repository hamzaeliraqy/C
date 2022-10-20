/***************************************************************************************************************************************************************
Filename:				Assignement2-NC.cpp
Version: 				1.0
Author:					Hamza Eliraqy
Student No:  			040976448
Course Name/Number:		CST8233 (Numerical Computing)
Professor:				Dr.Hala Own
****************************************************************************************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <math.h>

//declaring functions and variables to be used
using namespace std;
double meDian(double data[], int nData);
double moDe(double data[], int nData);
double ranGe(double data[], int nData);
void regRession(double xdata[], double ydata[], int nData, int op);
fstream file;
int choice = 0;
int choice2 = 0;
string fileName;
int nData;
string rData[20][20];
double xData[20];
double yData[20];
ofstream fileName2;

//Start of the program
int main()
{
	//local variables for operation
	double mean = 0.0;
	double sum = 0.0;
	double median = 0.0;
	double standardDev = 0.0;
	double inSD = 0.0;
	double mode = 0.0;
	double range = 0.0;
	//Menu
	do {
		cout << "Menu" << endl;
		cout << "1. Exponential Fit" << endl;
		cout << "2. Quit" << endl;
		cin >> choice;
		if (choice == 1) {
			cout << "Please enter the name of the file to open : ";
			cin >> fileName;
			if (!fileName.empty()) {
				file.open(fileName, ios::in);	//open the file based on user input
				
				if (file.is_open()) {		//check if the file exists
					string data;
					int i = 0;
					nData = -1;
					while (getline(file, data)) {
						istringstream iss(data);
						int a, b;
						if (nData >= 0) {
							if (!(iss >> a >> b)) {		//Specify how to get data
								break;
							}
							xData[i] = (double)a;
							yData[i] = (double)b;
							i++;
						}
						cout << data << endl;
						
						
						nData++;
					}
					cout << "There are " << nData << " records" << endl;
					file.close();
				}
			}
			cout << "Data summary (Prognostic index) : " << endl;
			for (int i = 0; i < nData; i++) {		//Calculate the sum
				sum += yData[i];
			}
			mean = sum / nData;				//Calculate the mean
			for (int j = 0; j < nData; j++) {	
				inSD += (float)pow(yData[j] - mean, 2);
			}
			standardDev = sqrt(inSD / (nData - 1));		//Calculate StanDev
			median = meDian(yData, nData);
			mode = moDe(yData, nData);
			range = ranGe(yData, nData);
			/*Prints out the mean median range mode and standardDev*/
			cout << " " << endl;
			cout << "Mean = " << mean << endl;
			cout << "Median = " << median << endl;
			cout << "Range = " << range << endl;
			cout << "Mode = " << mode << endl;
			cout << "Standard Deviation = " << standardDev << endl;
			cout << " " << endl;
			/*Calls function regRession() which calculates the regression with param 1*/
			regRession(xData, yData, nData, 1);		
			do {
				cout << " " << endl;
				cout << "MENU" << endl;
				cout << "1. Extrapolation" << endl;
				cout << "2. Main Menu" << endl;
				cin >> choice2;
				if (choice2 == 1) {
					/*Calls function regRession() which gets the final regression value based on days with param 2*/
					regRession(xData, yData, nData, 2);
				}
			} while (choice2 != 2);
		}
		
	} while (choice != 2);
	return 0;
}
/*function to calculate the median takes xData and nData as param*/
double meDian(double data[], int nData) {

	sort(data, data + nData);

	if (nData % 2 == 0) {
		return (data[nData / 2 - 1] + data[nData / 2]) / 2;
	}

	return data[nData / 2];
}
/*Function to calculate the mode takes xData and nData as param*/
double moDe(double data[], int nData) {
	//local variables for operations
	double number = data[0];
	double mode = number;
	int count = 1;
	int countMode = 1;

	for (int i = 1; i < nData; i++) {
		if (data[i] == number) {
			++count;
		}
		else {
			if (count > countMode) {
				countMode = count;
				mode = number;
			}
			count = 1;
			number = data[i];
		}
	}
	return mode;
}
/*Function to calculate the range takes xData and nData as param*/
double ranGe(double data[], int nData) {
	sort(data, data + nData);
	double range = data[nData - 1] - data[0];
	return range;
}

/*Function to get the regression takes xData yData nData and op as param*/
void regRession(double xdata[], double ydata[], int nData, int op) {
	//local variables for operations
	double x = 0;
	double x2 = 0;
	double px2 = 0;
	double y = 0;
	double y2 = 0;
	double xy = 0;
	double xN = 0;
	double yN = 0;
	double a0 = 0, a1= 0, a2 = 0, a3 = 0;
	double yF = 0, yF2 = 0;
	double ssr = 0, sse = 0;
	int days = 0;
	


	int j = 0;
	for (int i = 14; i >= 0; i--) {

		x = x + xdata[j];
		y = y + ydata[i];
		x2 = x2 + xdata[j] * xdata[j];
		y2 = y2 + log(ydata[i]);
		xy += xdata[j] * log(ydata[i]);

		
		j++;
	}

	px2 = x * x;

	a3 = ((nData * xy) - (x * y2)) / (nData * x2 - (px2));
	a0 = (y2 - a3 * x) / nData;

	
	for (int j = 0; j <= nData; j++) {
		yF = (a3 * xdata[j] + a0);
		yF2 = yF * yF;
		ssr += yF2;
	}
	sse = sqrt(ssr / (nData - 2));
	/*if called with op = 1 prints Linear Regression Fit*/
	if (op == 1) {
		cout << "Linear Regression Fit : Prognostic_index = " << exp(a0) << "*exp " << a3 << "*days" << endl;
		cout << "Stander Error = " << sse << endl;
	}
	/*if called with op = 2 asks user for number of days and prints the prognostic_index*/
	if (op == 2) {
		cout << "Please enter the days to extrapolate to : ";
		cin >> days;
		cout << "days = " << days << endl;
		double pro =  pow(2.718, a0) * pow(2.718, days* a3);
		cout << "Prognostic_index = " << pro << endl;
	}
}