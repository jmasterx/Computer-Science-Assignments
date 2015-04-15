/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public class OpcodeOut extends Opcode 
{
	@Override
	public void execute(MachineModel model, int[] args) 
	{
		//args: s1
		//prints contents of s1 to console
		
		int r1 = model.getRegister(args[0]);
		System.out.println(r1);
	}

	@Override
	public String getMnemonic() 
	{
		return "out";
	}

	@Override
	public int getArgumentCount() 
	{
		return 1;
	}

}
