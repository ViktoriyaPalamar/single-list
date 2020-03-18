#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "students.h"

#define FILE_PATH "student.txt"
#define FILE_OUTPUT "output.txt"
int main()
{
	FILE* fp = fopen(FILE_PATH, "r");
	if (!fp) {
		printf("Failed to open file");
		return EXIT_FAILURE;
	}
	FILE* pOut=fopen(FILE_OUTPUT,"w");
	SList* pRoot;
	unsigned int nAmountofstudent = ReadStudentData(fp, &pRoot);
	fclose(fp);

	SortByDate(&pRoot);
	printf("Full list:\n");
	PrintList(pRoot,stdin);
	printf("\n");
	

	AddStudent(&pRoot, GetStudent());
	SortByDate(&pRoot);
	printf("List of the most successful students:\n");
	fprintf(pOut,"List of the most successful students:\n");
	fprintf(pOut, "\n");
	PrintListOfBest(pRoot,pOut);

	RemoveStudents(&pRoot);
	printf("\nList of the students with score above %g:\n", LIMIT);
	fprintf(pOut,"\nList of the students with score above %g:\n", LIMIT);
	PrintList(pRoot,pOut);

	FreeList(&pRoot);
	return 0;
}