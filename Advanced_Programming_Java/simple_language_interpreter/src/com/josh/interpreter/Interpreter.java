/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public class Interpreter
{
	private ErrorList errorList;
	private FileSourceCodeLoader loader;
	private Compiler compiler;
	private Machine machine;
	private OpcodeContainer opcodes;
	
	public Interpreter()
	{
		errorList = new ErrorList();
		loader = new FileSourceCodeLoader();
		opcodes = new OpcodeContainer();
		loadOpcodes(opcodes);
		compiler = new TMLCompiler(opcodes);
		machine = new Machine();
	}
	
	/**
	 * Loads the opcodes used to perform various operations.
	 * Extending classes can override this method to add additional opcodes.
	 * @param opcodes the container to add opcodes to.
	 */
	
	protected void loadOpcodes(OpcodeContainer opcodes)
	{
		opcodes.add(new OpcodeAdd());
		opcodes.add(new OpcodeBnz());
		opcodes.add(new OpcodeDiv());
		opcodes.add(new OpcodeLin());
		opcodes.add(new OpcodeOut());
		opcodes.add(new OpcodeSub());
		opcodes.add(new OpcodeMul());
	}
	
	/**
	 * Prints any errors to the System.out and clears the error list.
	 */
	
	private void printAndClearErrors()
	{
		while(!errorList.empty())
		{
			Error e = errorList.pop();
			System.out.println(e);
		}
	}
	
	/**
	 * Loads and compiles the source code to an internal form.
	 * Prints errors if any.
	 * @param filename the filename of the source code to load.
	 * @return true if the source code was successfully loaded and compiled and is ready for execution.
	 */
	
	public boolean load(String filename)
	{
		loader.setFilename(filename);
		if(!loader.load(errorList))
		{
			printAndClearErrors();
			return false;
		}
		
		String src = loader.get();
		
		if(!compiler.compile(src,errorList))
		{
			printAndClearErrors();
			return false;
		}
		
		return true;
	}
	
	/**
	 * @return the source code that was loaded if a call to load has succeeded.
	 */
	
	public String getSourceString()
	{
		return loader.get();
	}
	
	/**
	 * Loads the compiled instruction list and executes it.
	 * @return true if at least 1 instruction was executed.
	 */
	
	public boolean run()
	{
		Instruction[] instructions = compiler.getInstructions();
		
		machine.load(instructions);
		return machine.run();
	}
	
	/**
	 * Prints the contents of each register of the virtual machine to System.out.
	 */
	
	public void printRegisters()
	{
		MachineModel model = machine.getModel();
		int regCount = model.getRegisterCount();
		
		for(int i = 0; i < regCount; i++)
		{
			int regVal = model.getRegister(i);
			System.out.println(String.format("R%d:%d", i,regVal));
		}
	}
}
