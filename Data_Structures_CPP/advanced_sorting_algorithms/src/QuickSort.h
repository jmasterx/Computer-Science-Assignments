/****************************************************************
 * File      :   QuickSort.h
 * Version   :   1.0
 * Date      :   April, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Templated Quick Sort using Median of 3 Partitioning
 *****************************************************************/

#ifndef QUICKSORT_H
#define QUICKSORT_H
template<class T>
class QuickSort {
	void quickSort(T array[],int start,int end) {
		int position;
		if (end - start < 2) return;
		position = partition(array,start,end);
		quickSort(array,start,position);
		quickSort(array,position,end);
	}

	int partition(T array[],int pivot, int right) {
		T l = array[pivot];
		T r = array[(right - pivot)/ 2 + pivot];
		T end = array[right-1];
		int i = pivot - 1;
		int j = right;

		//the left is the right
		if (r > l && r < end || r > end && r < l ) {
			l = r;
		}
		//the left is the end
		else if (end > l && end < r || end > r && end < l ){
			l = end;
		}

		while (true) {
			//get as close as possible to the left
			do  {
				j--;
			} while (array[j] > l);

			//get as close as possible to the right
			do  {
				i++;
			} while (array[i] < l);

			//swap them
			if  (i < j) {
				T temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			else {
				return j+1;
			}
		}
	}
public:
	void sort(T array[], int size) {
		quickSort(array,0,size);
	}
};
#endif