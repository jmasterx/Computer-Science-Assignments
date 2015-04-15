/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public class LaroJosAssign02
{
	public static void main(String[] args)
	{
		Interpreter interpreter = new Interpreter();
		String programFileName = "factorial.tml";
		
		if(interpreter.load(programFileName))
		{
			System.out.println(String.format("The program %s was loaded.",programFileName));
			System.out.println("Source Code Printing Started.");
			System.out.println(interpreter.getSourceString());
			System.out.println("Source Code Printing Finished.");
			System.out.println("Program Execution Started.");
			interpreter.run();
			System.out.println("Program Execution Stopped.");
			System.out.println("Printing Registers...");
			interpreter.printRegisters();
		}
	}
}
