/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: Generates the students and the new arrivals
 */
package com.joshl.simulator;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Random;

public class StudentArrivalGenerator extends RegistrationEventListener {
		private List<DepartmentDesk> desks;
		private final double INTERARRIVAL_TIME;
		private Clock clock;
		private Random rng;
		private RegistrationEventListener dispatcher;
		
		public StudentArrivalGenerator(List<DepartmentDesk> departments,double interArrivalTime, Clock clock,
				RegistrationEventListener dispatcher) {
			this.dispatcher = dispatcher;
			INTERARRIVAL_TIME = interArrivalTime;
			this.rng = new Random();
			this.clock = clock;
			desks = new ArrayList<DepartmentDesk>();
			
			for(DepartmentDesk dep : departments) {
				desks.add(dep);
			}
		}
		
		public Student generateStudent(double time) {
			Queue<DepartmentRegistration> registrations = new LinkedList<DepartmentRegistration>();
			for(DepartmentDesk desk : desks) {
				registrations.add(new DepartmentRegistration(
						desk.getDeptName(), rng.nextInt(desk.getCourseLimitPerStudent()) + 1));
			}
			return new Student(time,registrations);
		}
		
		//events
		
		@Override
		public void studentArrivedAtRegistration(Student s,double arrivalTime) {
			dispatcher.scheduleRegistrationArrival();
		}
		
		@Override
		public void scheduleRegistrationArrival() {
			double time = clock.getTime() + RandBox.expo(INTERARRIVAL_TIME);
			dispatcher.scheduleRegistrationArrivalAtTime(time);
		}
		
		@Override
		public void scheduleRegistrationArrivalAtTime(double time) {
			Event e = new RegistrationArrivalEvent(time, generateStudent(time));
			dispatcher.registerEvent(e);	
		}
		
		@Override
		public void scheduleRegistrationDeparture(double timeOfDeparture,Student s) {
			Event e = new RegistrationDepartureEvent(timeOfDeparture, s);
			dispatcher.registerEvent(e);
		}
}
