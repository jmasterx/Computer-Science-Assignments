/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;
public class Machine 
{
	MachineModel model;
	Instruction[] instructions;
	
	public Machine()
	{
		model = new MachineModel();
		instructions = null;
	}
	
	/**
	 * Loads the parameter instructions for later use.
	 * @param instructions the instruction list to load.
	 */
	
	public void load(Instruction[] instructions)
	{
		this.instructions = instructions;
	}
	
	/**
	 * Executes the instructions loaded in load on the virtual machine.
	 * @return true if at least 1 instruction was executed.
	 */
	
	public boolean run()
	{
		if(instructions == null || instructions.length == 0)
			return false;
		
		int numInstructions = instructions.length;
		model.reset(); //reset the registers
		
		//do until program is finished
		while(model.getSpecialRegister(MachineModel.REGISTER_INSTRUCTION_POINTER) < numInstructions)
		{
			//keep track of the next instruction to execute
			int curInstruction = model.getSpecialRegister(MachineModel.REGISTER_INSTRUCTION_POINTER);
			
			//increment the instruction pointer by 1
			int ip = model.getSpecialRegister(MachineModel.REGISTER_INSTRUCTION_POINTER) + 1;
			model.setSpecialRegister(MachineModel.REGISTER_INSTRUCTION_POINTER, ip);
			
			//fetch and execute the next instruction
			Instruction instruction = instructions[curInstruction];
			instruction.execute(model);
		}
		
		return true;
	}
	
	/**
	 * @return the virtual machine model for the interpreter.
	 */
	
	public MachineModel getModel()
	{
		return model;
	}
}
