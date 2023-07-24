//Harjoitusprojekti 1: Warming up program: Reverse.c
//Sources:I have taken help from the C-programming guide especially with linked list structure and file handling: https://lutpub.lut.fi/bitstream/handle/10024/162908/Nikula2021-COhjelmointiopasV21.pdf?sequence=1&isAllowed=y
//Correct memory allocation and freeing for line is based on this stackOverFlow discussion: https://stackoverflow.com/questions/33116474/linked-lists-with-strings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linked_list{ 
    char * line;
    struct linked_list *pNext;  
} LINKED_LIST;  

//Introducing functions: 

LINKED_LIST *addToList(LINKED_LIST *, int, char *);
LINKED_LIST * readInputFileToList(char[]); 
void printInReverseOrder(LINKED_LIST *);
LINKED_LIST * writeReverseOrderToOutputFile(LINKED_LIST *, FILE *);
LINKED_LIST *freeTheMemory(LINKED_LIST *);


//Adding to the linked list is based on the C-programming guide
LINKED_LIST * addToList(LINKED_LIST *pStart, int lineLength, char *line) {
	LINKED_LIST *pNew, *ptr;
	//int memory; 
	//printf("%s", line); 
	if ((pNew = (LINKED_LIST*)malloc(sizeof(LINKED_LIST))) == NULL) {
		fprintf(stderr, "error: malloc failed");
		exit(1);
	}
	// Memory allocation for pNew->line is based on this stackoverflow discussion: https://stackoverflow.com/questions/33116474/linked-lists-with-strings
	pNew->line = NULL;
	
	if ((pNew->line = malloc(strlen(line)+1)) == NULL) {
		fprintf(stderr, "error: malloc failed");
		exit(1);
	}
	// Setting values for new list item
	strcpy(pNew->line, line);
    pNew->pNext = NULL;  
	// Checking whether the list is empty or not and proceeding accordingly
    if (pStart == NULL) { 
        pStart=pNew;    
    } else { 
		ptr = pStart; 
		while (ptr->pNext != NULL) 
			ptr=ptr->pNext;
		ptr->pNext = pNew;		
    }
	return pStart;   
}
//Function where the input file is read and stored to the linked list
LINKED_LIST * readInputFileToList(char inputFileName[30]) {
	FILE *inputFile; 
	LINKED_LIST *pStart = NULL;
	char *line=NULL; 
	size_t len;
	int readLineLength; 
	int lineCounter=1;
	if (inputFileName != NULL) {
		if ((inputFile = fopen(inputFileName, "r")) == NULL) {
				fprintf(stderr, "error: cannot open file '%s'\n", inputFileName);
				exit(1);
		} else {
			//How to use getline() function: https://c-for-dummies.com/blog/?p=5445 
			// Using getline() function: https://riptutorial.com/c/example/8274/get-lines-from-a-file-using-getline-- 
			do {
				readLineLength = getline(&line, &len, inputFile); 
				if (readLineLength != -1) {
					pStart=addToList(pStart, readLineLength, line);
				}
			} while ( readLineLength > 1);
		}
		fclose(inputFile);
	} else {
		//How to use getline() function: https://c-for-dummies.com/blog/?p=5445 
		do {
			readLineLength = getline(&line, &len, stdin);
			if (readLineLength != -1) {
					pStart=addToList(pStart, readLineLength, line);
					lineCounter++;  
				} 
	} while (readLineLength > 1);
	}
	free(line);
	return(pStart);
}
// This function for printing the elements of linked list in reverse order to terminal is based on this source: https://www.geeksforgeeks.org/print-reverse-of-a-linked-list-without-actually-reversing/ 
void printInReverseOrder(LINKED_LIST *pStart) {
	if (pStart == NULL) {
		return; 
	}
	printInReverseOrder(pStart->pNext);
	fprintf(stdout, "%s", pStart->line);
}
// This function for writing the elements of linked list in reverse order to output file is based on this source: https://www.geeksforgeeks.org/print-reverse-of-a-linked-list-without-actually-reversing/ Difference is using fprintf instead of printf
LINKED_LIST * writeReverseOrderToOutputFile(LINKED_LIST *pStart, FILE *output) { 
	if (pStart == NULL) {
		return pStart; 
	}
	writeReverseOrderToOutputFile(pStart->pNext, output);
	fprintf(output, "%s", pStart->line);
	return pStart; 
}
//This is used to free all allocated memory and is based on the C-programming guide by Uolevi Nikula (2021)
LINKED_LIST *freeTheMemory(LINKED_LIST *pStart) {
	LINKED_LIST *ptr = pStart;
	while (ptr != NULL) {
		pStart = ptr->pNext; 
		//Freeing the memory is based on this: https://stackoverflow.com/questions/33116474/linked-lists-with-strings
		free(ptr->line);
		free(ptr);
		ptr = pStart; 
	}
	return(pStart);
}
int main(int argc, char * argv[]) {  
	LINKED_LIST *pStart = NULL;
	char inputFileName[30];
	char outputFileName[30];
	//char *line; 
	FILE *outputFile;
	if (argc == 0) {
		printf("No command-line arguments given\n");
	}
	// reading from standard input and reversing it to standard output
	else if (argc == 1) {
		printf("Reversing standard input to standard output\n");
		pStart = readInputFileToList(NULL);
		printInReverseOrder(pStart);
	// reading the input file specified by the user and reversing it to standard output
	} else if (argc == 2) {
		printf("Reading input file %s and reversing to stdout\n", argv[1]);
		strcpy(inputFileName, argv[1]);
		pStart = readInputFileToList(inputFileName);
		printInReverseOrder(pStart);
	} else if (argc == 3) { // Reading the input file specified by the user and reversing it to output file
		//Checking if file names are the same is based on this website: https://www.programiz.com/c-programming/library-function/string.h/strcmp
		if (strcmp(argv[1], argv[2]) == 0) {
			fprintf(stderr, "Input and ouput file must differ\n"); 
			exit(1);
		} else {
			printf("Reading %s and reversing the content in the file %s\n", argv[1], argv[2]);
			strcpy(inputFileName, argv[1]);
			strcpy(outputFileName, argv[2]);
			if((outputFile = fopen(outputFileName, "w")) == NULL) {
				fprintf(stderr, "error: cannot open file '%s'\n", outputFileName);
			} else {
				pStart = readInputFileToList(inputFileName);
				writeReverseOrderToOutputFile(pStart, outputFile);
				//Closing the output file
				fclose(outputFile);
			}
		}
		//Just test case: 
		/*int read; 
		char *line; 
		size_t len;
		if((outputFile = fopen(outputFileName, "r")) == NULL) {
			fprintf(stderr, "error: cannot open file '%s'\n", outputFileName);
		} else {
			printf("Printing the content of %s file just for test\n", outputFileName);
			do {
				read = getline(&line, &len, outputFile); 
				if ( read != -1) {
					printf("%s", line);
				}
			} while (read > 1);
			//Closing the output file
			fclose(outputFile);
		}*/
	} else {
		printf("usage: reverse <input> <output>\n");
		exit(1);
	}
	// If there was stored anything to linked list, freeing that memory now
	if (pStart != NULL) {
		freeTheMemory(pStart);
	} 
	return(0);
}
