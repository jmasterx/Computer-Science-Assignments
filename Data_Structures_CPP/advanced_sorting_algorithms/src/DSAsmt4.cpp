/****************************************************************
 * File      :   DSAsmt4.cpp
 * Version   :   1.0
 * Date      :   April, 2014
 * Author    :   Joshua Larouche (jlarouche13@ubishops.ca)
 * Subject   :   Main file for Assignment 4
 *****************************************************************/
#include "QuickSort.h"
#include "MergeSort.h"
#include "Tree.h"
#include "Timer.h"
#include <iostream>

#define NUM_TRIALS 5
#define NUM_REPETITIONS 4
int main() {
	std::cout << "Data Structures Assignment #4 - Joshua Larouche" << std::endl;
	std::cout << "Computing results..." << std::endl;
	int numArray[] = {10000,50000,100000,500000};
	long double quickStats[NUM_REPETITIONS][NUM_TRIALS];
	long double mergeStats[NUM_REPETITIONS][NUM_TRIALS];
	long double bstStats[NUM_REPETITIONS][NUM_TRIALS];

	QuickSort<int> qs;
	MergeSort<int> ms;
	Tree<int> tree;

	for(int i = 0; i < NUM_REPETITIONS; i++) {
		for(int j = 0; j < NUM_TRIALS; j++) {
			for(int k = 0; k < 3; k++) {
				int *array = new int[numArray[i]];
				for(int r = 0; r < numArray[i]; r++) {
					array[r] = rand() * (rand() / 2) ;
				}
				
				Timer timer;
				tree.clear();
				timer.start();
				if(k == 0) {
					qs.sort(array,numArray[i]);
					timer.stop();
					quickStats[i][j] = timer.getElapsedTimeInSec();
				}
				else if(k == 1) {
					ms.sort(array,numArray[i]);
					timer.stop();
					mergeStats[i][j] = timer.getElapsedTimeInSec();
				}
				else if(k == 2) {
					tree.sort(array,numArray[i]);
					timer.stop();
					bstStats[i][j] = timer.getElapsedTimeInSec();
				}
				delete[] array;
			}
		}
	}

	for(int i = 0; i < 3; i++) {
		std::string name;
		double stats[NUM_REPETITIONS][NUM_TRIALS];

		if(i == 0) {
			name = "Quick Sort";
			for(int r = 0; r < NUM_REPETITIONS; r++) 
				for(int t = 0; t < NUM_TRIALS;  t++) 
					stats[r][t] = quickStats[r][t];
		}
		else if(i == 1) {
			name = "Merge Sort";
			for(int r = 0; r < NUM_REPETITIONS; r++) 
				for(int t = 0; t < NUM_TRIALS;  t++) 
					stats[r][t] = mergeStats[r][t];
		}
		else if(i == 2) {
			name = "BST Sort";
			for(int r = 0; r < NUM_REPETITIONS; r++) 
				for(int t = 0; t < NUM_TRIALS;  t++) 
					stats[r][t] = bstStats[r][t];
		}

		std::cout << name.c_str() << std::endl;
		for(int j = 0; j < NUM_REPETITIONS; j++) {
			double sum = 0.0;
			std::cout << "  With " << numArray[j] 
			<< " elements" << std::endl;

			for(int k = 0; k < NUM_TRIALS; k++) {
				std::cout << "    Trial #" << (k + 1) << ": " <<
					stats[j][k] << " sec" << std::endl;

				sum += stats[j][k];
			}

			double average = sum / NUM_TRIALS;
			std::cout << "    Average: " <<
				average << " sec" << std::endl;
		}
	}
	return 0;
}