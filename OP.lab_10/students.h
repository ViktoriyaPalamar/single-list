#ifndef STUDENTS_H
#define STUDENTS_H

#define NAME 30
#define SURNAME 30
#define MARKS_LEN 5
#define YEAR_MAX 2020
#define SCORE_MAX 5
#define LIMIT  4.5

typedef struct
{
	unsigned int day;
	unsigned int month;
	unsigned int year;
} Birthday;

typedef struct
{
	char name[NAME];
	char surname[SURNAME];
	Birthday birthday;
	int scores[MARKS_LEN];
} Student;

typedef struct StudentList
{
	Student student;
	struct StudentList* next;
} List;

unsigned ReadStudentData(FILE* file, List** root);
void PrintList(List* root);
void PrintListOfBest(List* root);
int Compare(Birthday student1, Birthday student2);
List* swap(List* lst1, List* lst2, List* root);
void AddStudent(List** root, Student student);
void RemoveStudents(List** root);
void FreeList(List** root);
Student GetStudent();
double GetAverageGroupScore(List* root);
double GetAverage(int scores[], unsigned size);
void SortByDate(List** root);

#endif