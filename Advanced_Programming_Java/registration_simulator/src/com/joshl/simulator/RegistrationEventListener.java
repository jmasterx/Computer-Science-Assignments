/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: Event declarations for registration simulator with default implementations
 */
package com.joshl.simulator;

public class RegistrationEventListener {

	public void moveTimeTo(double newTime) {
	}
	
	public void simulationBegan(double totalTimeInSeconds) {
	}
	
	public void simulationEnded(double totalTimeInSeconds) {
	}
	
	public void scheduleRegistrationArrival() {
	}
	
	public void scheduleRegistrationArrivalAtTime(double time) {
	}
	
	public void scheduleRegistrationDeparture(double timeOfDeparture, Student s) {
	}
	
	public void scheduleDeskArrival(double timeOfArrival, Student s) {
	}
	
	public void scheduleDeskDeparture(double timeOfDeparture, Student s) {
	}
	
	public void moveStudentToNextDesk(Student s) {
	}
	
	public void studentArrivedAtRegistration(Student s,double arrivalTime) {
	}
	
	public void studentFinishedRegistration(Student s,double departureTime, double totalRegTime) {
	}
	
	public void studentArrivedAtDesk(Student s,double arrivalTime,String deskName) {
	}
	
	public void studentBeganProcessingAtDesk(Student s,double arrivalTime, double timeInQueue,String deskName) {
	}
	
	public void studentDepartedFromDesk(Student s,double departureTime, double timeInQueue, String deskName) {
	}
	
	public void registerEvent(Event e) {
	}
	
	public void studentEnteredQueue(Student s, String queueName, int newQueueSize,double time) {
	}
	
	public void studentLeftQueue(Student s, String queueName, int newQueueSize, double time) {
	}
}
