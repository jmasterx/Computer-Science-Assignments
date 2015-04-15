/* Author: Joshua Larouche
 * Date: March 16th, 2014
 * Description: Returns a random variable on an exponential curve
 */
package com.joshl.simulator;

public class RandBox {

	public static double expo(double mean) {
		return -mean * Math.log(Math.random());
	}
}
