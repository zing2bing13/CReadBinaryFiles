# CReadBinaryFiles
C Program to read a CSV file, populates a struct and writes the struct to a binary file.

# Program 1 – create.c

Write a program that reads a CSV file, parses each line, populates an appropriately defined struct and writes the struct to a file in binary format (over writing the file if it exists).
Create your own CSV file with at least ten lines of data.
Each CSV file line contains four fields:
• AccountNumber
• AccountName
• AccountBalance
• LastPaymentAmount

# Program 2 – index.c

Write a program that reads the data file generated in Program 1 and creates an index file based on AccountName (Ascending)
Give the index file an appropriate name (with the extension .idx).
This filename may be hard coded within the program.
For simplicity, write the index file to the program’s current directory.
The data filename will be passed to the program via a command line argument.

# Program 3 – display.c

Write a program that reads the data and index file generated in Programs 1 and 2 then displays the data to the screen.
Display column headers and format your output in an appropriate manner.
The data filename will be passed to the program via a command line argument.
