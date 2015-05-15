/*******************************************************************************
/*  Title: Assignment 8 - index.c
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
int checkRecordCount( void );
void checkOutputFile( void );
void writeIndexHeader( int iDataRecordCount );
void writeIndexFile( int iDataRecordCount, long filePos );
long fileSize(FILE *input);

int compare(const void *left, const void *right);


int main(int argc, char *argv[])
{
	int	iDataRecordCount;
	long lFilePos;

	indexKey = ACCOUNT_NAME;

	checkCommandArguments( argc );
	checkInputFile( argv[1] );
	checkOutputFile();

	// calculate record count in data file
	iDataRecordCount = checkRecordCount();

	lFilePos = ftell(fpData);

	writeIndexHeader( iDataRecordCount );
	writeIndexFile( iDataRecordCount, lFilePos ) ;

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

// Check the number of records in the data file
int checkRecordCount( void )
{
	int iRecordCount = 0;
	iRecordCount = fileSize( fpData ) / sizeof(Account);
	rewind(fpData);

	return iRecordCount;
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

// Check if the output file exists
void checkOutputFile( void )
{
	if( ( fpIndexFile = fopen("accountnameindex.idx", "wb" ) ) == NULL )
	{
		printf("accountnameindex.idx could not be opened.\n");
		exit( EXIT_FAILURE );
	}
}
// Write the index header to file
void writeIndexHeader( int iDataRecordCount )
{
	IndexHeader	indexHeader;

	// populate index header record
	indexHeader.idxKey = indexKey;
	strcpy(indexHeader.appName, APP_NAME);
	indexHeader.recCount = iDataRecordCount;

	// write Index Header to file
	fwrite(&indexHeader, sizeof( IndexHeader ), 1, fpIndexFile);
}

// Write the indexes to file
void writeIndexFile( int iDataRecordCount, long filePos )
{
	int iCount;
	int iReadCount;
	int	iWriteCount;
	int iIndexRecordCount = 0;

	Account temp = {"","", 0.0, 0.0};
	IndexRecord	*indexRecords;

	// allocate an array to hold an equal number
	// of index records
	indexRecords = (IndexRecord *) calloc(iDataRecordCount, sizeof(IndexRecord));

	// Read first data record
	iReadCount = fread( &temp, sizeof( Account ), 1, fpData );

	// Process all data records
	while (!feof(fpData) && (iReadCount == 1))
	{
		// populate index record from data record
		strcpy( indexRecords[iIndexRecordCount].key.sAccountName, temp.sAccountName );
		indexRecords[iIndexRecordCount].filePos = filePos;

		// added a record - increment to next element
		iIndexRecordCount++;              

		// store file pos before next read advances it
		filePos = ftell(fpData);

		// read next record
		iReadCount = fread(&temp, sizeof( Account ), 1, fpData);
	}

	// Sort array of index records in memory
	// based on return value of compare function
	qsort(indexRecords, iIndexRecordCount, sizeof( IndexRecord ), compare);

	printf("Index Header Size: %d\nIndex Record Size: %d\n\n"
		                    , sizeof( IndexHeader ), sizeof( IndexRecord ) );

	printf("Creating Index File\n\n");	
	printf("Index sorted by Account Name:\n\n");
	for( iCount = 0; iCount < iIndexRecordCount; iCount++ )
	{
		printf("%s\n", indexRecords[iCount].key.sAccountName);
	}

	// Write Index Records to file
	iWriteCount = fwrite(indexRecords, sizeof( IndexRecord ), iIndexRecordCount, fpIndexFile);

	printf("\n%d Records Processed\n", iWriteCount);

	// release this memory and set it free
	free( indexRecords );
}

// calculates the size of a file
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

// function that qsort points to
int compare(const void *left, const void *right)
{
	// cast pointer parameters to IndexRecord
	IndexRecord *pLeft = (IndexRecord *) left;
	IndexRecord *pRight = (IndexRecord *) right;

	// additional logic (switch statement or if/else)
	//  may be required here for multiple keys

	return strcmp((pLeft->key.sAccountName), (pRight->key.sAccountName));
}
