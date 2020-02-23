#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "students.h"

unsigned ReadStudentData(FILE* file, List** root)
{
	if (file == NULL) return 0;

	*root = NULL;
	Student student;
	int i = 0, j = 0;

	while (!feof(file))
	{
		if (fscanf(file, "%s %s %u %u %u",
			&student.surname,
			&student.name,
			&student.birthday.day,
			&student.birthday.month,
			&student.birthday.year) != 5) return 0;

		for (j = 0; j < MARKS_LEN; ++j)
			if (!fscanf(file, "%d", &student.scores[j])) return 0;

		AddStudent(root, student);
		++i;
	}

	return i;
}

void PrintList(List* root)
{
	while (root != NULL)
	{
		printf("%-12s %-12s %02u.%02u.%u\t", root->student.surname, root->student.name,
			root->student.birthday.day, root->student.birthday.month, root->student.birthday.year);
		for (int i = 0; i < MARKS_LEN; ++i) printf("%d ", root->student.scores[i]);
		printf("\n");
		root = root->next;
	}
}

void PrintListOfBest(List* root)
{
	double averageScore = GetAverageGroupScore(root);
	while (root != NULL)
	{
		if (GetAverage(root->student.scores, MARKS_LEN) > averageScore)
		{
			printf("%-12s %-12s %02u.%02u.%u\t", root->student.surname, root->student.name,
				root->student.birthday.day, root->student.birthday.month, root->student.birthday.year);
			for (int i = 0; i < MARKS_LEN; ++i) printf("%d ", root->student.scores[i]);
			printf("\n");
		}
		root = root->next;
	}
}

int Compare(Birthday student1, Birthday student2)
{
	if ((student1.year < student2.year)
		|| (student1.year == student2.year && student1.month < student2.month)
		|| (student1.year == student2.year && student1.month == student2.month && student1.day < student2.day)) return 1;
	if (student1.year == student2.year && student1.month == student2.month && student1.day == student2.day) return 0;
	return -1;
}

List* swap(List* lst1, List* lst2, List* root)
{
	List* prev1, * prev2, * next1, * next2;
	prev1 = root;
	prev2 = root;
	if (prev1 == lst1)
		prev1 = NULL;
	else
		while (prev1->next != lst1)
			prev1 = prev1->next;
	if (prev2 == lst2)
		prev2 = NULL;
	else
		while (prev2->next != lst2)
			prev2 = prev2->next;
	next1 = lst1->next;
	next2 = lst2->next;
	if (lst2 == next1)
	{
		lst2->next = lst1;
		lst1->next = next2;
		if (lst1 != root)
			prev1->next = lst2;
	}
	else
		if (lst1 == next2)
		{
			lst1->next = lst2;
			lst2->next = next1;
			if (lst2 != root)
				prev2->next = lst2;
		}
		else
		{
			if (lst1 != root)
				prev1->next = lst2;
			lst2->next = next1;
			if (lst2 != root)
				prev2->next = lst1;
			lst1->next = next2;
		}
	if (lst1 == root)
		return(lst2);
	if (lst2 == root)
		return(lst1);
	return(root);
}

void AddStudent(List** root, Student student)
{
	List* element = (List*)malloc(sizeof(List));
	if (element == NULL) return;

	element->student = student;
	element->next = (*root == NULL ? NULL : *root);
	*root = element;
}

void RemoveStudents(List** root)
{
	if (root == NULL) return;

	List* tmp, * tmp2;
	while (*root != NULL && GetAverage((*root)->student.scores, MARKS_LEN) < LIMIT)
	{
		tmp = (*root)->next;
		free(*root);
		*root = tmp;
	}

	tmp = *root;
	if (*root == NULL)
		return;
	while (tmp->next != NULL)
	{
		if (GetAverage(tmp->next->student.scores, MARKS_LEN) < LIMIT)
		{
			tmp2 = tmp->next->next;
			free(tmp->next);
			tmp->next = tmp2;
		}
		else tmp = tmp->next;
	}
}

void FreeList(List** root)
{
	List* element = *root;
	if (!element)
		printf("List is empty\n");
	while (element)
	{
		*root = element->next;
		free(element);
		element = *root;
	}
}

Student GetStudent()
{
	Student student;
	char* subjects[] = { "Math", "History", "Biology", "Physics", "English" };
	printf("Enter information about a new student:\n");
	printf("1.Last name: ");
	scanf("%s", &student.name);
	printf("2.First name: ");
	scanf("%s", &student.surname);

	printf("3.Birth day: ");
	while (!scanf("%u", &student.birthday.day) || student.birthday.day < 1 || student.birthday.day > 31)
		printf("Incorrect value\n");
	printf("4.Birth month: ");
	while (!scanf("%u", &student.birthday.month) || student.birthday.month < 1 || student.birthday.month > 12)
		printf("Incorrect value\n");
	printf("5.Birth year: ");
	while (!scanf("%u", &student.birthday.year) || student.birthday.year > YEAR_MAX)
		printf("Incorrect value\n");

	for (int i = 0; i < MARKS_LEN; ++i)
	{
		printf("- %s score: ", subjects[i]);
		while (!scanf("%d", &student.scores[i]) || student.scores[i] < 0 || student.scores[i] > SCORE_MAX)
			printf("Incorrect value\n");
	}

	return student;
}

double GetAverageGroupScore(List* root)
{
	unsigned amount = 0;
	long sum = 0;

	while (root != NULL)
	{
		for (unsigned i = 0; i < MARKS_LEN; ++i)
			sum += root->student.scores[i];
		root = root->next;
		++amount;
	}

	return 1.0 * sum / (amount * MARKS_LEN);
}

double GetAverage(int scores[], unsigned size)
{
	int sum = 0;
	for (unsigned i = 0; i < size; ++i) sum += scores[i];

	return 1.0 * sum / size;
}

void SortByDate(List** root)
{
	List* min;
	List* tmp;
	List* selected = *root;
	while (selected != NULL)
	{
		min = selected;
		tmp = selected->next;
		while (tmp != NULL)
		{
			if (Compare(min->student.birthday, tmp->student.birthday) < 0) min = tmp;
			tmp = tmp->next;
		}
		if (min != selected)
		{
			*root = swap(selected, min, *root);
			selected = min->next;
		}
		else selected = selected->next;
	}
}
