#define _CRT_SECURE_NO_WARNINGS // deactivate Visual Studio security warnings
#include "parser.h"
#include "operator.h"

int main() {

	char arr[50];
	char arr2[50] = "myfile.txt";

	printf("Input file name: ");
	scanf("%s", arr);
	Scanner(arr);
	if (errorCount == 0) Begin(arr2);
	
	system("PAUSE");
	return 0;
}
