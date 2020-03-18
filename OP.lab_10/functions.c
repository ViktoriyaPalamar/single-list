#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "students.h"

unsigned ReadStudentData(FILE* pFile, SList** ppRoot)
{
	if (pFile == NULL) {
		return 0;
	}

	*ppRoot = NULL;
	SStudent student;
	int i = 0, j = 0;

	while (!feof(pFile)) {
		if (fscanf(pFile, "%s %s %u %u %u",
			&student.chSurname,
			&student.chName,
			&student.Birthday.nDay,
			&student.Birthday.nMonth,
			&student.Birthday.nYear) != 5) {
			return 0;
		}

		for (j = 0; j < MARKS_LEN; ++j) {
			if (!fscanf(pFile, "%d", &student.nScores[j])) {
				return 0;
			}
		}

		AddStudent(ppRoot, student);
		++i;
	}

	return i;
}
//--------------------------------------------------------------------------------
void PrintList(SList* pRoot,FILE *pOut)
{
	while (pRoot != NULL){
		printf("%-12s %-12s %02u.%02u.%u\t", pRoot->student.chSurname, pRoot->student.chName,
			pRoot->student.Birthday.nDay, pRoot->student.Birthday.nMonth, pRoot->student.Birthday.nYear);
		fprintf(pOut,"%-12s %-12s %02u.%02u.%u\t", pRoot->student.chSurname, pRoot->student.chName,
			pRoot->student.Birthday.nDay, pRoot->student.Birthday.nMonth, pRoot->student.Birthday.nYear);
		
		for (int i = 0; i < MARKS_LEN; ++i) {
			printf("%d ", pRoot->student.nScores[i]);
			fprintf(pOut,"%d ", pRoot->student.nScores[i]);
			
		}
		printf("\n");
		fprintf(pOut,"\n");
		pRoot = pRoot->pNext;
	}
}
//--------------------------------------------------------------------------------
void PrintListOfBest(SList* pRoot,FILE *pOut)
{
	double dAverageScore = GetAverageGroupScore(pRoot);
	while (pRoot != NULL) {
		if (GetAverage(pRoot->student.nScores, MARKS_LEN) > dAverageScore) {
			printf("%-12s %-12s %02u.%02u.%u\t", pRoot->student.chSurname, pRoot->student.chName,
				pRoot->student.Birthday.nDay, pRoot->student.Birthday.nMonth, pRoot->student.Birthday.nYear);
			fprintf(pOut,"%-12s %-12s %02u.%02u.%u\t", pRoot->student.chSurname, pRoot->student.chName,
				pRoot->student.Birthday.nDay, pRoot->student.Birthday.nMonth, pRoot->student.Birthday.nYear);

			for (int i = 0; i < MARKS_LEN; ++i) {
				printf("%d ", pRoot->student.nScores[i]);
				fprintf(pOut,"%d ", pRoot->student.nScores[i]);
			}
			printf("\n");
		    fprintf(pOut,"\n");
		}
		pRoot = pRoot->pNext;
	}
}
//--------------------------------------------------------------------------------
int Compare(SBirthday student1, SBirthday student2)
{
	if ((student1.nYear < student2.nYear)
		|| (student1.nYear == student2.nYear && student1.nMonth < student2.nMonth)
		|| (student1.nYear == student2.nYear && student1.nMonth == student2.nMonth && student1.nDay < student2.nDay)) {
		return 1;
	}
	if (student1.nYear == student2.nYear && student1.nMonth == student2.nMonth && student1.nDay == student2.nDay) {
		return 0;
	}
	return -1;
}
//--------------------------------------------------------------------------------
SList* swap(SList *pLst1, SList *pLst2, SList *pRoot)
{
	SList *prev1, *prev2, *next1, *next2;
	prev1 = pRoot;
	prev2 = pRoot;
	next1 = pLst1->pNext;
	next2 = pLst2->pNext;
	if (prev1 == pLst1) {
		prev1 = NULL;
	} else {
		while (prev1->pNext != pLst1) {
			prev1 = prev1->pNext;
		}
	}
		if (prev2 == pLst2) {
			prev2 = NULL;
		} else {
			while (prev2->pNext != pLst2) {
				prev2 = prev2->pNext;
			}
			next1 = pLst1->pNext;
			next2 = pLst2->pNext;
		}
		if (pLst2 == next1) {
			pLst2->pNext = pLst1;
			pLst1->pNext = next2;
			if (pLst1 != pRoot) {
				prev1->pNext = pLst2;
			}
		} else {
			if (pLst1 == next2) {
				pLst1->pNext = pLst2;
				pLst2->pNext = next1;
				if (pLst2 != pRoot) {
					prev2->pNext = pLst2;
				}
			} else {
				if (pLst1 != pRoot) {
					prev1->pNext = pLst2;
				}
				pLst2->pNext = next1;
			} if (pLst2 != pRoot) {
				prev2->pNext = pLst1;
			}
			pLst1->pNext = next2;
		}
		
		if (pLst1 == pRoot) {
			return (pLst2);
		}
		if (pLst2 == pRoot) {
			return(pLst1);
		}
	return(pRoot);
}
//--------------------------------------------------------------------------------
void AddStudent(SList** ppRoot, SStudent student)
{
	SList* pElement = (SList*)malloc(sizeof(SList));
	if (pElement == NULL) {
		return;
	}

	pElement->student = student;
	pElement->pNext = (*ppRoot == NULL ? NULL : *ppRoot);
	*ppRoot = pElement;
}
//--------------------------------------------------------------------------------
void RemoveStudents(SList** ppRoot)
{
	if (ppRoot == NULL) {
		return;
	}

	SList *tmp, *tmp2;
	while (*ppRoot != NULL && GetAverage((*ppRoot)->student.nScores, MARKS_LEN) < LIMIT) {
		tmp = (*ppRoot)->pNext;
		free(*ppRoot);
		*ppRoot = tmp;
	}

	tmp = *ppRoot;
	if (*ppRoot == NULL) {
		return;
	}
	while (tmp->pNext != NULL) {
		if (GetAverage(tmp->pNext->student.nScores, MARKS_LEN) < LIMIT) {
			tmp2 = tmp->pNext->pNext;
			free(tmp->pNext);
			tmp->pNext = tmp2;
		} else {
			tmp = tmp->pNext;
		}
	}
}
//--------------------------------------------------------------------------------
void FreeList(SList** ppRoot)
{
	SList* pElement = *ppRoot;
	if (!pElement) {
		printf("List is empty\n");
	}
	while (pElement) {
		*ppRoot = pElement->pNext;
		free(pElement);
		pElement = *ppRoot;
	}
}
//--------------------------------------------------------------------------------
SStudent GetStudent()
{
	SStudent student;
	char* subjects[] = { "Math", "History", "Biology", "Physics", "English" };
	printf("Enter information about a new student:\n");
	printf("1.Last name: ");
	scanf("%s", &student.chName);
	printf("2.First name: ");
	scanf("%s", &student.chSurname);

	printf("3.Birth day: ");
	while (!scanf("%u", &student.Birthday.nDay) || student.Birthday.nDay < 1 || student.Birthday.nDay > 31) {
		printf("Incorrect value\n");
	}
	printf("4.Birth month: ");
	while (!scanf("%u", &student.Birthday.nMonth) || student.Birthday.nMonth < 1 || student.Birthday.nMonth > 12) {
		printf("Incorrect value\n");
	}
	printf("5.Birth year: ");
	while (!scanf("%u", &student.Birthday.nYear) || student.Birthday.nYear > YEAR_MAX) {
		printf("Incorrect value\n");
	}

	for (int i = 0; i < MARKS_LEN; ++i) {
		printf("- %s score: ", subjects[i]);
		while (!scanf("%d", &student.nScores[i]) || student.nScores[i] < 0 || student.nScores[i] > SCORE_MAX) {
			printf("Incorrect value\n");
		}
	}

	return student;
}
//--------------------------------------------------------------------------------
double GetAverageGroupScore(SList* pRoot)
{
	int nAmount = 0;
	long lSum = 0;

	while (pRoot != NULL) {
		for (unsigned i = 0; i < MARKS_LEN; ++i) {
			lSum += pRoot->student.nScores[i];
		}
		pRoot = pRoot->pNext;
		++nAmount;
	}

	return 1.0 * lSum / (nAmount * MARKS_LEN);
}
//--------------------------------------------------------------------------------
double GetAverage(int nScores[], unsigned nSize)
{
	int nSum = 0;
	for (unsigned i = 0; i < nSize; ++i) {
		nSum += nScores[i];
	}

	return 1.0 * nSum / nSize;
}
//--------------------------------------------------------------------------------
void SortByDate(SList** ppRoot)
{
	SList* min;
	SList* tmp;
	SList* pSelected = *ppRoot;
	while (pSelected != NULL) {
		min = pSelected;
		tmp = pSelected->pNext;
		while (tmp != NULL) {
			if (Compare(min->student.Birthday, tmp->student.Birthday) < 0) {
				min = tmp;
			}
			tmp = tmp->pNext;
		}
		if (min != pSelected) {
			*ppRoot = swap(pSelected, min, *ppRoot);
			pSelected = min->pNext;
		} else {
			pSelected = pSelected->pNext;
		}
	}
}
