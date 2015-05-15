/*******************************************************************************
/*  Title: Assignment 8 - display.c
/*  Author:  Stephen Vanderende
/*  Class COMP 2717
/*  Date: 03/13/12
/******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#define MAXCHAR 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APP_NAME "Assignment8"

typedef enum {ACCOUNT_NUMBER, ACCOUNT_NAME, BALANCE} IndexKey;

typedef union
{
	char sAccountNumber[ MAXCHAR ];
	char sAccountName[ MAXCHAR ];
	double dAccountBalance;
} KeyType;

typedef struct
{
	char sAccountNumber[MAXCHAR];
	char sAccountName[MAXCHAR];
	double dAccountBalance;
	double dLastPaymentAmount;
} Account;

typedef struct
{
	KeyType		key;
	long		filePos;
} IndexRecord;

typedef struct
{
	IndexKey	idxKey;	
	char		appName[ MAXCHAR ];
	int			recCount;
} IndexHeader;

IndexKey indexKey;

FILE *fpData;
FILE *fpIndexFile;

void checkCommandArguments( int iNumber );
void checkInputFile( char *sFileName );
void checkIndexFile ( char *sFileName );
void checkHeader ( void );
void displaySortedData( void );

long fileSize(FILE *input);


int main( int argc, char *argv[] )
{

	checkCommandArguments( argc );
	checkInputFile( argv[1] );
	checkIndexFile( "accountnameindex.idx" );
	checkHeader();
	displaySortedData();

	fclose( fpData );
	fclose( fpIndexFile );

	return 0;
}

// Check number of command line arguments
void checkCommandArguments( int iNumber )
{
	if ( iNumber < 2 )
	{
		printf("There are too few arguments.\n");
		exit( EXIT_FAILURE );
	}

	if ( iNumber > 2 )
	{
		printf("There are too many arguments.\n");
		exit( EXIT_FAILURE );
	}
}

// Check if the input file exists
void checkInputFile( char *sFileName )
{
	if( ( fpData = fopen(sFileName, "rb" ) ) == NULL )
	{
		printf("%s could not be opened.\n", sFileName);
		exit( EXIT_FAILURE );
	}
}

// Check if the index file exists
void checkIndexFile( char *sFileName )
{
	if( ( fpIndexFile = fopen(sFileName, "rb" ) ) == NULL )
	{
		printf("%s could not be opened.\n", sFileName);
		exit( EXIT_FAILURE );
	}
}

// Checks to see if the index header is the correct version and
// is not out of sync with data file
void checkHeader ( void )
{
	int iRecordCount;

	IndexHeader indexHeader;

	// read header record in index file
	fread(&indexHeader, sizeof( IndexHeader ), 1, fpIndexFile);

	// calculate record count in data file
	iRecordCount = fileSize(fpData) / sizeof( Account );

	// check that application versions match
	if (strcmp(indexHeader.appName, APP_NAME) != 0)
	{
		printf("Data File Format is Invalid\n");
		fclose(fpData);
		fclose(fpIndexFile);
		exit( EXIT_FAILURE );
	}

	// check that record count of data file matches index header
	if (indexHeader.recCount != iRecordCount)
	{
		printf("Record Count Mismatch\n");
		fclose(fpData);
		fclose(fpIndexFile);
		exit( EXIT_FAILURE );
	}

}

// Display the sorted data
void displaySortedData( void )
{
	int iReadCount;
	IndexRecord indexTemp;
	Account temp = { "", "", 0.0, 0.0 };

	printf("%10s%22s%18s%20s\n", "Account Number", "Account Name", "Account Balance", "Last Payment Amount");
	printf("--------------------------------------------------------------------------\n");

	// read first index record
	iReadCount = fread(&indexTemp, sizeof(IndexRecord), 1, fpIndexFile);
			
    // continue processing all index records
	while ((!feof(fpIndexFile)) && (iReadCount == 1))
	{
		// seek record in data file baased in file position
		// stored in previously read index record
		if (fseek(fpData, indexTemp.filePos, SEEK_SET) != 0)
		{
			printf("Seek Error\n");
			fclose(fpData);
			fclose(fpIndexFile);
			exit( EXIT_FAILURE );
		}

		// read data record and output
		fread(&temp, sizeof( Account ), 1, fpData);
		printf("%4s%32s%18.2f%20.2f\n", temp.sAccountNumber, temp.sAccountName, temp.dAccountBalance, temp.dLastPaymentAmount);

		// read next index record
		iReadCount = fread(&indexTemp, sizeof(IndexRecord), 1, fpIndexFile);		
	}
}

// Calculate the size of the file
long fileSize(FILE *input)
{
	long orgPos;
	long startPos;
	long endPos;

	orgPos = ftell(input);          // save orig file position
	rewind(input);
	startPos = ftell(input);
	fseek(input, 0, SEEK_END);
	endPos = ftell(input);
	fseek(input, orgPos, SEEK_SET); // restore orig position
	
	return(endPos - startPos);
}
