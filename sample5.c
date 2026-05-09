#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define MIN_BAL 500

// Colors
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

typedef struct {
    int accNo;
    char name[50];
    float balance;
    int pin;
} Account;

Account accounts[MAX];
int count = 0;

// Function declarations
void loadAccounts();
void saveAccounts();
void addAccount();
void deposit();
void withdraw();
void transferMoney();   // NEW
void listAccounts();
void searchAccount();
void deleteAccount();
void viewTransactions();
int findAccount(int);
int verifyPIN(int);
void logTransaction(int, char[], float);

// MAIN
int main() {
    int choice;
    loadAccounts();

    while (1) {
        printf("\n=====================================\n");
        printf("     BANK MANAGEMENT SYSTEM\n");
        printf("=====================================\n");
        printf("1. Add Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Search Account\n");
        printf("5. List Accounts\n");
        printf("6. Delete Account\n");
        printf("7. View Transactions\n");
        printf("8. Exit\n");
        printf("9. Transfer Money\n");  // NEW
        printf("=====================================\n");
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
                printf(GREEN "Saved & Exiting...\n" RESET);
                exit(0);
            case 9: transferMoney(); break;  // NEW
            default:
                printf(RED "Invalid choice!\n" RESET);
        }
    }
}

// Load accounts
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
        printf(RED "Account already exists!\n" RESET);
        return;
    }

    printf("Enter Name: ");
    scanf("%s", accounts[count].name);

    printf("Enter Balance: ");
    scanf("%f", &accounts[count].balance);

    printf("Set PIN: ");
    scanf("%d", &accounts[count].pin);

    count++;
    saveAccounts();
    printf(GREEN "Account Created Successfully!\n" RESET);
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
        printf(RED "Account not found!\n" RESET);
        return;
    }

    if (!verifyPIN(index)) return;

    printf("Enter Amount: ");
    scanf("%f", &amt);

    float old = accounts[index].balance;
    accounts[index].balance += amt;

    time_t t;
    time(&t);

    printf(GREEN "\n===== DEPOSIT SUCCESS =====\n" RESET);
    printf("Account No   : %d\n", accNo);
    printf("Old Balance  : %.2f\n", old);
    printf("Deposited    : +%.2f\n", amt);
    printf("New Balance  : %.2f\n", accounts[index].balance);
    printf("Time         : %s", ctime(&t));
    printf("===========================\n");

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
        printf(RED "Account not found!\n" RESET);
        return;
    }

    if (!verifyPIN(index)) return;

    printf("Enter Amount: ");
    scanf("%f", &amt);

    if (accounts[index].balance - amt < MIN_BAL) {
        printf(RED "Minimum balance required!\n" RESET);
        return;
    }

    float old = accounts[index].balance;
    accounts[index].balance -= amt;

    time_t t;
    time(&t);

    printf(GREEN "\n===== WITHDRAW SUCCESS =====\n" RESET);
    printf("Account No   : %d\n", accNo);
    printf("Old Balance  : %.2f\n", old);
    printf("Withdrawn    : -%.2f\n", amt);
    printf("New Balance  : %.2f\n", accounts[index].balance);
    printf("Time         : %s", ctime(&t));
    printf("============================\n");

    logTransaction(accNo, "Withdraw", amt);
    saveAccounts();
}

// 🔥 NEW FEATURE: Transfer Money
void transferMoney() {
    int fromAcc, toAcc;
    float amt;

    printf("\n===== TRANSFER MONEY =====\n");

    printf("From Account No: ");
    scanf("%d", &fromAcc);

    int fromIndex = findAccount(fromAcc);
    if (fromIndex == -1) {
        printf(RED "Sender account not found!\n" RESET);
        return;
    }

    if (!verifyPIN(fromIndex)) return;

    printf("To Account No: ");
    scanf("%d", &toAcc);

    int toIndex = findAccount(toAcc);
    if (toIndex == -1) {
        printf(RED "Receiver account not found!\n" RESET);
        return;
    }

    printf("Enter Amount to Transfer: ");
    scanf("%f", &amt);

    if (accounts[fromIndex].balance - amt < MIN_BAL) {
        printf(RED "Minimum balance must be maintained!\n" RESET);
        return;
    }

    float oldFrom = accounts[fromIndex].balance;
    float oldTo = accounts[toIndex].balance;

    accounts[fromIndex].balance -= amt;
    accounts[toIndex].balance += amt;

    time_t t;
    time(&t);

    printf(GREEN "\n===== TRANSFER SUCCESS =====\n" RESET);
    printf("From Account : %d\n", fromAcc);
    printf("To Account   : %d\n", toAcc);
    printf("Amount       : %.2f\n", amt);

    printf("\n--- Sender ---\n");
    printf("Old Balance  : %.2f\n", oldFrom);
    printf("New Balance  : %.2f\n", accounts[fromIndex].balance);

    printf("\n--- Receiver ---\n");
    printf("Old Balance  : %.2f\n", oldTo);
    printf("New Balance  : %.2f\n", accounts[toIndex].balance);

    printf("Time         : %s", ctime(&t));
    printf("============================\n");

    logTransaction(fromAcc, "TransferOut", amt);
    logTransaction(toAcc, "TransferIn", amt);

    saveAccounts();
}

// List accounts
void listAccounts() {
    printf("\n=====================================\n");
    printf("AccNo   Name       Balance\n");
    printf("=====================================\n");

    for (int i = 0; i < count; i++) {
        printf("%-8d %-10s %-10.2f\n",
            accounts[i].accNo,
            accounts[i].name,
            accounts[i].balance);
    }

    printf("=====================================\n");
}

// Search
void searchAccount() {
    int accNo;
    printf("Enter Account No: ");
    scanf("%d", &accNo);

    int i = findAccount(accNo);

    if (i == -1) {
        printf(RED "Not Found!\n" RESET);
        return;
    }

    printf("\n===== ACCOUNT DETAILS =====\n");
    printf("AccNo   : %d\n", accounts[i].accNo);
    printf("Name    : %s\n", accounts[i].name);
    printf("Balance : %.2f\n", accounts[i].balance);
    printf("===========================\n");
}

// Delete account
void deleteAccount() {
    int accNo;
    printf("Enter Account No: ");
    scanf("%d", &accNo);

    int i = findAccount(accNo);
    if (i == -1) {
        printf(RED "Not Found!\n" RESET);
        return;
    }

    if (!verifyPIN(i)) return;

    for (int j = i; j < count - 1; j++) {
        accounts[j] = accounts[j + 1];
    }

    count--;
    saveAccounts();
    printf(GREEN "Account Deleted!\n" RESET);
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
        printf(RED "Incorrect PIN!\n" RESET);
        return 0;
    }
    return 1;
}

// Log transaction
void logTransaction(int accNo, char type[], float amt) {
    FILE *fp = fopen("transactions.txt", "a");
    fprintf(fp, "%d %s %.2f\n", accNo, type, amt);
    fclose(fp);
}