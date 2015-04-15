/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: Main class for the registration simulator
 */
package com.joshl.simulator;

import java.util.List;
import java.util.PriorityQueue;

public class RegiSim extends RegistrationEventListener {
	private RegistrationEventDispatcher dispatcher;
	private DepartmentManager deptManager;
	private StudentArrivalGenerator studentArrivalGenerator;
	private Clock clock;
	private Statistics stats;
	private PriorityQueue<Event> events;
	
	public RegiSim(double interArrivalTime, double avgCourseTime, List<DepartmentDesk> departments) {
		dispatcher = new RegistrationEventDispatcher();
		clock = new Clock();
		deptManager = new DepartmentManager(departments, avgCourseTime, dispatcher, clock);
		studentArrivalGenerator = new StudentArrivalGenerator(departments, interArrivalTime, clock, dispatcher);
		stats = new Statistics(departments);
		events = new PriorityQueue<Event>();
		
		dispatcher.add(clock);
		dispatcher.add(deptManager);
		dispatcher.add(studentArrivalGenerator);
		dispatcher.add(stats);
		dispatcher.add(this);
	}
	
	public void run(double time) {
		
		events.clear();
		dispatcher.simulationBegan(time);
		dispatcher.scheduleRegistrationArrivalAtTime(0);
		
		while(clock.getTime() < time) {
			Event e = events.poll();
			dispatcher.moveTimeTo(e.getTime());
			e.process(clock, dispatcher);
		}
		
		dispatcher.simulationEnded(clock.getTime());
	}
	
	//events
	
	@Override
	public void registerEvent(Event e) {
		events.add(e);
	}
	
}
