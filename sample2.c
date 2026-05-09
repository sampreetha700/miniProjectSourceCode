#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Structure for Account
typedef struct {
    int accNo;
    char name[50];
    float balance;
} Account;

Account accounts[MAX];
int count = 0;

// Function Prototypes
void loadAccounts();
void saveAccounts();
void addAccount();
void deposit();
void withdraw();
void searchAccount();
void listAccounts();
int findAccount(int);

// Main Function
int main() {
    int choice;

    loadAccounts();

    while (1) {
        printf("\n===== Transaction Processing System =====\n");
        printf("1. Add Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Search Account\n");
        printf("5. List All Accounts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: searchAccount(); break;
            case 5: listAccounts(); break;
            case 6: 
                saveAccounts();
                printf("Data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Load accounts from file
void loadAccounts() {
    FILE *fp = fopen("accounts.txt", "r");
    if (fp == NULL) return;

    while (fscanf(fp, "%d %s %f", &accounts[count].accNo,
                  accounts[count].name,
                  &accounts[count].balance) != EOF) {
        count++;
    }
    fclose(fp);
}

// Save accounts to file
void saveAccounts() {
    FILE *fp = fopen("accounts.txt", "w");
    if (fp == NULL) {
        printf("Error saving file!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %.2f\n",
                accounts[i].accNo,
                accounts[i].name,
                accounts[i].balance);
    }
    fclose(fp);
}

// Add new account
void addAccount() {
    if (count >= MAX) {
        printf("Account limit reached!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &accounts[count].accNo);

    if (findAccount(accounts[count].accNo) != -1) {
        printf("Account already exists!\n");
        return;
    }

    printf("Enter Name: ");
    scanf("%s", accounts[count].name);

    printf("Enter Initial Balance: ");
    scanf("%f", &accounts[count].balance);

    if (accounts[count].balance < 0) {
        printf("Balance cannot be negative!\n");
        return;
    }

    count++;
    saveAccounts();
    printf("Account added successfully!\n");
}

// Deposit
void deposit() {
    int accNo;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    int index = findAccount(accNo);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid deposit amount!\n");
        return;
    }

    accounts[index].balance += amount;
    saveAccounts();
    printf("Deposit successful!\n");
}

// Withdraw
void withdraw() {
    int accNo;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    int index = findAccount(accNo);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid withdrawal amount!\n");
        return;
    }

    if (accounts[index].balance < amount) {
        printf("Insufficient balance!\n");
        return;
    }

    accounts[index].balance -= amount;
    saveAccounts();
    printf("Withdrawal successful!\n");
}

// Search Account
void searchAccount() {
    int accNo;
    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    int index = findAccount(accNo);

    if (index == -1) {
        printf("Account not found!\n");
    } else {
        printf("\nAccount Found:\n");
        printf("Account No: %d\n", accounts[index].accNo);
        printf("Name: %s\n", accounts[index].name);
        printf("Balance: %.2f\n", accounts[index].balance);
    }
}

// List all accounts
void listAccounts() {
    if (count == 0) {
        printf("No accounts available.\n");
        return;
    }

    printf("\n---- All Accounts ----\n");
    for (int i = 0; i < count; i++) {
        printf("AccNo: %d | Name: %s | Balance: %.2f\n",
               accounts[i].accNo,
               accounts[i].name,
               accounts[i].balance);
    }
}

// Find account index
int findAccount(int accNo) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].accNo == accNo)
            return i;
    }
    return -1;
}