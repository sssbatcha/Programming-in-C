Bank Account Management System
================================
Description:
------------
This is a simple C-based account management system that uses binary file operations
to store and manage client credit data. It allows users to add, update, delete,
search, and view account records stored in a binary file.

The system supports up to 100 client records stored in a binary file named "credit.dat".

Each account contains:
- Account Number (up to 20 characters)
- Last Name (up to 14 characters)
- First Name (up to 9 characters)
- Balance (double)

Compilation:
------------
To compile the program, use a C compiler like `gcc`:

    gcc credit.c -o credit

Running the Program:
--------------------
After compilation, run the program:

    ./credit   (on Linux/Mac)
    credit.exe (on Windows)

On the first run, the program will create and initialize a binary file `credit.dat`
with 100 blank records.

Menu Options:
-------------
1 - Export accounts to "accounts.txt":
    Generates a human-readable text file listing all active (non-empty) accounts.

2 - Update an account:
    Allows the user to charge or credit an existing account by modifying its balance.

3 - Add a new account:
    Adds a new account to the binary file in the first available empty slot.

4 - Delete an account:
    Deletes an account by blanking out the corresponding record.

5 - Display all accounts:
    Displays all active accounts currently stored in the binary file.

6 - Search by last name:
    Searches and displays accounts that match a given last name.

7 - Exit:
    Closes the program safely.

Notes:
------
- All input is performed via the terminal.
- Accounts are uniquely identified by their account number.
- Blank records are represented by an empty account number string.

File Descriptions:
------------------
- credit.dat : Binary file storing client records.
- accounts.txt : Text file generated for readable output of all active accounts.

Limitations:
------------
- Maximum of 100 accounts.
- No data encryption or password protection.
- Input validation is minimal (e.g., no checks for non-numeric balances).

License:
--------
This project is for educational purposes. Modify and use freely.

