#include<stdio.h>
#include<conio.h>
#define SIZE 2
struct Student{
    int sid;
    char name[30];
    float fees;

}s[SIZE];
void scanStudentDetails();
void displaySTudentDetails();
void main(){
    FILE *fp;
    struct Student t;
    char filename[30];
    int i;
    clrscr();
    printf("\nEnter the filename:");
    scanf("%s",filename);
    fp=fopen(filename,"w");
    scanStudentDetails();
    for(i=0;i<SIZE;i++){
	fwrite(&s[i],sizeof(s[i]),1,fp);
    }
    fclose(fp);
    fp=fopen(filename,"r");
    scanStudentDetails();
    for(i=0;i<SIZE;i++){
	fwrite(&t,sizeof(t),1,fp);
	displaySTudentDetails(t);
    }
    fclose(fp);
	getch();
}
void scanStudentDetails(){
    int i;
    float temp;
    for(i=0;i<SIZE;i++){
        printf("\nEnter the value of sid and name");
        scanf("%d%s",&s[i].sid,s[i].name);
        printf("\nEnter the fees:");
        scanf("%d",&temp);
        s[i].fees=temp;
    }
}

void displaySTudentDetails(struct Student t){
    int i;
    printf("\nsid\tname\tfees");
    printf("\n%d\t%s\t%,2f",t.sid,t.name,t.fees);
}