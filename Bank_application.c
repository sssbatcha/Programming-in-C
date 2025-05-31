#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "credit.dat"
#define MAX_RECORDS 100

// Structure definition
struct clientData {
    char acctNum[21];            // Changed from unsigned int to char[]
    char lastName[15];
    char firstName[10];
    double balance;
};

// Function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAllAccounts(FILE *fPtr);
void searchByLastName(FILE *fPtr);
void initializeFile();

// Main function
int main() {
    FILE *cfPtr;
    unsigned int choice;

    initializeFile();

    if ((cfPtr = fopen(FILE_NAME, "rb+")) == NULL) {
        printf("Error: File could not be opened.\n");
        exit(EXIT_FAILURE);
    }

    while ((choice = enterChoice()) != 7) {
        switch (choice) {
            case 1:
                textFile(cfPtr);
                break;
            case 2:
                updateRecord(cfPtr);
                break;
            case 3:
                newRecord(cfPtr);
                break;
            case 4:
                deleteRecord(cfPtr);
                break;
            case 5:
                displayAllAccounts(cfPtr);
                break;
            case 6:
                searchByLastName(cfPtr);
                break;
            default:
                puts("Invalid choice. Please try again.");
                break;
        }
    }

    fclose(cfPtr);
    return 0;
}

// Initialize binary file with blank records
void initializeFile() {
    FILE *fPtr = fopen(FILE_NAME, "rb");
    if (fPtr == NULL) {
        fPtr = fopen(FILE_NAME, "wb");
        if (fPtr == NULL) {
            puts("File could not be created.");
            exit(EXIT_FAILURE);
        }

        struct clientData blank = {"", "", "", 0.0};
        for (int i = 0; i < MAX_RECORDS; i++) {
            fwrite(&blank, sizeof(struct clientData), 1, fPtr);
        }

        puts("Initialized blank credit.dat file with 100 records.");
        fclose(fPtr);
    } else {
        fclose(fPtr);
    }
}

// Menu
unsigned int enterChoice(void) {
    unsigned int menuChoice;
    printf("\nEnter your choice\n"
           "1 - Export accounts to \"accounts.txt\"\n"
           "2 - Update an account\n"
           "3 - Add a new account\n"
           "4 - Delete an account\n"
           "5 - Display all accounts\n"
           "6 - Search by last name\n"
           "7 - Exit\n? ");
    scanf("%u", &menuChoice);
    return menuChoice;
}

// Create a text version of the accounts
void textFile(FILE *readPtr) {
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        puts("accounts.txt could not be opened.");
        return;
    }

    rewind(readPtr);
    fprintf(writePtr, "%-20s %-15s %-10s %10s\n", "AcctNum", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr)) {
        if (strlen(client.acctNum) != 0) {
            fprintf(writePtr, "%-20s %-15s %-10s %10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);
        }
    }

    fclose(writePtr);
    puts("Text file 'accounts.txt' created successfully.");
}

// Add a new account
void newRecord(FILE *fPtr) {
    struct clientData client = {"", "", "", 0.0};
    char inputAcct[21];
    int found = 0;

    printf("Enter new account number (up to 20 digits): ");
    scanf("%20s", inputAcct);

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (strcmp(client.acctNum, inputAcct) == 0) {
            found = 1;
            break;
        }
    }

    if (found) {
        puts("Account already exists.");
        return;
    }

    strcpy(client.acctNum, inputAcct);
    printf("Enter lastname, firstname, balance: ");
    scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance);

    // Write to first empty slot
    rewind(fPtr);
    for (int i = 0; i < MAX_RECORDS; i++) {
        long pos = ftell(fPtr);
        fread(&client, sizeof(struct clientData), 1, fPtr);
        if (strlen(client.acctNum) == 0) {
            fseek(fPtr, pos, SEEK_SET);
            strcpy(client.acctNum, inputAcct);
            printf("Enter lastname, firstname, balance again: ");
            scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance);
            fwrite(&client, sizeof(struct clientData), 1, fPtr);
            puts("Account created successfully.");
            return;
        }
    }

    puts("Database full. Cannot add new record.");
}

// Update existing account
void updateRecord(FILE *fPtr) {
    char inputAcct[21];
    double transaction;
    struct clientData client;
    int found = 0;

    printf("Enter account number to update: ");
    scanf("%20s", inputAcct);

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (strcmp(client.acctNum, inputAcct) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        puts("Account not found.");
        return;
    }

    printf("Current balance: %.2f\n", client.balance);
    printf("Enter charge (+) or payment (-): ");
    scanf("%lf", &transaction);
    client.balance += transaction;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);
    puts("Account updated successfully.");
}

// Delete account
void deleteRecord(FILE *fPtr) {
    char inputAcct[21];
    struct clientData client;
    struct clientData blank = {"", "", "", 0.0};
    int found = 0;

    printf("Enter account number to delete: ");
    scanf("%20s", inputAcct);

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (strcmp(client.acctNum, inputAcct) == 0) {
            found = 1;
            fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
            fwrite(&blank, sizeof(struct clientData), 1, fPtr);
            puts("Account deleted successfully.");
            break;
        }
    }

    if (!found) {
        puts("Account not found.");
    }
}

// Display all active accounts
void displayAllAccounts(FILE *fPtr) {
    struct clientData client;
    rewind(fPtr);

    printf("\n%-20s %-15s %-10s %10s\n", "AcctNum", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (strlen(client.acctNum) != 0) {
            printf("%-20s %-15s %-10s %10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);
        }
    }
}

// Search by last name
void searchByLastName(FILE *fPtr) {
    char searchName[15];
    struct clientData client;
    int found = 0;

    printf("Enter last name to search: ");
    scanf("%14s", searchName);

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (strcmp(client.lastName, searchName) == 0 && strlen(client.acctNum) != 0) {
            printf("\nAccount found:\n%-20s %-15s %-10s %10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);
            found = 1;
        }
    }

    if (!found) {
        printf("No account found with last name '%s'.\n", searchName);
    }
}
