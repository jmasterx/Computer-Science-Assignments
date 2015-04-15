/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: An event that can be scheduled for a student finishing registration
 */
package com.joshl.simulator;

public class RegistrationDepartureEvent extends Event {

	private Student student;
	public RegistrationDepartureEvent(double time, Student student) {
		super(time);
		this.student = student;
	}

	@Override
	public void process(Clock clock, RegistrationEventDispatcher dispatcher) {
		dispatcher.studentFinishedRegistration(student, clock.getTime(),clock.getTime() - student.getArrivalTime());
	}
	
	@Override 
	public String toString() {
		return "Registration Departure Event";
	}
}
