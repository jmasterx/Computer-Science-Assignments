/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: An event that can be scheduled for a student arriving at a desk 
 */
package com.joshl.simulator;

public class DeskArrivalEvent extends Event {
	private Student student;
	public DeskArrivalEvent(double time,Student student) {
		super(time);
		this.student = student;
	}

	@Override
	public void process(Clock clock, RegistrationEventDispatcher dispatcher) {
		dispatcher.studentArrivedAtDesk(
				student,clock.getTime(), student.peekNextDepartmentRegistration().getDeptName());
		
	}

	@Override 
	public String toString() {
		return "Desk Arival Event";
	}
}
