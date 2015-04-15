/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

import java.util.LinkedList;
import java.util.Queue;

public class ErrorList 
{
	private Queue<Error> errors;
	
	public ErrorList()
	{
		errors = new LinkedList<Error>();
	}
	
	/**
	 *  adds the parameter error to the error queue
	 * @param error the error to push.
	 */
	
	public void push(Error error)
	{
		errors.add(error);
	}
	
	/**
	 *  returns the top error and removes it from the error queue.
	 * @return the Error or null if the error queue is empty.
	 */
	
	public Error pop()
	{
		return errors.poll();
	}
	
	/**
	 * 
	 * @return true if the error queue is empty.
	 */
	
	public boolean empty()
	{
		return errors.isEmpty();
	}
}
