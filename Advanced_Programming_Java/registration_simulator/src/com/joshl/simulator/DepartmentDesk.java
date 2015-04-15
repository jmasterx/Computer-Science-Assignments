/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: A department desk for registration with a name and a course limit per student
 *              Allows students to queue in a line like a real desk
 */
package com.joshl.simulator;

import java.util.LinkedList;
import java.util.Queue;

public class DepartmentDesk {
	private String deptName;
	private Queue<Student> students;
	private Student curStudentBeingServed;
	private int courseLimitPerStudent;

	public DepartmentDesk(String deptName, int courseLimitPerStudent) {
		this.deptName = deptName;
		this.courseLimitPerStudent = courseLimitPerStudent;
		students = new LinkedList<Student>();
		curStudentBeingServed = null;
	}

	public boolean isBusy() {
		return curStudentBeingServed != null;
	}

	public void enqueueStudent(Student s) {
		students.add(s);
	}

	public Student dequeueStudent() {
		return students.poll();
	}
	
	public boolean hasStudentsQueued() {
		return !students.isEmpty();
	}

	public void startServingStudent(Student s) {
		curStudentBeingServed = s;
	}

	public void stopServingCurrentStudent() {
		curStudentBeingServed = null;
	}

	public Student getStudentBeingServed() {
		return curStudentBeingServed;
	}

	public String getDeptName() {
		return deptName;
	}

	public int getCourseLimitPerStudent() {
		return courseLimitPerStudent;
	}
	
	public int getNumQueuedStudents() {
		return students.size();
	}
	
	public void clear() {
		stopServingCurrentStudent();
		students.clear();
	}
}
