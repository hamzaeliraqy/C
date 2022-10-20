

#include "Assign-NC.h"
#include <stdio.h>
#include <math.h>
#define _USE_MATH_DEFINES

using namespace std;

double lagrange(int n, double x, double* xk, double* fk);

int main() {
	
	int choice;
	
	do {
		cout << "MENU" << endl;
		cout << "1) Function A" << endl;
		cout << "2) Function B" << endl;
		cout << "3) Quit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			cout << "In fucntion A" << endl;
			
			double z[41];
			double w[41];

			double p;
			double xk;

			
			double ab;

			for (int i = 0; i < 41; i++) {
				double a = (i + 1.0 - 1.0) * 2.0;
				double b = -1 + (a / 40);
				z[i] = b;
			}

			for (int i = 0; i < 41; i++) {
				double a = 1 + pow(z[i], 2);
				w[i] = sqrt(a);

			}
			cout << "When n = 5" << endl;
			cout << "X" << "          " << "XK" << "                    " << "P" << "                        " << "TRUE VALUE" << "                          " << "ABSOLUTE ERROR" << endl;
			for (int k = 0; k < 41; k++) {
				xk = z[k];

				p = lagrange(5+1, xk, z, w);
				ab = p - w[k];
				cout.precision(8);
				cout << k << "            " << z[k] << "               " << p << "            " << w[k] << "                         " << ab << endl;
			}
			
			cout << endl;
			cout << endl;
			cout << endl;
			cout << "When n = 10" << endl;
			cout << "X" << "          " << "XK" << "                    " << "P" << "                        " << "TRUE VALUE" << "                          " << "ABSOLUTE ERROR" << endl;
			for (int k = 0; k < 41; k++) {
				xk = z[k];

				p = lagrange(10, xk, z, w);
				ab = p - w[k];
				//set w
				cout.precision(10);
				cout << k << "             " << xk << "             " << p << "            " << w[k] << "                           " << ab << endl;
				
			}
			cout << endl;
			cout << endl;
			cout << endl;
			cout << "When n = 20" << endl;
			cout << "X" << "          " << "XK" << "                    " << "P" << "                        " << "TRUE VALUE" << "                          " << "ABSOLUTE ERROR" << endl;
			for (int k = 0; k < 41; k++) {
				xk = z[k];

				p = lagrange(20, xk, z, w);
				ab = p - w[k];
				cout.precision(10);
				cout << k << "            " << xk << "               " << p << "            " << w[k] << "                         " << ab << endl;
			}   
			
			
			break;
		case 2:
			cout << "In Function B" << endl;

			double z2[41];
			double w2[41];

			double p2;
			double xk2;

			

			double ab2;
			
			for (int i = 0; i < 41; i++) {
				double a = (i + 1.0 - 1.0) * 2.0;
				double b = -1 + (a / 40);
				z2[i] = b;
			}

			for (int i = 0; i < 41; i++) {
				double b = 25 * pow(z2[i], 2);
				double a = 1 / (1 + b);
				w2[i] = a;
				
			}
			cout << "When n = 5" << endl;
			cout << "X" << "          " << "XK" << "                    " << "P" << "                        " << "TRUE VALUE" << "                          " << "ABSOLUTE ERROR" << endl;
			for (int k = 0; k < 41; k++) {
				xk2 = z2[k];

				p2 = lagrange(5+1, xk2, z2, w2);
				ab2 = p2 - w2[k];
				cout << k << "              " << xk2 << "                " << p2 << "                  " << w2[k] << "                               " << ab2 << endl;
			}
			
			cout << endl;
			cout << endl;
			cout << endl;
			cout << "When n = 10" << endl;
			cout << "X" << "          " << "XK" << "                    " << "P" << "                        " << "TRUE VALUE" << "                          " << "ABSOLUTE ERROR" << endl;
			for (int k = 0; k < 41; k++) {
				xk2 = z2[k];

				p2 = lagrange(10, xk2, z2, w2);
				ab2 = p2 - w2[k];
				cout << k << "             " << xk2 << "               " << p2 << "                  " << w2[k] << "                                 " << ab2 << endl;
			}
			cout << endl;
			cout << endl;
			cout << endl;
			cout << "When n = 20" << endl;
			cout << "X" << "          " << "XK" << "                    " << "P" << "                        " << "TRUE VALUE" << "                          " << "ABSOLUTE ERROR" << endl;
			for (int k = 0; k < 41; k++) {
				xk2 = z2[k];

				p2 = lagrange(20, xk2, z2, w2);
				ab2 = p2 - w2[k];
				cout << k << "            " << xk2 << "             " << p2 << "                    " << w2[k] << "                                 " << ab2 << endl;
			}
			
			break;
		case 3:
			cout << "Quitting" << endl;
			break;

		default:
			cout << "Invalid input!" << endl;

		} 




		
	}while (choice != 3);
	return 0;
}

double lagrange(int n, double x, double* xk, double* fk) {

	
	double p, lk;
	p = 0.0;
	for (int k = 0; k < n; k++) {
		lk = 1.0;
		for (int i = 0; i < n; i++) {
			if (i == k)
				continue;
			lk *= (x - xk[i]) / (xk[k] - xk[i]);
		}
		p += lk * fk[k];
	}
	return p;

}
