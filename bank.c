#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERNAME_LEN 50
#define MAX_PIN_LEN 10

char currentUser[MAX_USERNAME_LEN];
float balance = 0.0;

int loginUser(void);
int checkCredentials(const char *username, const char *pin);
void loadBalance(void);
void saveBalance(void);
void optionList(void);
void viewBalance(void);
void depositMoney(void);
void withdrawMoney(void);
void exitProgram(void);

int main() {
    printf("Welcome to the Multi-User ATM System!\n");

    if (!loginUser()) {
        printf("Too many failed login attempts. Exiting...\n");
        return 1;
    }

    loadBalance();
    optionList();

    return 0;
}

// Login function: Asks for username and PIN, checks in users.txt
int loginUser(void) {
    char username[MAX_USERNAME_LEN];
    char pin[MAX_PIN_LEN];
    int attempts = 0;

    while (attempts < 3) {
        printf("Enter username: ");
        scanf("%s", username);

        printf("Enter PIN: ");
        scanf("%s", pin);

        if (checkCredentials(username, pin)) {
            strcpy(currentUser, username);
            printf("Login successful! Welcome, %s.\n", currentUser);
            return 1;
        } else {
            printf("Invalid username or PIN. Try again.\n");
            attempts++;
        }
    }
    return 0;
}

// Check username and PIN against users.txt
int checkCredentials(const char *username, const char *pin) {
    FILE *file = fopen("users.txt", "r");
    if (!file) {
        printf("Error opening users.txt file.\n");
        return 0;
    }

    char fileUser[MAX_USERNAME_LEN];
    char filePin[MAX_PIN_LEN];

    while (fscanf(file, "%s %s", fileUser, filePin) == 2) {
         printf("Read from file: user='%s', pin='%s'\n", fileUser, filePin);
        if (strcmp(username, fileUser) == 0 && strcmp(pin, filePin) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Load balance from user-specific file
void loadBalance(void) {
    char filename[100];
    sprintf(filename, "balance_%s.txt", currentUser);

    FILE *file = fopen(filename, "r");
    if (file) {
        fscanf(file, "%f", &balance);
        fclose(file);
    } else {
        // If no balance file, start with 0 balance
        balance = 0.0;
        // Optionally create the file now
        saveBalance();
    }
}

// Save balance to user-specific file
void saveBalance(void) {
    char filename[100];
    sprintf(filename, "balance_%s.txt", currentUser);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error saving balance file.\n");
        return;
    }

    fprintf(file, "%.2f", balance);
    fclose(file);
}

void optionList(void) {
    int option;

    while (1) {
        printf("\n======= ATM MENU (%s) =======\n", currentUser);
        printf("1. View Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Exit\n");
        printf("==============================\n");
        printf("Choose an option: ");

        scanf("%d", &option);

        switch (option) {
            case 1: viewBalance(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: exitProgram(); return;
            default: printf("Invalid option. Please try again.\n");
        }
    }
}

void viewBalance(void) {
    printf("Current balance for %s: $%.2f\n", currentUser, balance);
}

void depositMoney(void) {
    float amount;

    do {
        printf("Enter deposit amount: ");
        scanf("%f", &amount);
        if (amount <= 0) {
            printf("Amount must be greater than 0.\n");
        }
    } while (amount <= 0);

    balance += amount;
    saveBalance();

    printf("Deposited $%.2f. New balance: $%.2f\n", amount, balance);
}

void withdrawMoney(void) {
    float amount;

    do {
        printf("Enter withdrawal amount: ");
        scanf("%f", &amount);
        if (amount <= 0) {
            printf("Amount must be greater than 0.\n");
        } else if (amount > balance) {
            printf("Insufficient funds. Try again.\n");
        }
    } while (amount <= 0 || amount > balance);

    balance -= amount;
    saveBalance();

    printf("Withdrew $%.2f. New balance: $%.2f\n", amount, balance);
}

void exitProgram(void) {
    printf("Session ended. Have a good day, %s!\n", currentUser);
}
