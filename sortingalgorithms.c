#ifndef SORTINGALGORITHMS_C
#define SORTINGALGORITHMS_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.c"

typedef char pathName[500]; //declaration of a type for the path name of the file

/*
    This function asks for the file path of the .txt file that contains the records.
    Returns 1 is file is found, 0 if the file is not found. 
*/
int isFileValid (pathName path)
{
    FILE *fp;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 0; //file is not found
    } else {
        return 1; //file found
    }
    fclose(fp);
}
/*
    This functions reads the number of records in the .txt file. Assumes that .txt file is already in the correct format,
    with the first line being the number of records. Returns the number of records in the file.
*/
int getRecordCount(pathName path)
{
    int recordCount;
    FILE *fp;

    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return -1;
    } else {
            fscanf(fp, "%d", &recordCount);
    }
    fclose(fp);

    return recordCount;
}

/*
    This function prints out the contents of the array of structs/records into a .txt file called output.txt
    Helps ensure that the sorting algorithms are working correctly. For testing purposes.
*/

void printRecordsToFile(Record *records, int recordCount) {
    FILE *file = fopen("output.txt", "w"); //opens file in write mode, if output.txt does not exist, it will be created
    if (file == NULL) {
        printf("Error opening file!\n"); //accounts for other instances where file may not be opened
        return;
    }

    for(int i = 0; i < recordCount; i++) {
        fprintf(file, "ID number %d, %s\n", records[i].idNumber, records[i].name);
    }

    fclose(file);
}

/*
    This function allows for the swapping of different elements in an array of structs/records.
*/
void swapStructs (Record *a, Record *b) //T = 11
{
    Record temp = *a; 
    *a = *b; 
    *b = temp; 
}

/*
    This function performs a search for an
    entry in the SORTED array of structs/records.

    If found, it returns the index of the entry in the sorted array.
    If not, it returns -1.
*/

int binarySearchForRecord(Record *records, int recordCount, int idNumber, char name[])
{
    int low = 0, high = recordCount - 1, mid;
    int found = 0;

    while(!found && low <= high) {
        mid = (low + high) / 2;
        if (records[mid].idNumber == idNumber && strcmp(records[mid].name, name) == 0) {
            found = 1;
        } else if (records[mid].idNumber < idNumber || (records[mid].idNumber == idNumber && strcmp(records[mid].name, name) < 0)) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if(found) {
        return mid;
    } else {
        return -1;
    }
}

/*

*/
int min (int x, int y)
{
    if (x < y)
        return x;
    else 
        return y;
}

/*

*/
//note declaring arr[i] like this allocates memory on STACK, but using malloc allocates memory on HEAP
void merge (Record *arr, int lo, int mid, int hi)
{
    int i, j, k;
    int sizeArr1 = mid - lo + 1;
    int sizeArr2 = hi - mid;

    Record *arr1 = (Record *)malloc(sizeArr1 * sizeof(Record));
    Record *arr2 = (Record *)malloc(sizeArr2 * sizeof(Record));

    for (i = 0; i < sizeArr1; i++) {
        arr1[i] = arr[lo + i];
    }

    for (j = 0; j < sizeArr2; j++) {
        arr2[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = lo;

    while (i < sizeArr1 && j < sizeArr2) {
        if (arr1[i].idNumber <= arr2[j].idNumber) {
            arr[k] = arr1[i];
            i++;
        } else {
            arr[k] = arr2[j];
            j++;
        }
        k++;
    }

    while (i < sizeArr1) {
        arr[k] = arr1[i];
        i++;
        k++;
    }

    while (j < sizeArr2) {
        arr[k] = arr2[j];
        j++;
        k++;
    }

    free(arr1);
    free(arr2);
}

/*
    getPivot function rearranges using the Median of Three method so that the ones lower than the pivot are to the left, and the ones higher are to the right.
    it is used in the iterativeQuickSort function
*/

int getPivot(Record *arr, int lo, int hi, long long int *stepCount){
    int mid = lo + (hi - lo) / 2;
    //median of three method to find the pivot
    if (arr[mid].idNumber < arr[lo].idNumber) 
        swapStructs(&arr[lo], &arr[mid]);

    if (arr[hi].idNumber < arr[lo].idNumber) 
        swapStructs(&arr[lo], &arr[hi]);

    if (arr[hi].idNumber < arr[mid].idNumber) 
        swapStructs(&arr[mid], &arr[hi]);


    swapStructs(&arr[mid], &arr[hi]); //move pivot to the very right/end
    Record pivot = arr[hi];//store the information of the pivot


    int left = lo;
    int right = hi - 1; //-1 to account for the pivot
    while (left <= right){//keep swapping until the left is bigger than the right, or the other way around
        if (arr[left].idNumber <= pivot.idNumber)
            left++;
        else if (arr[right].idNumber >= pivot.idNumber)
            right--;
        else{//when left is bigger than the right, swap.
            swapStructs(&arr[left], &arr[right]);
            left++;
            right--;
        }
    }
    
    swapStructs(&arr[left], &arr[hi]);//move pivot to its correct location
    return left; // return the pivot's final position
}

void insertionSort(Record *arr, int n, long long int *stepCount)
{
    // TODO: Implement this sorting algorithm here.
    int i, j; 
    Record key;
    (*stepCount) += 4; //declares i and j of int data type, and temp of Record data type, initialie i to 1

    for (i = 1; i < n; i++) {
        (*stepCount)++; //accounts for i < n check in every iteration
        key = arr[i]; 
        j = i - 1;
        (*stepCount) += 5; //initializes key to arr[i], and j to i - 1
        while (j >= 0 && arr[j].idNumber > key.idNumber) {
            arr[j + 1] = arr[j];
            j -= 1;
            (*stepCount) += 9; //accounts for the while loop and contents
        }
        (*stepCount)++; //one last check for while loop
        arr[j + 1] = key; 
        (*stepCount) += 3;
    }
    (*stepCount)++; //last check of for loop
}

void selectionSort(Record *arr, int n, long long int *stepCount)
{
    // TODO: Implement this sorting algorithm here.
    int i, j, minIndex;
    (*stepCount) += 3; //declares i, j, and minIndex of int data type

    for (i = 0; i < n - 1; i++) {
        minIndex = i;
        (*stepCount)++;
        for (j = i + 1; j < n; j++) {
            if (arr[j].idNumber < arr[minIndex].idNumber) {
                minIndex = j;
                (*stepCount) += 2;
            }
            (*stepCount)++;
        }
        (*stepCount)++;
        if (minIndex != i) {
            swapStructs(&arr[minIndex], &arr[i]);
        }
        (*stepCount)++;
    }
    (*stepCount)++;
}

/*

*/
void iterativeMergeSort(Record *arr, int n, long long int *stepCount)
{
    int mid, currentSize, leftStart, rightEnd;

    for (currentSize = 1; currentSize <= n - 1; currentSize = 2 * currentSize) {
        for (leftStart = 0; leftStart < n - 1; leftStart += 2 * currentSize) {
            mid = min(leftStart + currentSize - 1, n - 1);
            rightEnd = min(leftStart + 2 * currentSize - 1, n - 1);
            merge(arr, leftStart, mid, rightEnd);
        }
    }
}

/*
* Define AT LEAST ONE more sorting algorithm here, apart from the
* ones given above. Make sure that the method accepts an array of
* record structures.
*/

//will probably add more notes here as to why a stack is used

void iterativeQuickSort(Record *arr, int lo, int hi, long long int *stepCount)
{
    //since were not using recursion, we'll utilize stacks
    int stack[hi - lo + 1];
    int top = -1; //initialized to -1, which means empty
    int pivot; 

    stack[++top] = lo; //lo takes the 0th index aka "pushing" a value to the stack
    stack[++top] = hi; //hi takes the 1th index

    //process stack elements until empty 
    while (top >= 0){
        hi = stack[top--]; // get the value of stack and then decrement, aka "popping" the stack
        lo = stack[top--]; // its back to lo's stack so gets it and decrements. were back at -1

        //getPivot gets the pivot by finding the median
        pivot = getPivot(arr, lo, hi, stepCount);

        //if there are elements on the left that are unsorted...
        if (pivot - 1 > lo){
            stack[++top] = lo; //lo is now at 0
            stack[++top] = pivot - 1; //the new "hi" is now at 1(which is to the left of pivot)

        }
        //if there are elements on the right that are unsorted...
        if (pivot + 1 < hi){
            stack[++top] = pivot + 1; //set the right of the pivot to the new "lo" 
            stack[++top] = hi; //the hi remains the same
        }
    }
}

#endif