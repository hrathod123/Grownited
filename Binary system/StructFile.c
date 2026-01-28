// #include<stdio.h>
// #include<conio.h>
// #define SIZE 2
// struct Student{
//     int sid;
//     char name[30];
//     float fees;

// }s[SIZE];
// void scanStudentDetails();
// void displaySTudentDetails();
// void main(){
//     FILE *fp;
//     struct Student t;
//     char filename[30];
//     int i;
//     clrscr();
//     printf("\nEnter the filename:");
//     scanf("%s",filename);
//     fp=fopen(filename,"w");
//     scanStudentDetails();
//     for(i=0;i<SIZE;i++){
//         fwrite(&s[i],sizeof(s[i]),1,fp);
//     }
//     fclose(fp);
//     fp=fopen(filename,"r");
//     scanStudentDetails();
//     for(i=0;i<SIZE;i++){
//         fwrite(&t,sizeof(t),1,fp);
//         displayStudentDetails(t);
//     }
//     fclose(fp);
//     getch();

// }
// void scanStudentDetails(){
//     int i;
//     float temp;
//     for(i=0;i<SIZE;i++){
//         printf("\nEnter the value of sid and name");
//         scanf("%d%s",&s[i].sid,s[i].name);
//         printf("\nEnter the fees:");
//         scanf("%d",&temp);
//         s[i].fees=temp;
//     }
// }

// void displayStudentDetails(struct Student t){
//     int i;
//     printf("\nsid\tname\tfees");
//     printf("\n%d\t%s\t%,2f",t.sid,t.name,t.fees);
// }








#include<stdio.h>
#include<conio.h>
#include<string.h>

struct Student
{
    int sid;
    char name[30];
    float fees;
};

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void sortStudents();

void main()
{
    int choice;
    clrscr();

    do
    {
        clrscr();
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====");
        printf("\n1. Add Student");
        printf("\n2. Display Students");
        printf("\n3. Search Student");
        printf("\n4. Update Student");
        printf("\n5. Delete Student");
        printf("\n6. Sort Students");
        printf("\n0. Exit");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudents(); break;
            case 0: break;
            default: printf("\nInvalid choice!");
        }
        getch();
    }
    while(choice!=0);
}
void addStudent()
{
    FILE *fp;
    struct Student s;

    fp = fopen("student.dat","ab");

    printf("\nEnter Student ID: ");
    scanf("%d",&s.sid);
    printf("Enter Name: ");
    scanf("%s",s.name);
    printf("Enter Fees: ");
    scanf("%f",&s.fees);

    fwrite(&s,sizeof(s),1,fp);
    fclose(fp);

    printf("\nStudent Added Successfully!");
}
void displayStudents()
{
    FILE *fp;
    struct Student s;

    fp = fopen("student.dat","rb");

    if(fp==NULL)
    {
        printf("\nNo Records Found!");
        return;
    }

    printf("\nSID\tName\tFees\n");
    while(fread(&s,sizeof(s),1,fp))
    {
        printf("%d\t%s\t%.2f\n",s.sid,s.name,s.fees);
    }
    fclose(fp);
}
void searchStudent()
{
    FILE *fp;
    struct Student s;
    int id, found=0;

    printf("\nEnter Student ID to search: ");
    scanf("%d",&id);

    fp = fopen("student.dat","rb");

    while(fread(&s,sizeof(s),1,fp))
    {
        if(s.sid == id)
        {
            printf("\nRecord Found!");
            printf("\n%d\t%s\t%.2f",s.sid,s.name,s.fees);
            found=1;
            break;
        }
    }
    fclose(fp);

    if(!found)
        printf("\nStudent Not Found!");
}
void updateStudent()
{
    FILE *fp;
    struct Student s;
    int id, found=0;

    printf("\nEnter Student ID to update: ");
    scanf("%d",&id);

    fp = fopen("student.dat","rb+");

    while(fread(&s,sizeof(s),1,fp))
    {
        if(s.sid == id)
        {
            printf("\nEnter New Name: ");
            scanf("%s",s.name);
            printf("Enter New Fees: ");
            scanf("%f",&s.fees);

            fseek(fp,-sizeof(s),SEEK_CUR);
            fwrite(&s,sizeof(s),1,fp);

            found=1;
            break;
        }
    }
    fclose(fp);

    if(found)
        printf("\nRecord Updated!");
    else
        printf("\nStudent Not Found!");
}
void deleteStudent()
{
    FILE *fp,*temp;
    struct Student s;
    int id, found=0;

    printf("\nEnter Student ID to delete: ");
    scanf("%d",&id);

    fp = fopen("student.dat","rb");
    temp = fopen("temp.dat","wb");

    while(fread(&s,sizeof(s),1,fp))
    {
        if(s.sid != id)
            fwrite(&s,sizeof(s),1,temp);
        else
            found=1;
    }

    fclose(fp);
    fclose(temp);

    remove("student.dat");
    rename("temp.dat","student.dat");

    if(found)
        printf("\nRecord Deleted!");
    else
        printf("\nStudent Not Found!");
}
void sortStudents()
{
    FILE *fp;
    struct Student s[50], temp;
    int i,j,n=0;

    fp = fopen("student.dat","rb");

    while(fread(&s[n],sizeof(s[n]),1,fp))
        n++;

    fclose(fp);

    for(i=0;i<n-1;i++)
    {
        for(j=i+1;j<n;j++)
        {
            if(s[i].sid > s[j].sid)
            {
                temp = s[i];
                s[i] = s[j];
                s[j] = temp;
            }
        }
    }

    fp = fopen("student.dat","wb");
    for(i=0;i<n;i++)
        fwrite(&s[i],sizeof(s[i]),1,fp);

    fclose(fp);
    printf("\nRecords Sorted Successfully!");
}
