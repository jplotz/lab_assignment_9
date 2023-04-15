#include <stdio.h>
#include <stdlib.h>

// use a hash table size of 11 as discussed in lecture
#define HASHSIZE 11

// This has to be turned into a linked list to implement separate chaining
struct RecordType
{
	int		id;
	char	name;
	int		order; 
    struct RecordType *next;
};

struct HashType
{
    struct RecordType *data;
};

int hash(int x)
{
    return x % HASHSIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
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

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

    printf("\nHash Records:\n");

	for (i=0;i<hashSz;++i)
	{
        printf("\tindex %d", i);
        if (pHashArray[i].data != NULL)
        {
            struct RecordType *temp = pHashArray[i].data;
            while (temp)
            {
                printf(" -> %d, %c, %d", temp->id, temp->name, temp->order);
                temp = temp->next;
            }
        }
        printf(" -> NULL\n");
	}
}

struct HashType *createHashArray(struct RecordType *plainRecords, int plainRecordsSize)
{
    int idx;
    struct HashType *arr = malloc(sizeof(struct HashType) * HASHSIZE);
    struct RecordType *temp;

    for (int i = 0; i < HASHSIZE; i++)
        arr[i].data = NULL;

    for (int i = 0; i < plainRecordsSize; i++)
    {
        idx = hash(plainRecords[i].id);
        // linked list code
        temp = arr[idx].data;
        // there is no linked list
        if (temp == NULL)
        {
            arr[idx].data = &plainRecords[i];
            temp = arr[idx].data;
        }
        // there is a linked list, append to the end of it (could add to front, it's implementation dependent I believe
        else
        {
            while (temp->next)
                temp = temp->next;
            temp->next = &plainRecords[i];
        }
    }
    return arr;
}

int main(void)
{
	struct RecordType *pRecords;
    struct HashType *hashRecords;

	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

    hashRecords = createHashArray(pRecords, recordSz);
    displayRecordsInHash(hashRecords, HASHSIZE);

    free(pRecords);
    free(hashRecords);
}
