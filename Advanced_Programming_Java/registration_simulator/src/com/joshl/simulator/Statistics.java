/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: Statistics module for registration simulator
 */
package com.joshl.simulator;

import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Statistics extends RegistrationEventListener {
	private class StatisticDesk {
		private String name;
		private int maxStudents;
		private int prevTime;
		private int prevNumStudents;
		private int changeCount;
		private int studentSum;
		private double studentWaitSum;
		private int studentWaitCount;
		
		public StatisticDesk(String name) {
			this.name = name;
			clear();
		}
		
		public void clear() {
			maxStudents = 0;
			prevTime = 0;
			changeCount = 0;
			studentSum = 0;
			prevNumStudents = 0;
			studentWaitSum = 0;
			studentWaitCount = 0;
		}
		
		public void addToQueueLengthAvg(int time, int numInQueue) {
			if(numInQueue > maxStudents) {
				maxStudents = numInQueue;
			}
			
			for(int i = prevTime; i < time; i++) {
				changeCount++;
				studentSum += prevNumStudents;
			}
			
			prevTime = time;
			prevNumStudents = numInQueue;
		}
		
		public void addToQueueWaitTimeAvg(double waitTime) {
			studentWaitSum += waitTime;
			studentWaitCount++;
		}
		
		public double getAverageQueueLength() {
			if(changeCount != 0)
				return (double)studentSum / (double)changeCount;
			else
				return 0;
		}
		
		public int getMaximumQueueLength() {
			return maxStudents;
		}
		
		public double getAverageWaitTime() {
			if(studentWaitCount != 0)
				return (double)studentWaitSum / (double)studentWaitCount;
			else
				return 0;
		}
		
		public String getName() {
			return name;
		}
	}
	
	private Map<String, StatisticDesk> desks;
	private int numFinishedRegistration;
	private double registrationTimeSum;

	public Statistics(List<DepartmentDesk> departments) {
		desks = new TreeMap<String, StatisticDesk>();

		for (DepartmentDesk dep : departments) {
			desks.put(dep.getDeptName(),new StatisticDesk(dep.getDeptName()));
		}
	}
	
	public void clear() {
		numFinishedRegistration = 0;
		registrationTimeSum = 0;
		for (Map.Entry<String, StatisticDesk> entry : desks.entrySet()) {
			entry.getValue().clear();
		}
	}
	
	public void printStatistics() {
		System.out.println("Global Statistics:");
		System.out.printf("\t%d students finished registration.\n", getNumStudentsRegistered());
		System.out.printf("\tAverage registration time was %.2f minutes.\n", secToMin(getAverageRegistrationTime()));
		System.out.printf("\tMaximum queue length: %d\n",getMaximumQueueLength());
		System.out.printf("\tAverage queue length: %.2f\n",getAverageQueueLength());
		System.out.printf("\tAverage wait time:    %.2f minutes\n",secToMin(getAverageWaitTime()));
		
		for (Map.Entry<String, StatisticDesk> entry : desks.entrySet()) {
			System.out.printf("Statistics for %s desk:\n",entry.getValue().getName());
			System.out.printf("\tMaximum queue length: %d\n",entry.getValue().getMaximumQueueLength());
			System.out.printf("\tAverage queue length: %.2f\n",entry.getValue().getAverageQueueLength());
			System.out.printf("\tAverage wait time:    %.2f minutes\n",secToMin(entry.getValue().getAverageWaitTime()));
		}
	}
	
	public int getMaximumQueueLength() {
		int length = 0;
		for (Map.Entry<String, StatisticDesk> entry : desks.entrySet()) {
			if(entry.getValue().getMaximumQueueLength() > length) {
				length = entry.getValue().getMaximumQueueLength();
			}
		}
		
		return length;
	}
	
	public double getAverageQueueLength() {
		double total = 0.0;
		int count = 0;
		
		for (Map.Entry<String, StatisticDesk> entry : desks.entrySet()) {
			total += entry.getValue().getAverageQueueLength();
			count++;
		}
		
		if(count == 0)
			return 0;
		else
		return total / (double)count;
	}
	
	public double getAverageWaitTime() {
		double total = 0.0;
		int count = 0;
		
		for (Map.Entry<String, StatisticDesk> entry : desks.entrySet()) {
			total += entry.getValue().getAverageWaitTime();
			count++;
		}
		
		if(count == 0)
			return 0;
		else
		return total / (double)count;
	}
	
	public double secToMin(double secs) {
		return secs / 60.0;
	}
	
	public int getNumStudentsRegistered() {
		return numFinishedRegistration;
	}
	
	public double getAverageRegistrationTime() {
		if(getNumStudentsRegistered() > 0)
				return registrationTimeSum / (double)numFinishedRegistration;
			else
				return 0;
	}
	
	//events
	
	@Override
	public void simulationBegan(double totalTimeInSeconds) {
		clear();
		System.out.println("============================================");
		System.out.printf("Simulation started for %.2f minutes.\n", secToMin(totalTimeInSeconds));
	}
	
	@Override
	public void simulationEnded(double totalTimeInSeconds) {
		System.out.printf("Simulation ended after %.2f minutes.\n", secToMin(totalTimeInSeconds));
		printStatistics();
	}
	
	@Override
	public void studentFinishedRegistration(Student s,double departureTime, double totalRegTime) {
		numFinishedRegistration++;
		registrationTimeSum += totalRegTime;
	}
	
	@Override
	public void studentDepartedFromDesk(Student s, double departureTime, double timeInQueue, String deskName) {
		desks.get(deskName).addToQueueWaitTimeAvg(timeInQueue);
	}
	
	@Override
	public void studentEnteredQueue(Student s, String queueName, int newQueueSize,double time) {
		desks.get(queueName).addToQueueLengthAvg((int)time, newQueueSize);
	}
	
	@Override
	public void studentLeftQueue(Student s, String queueName, int newQueueSize, double time) {
		desks.get(queueName).addToQueueLengthAvg((int)time, newQueueSize);
	}
}
