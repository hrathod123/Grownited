#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Product {
    int id;
    char name[50];
    char category[30];
    int quantity;
    float price;
    int active;   // 1 = available, 0 = deleted
};

struct Product products[MAX];
int count = 0;

/* ---------- FUNCTION DECLARATIONS ---------- */
int login();
void addProduct();
void displayProducts();
void searchProductById();
void sortProductsByPrice();
void updateProductById();
void deleteProductById();
void printProduct(struct Product p);

/* ---------- LOGIN FUNCTION ---------- */
int login() {
    char username[20], password[20];
    int attempts = 3;

    while (attempts > 0) {
        printf("\nLogin Attempts Left: %d", attempts);
        printf("\nEnter Username: ");
        scanf("%s", username);

        printf("Enter Password: ");
        scanf("%s", password);

        if (strcmp(username, "admin") == 0 &&
            strcmp(password, "1234") == 0) {
            printf("\nLogin Successful!\n");
            return 1;
        } else {
            printf("\nInvalid Username or Password!\n");
        }
        attempts--;
    }

    printf("\nAccess Denied. Exiting program...\n");
    return 0;
}

/* ---------- ADD PRODUCT ---------- */
void addProduct() {
    if (count == MAX) {
        printf("\nInventory Full!");
        return;
    }

    printf("\nEnter Product ID: ");
    scanf("%d", &products[count].id);

    printf("Enter Product Name: ");
    scanf(" %[^\n]", products[count].name);

    printf("Enter Category: ");
    scanf(" %[^\n]", products[count].category);

    printf("Enter Quantity: ");
    scanf("%d", &products[count].quantity);

    printf("Enter Price: ");
    scanf("%f", &products[count].price);

    products[count].active = 1;
    count++;

    printf("\nProduct Added Successfully!\n");
}

/* ---------- DISPLAY PRODUCTS ---------- */
void displayProducts() {
    int i, found = 0;

    printf("\nID\tNAME\tCATEGORY\tQTY\tPRICE");
    for (i = 0; i < count; i++) {
        if (products[i].active == 1) {
            printProduct(products[i]);
            found = 1;
        }
    }

    if (!found)
        printf("\nNo Active Products Found!");
}

/* ---------- SEARCH PRODUCT ---------- */
void searchProductById() {
    int id, i;

    printf("\nEnter Product ID to Search: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++) {
        if (products[i].id == id && products[i].active == 1) {
            printf("\nProduct Found:");
            printProduct(products[i]);
            return;
        }
    }

    printf("\nProduct Not Found!");
}

/* ---------- SORT BY PRICE (FIXED) ---------- */
void sortProductsByPrice() {
    int i, j;
    struct Product temp;

    /* Sort only active products */
    for (i = 0; i < count - 1; i++) {
        if (products[i].active == 0) continue;

        for (j = i + 1; j < count; j++) {
            if (products[j].active == 0) continue;

            if (products[i].price > products[j].price) {
                temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }

    printf("\nProducts Sorted by Price (Ascending):");
    displayProducts();   // 🔥 SHOW SORTED DATA
}

/* ---------- UPDATE PRODUCT ---------- */
void updateProductById() {
    int id, i;

    printf("\nEnter Product ID to Update: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++) {
        if (products[i].id == id && products[i].active == 1) {

            printf("Enter New Name: ");
            scanf(" %[^\n]", products[i].name);

            printf("Enter New Category: ");
            scanf(" %[^\n]", products[i].category);

            printf("Enter New Quantity: ");
            scanf("%d", &products[i].quantity);

            printf("Enter New Price: ");
            scanf("%f", &products[i].price);

            printf("\nProduct Updated Successfully!");
            return;
        }
    }

    printf("\nProduct Not Found!");
}

/* ---------- DELETE PRODUCT ---------- */
void deleteProductById() {
    int id, i;

    printf("\nEnter Product ID to Delete: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++) {
        if (products[i].id == id && products[i].active == 1) {
            products[i].active = 0;
            printf("\nProduct Deleted Successfully!");
            return;
        }
    }

    printf("\nProduct Not Found!");
}

/* ---------- PRINT PRODUCT ---------- */
void printProduct(struct Product p) {
    printf("\n%d\t%s\t%s\t%d\t%.2f",
           p.id, p.name, p.category, p.quantity, p.price);
}

/* ---------- MAIN ---------- */
int main() {
    int choice;

    if (!login())
        return 0;

    do {
        printf("\n\n===== PRODUCT INVENTORY MANAGEMENT =====");
        printf("\n1. Add Product");
        printf("\n2. Display All Products");
        printf("\n3. Search Product by ID");
        printf("\n4. Sort Products by Price");
        printf("\n5. Update Product By ID");
        printf("\n6. Delete Product By ID");
        printf("\n0. Exit");
        printf("\nEnter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addProduct(); break;
            case 2: displayProducts(); break;
            case 3: searchProductById(); break;
            case 4: sortProductsByPrice(); break;
            case 5: updateProductById(); break;
            case 6: deleteProductById(); break;
            case 0: printf("\nExiting Program...\n"); break;
            default: printf("\nInvalid Choice!");
        }

    } while (choice != 0);

    return 0;
}
