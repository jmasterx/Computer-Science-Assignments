/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: An event that can be scheduled for a student departing from a desk 
 */
package com.joshl.simulator;

public class DeskDepartureEvent extends Event {
	private Student student;
	public DeskDepartureEvent(double time, Student student) {
		super(time);
		this.student = student;
		// TODO Auto-generated constructor stub
	}

	@Override
	public void process(Clock clock, RegistrationEventDispatcher dispatcher) {
		dispatcher.studentDepartedFromDesk(
				student, clock.getTime(), 
				clock.getTime() - student.peekNextDepartmentRegistration().getArrivalTime(),
				student.peekNextDepartmentRegistration().getDeptName());
	}

	@Override 
	public String toString() {
		return "Desk Departure Event";
	}
}
