/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public abstract class Opcode
{
	/**
	 * Executes the operation with the parameter arguments on the parameter virtual machine.
	 * @param model the virtual machine model to execute the operation on.
	 * @param args the arguments passed to the operation.
	 */
	
	public abstract void execute(MachineModel model, int[] args);
	
	/**
	 * @return the mnemonic String associated with the operation. Ex: 'mul', 'add'.
	 */
	
	public abstract String getMnemonic();
	
	/**
	 * @return the number of arguments required for this operation to execute.
	 */
	
	public abstract int getArgumentCount();
	
	@Override
	public boolean equals(Object obj)
	{
		if(obj instanceof Opcode)
		{
			Opcode op = (Opcode)obj;
			return op.getMnemonic().equals(getMnemonic())
				&& op.getArgumentCount() == getArgumentCount();
		}
		return false;
	}
	
	@Override
	public String toString()
	{
		return "Opcode mnemonic:" + getMnemonic() 
			 + ", Number of arguments:" + String.valueOf(getArgumentCount());
	}
}
