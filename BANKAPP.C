<<<<<<< HEAD
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define DATAFILE "bank.dat"
#define TEMPFILE "temp.dat"

/* ---------- STRUCT ---------- */
struct Account {
    int accNo;
    char name[30];
    int pin;           /* customer login pin */
    double balance;
};

/* ---------- UTIL ---------- */
void pauseScreen() {
    printf("\n\nPress any key to continue...");
    getch();
}

void clearInputBuffer() {
    /* Turbo C sometimes needs this after scanf */
    while (kbhit()) getch();
}

/* Find account position in file (for update-in-place) */
int findAccountPos(int accNo, struct Account *outAcc, long *outPos) {
    FILE *fp;
    struct Account a;
    long pos = 0;

    fp = fopen(DATAFILE, "rb");
    if (fp == NULL) return 0;

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo == accNo) {
            if (outAcc) *outAcc = a;
            if (outPos) *outPos = pos;
            fclose(fp);
            return 1;
        }
        pos += sizeof(a);
    }
    fclose(fp);
    return 0;
}

int updateAccountAtPos(long pos, struct Account *a) {
    FILE *fp = fopen(DATAFILE, "rb+");
    if (fp == NULL) return 0;

    fseek(fp, pos, SEEK_SET);
    if (fwrite(a, sizeof(*a), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

int isAccNoUnique(int accNo) {
    return !findAccountPos(accNo, NULL, NULL);
}

/* ---------- ADMIN FUNCTIONS ---------- */
void adminCreateAccount() {
    FILE *fp;
    struct Account a;

    clrscr();
    printf("=== ADMIN: CREATE ACCOUNT ===\n\n");

    /* Initialize to avoid garbage */
    a.accNo = 0;
    a.pin = 0;
    a.balance = 0.0;
    strcpy(a.name, "");

    printf("Enter New Account Number: ");
    scanf("%d", &a.accNo);

    if (!isAccNoUnique(a.accNo)) {
        printf("\nAccount number already exists!");
        pauseScreen();
        return;
    }

    printf("Enter Customer Name: ");
    clearInputBuffer();
    scanf(" %[^\n]", a.name);

    printf("Set 4-digit PIN: ");
    scanf("%d", &a.pin);

    printf("Enter Opening Balance: ");
    scanf("%lf", &a.balance);

    if (a.balance < 0) a.balance = 0;

    fp = fopen(DATAFILE, "ab");
    if (fp == NULL) {
        printf("\nError: Cannot open data file!");
        pauseScreen();
        return;
    }

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully!");
    pauseScreen();
}

void adminDisplayAll() {
    FILE *fp;
    struct Account a;
    int count = 0;

    clrscr();
    printf("=== ADMIN: DISPLAY ALL ACCOUNTS ===\n\n");

    fp = fopen(DATAFILE, "rb");
    if (fp == NULL) {
        printf("No accounts found (data file missing).");
        pauseScreen();
        return;
    }

    printf("AccNo\tName\t\t\tBalance\n");
    printf("----------------------------------------------------\n");

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        printf("%d\t%-20s\t%.2lf\n", a.accNo, a.name, a.balance);
        count++;
    }
    fclose(fp);

    if (count == 0) printf("\nNo accounts in file.");
    pauseScreen();
}

void adminSearchAccount() {
    struct Account a;
    int acc;

    clrscr();
    printf("=== ADMIN: SEARCH ACCOUNT ===\n\n");
    printf("Enter Account Number: ");
    scanf("%d", &acc);

    if (findAccountPos(acc, &a, NULL)) {
        printf("\nAccount Found!\n");
        printf("AccNo   : %d\n", a.accNo);
        printf("Name    : %s\n", a.name);
        printf("Balance : %.2lf\n", a.balance);
        /* Note: For security, we do not display PIN */
    } else {
        printf("\nAccount Not Found!");
    }

    pauseScreen();
}

void adminDeleteAccount() {
    FILE *fp, *temp;
    struct Account a;
    int acc, found = 0;

    clrscr();
    printf("=== ADMIN: DELETE ACCOUNT ===\n\n");
    printf("Enter Account Number to Delete: ");
    scanf("%d", &acc);

    fp = fopen(DATAFILE, "rb");
    if (fp == NULL) {
        printf("\nNo accounts found.");
        pauseScreen();
        return;
    }

    temp = fopen(TEMPFILE, "wb");
    if (temp == NULL) {
        fclose(fp);
        printf("\nError: Cannot create temp file.");
        pauseScreen();
        return;
    }

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo == acc) {
            found = 1;
            continue; /* skip writing => deletes */
        }
        fwrite(&a, sizeof(a), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    /* Replace old file */
    remove(DATAFILE);
    rename(TEMPFILE, DATAFILE);

    if (found)
        printf("\nAccount deleted successfully!");
    else
        printf("\nAccount not found!");

    pauseScreen();
}

/* ---------- CUSTOMER FUNCTIONS ---------- */
int customerLogin(struct Account *loggedAcc, long *pos) {
    int acc, pin;
    struct Account a;
    long p = 0;

    clrscr();
    printf("=== CUSTOMER LOGIN ===\n\n");
    printf("Account Number: ");
    scanf("%d", &acc);
    printf("PIN: ");
    scanf("%d", &pin);

    if (findAccountPos(acc, &a, &p)) {
        if (a.pin == pin) {
            *loggedAcc = a;
            *pos = p;
            return 1;
        } else {
            printf("\nInvalid PIN!");
            pauseScreen();
            return 0;
        }
    } else {
        printf("\nAccount not found!");
        pauseScreen();
        return 0;
    }
}

void customerShowDetails(struct Account *a) {
    clrscr();
    printf("=== CUSTOMER: ACCOUNT DETAILS ===\n\n");
    printf("AccNo   : %d\n", a->accNo);
    printf("Name    : %s\n", a->name);
    printf("Balance : %.2lf\n", a->balance);
    pauseScreen();
}

void customerDeposit(struct Account *a, long pos) {
    double amt;

    clrscr();
    printf("=== CUSTOMER: DEPOSIT ===\n\n");
    printf("Current Balance: %.2lf\n", a->balance);
    printf("Enter Amount to Deposit: ");
    scanf("%lf", &amt);

    if (amt <= 0) {
        printf("\nInvalid amount!");
        pauseScreen();
        return;
    }

    a->balance += amt;
    if (updateAccountAtPos(pos, a)) {
        printf("\nDeposit successful!");
        printf("\nNew Balance: %.2lf", a->balance);
    } else {
        printf("\nError updating account!");
    }
    pauseScreen();
}

void customerWithdraw(struct Account *a, long pos) {
    double amt;

    clrscr();
    printf("=== CUSTOMER: WITHDRAW ===\n\n");
    printf("Current Balance: %.2lf\n", a->balance);
    printf("Enter Amount to Withdraw: ");
    scanf("%lf", &amt);

    if (amt <= 0) {
        printf("\nInvalid amount!");
        pauseScreen();
        return;
    }

    if (amt > a->balance) {
        printf("\nInsufficient balance!");
        pauseScreen();
        return;
    }

    a->balance -= amt;
    if (updateAccountAtPos(pos, a)) {
        printf("\nWithdraw successful!");
        printf("\nNew Balance: %.2lf", a->balance);
    } else {
        printf("\nError updating account!");
    }
    pauseScreen();
}

void customerCheckBalance(struct Account *a) {
    clrscr();
    printf("=== CUSTOMER: CHECK BALANCE ===\n\n");
    printf("Account: %d\n", a->accNo);
    printf("Balance: %.2lf\n", a->balance);
    pauseScreen();
}

void customerTransfer(struct Account *sender, long senderPos) {
    int recvAccNo;
    double amt;
    struct Account receiver;
    long receiverPos;

    clrscr();
    printf("=== CUSTOMER: TRANSFER MONEY ===\n\n");
    printf("Your Balance: %.2lf\n\n", sender->balance);

    printf("Enter Receiver Account Number: ");
    scanf("%d", &recvAccNo);

    if (recvAccNo == sender->accNo) {
        printf("\nYou cannot transfer to your own account!");
        pauseScreen();
        return;
    }

    if (!findAccountPos(recvAccNo, &receiver, &receiverPos)) {
        printf("\nReceiver account not found!");
        pauseScreen();
        return;
    }

    printf("Enter Amount to Transfer: ");
    scanf("%lf", &amt);

    if (amt <= 0) {
        printf("\nInvalid amount!");
        pauseScreen();
        return;
    }

    if (amt > sender->balance) {
        printf("\nInsufficient balance!");
        pauseScreen();
        return;
    }

    /* Update both */
    sender->balance -= amt;
    receiver.balance += amt;

    if (!updateAccountAtPos(senderPos, sender)) {
        printf("\nError updating sender account!");
        pauseScreen();
        return;
    }

    if (!updateAccountAtPos(receiverPos, &receiver)) {
        /* best-effort rollback */
        sender->balance += amt;
        updateAccountAtPos(senderPos, sender);
        printf("\nError updating receiver account! Transfer cancelled.");
        pauseScreen();
        return;
    }

    printf("\nTransfer successful!");
    printf("\nNew Balance: %.2lf", sender->balance);
    pauseScreen();
}

/* ---------- MENUS ---------- */
void adminMenu() {
    int ch;
    while (1) {
        clrscr();
        printf("=== ADMIN MENU ===\n\n");
        printf("1. Create Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search Account\n");
        printf("4. Delete Account\n");
        printf("5. Back\n\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: adminCreateAccount(); break;
            case 2: adminDisplayAll(); break;
            case 3: adminSearchAccount(); break;
            case 4: adminDeleteAccount(); break;
            case 5: return;
            default: printf("\nInvalid choice!"); pauseScreen();
        }
    }
}

void customerMenu() {
    struct Account acc;
    long pos;
    int ch;

    if (!customerLogin(&acc, &pos)) return;

    while (1) {
        clrscr();
        printf("=== CUSTOMER MENU ===\n\n");
        printf("Welcome, %s (Acc: %d)\n", acc.name, acc.accNo);
        printf("1. View Account Details\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Check Balance\n");
        printf("5. Transfer Money\n");
        printf("6. Logout\n\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        /* Refresh latest copy from file sometimes (optional but safer) */
        if (ch >= 1 && ch <= 5) {
            struct Account latest;
            long latestPos;
            if (findAccountPos(acc.accNo, &latest, &latestPos)) {
                acc = latest;
                pos = latestPos;
            }
        }

        switch (ch) {
            case 1: customerShowDetails(&acc); break;
            case 2: customerDeposit(&acc, pos); break;
            case 3: customerWithdraw(&acc, pos); break;
            case 4: customerCheckBalance(&acc); break;
            case 5: customerTransfer(&acc, pos); break;
            case 6: return;
            default: printf("\nInvalid choice!"); pauseScreen();
        }
    }
}

int adminLogin() {
    /* Simple hardcoded admin credentials */
    char user[20], pass[20];

    clrscr();
    printf("=== ADMIN LOGIN ===\n\n");
    printf("Username: ");
    clearInputBuffer();
    scanf(" %19s", user);
    printf("Password: ");
    scanf(" %19s", pass);

    if (strcmp(user, "admin") == 0 && strcmp(pass, "1234") == 0) {
        return 1;
    }
    printf("\nInvalid admin credentials!");
    pauseScreen();
    return 0;
}

/* ---------- MAIN ---------- */
void main() {
    int ch;

    while (1) {
        clrscr();
        printf("=== BANKING SYSTEM ===\n\n");
        printf("1. Admin\n");
        printf("2. Customer\n");
        printf("3. Exit\n\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                if (adminLogin()) adminMenu();
                break;
            case 2:
                customerMenu();
                break;
            case 3:
                clrscr();
                printf("Thank you!");
                getch();
                return;
            default:
                printf("\nInvalid choice!");
                pauseScreen();
        }
    }
}
=======
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define DATAFILE "bank.dat"
#define TEMPFILE "temp.dat"

/* ---------- STRUCT ---------- */
struct Account {
    int accNo;
    char name[30];
    int pin;           /* customer login pin */
    double balance;
};

/* ---------- UTIL ---------- */
void pauseScreen() {
    printf("\n\nPress any key to continue...");
    getch();
}

void clearInputBuffer() {
    /* Turbo C sometimes needs this after scanf */
    while (kbhit()) getch();
}

/* Find account position in file (for update-in-place) */
int findAccountPos(int accNo, struct Account *outAcc, long *outPos) {
    FILE *fp;
    struct Account a;
    long pos = 0;

    fp = fopen(DATAFILE, "rb");
    if (fp == NULL) return 0;

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo == accNo) {
            if (outAcc) *outAcc = a;
            if (outPos) *outPos = pos;
            fclose(fp);
            return 1;
        }
        pos += sizeof(a);
    }
    fclose(fp);
    return 0;
}

int updateAccountAtPos(long pos, struct Account *a) {
    FILE *fp = fopen(DATAFILE, "rb+");
    if (fp == NULL) return 0;

    fseek(fp, pos, SEEK_SET);
    if (fwrite(a, sizeof(*a), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

int isAccNoUnique(int accNo) {
    return !findAccountPos(accNo, NULL, NULL);
}

/* ---------- ADMIN FUNCTIONS ---------- */
void adminCreateAccount() {
    FILE *fp;
    struct Account a;

    clrscr();
    printf("=== ADMIN: CREATE ACCOUNT ===\n\n");

    /* Initialize to avoid garbage */
    a.accNo = 0;
    a.pin = 0;
    a.balance = 0.0;
    strcpy(a.name, "");

    printf("Enter New Account Number: ");
    scanf("%d", &a.accNo);

    if (!isAccNoUnique(a.accNo)) {
        printf("\nAccount number already exists!");
        pauseScreen();
        return;
    }

    printf("Enter Customer Name: ");
    clearInputBuffer();
    scanf(" %[^\n]", a.name);

    printf("Set 4-digit PIN: ");
    scanf("%d", &a.pin);

    printf("Enter Opening Balance: ");
    scanf("%lf", &a.balance);

    if (a.balance < 0) a.balance = 0;

    fp = fopen(DATAFILE, "ab");
    if (fp == NULL) {
        printf("\nError: Cannot open data file!");
        pauseScreen();
        return;
    }

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully!");
    pauseScreen();
}

void adminDisplayAll() {
    FILE *fp;
    struct Account a;
    int count = 0;

    clrscr();
    printf("=== ADMIN: DISPLAY ALL ACCOUNTS ===\n\n");

    fp = fopen(DATAFILE, "rb");
    if (fp == NULL) {
        printf("No accounts found (data file missing).");
        pauseScreen();
        return;
    }

    printf("AccNo\tName\t\t\tBalance\n");
    printf("----------------------------------------------------\n");

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        printf("%d\t%-20s\t%.2lf\n", a.accNo, a.name, a.balance);
        count++;
    }
    fclose(fp);

    if (count == 0) printf("\nNo accounts in file.");
    pauseScreen();
}

void adminSearchAccount() {
    struct Account a;
    int acc;

    clrscr();
    printf("=== ADMIN: SEARCH ACCOUNT ===\n\n");
    printf("Enter Account Number: ");
    scanf("%d", &acc);

    if (findAccountPos(acc, &a, NULL)) {
        printf("\nAccount Found!\n");
        printf("AccNo   : %d\n", a.accNo);
        printf("Name    : %s\n", a.name);
        printf("Balance : %.2lf\n", a.balance);
        /* Note: For security, we do not display PIN */
    } else {
        printf("\nAccount Not Found!");
    }

    pauseScreen();
}

void adminDeleteAccount() {
    FILE *fp, *temp;
    struct Account a;
    int acc, found = 0;

    clrscr();
    printf("=== ADMIN: DELETE ACCOUNT ===\n\n");
    printf("Enter Account Number to Delete: ");
    scanf("%d", &acc);

    fp = fopen(DATAFILE, "rb");
    if (fp == NULL) {
        printf("\nNo accounts found.");
        pauseScreen();
        return;
    }

    temp = fopen(TEMPFILE, "wb");
    if (temp == NULL) {
        fclose(fp);
        printf("\nError: Cannot create temp file.");
        pauseScreen();
        return;
    }

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo == acc) {
            found = 1;
            continue; /* skip writing => deletes */
        }
        fwrite(&a, sizeof(a), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    /* Replace old file */
    remove(DATAFILE);
    rename(TEMPFILE, DATAFILE);

    if (found)
        printf("\nAccount deleted successfully!");
    else
        printf("\nAccount not found!");

    pauseScreen();
}

/* ---------- CUSTOMER FUNCTIONS ---------- */
int customerLogin(struct Account *loggedAcc, long *pos) {
    int acc, pin;
    struct Account a;
    long p = 0;

    clrscr();
    printf("=== CUSTOMER LOGIN ===\n\n");
    printf("Account Number: ");
    scanf("%d", &acc);
    printf("PIN: ");
    scanf("%d", &pin);

    if (findAccountPos(acc, &a, &p)) {
        if (a.pin == pin) {
            *loggedAcc = a;
            *pos = p;
            return 1;
        } else {
            printf("\nInvalid PIN!");
            pauseScreen();
            return 0;
        }
    } else {
        printf("\nAccount not found!");
        pauseScreen();
        return 0;
    }
}

void customerShowDetails(struct Account *a) {
    clrscr();
    printf("=== CUSTOMER: ACCOUNT DETAILS ===\n\n");
    printf("AccNo   : %d\n", a->accNo);
    printf("Name    : %s\n", a->name);
    printf("Balance : %.2lf\n", a->balance);
    pauseScreen();
}

void customerDeposit(struct Account *a, long pos) {
    double amt;

    clrscr();
    printf("=== CUSTOMER: DEPOSIT ===\n\n");
    printf("Current Balance: %.2lf\n", a->balance);
    printf("Enter Amount to Deposit: ");
    scanf("%lf", &amt);

    if (amt <= 0) {
        printf("\nInvalid amount!");
        pauseScreen();
        return;
    }

    a->balance += amt;
    if (updateAccountAtPos(pos, a)) {
        printf("\nDeposit successful!");
        printf("\nNew Balance: %.2lf", a->balance);
    } else {
        printf("\nError updating account!");
    }
    pauseScreen();
}

void customerWithdraw(struct Account *a, long pos) {
    double amt;

    clrscr();
    printf("=== CUSTOMER: WITHDRAW ===\n\n");
    printf("Current Balance: %.2lf\n", a->balance);
    printf("Enter Amount to Withdraw: ");
    scanf("%lf", &amt);

    if (amt <= 0) {
        printf("\nInvalid amount!");
        pauseScreen();
        return;
    }

    if (amt > a->balance) {
        printf("\nInsufficient balance!");
        pauseScreen();
        return;
    }

    a->balance -= amt;
    if (updateAccountAtPos(pos, a)) {
        printf("\nWithdraw successful!");
        printf("\nNew Balance: %.2lf", a->balance);
    } else {
        printf("\nError updating account!");
    }
    pauseScreen();
}

void customerCheckBalance(struct Account *a) {
    clrscr();
    printf("=== CUSTOMER: CHECK BALANCE ===\n\n");
    printf("Account: %d\n", a->accNo);
    printf("Balance: %.2lf\n", a->balance);
    pauseScreen();
}

void customerTransfer(struct Account *sender, long senderPos) {
    int recvAccNo;
    double amt;
    struct Account receiver;
    long receiverPos;

    clrscr();
    printf("=== CUSTOMER: TRANSFER MONEY ===\n\n");
    printf("Your Balance: %.2lf\n\n", sender->balance);

    printf("Enter Receiver Account Number: ");
    scanf("%d", &recvAccNo);

    if (recvAccNo == sender->accNo) {
        printf("\nYou cannot transfer to your own account!");
        pauseScreen();
        return;
    }

    if (!findAccountPos(recvAccNo, &receiver, &receiverPos)) {
        printf("\nReceiver account not found!");
        pauseScreen();
        return;
    }

    printf("Enter Amount to Transfer: ");
    scanf("%lf", &amt);

    if (amt <= 0) {
        printf("\nInvalid amount!");
        pauseScreen();
        return;
    }

    if (amt > sender->balance) {
        printf("\nInsufficient balance!");
        pauseScreen();
        return;
    }

    /* Update both */
    sender->balance -= amt;
    receiver.balance += amt;

    if (!updateAccountAtPos(senderPos, sender)) {
        printf("\nError updating sender account!");
        pauseScreen();
        return;
    }

    if (!updateAccountAtPos(receiverPos, &receiver)) {
        /* best-effort rollback */
        sender->balance += amt;
        updateAccountAtPos(senderPos, sender);
        printf("\nError updating receiver account! Transfer cancelled.");
        pauseScreen();
        return;
    }

    printf("\nTransfer successful!");
    printf("\nNew Balance: %.2lf", sender->balance);
    pauseScreen();
}

/* ---------- MENUS ---------- */
void adminMenu() {
    int ch;
    while (1) {
        clrscr();
        printf("=== ADMIN MENU ===\n\n");
        printf("1. Create Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search Account\n");
        printf("4. Delete Account\n");
        printf("5. Back\n\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: adminCreateAccount(); break;
            case 2: adminDisplayAll(); break;
            case 3: adminSearchAccount(); break;
            case 4: adminDeleteAccount(); break;
            case 5: return;
            default: printf("\nInvalid choice!"); pauseScreen();
        }
    }
}

void customerMenu() {
    struct Account acc;
    long pos;
    int ch;

    if (!customerLogin(&acc, &pos)) return;

    while (1) {
        clrscr();
        printf("=== CUSTOMER MENU ===\n\n");
        printf("Welcome, %s (Acc: %d)\n", acc.name, acc.accNo);
        printf("1. View Account Details\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Check Balance\n");
        printf("5. Transfer Money\n");
        printf("6. Logout\n\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        /* Refresh latest copy from file sometimes (optional but safer) */
        if (ch >= 1 && ch <= 5) {
            struct Account latest;
            long latestPos;
            if (findAccountPos(acc.accNo, &latest, &latestPos)) {
                acc = latest;
                pos = latestPos;
            }
        }

        switch (ch) {
            case 1: customerShowDetails(&acc); break;
            case 2: customerDeposit(&acc, pos); break;
            case 3: customerWithdraw(&acc, pos); break;
            case 4: customerCheckBalance(&acc); break;
            case 5: customerTransfer(&acc, pos); break;
            case 6: return;
            default: printf("\nInvalid choice!"); pauseScreen();
        }
    }
}

int adminLogin() {
    /* Simple hardcoded admin credentials */
    char user[20], pass[20];

    clrscr();
    printf("=== ADMIN LOGIN ===\n\n");
    printf("Username: ");
    clearInputBuffer();
    scanf(" %19s", user);
    printf("Password: ");
    scanf(" %19s", pass);

    if (strcmp(user, "admin") == 0 && strcmp(pass, "1234") == 0) {
        return 1;
    }
    printf("\nInvalid admin credentials!");
    pauseScreen();
    return 0;
}

/* ---------- MAIN ---------- */
void main() {
    int ch;

    while (1) {
        clrscr();
        printf("=== BANKING SYSTEM ===\n\n");
        printf("1. Admin\n");
        printf("2. Customer\n");
        printf("3. Exit\n\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                if (adminLogin()) adminMenu();
                break;
            case 2:
                customerMenu();
                break;
            case 3:
                clrscr();
                printf("Thank you!");
                getch();
                return;
            default:
                printf("\nInvalid choice!");
                pauseScreen();
        }
    }
}
>>>>>>> 95700157b467a45732e969c19d3e713f7c43635b
