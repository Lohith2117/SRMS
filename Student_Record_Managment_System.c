/* -------------------------------------Student Record Managment System [SRMS]------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100        // Maximum number of students
#define FILE_NAME "students.dat"

// Structure for student
typedef struct {
    int roll;
    char name[50];
    float marks[5];
    float total;
    float average;
    char grade;
} Student;

// Function prototypes
void addStudent();
void displayAll();
void saveToFile(Student s);
void loadFromFile();
void searchStudent();
void sortStudentsMenu();
void sortByTotal_Bubble();
void sortByTotal_Insertion();
void sortByTotal_Selection();
void calculateResult(Student *s);
void updateRecord();
void deleteRecord();
void binarySearch();
void linearSearch();

// Array to store loaded data
Student students[MAX];
int count = 0;

// Function pointer for sorting algorithms
typedef void (*SortFunction)();


/* --------------------- Calculate Student Result ---------------------- */
void calculateResult(Student *s) {
    s->total = 0;
    for(int i = 0; i < 5; i++)
        s->total += s->marks[i];

    s->average = s->total / 5.0;

    // Grade assignment
    if (s->average >= 90) s->grade = 'A';
    else if (s->average >= 75) s->grade = 'B';
    else if (s->average >= 60) s->grade = 'C';
    else if (s->average >= 40) s->grade = 'D';
    else s->grade = 'F';
}
/* --------------------- Add Student ---------------------- */
void addStudent() {
    Student s;
    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);

    printf("Enter Student Name: ");
    scanf(" %[^\n]", s.name);

    printf("Enter marks of 5 subjects (0–100 only):\n");
    for (int i = 0; i < 5; i++) {
        do {
            printf("Subject %d: ", i + 1);
            scanf("%f", &s.marks[i]);

            if (s.marks[i] < 0 || s.marks[i] > 100) {
                printf("❌ Invalid input! Marks must be between 0 and 100.\n");
            }
        } while (s.marks[i] < 0 || s.marks[i] > 100);
    }

    calculateResult(&s);
    saveToFile(s);

    printf("\n✅ Record Added Successfully!\n");
}


/* --------------------- Save to Binary File ---------------------- */
void saveToFile(Student s) {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
}


/* --------------------- Load All Records ---------------------- */
void loadFromFile() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("\nNo records found.\n");
        return;
    }

    count = 0;
    while (fread(&students[count], sizeof(Student), 1, fp))
        count++;

    fclose(fp);
}


/* --------------------- Display All Students ---------------------- */
void displayAll() {
    loadFromFile();
    printf("\n===== All Student Records =====\n");

    for(int i = 0; i < count; i++) {
        printf("\nRoll: %d", students[i].roll);
        printf("\nName: %s", students[i].name);
        printf("\nTotal: %.2f", students[i].total);
        printf("\nAverage: %.2f", students[i].average);
        printf("\nGrade: %c\n", students[i].grade);
    }
}


/* --------------------- Linear Search ---------------------- */
void linearSearch() {
    loadFromFile();
    int roll;
    printf("\nEnter Roll Number to Search: ");
    scanf("%d", &roll);

    for(int i = 0; i < count; i++) {
        if(students[i].roll == roll) {
            printf("\nRecord Found!");
            printf("\nName: %s", students[i].name);
            printf("\nTotal: %.2f\n", students[i].total);
            return;
        }
    }
    printf("\nRecord Not Found.\n");
}


/* --------------------- Binary Search (After Sorting) ---------------------- */
void binarySearch() {
    loadFromFile();
    sortByTotal_Bubble();  // Ensure sorting before binary search

    int roll;
    printf("\nEnter Roll Number for Binary Search: ");
    scanf("%d", &roll);

    int low = 0, high = count - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (students[mid].roll == roll) {
            printf("\nRecord Found!");
            printf("\nName: %s", students[mid].name);
            printf("\nTotal: %.2f\n", students[mid].total);
            return;
        }
        if (roll > students[mid].roll)
            low = mid + 1;
        else
            high = mid - 1;
    }
    printf("\nRecord Not Found.\n");
}


/* --------------------- Sorting Algorithms ---------------------- */

// Bubble Sort
void sortByTotal_Bubble() {
    loadFromFile();
    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(students[j].total < students[j+1].total) {
                Student temp = students[j];
                students[j] = students[j+1];
                students[j+1] = temp;
            }
        }
    }
    printf("\nRecords sorted using Bubble Sort.\n");
}

// Insertion Sort
void sortByTotal_Insertion() {
    loadFromFile();
    for(int i = 1; i < count; i++) {
        Student key = students[i];
        int j = i - 1;

        while(j >= 0 && students[j].total < key.total) {
            students[j+1] = students[j];
            j--;
        }
        students[j+1] = key;
    }
    printf("\nRecords sorted using Insertion Sort.\n");
}

// Selection Sort
void sortByTotal_Selection() {
    loadFromFile();
    for(int i = 0; i < count - 1; i++) {
        int max_idx = i;
        for(int j = i + 1; j < count; j++) {
            if(students[j].total > students[max_idx].total)
                max_idx = j;
        }
        Student temp = students[i];
        students[i] = students[max_idx];
        students[max_idx] = temp;
    }
    printf("\nRecords sorted using Selection Sort.\n");
}


/* --------------------- Sorting Menu Using Function Pointer ---------------------- */
void sortStudentsMenu() {
    SortFunction sorter;

    int choice;
    printf("\nChoose Sorting Method:\n");
    printf("1. Bubble Sort\n2. Insertion Sort\n3. Selection Sort\nEnter choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: sorter = sortByTotal_Bubble; break;
        case 2: sorter = sortByTotal_Insertion; break;
        case 3: sorter = sortByTotal_Selection; break;
        default: printf("\nInvalid Choice\n"); return;
    }

    sorter();  // Function pointer call
}


/* --------------------- Update a Record ---------------------- */
void updateRecord() {
    loadFromFile();
    int roll;
    printf("\nEnter roll to modify: ");
    scanf("%d", &roll);

    int found = 0;
    for(int i = 0; i < count; i++) {
        if(students[i].roll == roll) {
            found = 1;

            printf("Enter new name: ");
            scanf(" %[^\n]", students[i].name);

            printf("Enter new marks:\n");
            for(int j = 0; j < 5; j++) {
                printf("Subject %d: ", j+1);
                scanf("%f", &students[i].marks[j]);
            }

            calculateResult(&students[i]);
            break;
        }
    }

    if (!found) {
        printf("\nRecord not found.\n");
        return;
    }

    FILE *fp = fopen(FILE_NAME, "wb");
    fwrite(students, sizeof(Student), count, fp);
    fclose(fp);

    printf("\nRecord updated successfully!\n");
}


/* --------------------- Delete Record ---------------------- */
void deleteRecord() {
    loadFromFile();
    int roll;
    printf("\nEnter roll to delete: ");
    scanf("%d", &roll);

    int idx = -1;
    for(int i = 0; i < count; i++) {
        if(students[i].roll == roll)
            idx = i;
    }

    if (idx == -1) {
        printf("\nRecord not found.\n");
        return;
    }

    // Shift elements left
    for(int i = idx; i < count - 1; i++)
        students[i] = students[i+1];

    count--;

    FILE *fp = fopen(FILE_NAME, "wb");
    fwrite(students, sizeof(Student), count, fp);
    fclose(fp);

    printf("\nRecord deleted successfully!\n");
}


/* --------------------- Main Menu ---------------------- */
void menu() {
    int choice;

    while(1) {
        printf("\n============ STUDENT MANAGEMENT SYSTEM ============\n");
        printf("1. Add Student\n");
        printf("2. Display All Records\n");
        printf("3. Linear Search\n");
        printf("4. Binary Search\n");
        printf("5. Sort Students\n");
        printf("6. Update Record\n");
        printf("7. Delete Record\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: linearSearch(); break;
            case 4: binarySearch(); break;
            case 5: sortStudentsMenu(); break;
            case 6: updateRecord(); break;
            case 7: deleteRecord(); break;
            case 8: printf("\nExiting...\n"); exit(0);
            default: printf("\nInvalid Choice!\n");
        }
    }
}


/* --------------------- Main Function ---------------------- */
int main() {
    menu(); // Recursive Menu Call Allowed
    return 0;
}