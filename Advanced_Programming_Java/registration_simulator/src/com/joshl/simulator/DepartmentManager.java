/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: Manages the events related to department desks
 */
package com.joshl.simulator;

import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class DepartmentManager extends RegistrationEventListener {
	private Map<String, DepartmentDesk> desks;
	private final double AVERAGE_COURSE_SIGN_UP_TIME;
	private RegistrationEventListener dispatcher;
	private Clock clock;

	public DepartmentManager(List<DepartmentDesk> departments,
			double avgRegTime, RegistrationEventListener dispatcher, Clock clock) {
		AVERAGE_COURSE_SIGN_UP_TIME = avgRegTime;
		this.dispatcher = dispatcher;
		this.clock = clock;
		desks = new TreeMap<String, DepartmentDesk>();
		//add the different desks
		for (DepartmentDesk dep : departments) {
			desks.put(dep.getDeptName(), dep);
		}
	}

	//events
	
	@Override
	public void studentArrivedAtDesk(Student s, double arrivalTime,
			String deskName) {
		DepartmentDesk desk = desks.get(deskName);
		s.peekNextDepartmentRegistration().setArrivalTime(arrivalTime);
		//add the student to the queue they are in
		if (desk.isBusy()) {
			desk.enqueueStudent(s);
			dispatcher.studentEnteredQueue(s, deskName,desk.getNumQueuedStudents(),arrivalTime);
		} else {
			desk.startServingStudent(s);
			//student is now being served
			dispatcher.studentBeganProcessingAtDesk(
					s, arrivalTime, arrivalTime - s.peekNextDepartmentRegistration().getArrivalTime(), deskName);
		}
	}

	@Override
	public void studentBeganProcessingAtDesk(Student s, double arrivalTime, double timeInQueue,
			String deskName) {
		dispatcher.scheduleDeskDeparture(
				arrivalTime
						+ RandBox.expo(AVERAGE_COURSE_SIGN_UP_TIME
								* s.peekNextDepartmentRegistration()
										.getNumCourses()), s);
	}

	@Override
	public void studentDepartedFromDesk(Student s, double departureTime, double timeInQueue,
			String deskName) {
		DepartmentDesk desk = desks.get(deskName);
		desk.stopServingCurrentStudent();
		//start processing the next student in the queue for this desk
		if(desk.hasStudentsQueued()) {
			Student newStudent = desk.dequeueStudent();
			dispatcher.studentLeftQueue(newStudent, deskName,desk.getNumQueuedStudents(),departureTime);
			desk.startServingStudent(newStudent);
			dispatcher.studentBeganProcessingAtDesk(
					newStudent, departureTime, departureTime - 
					s.peekNextDepartmentRegistration().getArrivalTime(), deskName);	
		}
		
		//student is finished with this department
		s.dequeueNextDepartmentRegistration();
		//if student has more desks to visit, move to next desk
		if (!s.isFinishedRegistration()) {
			dispatcher.moveStudentToNextDesk(s);
		} else { //the student is finished registration
			dispatcher.scheduleRegistrationDeparture(departureTime, s);
		}
	}

	@Override
	public void moveStudentToNextDesk(Student s) {
		dispatcher.scheduleDeskArrival(clock.getTime(), s);
	}

	@Override
	public void scheduleDeskArrival(double timeOfArrival, Student s) {
		DeskArrivalEvent e = new DeskArrivalEvent(timeOfArrival, s);
		dispatcher.registerEvent(e);
	}
	
	@Override
	public void scheduleDeskDeparture(double timeOfDeparture, Student s) {
		DeskDepartureEvent e = new DeskDepartureEvent(timeOfDeparture, s);
		dispatcher.registerEvent(e);
	}

	@Override
	public void simulationBegan(double totalTimeInSeconds) {

		for (Map.Entry<String, DepartmentDesk> entry : desks.entrySet()) {
			entry.getValue().clear();
		}
	}

}
