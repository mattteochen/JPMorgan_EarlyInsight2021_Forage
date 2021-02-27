#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 300

typedef struct transaction{
    char customerId[MAX];
    int age;
    char gender[MAX];
    char customerZipCode[MAX];
    char merchantId[MAX];
    char merchantZipCode[MAX];
    char category[MAX];
    double amount;
    bool isFraud;
    struct transaction *next;
} T;

typedef struct id{
    int size;
    char merchantId[MAX];
    struct id *next;
} I;

void manageString(char s[]);
void split(char **table, T **head, T **tail);
void fieldToZero(char field[], int *counter);
void throwError();
void insertList(T **head, T **tail, T *node);
void freeList(T *head);
void getUniqueMerchantId(T *head, int *size, I **uniqueMerchantIdHead, I **uniqueMerchantIdTail);
bool verifyMerchantId(char id[], I *head);
void insertUniqueMerchantId(char id[], I **head, I **tail);
bool compareString(char sOne[], char sTwo[]);
void freeListI(I *head);
void getTotalNumberOfFraudulentTransactions(T *head, int *size);
void getTotalNumberOfTransactions(T *head, int *size, bool condition);
void getFraudulentTransactionsForMerchantId(T *head, int *size, char id[]);
T *createNode(char customerId[], int age, char gender[], char customerZipCode[], char merchantId[], char merchantZipCode[], char category[], double amount, bool isFraud);
void getFraudulentTransactionsForMerchantIdAndCondition(T *head, int *size, char id[], bool condition);
int compare(const void *pOne, const void *pTwo);
void getAllTransactionsSortedByAmount(T *head, int size);
void getFraudPercentageForMen(T *head);
void getCustomerIdsWithNumberOfFraudulentTransactions(T *head, I **uniqueCustomerIdHead, I **uniqueCustomerIdTail, int value);
void getCustomerIdToNumberOfTransactions(T *head);
void throwNullList();
void getMerchantIdToTotalAmountOfFraudulentTransactions(T *head);
bool verifyMerchantIdSecond(char id[], I **head, int isFraud);
void insertUniqueMerchantIdSecond(char id[], I **head, I **tail, int isFraud);
void caseOne(char *s, char dest[]);
void caseTwo(char *s, char dest[]);
void caseThree(char *s, char dest[], int *isFraud);
void ask(int *chose);

int main(){
    FILE *f = fopen ("final.txt", "r");
    char string[MAX];
    char **table = (char**)malloc(sizeof(char*)*8);
    T *head = NULL;
    T *tail = NULL;
    I *uniqueCustomerIdHead = NULL;
    I *uniqueCustomerIdTail = NULL;
    I *uniqueMerchantIdHead = NULL;
    I *uniqueMerchantIdTail = NULL;
    int task = 0, total = 0, counter = 0, anotherRound = 0, chose = 0;
    long row = 0;

    if (f == NULL || table == NULL){
        printf ("file error\n");
        return -1;
    }

    for (int i=0; i<8; i++){
        table[i] = (char*)malloc(sizeof(char)*MAX);
        if (table[i] == NULL)
            throwNullList();
    }

    //file reading
    while (!feof(f)){
        if (counter == 8)
            counter = 0;
        while (counter < 8){
            fgets(table[counter], MAX, f);
            counter++;
            row++;
        }
        split(table, &head, &tail);
        total++;
    }
    
    ask(&chose);

    do{
        switch (chose){

            case 1:
            //task 1
            getUniqueMerchantId(head, &task, &uniqueMerchantIdHead, &uniqueMerchantIdTail);
            task = 0;
            printf ("\n\n");
            break;

            case 2:
            //task 2
            getTotalNumberOfFraudulentTransactions(head, &task);
            task = 0;
            printf ("\n\n");
            break;
            
            case 3:
            //task 3
            printf ("Which type of transaction do you want to seearch? 0/1");
            scanf ("%d", &chose);
            if (chose == 0)
                getTotalNumberOfTransactions(head, &task, false);
            else
                getTotalNumberOfTransactions(head, &task, true);
            task = 0;
            printf ("\n\n");
            break;
            
            case 4:
            //task 4
            printf ("Which merchant do you want to search?");
            scanf ("%s", string);
            getFraudulentTransactionsForMerchantId(head, &task, string);
            task = 0;
            printf ("\n\n");
            break;

            case 5:
            //task 5
            printf ("Which merchant do you want to search?");
            scanf ("%s", string);
            printf ("Which type of transaction do you want to seearch? 0/1");
            scanf ("%d", &chose);
            if (chose == 0)
                getFraudulentTransactionsForMerchantIdAndCondition(head, &task, string, false);
            else
                getFraudulentTransactionsForMerchantIdAndCondition(head, &task, string, true);
            task = 0;
            printf ("\n\n");
            break;
            
            case 6:
            //task 6
            getAllTransactionsSortedByAmount(head, total);
            printf ("\n\n");
            break;

            case 7:
            //task 7
            getFraudPercentageForMen(head);
            printf ("\n\n");
            break;

            case 8:
            //task 8
            printf ("Insert the number of fraudolent transactions:");
            scanf ("%d", &chose);
            printf ("Working for you...\n");
            getCustomerIdsWithNumberOfFraudulentTransactions(head, &uniqueCustomerIdHead, &uniqueCustomerIdTail, chose);
            printf ("\n\n");
            break;

            case 9:
            //task 9
            printf ("Working for you...\nI'm coming back soon...\n");
            getCustomerIdToNumberOfTransactions(head);
            printf ("\n\n");
            break;

            case 10:
            //task 10
            printf ("Working for you...\n");
            getMerchantIdToTotalAmountOfFraudulentTransactions(head);
            printf ("\n\n");
            break;
        }
        do{
            printf ("Do you want to do another task? Please insert 0 or 1\n");
            scanf ("%d", &anotherRound);
        } while (anotherRound != 0 && anotherRound != 1);
        if (anotherRound == 1)
            ask(&chose);
    } while (anotherRound == 1);

    freeList(head);
    freeListI(uniqueMerchantIdHead);
    freeListI(uniqueCustomerIdHead);
    
    return 0;
}

void ask(int *chose){
    do{
        printf ("What do you want to see? Please insert a value 1 <= value <= 10\n");
        scanf ("%d", &(*chose));
    } while (*chose > 10 || *chose < 1);
}

void throwNullList(){
    printf ("No data\n");
}

void getMerchantIdToTotalAmountOfFraudulentTransactions(T *head){
    T *current = head;
    I *merchantIdHead = NULL;
    I *merchantIdTail = NULL;
    int isFraud = 0;

    if (head == NULL){
        throwNullList();
        return;
    }

    while (current != NULL){
        isFraud = 0;
        if (current->isFraud == true)
            isFraud = 1;
        if (verifyMerchantIdSecond(current->merchantId, &merchantIdHead, isFraud) == false){
            insertUniqueMerchantIdSecond(current->merchantId, &merchantIdHead, &merchantIdTail, isFraud);
        }
        current = current->next;
    }

    printf ("Task ten:\n");
    I *point = merchantIdHead;
    while (point != NULL){
        printf ("Merchant Id: %s -> Number of fraudolent transaction: %d\n", point->merchantId, point->size);
        point = point->next;
    }

    freeListI(merchantIdHead);
}

void getCustomerIdToNumberOfTransactions(T *head){
    T *current = head;
    I *customerIdHead = NULL;
    I *customerIdTail = NULL;
    int isFraud = 0;

    if (head == NULL){
        throwNullList();
        return;
    }

    while (current != NULL){
        isFraud = 0;
        if (current->isFraud == true)
            isFraud = 1;
        if (verifyMerchantIdSecond(current->customerId, &customerIdHead, isFraud) == false){
            insertUniqueMerchantIdSecond(current->customerId, &customerIdHead, &customerIdTail, isFraud);
        }
        current = current->next;
    }

    printf ("Task nine:\n");
    I *point = customerIdHead;
    while (point != NULL){
        printf ("Customer Id: %s -> Number of fraudolent transaction: %d\n", point->merchantId, point->size);
        point = point->next;
    }

    freeListI(customerIdHead);
}

void getCustomerIdsWithNumberOfFraudulentTransactions(T *head, I **uniqueCustomerIdHead, I **uniqueCustomerIdTail, int value){
    //first of all we must create a data structure where we can group the customerId
    T *point = head;

    if (point == NULL){
        throwNullList();
        return;
    }

    while (point != NULL){
        if (point->isFraud == true){
            if (verifyMerchantId(point->customerId, *uniqueCustomerIdHead) == false)
                insertUniqueMerchantId(point->customerId, uniqueCustomerIdHead, uniqueCustomerIdTail);
        }
        point = point->next;
    }

    I *current = *uniqueCustomerIdHead;

    if (current == NULL){
        throwNullList();
        return;
    }

    printf ("Task eight\n:");
    if (current == NULL){
        printf ("No transaction for this value\n");
        return;
    }
    while (current != NULL){
        if (current->size >= value)
            printf ("Customer: %s\n", current->merchantId);
        current = current->next;
    }
}

void getFraudPercentageForMen(T *head){
    //calculate the number of transactions made from man and those that are fraudolent
    int totalManTransaction = 0, fraudolentManTransaction = 0;
    T *point = head;

    if (head == NULL){
        throwError();
        return;
    }

    while (point != NULL){
        if (strcmp(point->gender, "M") == 0){
            totalManTransaction++;
            if (point->isFraud == true)
                fraudolentManTransaction++;
        }
        point = point->next;
    }

    printf ("Task seven: %.2lf\n", (double)(fraudolentManTransaction*100)/totalManTransaction);
}

int compare(const void *pOne, const void *pTwo){
    return (*(double*)pOne - *(double*)pTwo);
}

void getAllTransactionsSortedByAmount(T *head, int size){
    double *list = (double*)calloc(size, sizeof(double));
    T *point = head;
    int i = 0;
    
    if (list == NULL){
        throwError();
        return;
    }
    for (int i=0; i<size; i++)
        list[i] = 0;
    
    while (point != NULL){
        //printf ("%lf\n", point->amount);
        list[i] = point->amount;
        i++;
        point = point->next;
    }

    qsort(list, size, sizeof(double), compare);

    printf ("Task six:\n");
    for (int i=0; i<size; i++)
        printf ("%d. %lf\n", i, list[i]);

    free(list);
}

void getFraudulentTransactionsForMerchantIdAndCondition(T *head, int *size, char id[], bool condition){
    T *point = head;
    T *thisHead = NULL;
    T *thisTail = NULL;

    if (head == NULL){
        throwError();
        return;
    }

    while (point != NULL){
        if (strcmp(point->merchantId, id) == 0 && point->isFraud == condition){
            T *newNode = createNode(point->customerId, point->age, point->gender, point->customerZipCode, point->merchantId, point->merchantZipCode, point->category, point->amount, point->isFraud);
            if (newNode != NULL){
                insertList(&thisHead, &thisTail, newNode);
                (*size)++;
            }
        }
        point = point->next;
    }
    printf ("Test five: %d\n", *size);
    T *current = thisHead;

    while (current != NULL){
        printf ("%s, %d, %s, %s, %s, %s, %s, %lf, %d\n", current->customerId, current->age, current->gender, current->customerZipCode, current->merchantId, current->merchantZipCode, current->category, current->amount, current->isFraud);
        current = current->next;
    }

    free(thisHead);
}

T *createNode(char customerId[], int age, char gender[], char customerZipCode[], char merchantId[], char merchantZipCode[], char category[], double amount, bool isFraud){
    T *newNode = (T*)malloc(sizeof(T));

    if (newNode == NULL){
        throwError();
        return NULL;
    }
    strcpy(newNode->customerId, customerId);
    strcpy(newNode->gender, gender);
    strcpy(newNode->customerZipCode, customerZipCode);
    strcpy(newNode->merchantId, merchantId);
    strcpy(newNode->merchantZipCode, merchantZipCode);
    strcpy(newNode->category, category);
    newNode->age = age;
    newNode->amount = amount;
    newNode->isFraud = isFraud;

    return newNode;
}

void getFraudulentTransactionsForMerchantId(T *head, int *size, char id[]){
    T *point = head;
    T *thisHead = NULL;
    T *thisTail = NULL;

    if (head == NULL){
        throwError();
        return;
    }

    while (point != NULL){
        if (strcmp(point->merchantId, id) == 0 && point->isFraud == true){
            T *newNode = createNode(point->customerId, point->age, point->gender, point->customerZipCode, point->merchantId, point->merchantZipCode, point->category, point->amount, point->isFraud);
            if (newNode != NULL){
                insertList(&thisHead, &thisTail, newNode);
                (*size)++;
            }
        }
        point = point->next;
    }
    printf ("Test four: %d\n", *size);
    T *current = thisHead;

    while (current != NULL){
        printf ("%s, %d, %s, %s, %s, %s, %s, %lf, %d\n", current->customerId, current->age, current->gender, current->customerZipCode, current->merchantId, current->merchantZipCode, current->category, current->amount, current->isFraud);
        current = current->next;
    }

    free(thisHead);
}

void getTotalNumberOfTransactions(T *head, int *size, bool condition){
    T *point = head;

    if (head == NULL){
        throwError();
        return;
    }

    while (point != NULL){
        if (point->isFraud == condition)
            (*size)++;
        point = point->next;
    }
    printf ("Test three: %d\n", *size);
}

bool verifyMerchantId(char id[], I *head){
    I *point = head;

    if (head == NULL)
        return false;

    while (point != NULL){
        if (strlen(point->merchantId) == strlen(id) && compareString(point->merchantId, id) == true){
            point->size++; //this size represent the numbers of lines with this ID
            return true;
        }
        point = point->next;
    }
    return false;
}

bool verifyMerchantIdSecond(char id[], I **head, int isFraud){
    I *point = *head;

    if (*head == NULL)
        return false;

    while (point != NULL){
        if (strlen(point->merchantId) == strlen(id) && compareString(point->merchantId, id) == true){
            if (isFraud == 1)
                point->size = point->size+1;
            return true;
        }
        point = point->next;
    }
    return false;
}

void insertUniqueMerchantId(char id[], I **head, I **tail){
    I *newNode = (I*)malloc(sizeof(I));

    if (newNode ==NULL){
        throwError();
        return;
    }
    newNode->next = NULL;
    strcpy(newNode->merchantId, id);
    newNode->size = 1;

    if (*head == NULL){
        *head = newNode;
        *tail = newNode;
    }
    else{
        (*tail)->next = newNode;
        *tail = newNode;
    }
}

void insertUniqueMerchantIdSecond(char id[], I **head, I **tail, int isFraud){
    I *newNode = (I*)malloc(sizeof(I));

    if (newNode ==NULL){
        throwError();
        return;
    }
    newNode->next = NULL;
    strcpy(newNode->merchantId, id);
    if (isFraud == 0)
        newNode->size = 0;
    else
        newNode->size = 1;

    if (*head == NULL){
        *head = newNode;
        *tail = newNode;
    }
    else{
        (*tail)->next = newNode;
        *tail = newNode;
    }
}

void getUniqueMerchantId(T *head, int *size, I **uniqueMerchantIdHead, I **uniqueMerchantIdTail){
    T *point = head;
    bool verify = false;

    if (point == NULL){
        throwNullList();
        return;
    }

    while (point != NULL){
        verify = verifyMerchantId(point->merchantId, *uniqueMerchantIdHead);
        if (verify == false){
            insertUniqueMerchantId(point->merchantId, uniqueMerchantIdHead, uniqueMerchantIdTail);
            (*size)++;
        }
        point = point->next;
    }
    printf ("Test one: %d\n", *size);
}

void getTotalNumberOfFraudulentTransactions(T *head, int *size){
    T *point = head;

    if (point == NULL){
        throwError();
        return;
    }

    while (point != NULL){
        if (point->isFraud == true)
            (*size)++;
        point = point->next;
    }
    printf ("Task two: %d\n", *size);
}

bool compareString(char sOne[], char sTwo[]){
    for (int i=0; i<strlen(sOne); i++){
        if (sOne[i] != sTwo[i])
            return false;
    }
    return true;
}

void caseOne(char *s, char dest[]){
    int i = 0, j = 0;
    while (s[i] != 39)
        i++;
    i++;
    while (i < strlen(s) && s[i] != 39){
        dest[j] = s[i];
        i++;
        j++;
    }
    dest[j] = '\0';
}

void caseTwo(char *s, char dest[]){
    int i = 1, j = 0;
    while (i < strlen(s) && s[i] != 39){
        dest[j] = s[i];
        i++;
        j++;
    }
    dest[j] = '\0';
}

void caseThree(char *s, char dest[], int *isFraud){
    int i = 0,j = 0;

    while (i < strlen(s) && s[i] != ' '){
        dest[j] = s[i];
        i++;
        j++;
    }
    dest[j] = '\0';
    if (s[i+1] == '0')
        (*isFraud) = 0;
    else
        (*isFraud) = 1;
}

void split(char **table, T **head, T **tail){
    //this function works on the strings and save the data in a data structure
    int i = 0, position = 0, fieldCounter = 0, age = 0, isFraud = 0;
    double amount = 0;
    char field[MAX];
    T *newNode = (T*)malloc(sizeof(T));

    if (newNode == NULL){
        throwError();
        return;
    }
    newNode->next = NULL;

    while (position < 8){
        switch (position){
            case 0:
                caseOne(table[position], field);
                strcpy(newNode->customerId, field);
                position++;
                //fieldToZero(field, &fieldCounter);
                break;
            case 1:
                caseTwo(table[position], field);
                age = atoi(field);
                newNode->age = age;
                position++;
                //fieldToZero(field, &fieldCounter);
                break;
            case 2:
                caseTwo(table[position], field);
                strcpy(newNode->gender, field);
                position++;
                //fieldToZero(field, &fieldCounter);
                break;
            case 3:
                caseTwo(table[position], field);
                strcpy(newNode->customerZipCode, field);
                position++;
                //fieldToZero(field, &fieldCounter);
                break;
            case 4:
                caseTwo(table[position], field);
                strcpy(newNode->merchantId, field);
                position++;
                //fieldToZero(field, &fieldCounter);
                break;
            case 5:
                caseTwo(table[position], field);
                strcpy(newNode->merchantZipCode, field);
                position++;
                //fieldToZero(field, &fieldCounter);
                break;
            case 6:
                caseTwo(table[position], field);
                strcpy(newNode->category, field);
                position++;
                //fieldToZero(field, &fieldCounter);
                break;
            case 7:
                caseThree(table[position], field, &isFraud);
                sscanf(field, "%lf", &amount);
                newNode->amount = amount;
                if (isFraud == 0)
                    newNode->isFraud = false;
                else
                    newNode->isFraud = true;
                position++;
                break;
        }
    }

    //printf ("%s, %d, %s, %s, %s, %s, %s, %lf, %d\n", newNode->customerId, newNode->age, newNode->gender, newNode->customerZipCode, newNode->merchantId, newNode->merchantZipCode, newNode->category, newNode->amount, newNode->isFraud);
    insertList(head, tail, newNode);
}

void insertList(T **head, T **tail, T *node){
    if (*head == NULL){
        *head = node;
        *tail = node;
    }
    else{
        (*tail)->next = node;
        *tail = node;
    }
}

void freeList(T *head){
    T *c = head;
    T *p = NULL;

    while (c != NULL){
        if (p != NULL)
            free(p);
        p = c;
        c = c->next;
    }
    free(p);
}

void freeListI(I *head){
    I *c = head;
    I *p = NULL;

    while (c != NULL){
        if (p != NULL)
            free(p);
        p = c;
        c = c->next;
    }
    free(p);
}

void fieldToZero(char field[], int *counter){
    field[0] = '\0';
    *counter = 0;
}

void throwError(){
    printf ("Allocation error\n");
}
