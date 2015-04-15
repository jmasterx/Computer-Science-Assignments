/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public class OpcodeDiv extends Opcode 
{
	@Override
	public void execute(MachineModel model, int[] args) 
	{
		//args: r, s1, s2
		//divides registers s1,s2 and stores in r
		
		int r1 = model.getRegister(args[1]);
		int r2 = model.getRegister(args[2]);
		int result = r1 / r2;
		model.setRegister(args[0], result);
	}

	@Override
	public String getMnemonic() 
	{
		return "div";
	}

	@Override
	public int getArgumentCount() 
	{
		return 3;
	}

}
