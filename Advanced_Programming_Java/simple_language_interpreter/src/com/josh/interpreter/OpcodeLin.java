/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public class OpcodeLin extends Opcode 
{
	@Override
	public void execute(MachineModel model, int[] args)
	{
		//args: r, x
		//stores integer x into register r
		
		model.setRegister(args[0], args[1]);
	}

	@Override
	public String getMnemonic() 
	{
		return "lin";
	}

	@Override
	public int getArgumentCount() 
	{
		return 2;
	}

}
