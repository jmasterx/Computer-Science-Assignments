/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: An event that can be scheduled for a student arriving at registration
 */
package com.joshl.simulator;

public class RegistrationArrivalEvent extends Event {
	private Student student;
	public RegistrationArrivalEvent(double time, Student student) {
		super(time);
		this.student = student;
	}

	@Override
	public void process(Clock clock, RegistrationEventDispatcher dispatcher) {
		dispatcher.studentArrivedAtRegistration(student, clock.getTime());
		dispatcher.moveStudentToNextDesk(student);
	}

	@Override 
	public String toString() {
		return "Registration Arrival Event";
	}
}
