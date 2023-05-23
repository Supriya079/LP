/*
	export OMP_NUM_THREADS=4
	g++ -fopenmp bubblemerge.cpp
	./a.out
*/

#include <iostream>
#include <algorithm>
#include <omp.h>

void generateArray(int arr[], int size){
    for(int i=0;i<size;i++){
        arr[i] = rand() % size;
    }
}

void printArray(int arr[], int size){
    for(int i=0;i<size;i++){
        std::cout<<arr[i]<<" ";
    }
}

// Sequential Bubble Sort
void sequentialBubbleSort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                // Swap elements
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(int arr[], int n) {
    int i, j;
    #pragma omp parallel for private(i, j) shared(arr)
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                // Swap elements
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Sequential Merge Sort
void sequentialMergeSort(int arr[], int l, int r) {

    if (l < r) {	// check array have elements or not l!=r
        
        int m = l + (r - l) / 2;

        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);

        // Merge the sorted halves
        int i, j, k;
        int n1 = m - l + 1;
        int n2 = r - m;

        int left[n1], right[n2];

        for (i = 0; i < n1; i++)
            left[i] = arr[l + i];
        for (j = 0; j < n2; j++)
            right[j] = arr[m + 1 + j];

        i = 0;
        j = 0;
        k = l;

        while (i < n1 && j < n2) {
            if (left[i] <= right[j]) {
                arr[k] = left[i];
                i++;
            }
            else {
                arr[k] = right[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = left[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = right[j];
            j++;
            k++;
        }
    }
}

// Parallel Merge Sort (using recursion)
void parallelMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);

            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        
        // Merge the sorted halves
        int i, j, k;
        int n1 = m - l + 1;
        int n2 = r - m;

        int left[n1], right[n2];
        
        for (i = 0; i < n1; i++)
            left[i] = arr[l + i];
        for (j = 0; j < n2; j++)
            right[j] = arr[m + 1 + j];

        i = 0;
        j = 0;
        k = l;
        
        while (i < n1 && j < n2) {
            if (left[i] <= right[j]) {
                arr[k] = left[i];
                i++;
            }
            else {
                arr[k] = right[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = left[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = right[j];
            j++;
            k++;
        }
    }
}

int main() {

    const int size = 10000;
    int arr[size];
    generateArray(arr,size);
    
    int parallelArr[size];
    std::copy(arr,arr+size,parallelArr);

    // Sequential Bubble Sort
    double startTime = omp_get_wtime();
    sequentialBubbleSort(arr,size);
    double endTime = omp_get_wtime();
//  std::cout<<"Unsorted Array";
//  printArray(parallelArr,size);
    double sequentialTime = endTime - startTime;
    std::cout << "Time for Sequential Bubble Sort: " << sequentialTime << std::endl;
    
    // Parallel Bubble Sort
    startTime = omp_get_wtime();
    parallelBubbleSort(parallelArr, size);
    endTime = omp_get_wtime();
  //  std::cout<<"Parallel Sorted Array";
//    printArray(parallelArr,size);
    double parallelTime = endTime - startTime;
    std::cout << "Time for Parallel Bubble Sort: " << parallelTime << std::endl;
    
    // Sequential Merge Sort
    startTime = omp_get_wtime();
    sequentialMergeSort(arr, 0, size - 1);
    endTime = omp_get_wtime();
    sequentialTime = endTime - startTime;
    std::cout << "Time for Sequential Merge Sort: " << sequentialTime << std::endl;
    
     // Parallel Merge Sort
    startTime = omp_get_wtime();
    parallelMergeSort(parallelArr, 0, size - 1);
    endTime = omp_get_wtime();
    parallelTime = endTime - startTime;
    std::cout << "Time for Parallel Merge Sort: " << parallelTime << std::endl;

    return 0;
}
