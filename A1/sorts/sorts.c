#include "sorts.h"

// Merge Sort 
static void merge(int arr[], int left, int mid, int right) 
{
    int leftSize = mid - left + 1;
    int rightSize = right - mid;
    int *leftValues = malloc((size_t)leftSize * sizeof(int));
    int *rightValues = malloc((size_t)rightSize * sizeof(int));

    if (leftValues == NULL || rightValues == NULL) {
        free(leftValues);
        free(rightValues);
        return;
    }

    for (int i = 0; i < leftSize; ++i) {
        leftValues[i] = arr[left + i];
    }
    for (int j = 0; j < rightSize; ++j) {
        rightValues[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;
    while (i < leftSize && j < rightSize) {
        if (leftValues[i] <= rightValues[j]) {
            arr[k++] = leftValues[i++];
        } else {
            arr[k++] = rightValues[j++];
        }
    }
    while (i < leftSize) {
        arr[k++] = leftValues[i++];
    }
    while (j < rightSize) {
        arr[k++] = rightValues[j++];
    }

    free(leftValues);
    free(rightValues);
}

void mergeSort(int arr[], int left, int right) 
{
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Bubble Sort
void bubbleSort(int arr[], int n) 
{
    for (int end = n - 1; end > 0; --end) {
        int swapped = 0;
        for (int i = 0; i < end; ++i) {
            if (arr[i] > arr[i + 1]) {
                int temporary = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temporary;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

void printArray(int arr[], int n) 
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}