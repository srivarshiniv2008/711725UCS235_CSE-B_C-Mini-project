// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.

#include <stdio.h>
#include <stdlib.h>

// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance

    int status;           
    char dob[15];         
    char address[50];     
    char phone[15];       

}; // end structure clientData

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void accountDetails(FILE *fPtr);

int main(int argc, char *argv[])
{
    FILE *cfPtr;         
    unsigned int choice; 

    // fopen opens the file; exits if file cannot be opened
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(-1);
    }

    // enable user to specify action
    while ((choice = enterChoice()) != 9)
    {
        switch (choice)
        {
        // create text file from record file
        case 1:
            textFile(cfPtr);
            break;

        // update record
        case 2:
            updateRecord(cfPtr);
            break;

        // create record
        case 3:
            newRecord(cfPtr);
            break;

        // delete existing record
        case 4:
            deleteRecord(cfPtr);
            break;

        // display account details
        case 5:
            accountDetails(cfPtr);
            break;

        default:
            puts("Incorrect choice");
            break;

        } // end switch
    }     // end while

    fclose(cfPtr); // fclose closes the file
} // end main


// create formatted text file for printing
void textFile(FILE *readPtr)
{
    FILE *writePtr; 
    int result;     

    struct clientData client = {0, "", "", 0.0, 0, "", "", ""};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "%-6s%-16s%-11s%-12s%-15s%-15s%-10s%10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "DOB",
                "Address",
                "Phone",
                "Status",
                "Balance");

        while (!feof(readPtr))
        {
            result = fread(&client, sizeof(struct clientData), 1, readPtr);

            if (result != 0 && client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-6d%-16s%-11s%-12s%-15s%-15s%-10d%10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.dob,
                        client.address,
                        client.phone,
                        client.status,
                        client.balance);
            }
        }

        fclose(writePtr);
    }
}


// update balance in record
void updateRecord(FILE *fPtr)
{
    unsigned int account; 
    double transaction;   

    struct clientData client = {0, "", "", 0.0, 0, "", "", ""};

    printf("%s", "Enter account to update ( 1 - 100 ): ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account #%d has no information.\n", account);
    }
    else
    {
        printf("%-6d%-16s%-11s%10.2f\n\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        printf("%s", "Enter charge ( + ) or payment ( - ): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        printf("%-6d%-16s%-11s%10.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);
    }
}


// delete an existing record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0, 0, "", "", ""};

    unsigned int accountNum;

    printf("%s", "Enter account number to delete ( 1 - 100 ): ");
    scanf("%d", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account %d does not exist.\n", accountNum);
    }
    else
    {
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

        printf("Account deleted successfully.\n");
    }
}


// create and insert record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0, 0, "", "", ""};

    unsigned int accountNum;

    printf("%s", "Enter new account number ( 1 - 100 ): ");
    scanf("%d", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account #%d already contains information.\n",
               client.acctNum);
    }
    else
    {
        printf("Enter lastname firstname balance status dob address phone\n");

        scanf("%14s %9s %lf %d %14s %49s %14s",
              client.lastName,
              client.firstName,
              &client.balance,
              &client.status,
              client.dob,
              client.address,
              client.phone);

        client.acctNum = accountNum;

        fseek(fPtr,
              (client.acctNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Account added successfully.\n");
    }
}


// display full account details
void accountDetails(FILE *fPtr)
{
    unsigned int account;

    struct clientData client = {0, "", "", 0.0, 0, "", "", ""};

    printf("Enter account number (1 - 100): ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("No record found.\n");
    }
    else
    {
        printf("\nAccount Number : %u\n", client.acctNum);
        printf("First Name     : %s\n", client.firstName);
        printf("Last Name      : %s\n", client.lastName);
        printf("Balance        : %.2lf\n", client.balance);
        printf("Status         : %d\n", client.status);
        printf("DOB            : %s\n", client.dob);
        printf("Address        : %s\n", client.address);
        printf("Phone Number   : %s\n", client.phone);
    }
}


// enable user to input menu choice
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("%s",
           "\nEnter your choice\n"
           "1 - store formatted text file\n"
           "2 - update an account\n"
           "3 - add a new account\n"
           "4 - delete an account\n"
           "5 - display account details\n"
           "9 - end program\n? ");

    scanf("%u", &menuChoice);

    return menuChoice;
}