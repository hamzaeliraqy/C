

#include <stdio.h>

int mainBuffer(int, char**);
int mainScanner(int, char**);
int mainParser(int, char**);

int main(int argc, char** argv) {
	mainParser(argc, argv);
	return 0;
}