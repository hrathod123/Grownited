#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define SIZE 1000

struct Student {
    int rno;
    char name[50];
    int std;
    int marks;
    int active;
};

struct Student srecords[SIZE];

int index = 0;
int rnos = 1;

void printStudent(struct Student s);

/* ---------------- ADD STUDENT ---------------- */
void addStudent() {
    struct Student s;

    if (index == SIZE) {
        printf("\nStudent list is full.");
        return;
    }

    s.rno = rnos;
    flushall();

    printf("\nEnter Name: ");
    gets(s.name);

    printf("Enter Std: ");
    scanf("%d", &s.std);

    printf("Enter Marks: ");
    scanf("%d", &s.marks);

    s.active = 1;
    srecords[index] = s;

    index++;
    rnos++;

    printf("\nStudent record inserted successfully.");
}

/* ---------------- DISPLAY STUDENTS ---------------- */
void displayStudents() {
    int i, found = 0;

    if (index == 0) {
        printf("\nStudent list is empty.");
        return;
    }

    printf("\nRNO\tNAME\tSTD\tMARKS");
    for (i = 0; i < index; i++) {
        if (srecords[i].active == 1) {
            printStudent(srecords[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("\nNo active student records found.");
    }
}

/* ---------------- DELETE STUDENT ---------------- */
void deleteStudentbyId() {
    int i, rno, flag = 1;

    printf("\nEnter Rno to delete: ");
    scanf("%d", &rno);

    for (i = 0; i < index; i++) {
        if (srecords[i].rno == rno && srecords[i].active == 1) {
            srecords[i].active = 0;
            printf("\nStudent Rno %d deleted successfully.", rno);
            flag = 0;
            break;
        }
    }

    if (flag) {
        printf("\nStudent Rno %d not found.", rno);
    }
}

/* ---------------- SEARCH STUDENT ---------------- */
void searchStudentById() {
    int i, rno, flag = 1;

    printf("\nEnter Rno to search: ");
    scanf("%d", &rno);

    for (i = 0; i < index; i++) {
        if (srecords[i].rno == rno && srecords[i].active == 1) {
            printf("\nRNO\tNAME\tSTD\tMARKS");
            printStudent(srecords[i]);
            flag = 0;
            break;
        }
    }

    if (flag) {
        printf("\nStudent Rno %d not found.", rno);
    }
}

/* ---------------- UPDATE STUDENT ---------------- */
void updateStudentById() {
    int i, rno, flag = 1;

    printf("\nEnter Rno to update: ");
    scanf("%d", &rno);

    for (i = 0; i < index; i++) {
        if (srecords[i].rno == rno && srecords[i].active == 1) {

            flushall();
            printf("Enter New Name: ");
            gets(srecords[i].name);

            printf("Enter New Std: ");
            scanf("%d", &srecords[i].std);

            printf("Enter New Marks: ");
            scanf("%d", &srecords[i].marks);

            printf("\nStudent Rno %d updated successfully.", rno);
            flag = 0;
            break;
        }
    }

    if (flag) {
        printf("\nStudent Rno %d not found.", rno);
    }
}

/* ---------------- SORT STUDENTS ---------------- */
void sortStudentByMarks() {
    struct Student temp;
    int i, j;

    for (i = 0; i < index - 1; i++) {
        for (j = i + 1; j < index; j++) {
            if (srecords[i].marks < srecords[j].marks) {
                temp = srecords[i];
                srecords[i] = srecords[j];
                srecords[j] = temp;
            }
        }
    }

    printf("\nStudents sorted by marks.");
    displayStudents();
}

/* ---------------- PRINT STUDENT ---------------- */
void printStudent(struct Student s) {
    printf("\n%d\t%s\t%d\t%d", s.rno, s.name, s.std, s.marks);
}

/* ---------------- MAIN ---------------- */
void main() {
    int choice;
    clrscr();

    do {
        printf("\n\n----- STUDENT MANAGEMENT SYSTEM -----");
        printf("\n1. Add Student");
        printf("\n2. Update Student By Id");
        printf("\n3. Delete Student By Id");
        printf("\n4. Display All Students");
        printf("\n5. Search Student By Id");
        printf("\n6. Sort Students By Marks");
        printf("\n7. Exit");
        printf("\nEnter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: updateStudentById(); break;
            case 3: deleteStudentbyId(); break;
            case 4: displayStudents(); break;
            case 5: searchStudentById(); break;
            case 6: sortStudentByMarks(); break;
            case 7:
                printf("\nExiting Application...");
                delay(1000);
                exit(0);
        }

    } while (choice != 7);

    getch();
}
