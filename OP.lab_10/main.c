#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "students.h"

#define FILE_PATH "student.txt"

int main()
{
	FILE* fp = fopen(FILE_PATH, "r");
	if (!fp)
	{
		printf("Failed to open file");
		return EXIT_FAILURE;
	}

	List* root;
	unsigned int amountofstudent = ReadStudentData(fp, &root);
	fclose(fp);

	SortByDate(&root);
	printf("Full list:\n");
	PrintList(root);
	printf("\n");

	AddStudent(&root, GetStudent());
	SortByDate(&root);
	printf("List of the most successful students:\n");
	PrintListOfBest(root);

	RemoveStudents(&root);
	printf("\nList of the students with score above %g:\n", LIMIT);
	PrintList(root);

	FreeList(&root);
	return 0;
}