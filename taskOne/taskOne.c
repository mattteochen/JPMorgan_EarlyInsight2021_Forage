#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//I assume 100 is the max length of a word in the sanction list
#define MAX 100

typedef struct nodeOne{
	int val;
	struct nodeOne *next;
} One;

typedef struct nodeWord{
	char word[MAX];
	struct nodeWord *next;
} Word;

void addListWord(Word **head, Word **tail, char word[]);
void modifyWord(char t[]);
bool searchIfThere(One **head, int length);
void addListOne(One **head, One **tail, int length, int *size);
void addToMap(Word *map[], char word[], int size);
void throwError();
void testPrintOne(One *head);
void testPrintWord(Word *head);
void testPrintMap(Word *map[], int size);
bool comparison(Word *map[], char input[], int size);
bool compare(char testOne[], char testTwo[], double *percentage);
void freeWord(Word *head);
void freeOne(One *head);

int main(int argc, char **argv){
	if (argc < 2)
		return -1;
	/*
	 * first of all I upload the file that cointains the sanction list in a optimized data structure as the following:
	 * an array of liked list where every list on an index of the array 
	 * group every sanction word by lenght, so when we will search for a hit this process 
	 * could speed up our time complexity
	*/
	FILE *sanction = fopen(argv[1], "r");
	/*
	FILE *wordList = fopen(argv[2], "r");

	if (wordList == NULL){
		printf ("File error\n");
		return -1;
	}
	*/
	if (sanction == NULL){
		printf ("File error\n");
		return -1;
	}

	int numberOfName = 0, length = 0, listOneSize = 0;
	char temporary[MAX];
	One *headOne = NULL;
	One *tailOne = NULL;
	Word *headWord = NULL;
	Word *tailWord = NULL;

	//count how many names are there in the list and set up list where I save the lenght of all the words
	if (fgets(temporary, MAX, sanction) != NULL){
		//the function modifyWord  deletes the '\n' cHaracter in the string if is there
		modifyWord(temporary);
		length = strlen(temporary);
		addListOne(&headOne, &tailOne, length, &listOneSize);
		//at the same time I save our words for later in a list
		addListWord(&headWord, &tailWord, temporary);
		while (!feof(sanction)){
			fgets(temporary, MAX, sanction);
			modifyWord(temporary);
			length = strlen(temporary);
			addListOne(&headOne, &tailOne, length, &listOneSize);
			addListWord(&headWord, &tailWord, temporary);
		}
	}
	fclose(sanction);
	/*
	Word *headWordList = NULL;
	Word *tailWordList = NULL;
	if (fgets(temporary, MAX, wordList) != NULL){
		//the function modifyWord  deletes the '\n' cHaracter in the string if is there
		modifyWord(temporary);
		addListWord(&headWordList, &tailWordList, temporary);
		while (!feof(wordList)){
			fgets(temporary, MAX, wordList);
			modifyWord(temporary);
			addListWord(&headWordList, &tailWordList, temporary);
		}
	}
	fclose(wordList);
	*/
	//test print
	//testPrintOne(headOne);
	//testPrintWord(headWord);
	//testPrintWord(headWordList);
	//end test print

	//now I have the size of the array that I need to allocate
	Word *map[listOneSize];
	for (int i=0; i<listOneSize; i++)
		map[i] = NULL;

	//so now we can save our words in the structure
	Word *current = headWord;
	while (current != NULL){
		addToMap(map, current->word, listOneSize);
		current = current->next;
	}

	//test print map
	//testPrintMap(map, listOneSize);

	//now we are ready to do the comparison
	char input[MAX];
	printf ("insert the word:");
	fflush(stdin);
	gets(input);

	//for the comparison I assume a 75% match
	if (comparison(map, input, listOneSize) == true)
		printf ("HIT\n");
	else
		printf ("NOT HIT\n");

	//uncomment bewlow if you want to matrch the wordList.txt file words
	/*
	Word *point = headWordList;
	while (point != NULL){
		printf ("checking: %s\n", point->word);
		if (comparison(map, point->word, listOneSize) == true)
			printf ("HIT\n");
		else
			printf ("NOT HIT\n");
		point = point->next;
	}
	*/

	freeOne(headOne);
	freeWord(headWord);
	//free(headWordList);
	for (int i=0; i<listOneSize; i++)
		freeWord(map[i]);
	return 0;
}

void freeOne(One *head){
	One *current = head, *previous = NULL;

	if (head == NULL)
		return;
	
	while (current != NULL){
		if (previous != NULL)
			free(previous);
		previous = current;
		current = current->next;
	}
	free(previous);
}

void freeWord(Word *head){
	Word *current = head, *previous = NULL;

	if (head == NULL)
		return;
	
	while (current != NULL){
		if (previous != NULL)
			free(previous);
		previous = current;
		current = current->next;
	}
	free(previous);
}

bool compare(char testOne[], char testTwo[], double *percentage){
	double match = 0, total = strlen(testOne);

	for (int i=0; i<strlen(testOne); i++){
		if (testOne[i] == testTwo[i])
			match++;
	}

	(*percentage) = (match*100)/total;
	//printf ("percetange: %.2f\n", (*percentage));
	if ((*percentage) >= 75)
		return true;
	return false;
}

bool comparison(Word *map[], char input[], int size){
	double percentage = 0;

	for (int i=0; i<size; i++){
		if (map[i] != NULL){
			if (strlen(map[i]->word) == strlen(input)){
				Word *current = map[i];
				while (current != NULL){
					if (compare(current->word, input, &percentage) == true){
						return true;
					}
					current = current->next;
				}
				return false;
			}
		}
	}
	return false;
}

void testPrintMap(Word *map[], int size){
	for (int i=0; i<size; i++){
		Word *c = map[i];
		while (c != NULL){
			printf ("%s - ", c->word);
			c = c->next;
		}
		printf ("\n");
	}
}

void testPrintOne(One *head){
	One *c = head;
	while (c != NULL){
		printf ("%d\n", c->val);
		c = c->next;
	}
}

void testPrintWord(Word *head){
	Word *c = head;
	while (c != NULL){
		printf ("%s\n", c->word);
		c = c->next;
	}
}

void throwError(){
	printf ("allocation error\n");
}

void addToMap(Word *map[], char word[], int size){
	for (int i=0; i<size; i++){
		if (map[i] != NULL){
			if (strlen(map[i]->word) == strlen(word)){
				//we add in this list, where other words have the same length
				Word *temp = map[i];
				Word *newNode = (Word*)malloc(sizeof(Word));
			
				if (newNode == NULL){
					throwError();
					return;
				}
				strcpy(newNode->word, word);
				newNode->next = NULL;
				map[i] = newNode;
				newNode->next = temp;
				return;
			}
		}
		//if the else statement is going in this is the first word with this length, so there will be a new list which will be saved in the fisrt free position in the map
		else if (map[i] == NULL){
			map[i] = (Word*)malloc(sizeof(Word));

			if (map[i] == NULL){
				throwError();
				return;
			}
			strcpy(map[i]->word, word);
			map[i]->next = NULL;
			return;
		}
	}
}

void addListWord(Word **head, Word **tail, char word[]){
	Word *newWord = (Word*)malloc(sizeof(Word));
	
	if (newWord == NULL){
		throwError();
		return;
	}
	newWord->next = NULL;
	strcpy(newWord->word, word);

	if (*head == NULL){
		*head = newWord;
		*tail = newWord;
	}
	else{
		(*tail)->next = newWord;
		*tail = newWord;
	}
}

void modifyWord(char t[]){
	int i = 0;
	
	if (strlen(t) == 0){
		printf ("error! word not valid\n");
		return;
	}

	while (i < strlen(t)){
		//delete the new line character
		if (t[i] == 10){
			t[i] = '\0';
			return;
		}
		i++;
	}
}

bool searchIfThere(One **head, int length){
	One *current = *head;
	
	//condition verified in the call function
	if (*head == NULL)
		return false;

	while (current != NULL){
		if (current->val == length)
			return true;
		current = current->next;
	}

	return false;
}

void addListOne(One **head, One **tail, int length, int *size){
	One *newNode = (One*)malloc(sizeof(One));
	
	if (newNode == NULL){
		throwError();
		return;
	
	}
	newNode->val = length;
	newNode->next = NULL;
	
	if (*head == NULL){
		*head = newNode;
		*tail = newNode;
		(*size)++;
	}
	else{
		if (searchIfThere(head, length) == false){
			(*tail)->next = newNode;
			*tail = newNode;
			(*size)++;
		}
	}	
}



	
