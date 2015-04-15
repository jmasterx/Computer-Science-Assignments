/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

import java.util.Map;
import java.util.TreeMap;

public class OpcodeContainer 
{
	private Map<String, Opcode> opcodes;
	
	public OpcodeContainer()
	{
		opcodes = new TreeMap<String, Opcode>();
	}
	
	/**
	 * Adds an operation that can be executed by an interpreter.
	 * @param opcode
	 * @return true if the operation was not already contained and was added.
	 */
	
	public boolean add(Opcode opcode)
	{
		if(!opcodes.containsKey(opcode.getMnemonic()))
		{
			opcodes.put(opcode.getMnemonic(), opcode);
			return true;
		}
		
		return false;
	}
	
	/**
	 * Removes the parameter operation.
	 * @param opcode the operation to remove.
	 * @return true if the parameter operation was removed.
	 */
	
	public boolean remove(Opcode opcode)
	{
		if(opcodes.containsKey(opcode.getMnemonic()))
		{
			opcodes.remove(opcode.getMnemonic());
			return true;
		}
		
		return false;
	}
	
	/**
	 * @param mnemonic the mnemonic of the Opcode to search for.
	 * @return true if an operation with the parameter mnemonic is contained.
	 */
	
	public boolean contains(String mnemonic)
	{
		return opcodes.containsKey(mnemonic);
	}
	
	/**
	 * @param mnemonic the mnemonic of the Opcode to search for.
	 * @return the Opcode with the parameter mnemonic or null if not found.
	 */
	
	public Opcode get(String mnemonic)
	{
		return opcodes.get(mnemonic);
	}
}
