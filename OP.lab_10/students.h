#ifndef STUDENTS_H
#define STUDENTS_H

#define NAME 30
#define SURNAME 30
#define MARKS_LEN 5
#define YEAR_MAX 2020
#define SCORE_MAX 5
#define LIMIT  4.5

typedef struct{
	unsigned int nDay;
	unsigned int nMonth;
	unsigned int nYear;
} SBirthday;

typedef struct{
	char chName[NAME];
	char chSurname[SURNAME];
	SBirthday Birthday;
	int nScores[MARKS_LEN];
} SStudent;

typedef struct SStudentList{
	SStudent student;
	struct SStudentList* pNext;
} SList;

//зчитує дані про студентів з файлу
unsigned ReadStudentData(FILE* pFile, SList** ppRoot);

//виведення на екран списку студентів
void PrintList(SList* pRoot,FILE *pOut);

//виведення найкращих студентів, які мають бал вище середнього
void PrintListOfBest(SList* pRoot,FILE *pOut);

//порівнює вік двох студентів, якщо умови не виконуються, повертає -1
int Compare(SBirthday student1, SBirthday student2);

//повертає вказівник на корінь списку
SList* swap(SList* pLst1, SList* pLst2, SList* pRoot);

//додає новий елемент у відсортований список
void AddStudent(SList** ppRoot, SStudent student);

//видаляє список елементів
void RemoveStudents(SList** ppRoot);

//очищує список
void FreeList(SList** ppRoot);

//повертає дані студента
SStudent GetStudent();

//повертає значення середньої оцінки в групі
double GetAverageGroupScore(SList* pRoot);

//повертає середній бал
double GetAverage(int nScores[], unsigned nSize);

//сортує дані студентів
void SortByDate(SList** ppRoot);

#endif