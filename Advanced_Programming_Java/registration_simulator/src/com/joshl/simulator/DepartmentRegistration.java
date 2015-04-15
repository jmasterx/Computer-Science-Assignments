/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: A department registration object for students
 */
package com.joshl.simulator;

public class DepartmentRegistration {
	private String deptName;
	private int numCourses;
	private double arrivalTime;

	public DepartmentRegistration() {
		setDeptName("");
		setArrivalTime(0);
		setNumCourses(0);
	}

	public DepartmentRegistration(String deptName, int numCourses) {
		this();
		setDeptName(deptName);
		setNumCourses(numCourses);
	}

	public String getDeptName() {
		return deptName;
	}

	private void setDeptName(String deptName) {
		this.deptName = deptName;
	}

	public int getNumCourses() {
		return numCourses;
	}

	private void setNumCourses(int numCourses) {
		this.numCourses = numCourses;
	}

	public double getArrivalTime() {
		return arrivalTime;
	}

	public void setArrivalTime(double arrivalTime) {
		this.arrivalTime = arrivalTime;
	}

}
