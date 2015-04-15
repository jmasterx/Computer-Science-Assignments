/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: An event that can be scheduled in the future
 */
package com.joshl.simulator;


public abstract class Event implements Comparable<Event> {
	private Double time;
	
	public Event(double time) {
		this.time = new Double(time);
	}

	@Override
	public int compareTo(Event arg0) {
		return time.compareTo(arg0.time);
	}
	
	public double getTime() {
		return time;
	}
	public abstract void process(Clock clock, RegistrationEventDispatcher dispatcher);
	
	@Override 
	public String toString() {
		return "Base Event";
	}
}
