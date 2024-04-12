#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType* next; 

};

// Fill out this structure
struct HashType
{
	struct RecordType* head;
};

// Compute the hash function
int hash(int key)
{
	const int hashSz = 23; 
    return key % hashSz;
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
// Display records in the hash structure
// Display records in the hash structure
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    struct RecordType* current;

    for (int i = 0; i < hashSz; ++i)
    {
        current = pHashArray[i].head;
        if (current != NULL)
        {
            printf("index %d -> ", i);
            while (current->next != NULL)
            {
                printf("%d, %c, %d -> ", current->id, current->name, current->order);
                current = current->next;
            }
            printf("%d, %c, %d\n", current->id, current->name, current->order);
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    struct HashType *pHashArray;
    int hashSz = 23; 

    recordSz = parseData("input.txt", &pRecords);

    pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    for (int i = 0; i < hashSz; ++i) {
        pHashArray[i].head = NULL; 
    }
    for (int i = 0; i < recordSz; ++i) {
        int index = hash(pRecords[i].id);
        struct RecordType* newNode = (struct RecordType*)malloc(sizeof(struct RecordType));
        *newNode = pRecords[i]; 
        newNode->next = pHashArray[index].head; 
        pHashArray[index].head = newNode;
    }

    printRecords(pRecords, recordSz); 
    displayRecordsInHash(pHashArray, hashSz); 

    for (int i = 0; i < hashSz; ++i) {
        struct RecordType* current = pHashArray[i].head;
        while (current != NULL) {
            struct RecordType* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pHashArray);
    free(pRecords);

    return 0;
}