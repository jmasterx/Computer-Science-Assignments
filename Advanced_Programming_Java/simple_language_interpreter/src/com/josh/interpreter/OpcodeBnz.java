/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public class OpcodeBnz extends Opcode
{
	@Override
	public void execute(MachineModel model, int[] args) 
	{
		//args: s1, ip
		//if contents of s1 != 0, set instruction pointer to ip
		
		int r1 = model.getRegister(args[0]);
		
		if(r1 != 0)
		{
			model.setSpecialRegister(MachineModel.REGISTER_INSTRUCTION_POINTER, args[1]);
		}
	}

	@Override
	public String getMnemonic() 
	{
		return "bnz";
	}

	@Override
	public int getArgumentCount() 
	{
		return 2;
	}

}
