/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: Represents a student in the registration simulation
 */
package com.joshl.simulator;

import java.util.Queue;

public class Student {
	private double arrivalTime;
	private Queue<DepartmentRegistration> deptRegistrations;

	public Student(double arrivalTime,
			Queue<DepartmentRegistration> registrationsList) {
		setArrivalTime(arrivalTime);
		setDeptRegistrations(registrationsList);
	}

	public double getArrivalTime() {
		return arrivalTime;
	}

	private void setArrivalTime(double arrivalTime) {
		this.arrivalTime = arrivalTime;
	}

	private void setDeptRegistrations(
			Queue<DepartmentRegistration> deptRegistrations) {
		this.deptRegistrations = deptRegistrations;
	}

	public DepartmentRegistration dequeueNextDepartmentRegistration() {
		return deptRegistrations.poll();
	}
	
	public DepartmentRegistration peekNextDepartmentRegistration() {
		return deptRegistrations.peek();
	}
	
	public boolean isFinishedRegistration() {
		return deptRegistrations.isEmpty();
	}

}
