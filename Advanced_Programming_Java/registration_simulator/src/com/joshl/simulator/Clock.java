/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: A simple clock that moves forward when it receives an event to do so
 */
package com.joshl.simulator;
public class Clock extends RegistrationEventListener {
	private double time;
	
	public Clock() {
		time = 0;
	}
	
	public double getTime() {
		return time;
	}
	
	//events
	
	@Override
	public void moveTimeTo(double newTime) {
		time = newTime;
	}
	
	@Override
	public void simulationBegan(double totalTimeInSeconds) {
		time = 0;
	}
}
