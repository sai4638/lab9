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
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	struct RecordType* current;

	int i;

	for (i=0;i<hashSz;++i)
	{
		current = pHashArray[i].head;

        // Only print if the index has records
        if (current != NULL)
        {
            printf("index %d -> ", i);
            // Store records in an array for reverse printing
            struct RecordType* records[1000]; // Assuming the maximum size of records won't exceed 1000
            int count = 0;
            while (current != NULL)
            {
                records[count++] = current;
                current = current->next;
            }
            // Print records in reverse order
            for (int j = count - 1; j >= 0; --j)
            {
                printf("%d, %c, %d", records[j]->id, records[j]->name, records[j]->order);
                if (j > 0)
                    printf(" -> ");
            }
            printf("\n");
        }
	}
}

int main(void)
{
	struct RecordType *pRecords;
    int recordSz = 0;
    struct HashType *pHashArray;
    int hashSz = 23; // Hash table size

    recordSz = parseData("input.txt", &pRecords);

    pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    for (int i = 0; i < hashSz; ++i) {
        pHashArray[i].head = NULL; // Initialize hash table
    }
    for (int i = 0; i < recordSz; ++i) {
        int index = hash(pRecords[i].id);
        struct RecordType* newNode = (struct RecordType*)malloc(sizeof(struct RecordType));
        *newNode = pRecords[i]; // copy data
        newNode->next = pHashArray[index].head; // Insert at the head
        pHashArray[index].head = newNode;
    }
    displayRecordsInHash(pHashArray, hashSz);

    // Free resources
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
