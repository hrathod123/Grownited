/*  Product Inventory Management System (Turbo C++ 3.2 Compatible)
    Features:
    - Login (3 attempts) : admin / 1234
    - Add, Display, Search by ID, Sort by Price, Update by ID, Delete by ID (logical), Exit
*/

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

struct Product {
    int id;
    char name[50];
    char category[30];
    int quantity;
    float price;
    int active;   /* 1 = available, 0 = deleted */
};

struct Product products[MAX];
int count = 0;

/* ---------- Utility Functions ---------- */

void clrs()
{
    clrscr(); /* Turbo C++ */
}

void pauseScreen()
{
    printf("\nPress any key to continue...");
    getch();
}

int isIdUnique(int id)
{
    int i;
    for(i=0;i<count;i++)
    {
        if(products[i].active==1 && products[i].id==id)
            return 0;
    }
    return 1;
}

int findIndexById(int id)
{
    int i;
    for(i=0;i<count;i++)
    {
        if(products[i].id==id && products[i].active==1)
            return i;
    }
    return -1;
}

/* flush remaining newline from input buffer */
void flushLine()
{
    int ch;
    while((ch=getchar())!='\n' && ch!=EOF) { }
}

/* read a full line safely (Turbo C++) */
void readLine(char *s, int size)
{
    int i=0, ch;
    /* skip leading newlines */
    while((ch=getchar())=='\n' || ch=='\r') { }
    if(ch==EOF) { s[0]='\0'; return; }
    s[i++] = (char)ch;

    while(i < size-1 && (ch=getchar())!=EOF && ch!='\n' && ch!='\r')
    {
        s[i++] = (char)ch;
    }
    s[i] = '\0';

    /* if line still had extra chars, flush */
    if(ch!='\n' && ch!='\r') flushLine();
}

/* ---------- Login ---------- */

int login()
{
    char user[30], pass[30];
    int attempts = 3;

    while(attempts > 0)
    {
        clrs();
        printf("===== LOGIN =====\n");
        printf("Username: ");
        scanf("%s", user);
        printf("Password: ");
        scanf("%s", pass);

        if(strcmp(user, "admin")==0 && strcmp(pass, "1234")==0)
        {
            printf("\nLogin Successful!");
            pauseScreen();
            return 1;
        }
        else
        {
            attempts--;
            printf("\nInvalid Username/Password!");
            printf("\nAttempts left: %d", attempts);
            pauseScreen();
        }
    }

    clrs();
    printf("Access Denied. Exiting program...");
    getch();
    return 0;
}

/* ---------- Core Operations ---------- */

void addProduct()
{
    struct Product p;
    clrs();

    if(count >= MAX)
    {
        printf("Inventory Full! Cannot add more products.");
        pauseScreen();
        return;
    }

    printf("===== ADD PRODUCT =====\n");

    printf("Enter Product ID: ");
    scanf("%d", &p.id);

    if(!isIdUnique(p.id))
    {
        printf("\nThis ID already exists (active product). Use a different ID.");
        pauseScreen();
        return;
    }

    printf("Enter Product Name: ");
    flushLine();              /* remove leftover newline */
    readLine(p.name, 50);

    printf("Enter Category: ");
    readLine(p.category, 30);

    printf("Enter Quantity: ");
    scanf("%d", &p.quantity);

    printf("Enter Price: ");
    scanf("%f", &p.price);

    p.active = 1;

    products[count] = p;
    count++;

    printf("\nProduct Added Successfully!");
    pauseScreen();
}

void displayAll()
{
    int i, found=0;
    clrs();

    printf("===== ALL PRODUCTS (Active Only) =====\n");
    printf("-----------------------------------------------------------------\n");
    printf("ID\tName\t\t\tCategory\tQty\tPrice\n");
    printf("-----------------------------------------------------------------\n");

    for(i=0;i<count;i++)
    {
        if(products[i].active==1)
        {
            found = 1;
            printf("%d\t%-20s\t%-10s\t%d\t%.2f\n",
                   products[i].id,
                   products[i].name,
                   products[i].category,
                   products[i].quantity,
                   products[i].price);
        }
    }

    if(!found)
        printf("No active products found.");

    pauseScreen();
}

void searchById()
{
    int id, idx;
    clrs();

    printf("===== SEARCH PRODUCT BY ID =====\n");
    printf("Enter Product ID: ");
    scanf("%d", &id);

    idx = findIndexById(id);

    if(idx==-1)
    {
        printf("\nProduct not found (or deleted).");
    }
    else
    {
        printf("\nProduct Found:\n");
        printf("ID       : %d\n", products[idx].id);
        printf("Name     : %s\n", products[idx].name);
        printf("Category : %s\n", products[idx].category);
        printf("Quantity : %d\n", products[idx].quantity);
        printf("Price    : %.2f\n", products[idx].price);
    }

    pauseScreen();
}

void sortByPrice()
{
    int i, j;
    struct Product temp;
    clrs();

    if(count<=1)
    {
        printf("Not enough products to sort.");
        pauseScreen();
        return;
    }

    /* Bubble sort only among active products while keeping deleted as-is */
    for(i=0;i<count-1;i++)
    {
        for(j=0;j<count-1-i;j++)
        {
            if(products[j].active==1 && products[j+1].active==1)
            {
                if(products[j].price > products[j+1].price)
                {
                    temp = products[j];
                    products[j] = products[j+1];
                    products[j+1] = temp;
                }
            }
        }
    }

    printf("Active products sorted by Price (Ascending) successfully!");
    pauseScreen();
}

void updateById()
{
    int id, idx;
    clrs();

    printf("===== UPDATE PRODUCT BY ID =====\n");
    printf("Enter Product ID: ");
    scanf("%d", &id);

    idx = findIndexById(id);

    if(idx==-1)
    {
        printf("\nProduct not found (or deleted).");
        pauseScreen();
        return;
    }

    printf("\nCurrent Details:\n");
    printf("Name     : %s\n", products[idx].name);
    printf("Category : %s\n", products[idx].category);
    printf("Quantity : %d\n", products[idx].quantity);
    printf("Price    : %.2f\n", products[idx].price);

    printf("\n--- Enter New Details ---\n");

    printf("Enter New Name: ");
    flushLine();
    readLine(products[idx].name, 50);

    printf("Enter New Category: ");
    readLine(products[idx].category, 30);

    printf("Enter New Quantity: ");
    scanf("%d", &products[idx].quantity);

    printf("Enter New Price: ");
    scanf("%f", &products[idx].price);

    printf("\nProduct Updated Successfully!");
    pauseScreen();
}

void deleteById()
{
    int id, idx;
    clrs();

    printf("===== DELETE PRODUCT BY ID =====\n");
    printf("Enter Product ID: ");
    scanf("%d", &id);

    idx = findIndexById(id);

    if(idx==-1)
    {
        printf("\nProduct not found (or already deleted).");
    }
    else
    {
        products[idx].active = 0; /* logical delete */
        printf("\nProduct Deleted Successfully (Logical Delete).");
    }

    pauseScreen();
}

/* ---------- Menu ---------- */

void menu()
{
    int choice;

    do {
        clrs();
        printf("===== PRODUCT INVENTORY MANAGEMENT =====\n");
        printf("1. Add Product\n");
        printf("2. Display All Products\n");
        printf("3. Search Product by ID\n");
        printf("4. Sort Products by Price\n");
        printf("5. Update Product By ID\n");
        printf("6. Delete Product By ID\n");
        printf("0. Exit\n");
        printf("----------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1: addProduct(); break;
            case 2: displayAll(); break;
            case 3: searchById(); break;
            case 4: sortByPrice(); break;
            case 5: updateById(); break;
            case 6: deleteById(); break;
            case 0: clrs(); printf("Exiting... Thank you!"); getch(); break;
            default:
                printf("\nInvalid Choice!");
                pauseScreen();
        }
    } while(choice != 0);
}

int main()
{
    if(login())
    {
        menu();
    }
    return 0;
}
