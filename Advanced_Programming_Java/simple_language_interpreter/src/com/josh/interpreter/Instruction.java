/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public class Instruction
{
	private Opcode opcode;
	private int[] args;
	
	public Instruction(Opcode opcode, int[] args)
	{
		this.opcode = opcode;
		this.args = args;
	}
	
	/**
	 * Executes the specified operation with the specified arguments on the parameter machine model.
	 * @param model the model to execute the instruction on.
	 */
	
	public void execute(MachineModel model)
	{
		opcode.execute(model, args);
	}
}
