#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define MIN_BAL 500

typedef struct {
    int accNo;
    char name[50];
    float balance;
    int pin;
} Account;

Account accounts[MAX];
int count = 0;

// Function prototypes
void loadAccounts();
void saveAccounts();
void addAccount();
void deposit();
void withdraw();
void searchAccount();
void listAccounts();
void deleteAccount();
void viewTransactions();
int findAccount(int);
int verifyPIN(int);
void logTransaction(int, char[], float);

// Main function
int main() {
    int choice;
    loadAccounts();

    while (1) {
        printf("\n========= TRANSACTION SYSTEM =========\n");
        printf("1. Add Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Search Account\n");
        printf("5. List Accounts\n");
        printf("6. Delete Account\n");
        printf("7. View Transactions\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: searchAccount(); break;
            case 5: listAccounts(); break;
            case 6: deleteAccount(); break;
            case 7: viewTransactions(); break;
            case 8:
                saveAccounts();
                printf("Saved & Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
}

// Load accounts from file
void loadAccounts() {
    FILE *fp = fopen("accounts.txt", "r");
    if (fp == NULL) return;

    while (fscanf(fp, "%d %s %f %d",
        &accounts[count].accNo,
        accounts[count].name,
        &accounts[count].balance,
        &accounts[count].pin) != EOF) {
        count++;
    }
    fclose(fp);
}

// Save accounts
void saveAccounts() {
    FILE *fp = fopen("accounts.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %.2f %d\n",
            accounts[i].accNo,
            accounts[i].name,
            accounts[i].balance,
            accounts[i].pin);
    }
    fclose(fp);
}

// Add account
void addAccount() {
    printf("\n===== ADD ACCOUNT =====\n");

    printf("Enter Account No: ");
    scanf("%d", &accounts[count].accNo);

    if (findAccount(accounts[count].accNo) != -1) {
        printf("Account already exists!\n");
        return;
    }

    printf("Enter Name: ");
    scanf("%s", accounts[count].name);

    printf("Enter Balance: ");
    scanf("%f", &accounts[count].balance);

    printf("Set 4-digit PIN: ");
    scanf("%d", &accounts[count].pin);

    count++;
    saveAccounts();
    printf("Account created successfully!\n");
}

// Deposit
void deposit() {
    int accNo, index;
    float amt;

    printf("\n===== DEPOSIT =====\n");

    printf("Enter Account No: ");
    scanf("%d", &accNo);

    index = findAccount(accNo);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }

    if (!verifyPIN(index)) return;

    printf("Enter amount: ");
    scanf("%f", &amt);

    float oldBalance = accounts[index].balance;
    accounts[index].balance += amt;

    printf("\n===== DEPOSIT SUCCESS =====\n");
    printf("Account No   : %d\n", accNo);
    printf("Old Balance  : %.2f\n", oldBalance);
    printf("Deposited    : +%.2f\n", amt);
    printf("New Balance  : %.2f\n", accounts[index].balance);
    printf("===========================\n");
    printf("Transaction Complete ✔\n");

    logTransaction(accNo, "Deposit", amt);
    saveAccounts();
}

// Withdraw
void withdraw() {
    int accNo, index;
    float amt;

    printf("\n===== WITHDRAW =====\n");

    printf("Enter Account No: ");
    scanf("%d", &accNo);

    index = findAccount(accNo);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }

    if (!verifyPIN(index)) return;

    printf("Enter amount: ");
    scanf("%f", &amt);

    if (accounts[index].balance - amt < MIN_BAL) {
        printf("Minimum balance of 500 required!\n");
        return;
    }

    float oldBalance = accounts[index].balance;
    accounts[index].balance -= amt;

    printf("\n===== WITHDRAW SUCCESS =====\n");
    printf("Account No   : %d\n", accNo);
    printf("Old Balance  : %.2f\n", oldBalance);
    printf("Withdrawn    : -%.2f\n", amt);
    printf("New Balance  : %.2f\n", accounts[index].balance);
    printf("============================\n");
    printf("Transaction Complete ✔\n");

    logTransaction(accNo, "Withdraw", amt);
    saveAccounts();
}

// Search account
void searchAccount() {
    int accNo;
    printf("\nEnter Account No: ");
    scanf("%d", &accNo);

    int index = findAccount(accNo);

    if (index == -1) {
        printf("Account not found!\n");
    } else {
        printf("\n===== ACCOUNT DETAILS =====\n");
        printf("Account No : %d\n", accounts[index].accNo);
        printf("Name       : %s\n", accounts[index].name);
        printf("Balance    : %.2f\n", accounts[index].balance);
        printf("===========================\n");
    }
}

// List accounts
void listAccounts() {
    if (count == 0) {
        printf("No accounts available.\n");
        return;
    }

    printf("\n=========== ACCOUNT LIST ===========\n");
    printf("AccNo\tName\tBalance\n");
    printf("-----------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t%.2f\n",
            accounts[i].accNo,
            accounts[i].name,
            accounts[i].balance);
    }

    printf("===================================\n");
}

// Delete account
void deleteAccount() {
    int accNo;
    printf("\nEnter Account No to delete: ");
    scanf("%d", &accNo);

    int index = findAccount(accNo);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }

    if (!verifyPIN(index)) return;

    for (int i = index; i < count - 1; i++) {
        accounts[i] = accounts[i + 1];
    }

    count--;
    saveAccounts();
    printf("Account deleted successfully!\n");
}

// View transactions
void viewTransactions() {
    FILE *fp = fopen("transactions.txt", "r");
    char line[100];

    if (fp == NULL) {
        printf("No transactions yet.\n");
        return;
    }

    printf("\n======= TRANSACTION HISTORY =======\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    printf("==================================\n");

    fclose(fp);
}

// Find account
int findAccount(int accNo) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].accNo == accNo)
            return i;
    }
    return -1;
}

// Verify PIN
int verifyPIN(int index) {
    int pin;
    printf("Enter PIN: ");
    scanf("%d", &pin);

    if (accounts[index].pin != pin) {
        printf("Incorrect PIN!\n");
        return 0;
    }
    return 1;
}

// Log transactions
void logTransaction(int accNo, char type[], float amt) {
    FILE *fp = fopen("transactions.txt", "a");
    fprintf(fp, "%d %s %.2f\n", accNo, type, amt);
    fclose(fp);
}