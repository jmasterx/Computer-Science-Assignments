/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: Implementation of observer pattern with multiple observers and broadcasters
 */
package com.joshl.simulator;

import java.util.ArrayList;
import java.util.List;

public class RegistrationEventDispatcher extends RegistrationEventListener {
	private List<RegistrationEventListener> observers;
	
	public RegistrationEventDispatcher() {
		observers = new ArrayList<RegistrationEventListener>();
	}
	
	public void add(RegistrationEventListener listener) {
		observers.add(listener);
	}
	
	public void moveTimeTo(double newTime) {
		for(RegistrationEventListener r : observers)
			r.moveTimeTo(newTime);
	}
	
	public void simulationBegan(double totalTimeInSeconds) {
		for(RegistrationEventListener r : observers)
			r.simulationBegan(totalTimeInSeconds);
	}
	
	public void simulationEnded(double totalTimeInSeconds) {
		for(RegistrationEventListener r : observers)
			r.simulationEnded(totalTimeInSeconds);
	}
	
	public void scheduleRegistrationArrival() {
		for(RegistrationEventListener r : observers)
			r.scheduleRegistrationArrival();
	}
	
	public void scheduleRegistrationArrivalAtTime(double time) {
		for(RegistrationEventListener r : observers)
			r.scheduleRegistrationArrivalAtTime(time);
	}
	
	public void scheduleRegistrationDeparture(double timeOfDeparture, Student s) {
		for(RegistrationEventListener r : observers)
			r.scheduleRegistrationDeparture(timeOfDeparture, s);
	}
	
	public void scheduleDeskArrival(double timeOfArrival, Student s) {
		for(RegistrationEventListener r : observers)
			r.scheduleDeskArrival(timeOfArrival, s);
	}
	
	public void scheduleDeskDeparture(double timeOfDeparture, Student s) {
		for(RegistrationEventListener r : observers)
			r.scheduleDeskDeparture(timeOfDeparture, s);
	}
	
	public void moveStudentToNextDesk(Student s) {
		for(RegistrationEventListener r : observers)
			r.moveStudentToNextDesk(s);
	}
	
	public void studentArrivedAtRegistration(Student s,double arrivalTime) {
		for(RegistrationEventListener r : observers)
			r.studentArrivedAtRegistration(s, arrivalTime);
	}
	
	public void studentFinishedRegistration(Student s,double departureTime, double regTime) {
		for(RegistrationEventListener r : observers)
			r.studentFinishedRegistration(s, departureTime,regTime);
	}
	
	public void studentArrivedAtDesk(Student s,double arrivalTime,String deskName) {
		for(RegistrationEventListener r : observers)
			r.studentArrivedAtDesk(s, arrivalTime, deskName);
	}
	
	public void studentBeganProcessingAtDesk(Student s,double arrivalTime, double timeInQueue, String deskName) {
		for(RegistrationEventListener r : observers)
			r.studentBeganProcessingAtDesk(s, arrivalTime, timeInQueue, deskName);
	}
	
	public void studentDepartedFromDesk(Student s,double departureTime, double timeInQueue, String deskName) {
		for(RegistrationEventListener r : observers)
			r.studentDepartedFromDesk(s, departureTime, timeInQueue, deskName);
	}
	
	public void registerEvent(Event e) {
		for(RegistrationEventListener r : observers)
			r.registerEvent(e);
	}
	
	public void studentEnteredQueue(Student s, String queueName, int newQueueSize, double time) {
		for(RegistrationEventListener r : observers)
			r.studentEnteredQueue(s, queueName,newQueueSize, time);
	}
	
	public void studentLeftQueue(Student s, String queueName, int newQueueSize, double time) {
		for(RegistrationEventListener r : observers)
			r.studentLeftQueue(s, queueName,newQueueSize, time);
	}
	
}
