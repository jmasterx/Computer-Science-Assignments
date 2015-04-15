/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public interface Compiler 
{	
	/**
	 * Should return an array of Instruction
	 */
	
	public Instruction[] getInstructions();	
	
	/**
	 * @param input String containing the source code to compile.
	 * @param errorList the list to add Error objects if needed.
	 * @return true if compiling succeeded. 
	 * Otherwise the errorList may have errors inside.
	 */
	
	public boolean compile(String input, ErrorList errorList);
}
