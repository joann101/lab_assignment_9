#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType *next;
    // create a next pointer (for linkedlist structure)
};

// Fill out this structure
struct HashType
{
    // create a pointer to a RecordType
    struct RecordType** RT;
};

// Compute the hash function
int hash(int x, int tableSize)
{
    // hash function is x % tableSize
    return x%tableSize;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
    // call the hash function to get the index
    int index = hash(record->id,tableSize);
    
    struct RecordType* newNode = malloc(sizeof(struct RecordType));
    newNode->id=record->id;
    newNode->name=record->name;
    newNode->order=record->order;
    newNode->next=NULL;

    // if the RecordType at that index is NULL
    // set 'record' equal to the HashType pointer in the table at index
    if (hashTable->RT[index] == NULL) 
    {
        hashTable->RT[index]= newNode;
    }

    // else
    // traverse to the end of the linkedlist and add 'record' to the end of it
    else 
    {
        struct HashType *tmp = hashTable;
    
        newNode->next = hashTable->RT[index];
        hashTable->RT[index] = newNode;
    }
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable, int tableSize)
{
    struct HashType *tmp = hashTable;
    // for each entry in the table
    // print the contents in that index
    // The output should look like this (for example): "Index 1 -> 21715, Q, 16 -> 28876, a, 26 -> NULL"
    for(int i = 0; i<tableSize; i++)
    {
        printf("Index %d",i);
        while(tmp->RT[i] != NULL)
        {
            printf(" -> %d, %c, %d",tmp->RT[i]->id,tmp->RT[i]->name,tmp->RT[i]->order);
            tmp->RT[i]=tmp->RT[i]->next;
        }
        printf("\n");
    }
        
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize the hash table
    struct HashType *phashTable= malloc(sizeof(struct HashType));

    // create a variable hashTableSize and assign it a value
    int hashTableSize = 11;
    
    // initialize a hashTable, use calloc (so everything is assigned to NULL)
    phashTable->RT = calloc(hashTableSize,sizeof(struct RecordType));
    
    // for each record in pRecords, insert it into the hash table using the insertRecord function
    for(int i =0; i<recordSz; i++)
    {
       insertRecord(phashTable, pRecords, hashTableSize);
       pRecords++;
    }

    // call the display records function
    displayRecordsInHash(phashTable, hashTableSize);

    // free all the allocated memory
    free(pRecords);
    free(phashTable);

    return 0;
}