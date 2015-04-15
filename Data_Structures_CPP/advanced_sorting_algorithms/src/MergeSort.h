/****************************************************************
 * File      :   MergeSort.h
 * Version   :   1.0
 * Date      :   April, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Templated Merge Sort
 *****************************************************************/

#ifndef MERGESORT_H
#define MERGESORT_H
template<class T>
class MergeSort {
	void mergeSort(T array[], T tempArray[], int low, int high) {
		int pivot;
		if(low < high) {
			pivot = (low + high) / 2; //middle of array
			//left side
			mergeSort(array,tempArray,low,pivot);
			//right side
			mergeSort(array,tempArray,pivot + 1,high);
			//merge them
			merge(array,tempArray,low,high,pivot);
		}
	}

	void merge(T array[], T tempArray[], int low, int high, int pivot) {
		int h = low;
		int i = low;
		int j = pivot + 1; 

		//start from low and go toward pivot
		//start from pivot and go toward high point
		while((h <= pivot) && (j <= high)) {
			//either grab from first half or second half
			if(array[h] <= array[j]) {
				tempArray[i] = array[h];
				h++;
			}
			else {
				tempArray[i] = array[j];
				j++;
			}
			i++;
		}

		//copy from right
		if(h > pivot) {
			for(int k = j; k <= high;k++) {
				tempArray[i] = array[k];
				i++;
			}
		}
		//copy from left
		else {
			for(int k = h; k <= pivot; k++) {
				tempArray[i] = array[k];
				i++;
			}
		}

		//temp contains merged list
		//transfer to main array
		for(int k = low; k <= high; k++) {
			array[k] = tempArray[k];
		}
	}

public:
	void sort(T array[], int size) {
		T* tempArray = new T[size];
		mergeSort(array,tempArray,0,size - 1);
		delete[] tempArray;
	}
};
#endif