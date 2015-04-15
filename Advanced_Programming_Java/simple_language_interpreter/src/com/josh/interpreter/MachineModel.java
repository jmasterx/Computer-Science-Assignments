/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public class MachineModel 
{
	public static final int MAX_GENERAL_REGISTERS = 32;
	public static final int MAX_SPECIAL_REGISTERS = 1;
	
	public static final int REGISTER_INSTRUCTION_POINTER = 0;
	
	private int[] specialRegisters;
	private int[] generalRegisters;
	
	public MachineModel()
	{
		this.generalRegisters = new int[MAX_GENERAL_REGISTERS];
		this.specialRegisters = new int[MAX_SPECIAL_REGISTERS];
	}
	
	/**
	 * @param register the parameter register.
	 * @return the parameter register's value.
	 */
	
	public int getRegister(int register)
	{
		return generalRegisters[register];
	}
	
	/**
	 * @param register the parameter register.
	 * @param value the value to assign to the parameter register.
	 */
	
	public void setRegister(int register, int value)
	{
		generalRegisters[register] = value;
	}
	
	/**
	 * @return the number of general purpose registers.
	 */
	
	public int getRegisterCount()
	{
		return generalRegisters.length;
	}
	
	/**
	 * @param register the constant of the special register.
	 * @return the value of the special register.
	 */
	
	public int getSpecialRegister(int register)
	{
		return specialRegisters[register];
	}
	
	/**
	 * @param register the constant of the special register.
	 * @param value the value to assign to the special register.
	 */
	
	public void setSpecialRegister(int register, int value)
	{
		specialRegisters[register] = value;
	}
	
	/**
	 * Resets all registers to 0.
	 */
	
	public void reset()
	{
		for(int i = 0; i < specialRegisters.length; i++)
		{
			specialRegisters[i] = 0;
		}
		
		for(int i = 0; i < generalRegisters.length; i++)
		{
			generalRegisters[i] = 0;
		}
	}
}
