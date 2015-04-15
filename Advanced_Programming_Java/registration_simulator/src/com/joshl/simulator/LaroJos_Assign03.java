/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: Main file for Registration Simulator Assignment
 */
package com.joshl.simulator;

import java.util.ArrayList;
import java.util.List;

public class LaroJos_Assign03 {
	public static void main(String[] args) {
		List<DepartmentDesk> desks = new ArrayList<DepartmentDesk>();
		desks.add(new DepartmentDesk("CSC", 2));
		desks.add(new DepartmentDesk("MATH", 3));
		
		RegiSim sim = new RegiSim(120.0, 40.0, desks);
		
		sim.run(60 * 60 * 5);
	}

}
