/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

import java.util.ArrayList;
import java.util.Map;
import java.util.TreeMap;

import com.josh.interpreter.Error.Severity;

public class TMLCompiler implements Compiler
{
	private String[][] tokens;
	private Map<String, String> identifierMap;
	private OpcodeContainer opcodes;
	private ArrayList<Instruction> instructions;
	
	public TMLCompiler(OpcodeContainer opcodes)
	{
		this.opcodes = opcodes;
		this.identifierMap = new TreeMap<String, String>();
		this.instructions = new ArrayList<Instruction>();
	}
	
	/**
	 * @param input the input String to test.
	 * @return true if the input String is a number.
	 */
	
	private static boolean isNumber(String input)
	{
		 try  
		  {  
		    Double.parseDouble(input);  
		  }  
		  catch(NumberFormatException nfe)  
		  {  
		    return false;  
		  }  
		  return true; 
	}
	
	/**
	 * @param input the input String to test.
	 * @return true if the input String is an integer.
	 */
	
	private static boolean isInteger(String input)
	{
		   try
		   { 
		       Integer.parseInt(input); 
		   }
		   catch(NumberFormatException e)
		   { 
		       return false; 
		   }
		   return true;
	}
	
	/**
	 * @param input the input String to test.
	 * @return the integer version of the input String or 0 if it is not an integer.
	 */
	
	private static int parseInteger(String input)
	{
		int number = 0;
		
		   try
		   { 
		       number = Integer.parseInt(input); 
		   }
		   catch(NumberFormatException e)
		   { 
		       return number; 
		   }
		   return number;
	}
	
	/**
	 * Converts the input string into a 2D array of strings.
	 * Splits on white space. An empty line array has a single token with the value of an empty string.
	 * @param input
	 * @param errorList
	 * @return currently always returns true but may push a warning to the error list.
	 */
	
	private boolean tokenizeSourceCode(String input, ErrorList errorList)
	{
		//break the input into lines
		String[] lines = input.split("\n\r?|\r");
		//get the line count
		int lineCount = lines.length;
		//2D array of tokens
		tokens = new String[lineCount][];
		
		//read each line and tokenize
		for (int i = 0; i < lineCount; ++i)
		{
			//tokenize input based on tabs and spaces
			tokens[i] = lines[i].trim().split("[ \t]+");
		}
		
		//warn if nothing was really loaded
		if(tokens.length == 0)
		{
			errorList.push(new Error("0 lines of source code were processed.",Severity.SEVERITY_WARNING));
		}
		
		return true;
	}
	
	/**
	 * Checks if the parameter line contains code that should be parsed.
	 * @param line
	 * @return true if the parameter line contains code that should be parsed.
	 */
	
	private boolean isCodeLine(String[] line)
	{
		return line[0].length() > 0;
	}
	
	/**
	 * Checks that the identifier:
	 * -Is not already defined.
	 * -Is not a reserved keyword such as an operation mnemonic.
	 * -Is not a number.
	 * @param identifier
	 * @param lineNumber
	 * @param errorList
	 * @return true if the identifier is valid.
	 */
	
	private boolean validateIdentifier(String identifier, int lineNumber, ErrorList errorList)
	{
		boolean hasError = false;
		
		if(identifierMap.containsKey(identifier)) //identifier is already defined
		{
			String error = String.format(
					"The identifier '%s' is already defined.", identifier);
			
			errorList.push(new Error(error,Severity.SEVERITY_ERROR,lineNumber));
			hasError = true;
		}
		else if(opcodes.contains(identifier)) //identifier is an opcode
		{
			String error = String.format(
					"The identifier '%s' is a reserved keyword and cannot be used.", 
					identifier);
			
			errorList.push(new Error(error,Severity.SEVERITY_ERROR,lineNumber));
			hasError = true;
		}
		else if(isNumber(identifier)) //identifier is a number
		{
			String error = String.format(
					"The identifier '%s' is a number and cannot be used.", 
					identifier);
			
			errorList.push(new Error(error,Severity.SEVERITY_ERROR,lineNumber));
			hasError = true;
		}
		return !hasError;
	}
	
	/**
	 *  Converts the identifiers to numbers. The numbers represent the instruction index.
	 *  The identifier names are then mapped to their number for later lookup.
	 * @param errorList
	 * @return true if all identifiers were converted and mapped and valid.
	 */
	
	private boolean convertAndMapIdentifiersToNumbers(ErrorList errorList)
	{
		identifierMap.clear();
		boolean hasError = false;
		int identifierCount = 0;
		for(int i = 0; i < tokens.length; i++)
		{
			int lineNumber = i + 1;
			
			if(isCodeLine(tokens[i])) //not a blank line
			{
				String identifier = tokens[i][0];
				
				if(validateIdentifier(identifier, lineNumber, errorList))
				{
					String val = String.valueOf(identifierCount);
					identifierMap.put(identifier, val);
					tokens[i][0] = val;
				}
				else
				{
					hasError = true;
				}
				
				identifierCount++;
			}
		}
		return !hasError;
	}
	
	/**
	 * Replaces arguments that match an identifier with the identifier's instruction index.
	 * @param errorList
	 * @return currently always returns true.
	 */
	
	private boolean convertArgsToIdentifierNumbers(ErrorList errorList)
	{
		for(int i = 0; i < tokens.length; i++)
		{
			if(isCodeLine(tokens[i])) // not a blank line
			{
				for(int j = 0; j < tokens[i].length; j++)
				{
					String token = tokens[i][j];
					
					//if it matches an identifier, replace with line number
					if(identifierMap.containsKey(token))
					{
						String identifierNumber = identifierMap.get(token);
						tokens[i][j] = identifierNumber;
					}
				}
			}
		}
		
		return true;
	}
	
	/**
	 * Converts the tokenized source code into a list of instructions that can be executed.
	 * Checks the following:
	 * -At least an identifier and an opcode must be present.
	 * -The operation must exist.
	 * -The number of arguments passed to the operation must be the expected amount.
	 * -The arguments passed are integers.
	 * @param errorList
	 * @return true if all instructions were successfully generated.
	 */
	
	private boolean generateInstructions(ErrorList errorList)
	{
		final int MIN_TOKENS = 2; //identifier, opcode
		
		instructions.clear();
		boolean hasError = false;
		
		//go through each line of code
		for(int i = 0; i < tokens.length; i++)
		{
			final int lineNumber = i + 1;
			final int numArgs = tokens[i].length - MIN_TOKENS; //how many arguments passed to the operation
			Opcode operation = null;
			
			if(isCodeLine(tokens[i])) //not a blank line
			{
				//needs at least an identifier and an opcode
				if(tokens[i].length >= MIN_TOKENS)
				{
					//second token should be the opcode
					String op = tokens[i][1];
					
					if(opcodes.contains(op)) //opcode is valid
					{
						operation = opcodes.get(op);
					}
					else //opcode does not exist
					{
						String error = String.format("Invalid opcode '%s'.",op);
						errorList.push(new Error(error,Severity.SEVERITY_ERROR,lineNumber));
						hasError = true;
					}
				}
				else //not enough arguments
				{
					String error = "At least an identifier and an opcode are required.";
					errorList.push(new Error(error,Severity.SEVERITY_ERROR,lineNumber));
					hasError = true;
				}
				
				//if there is no operation, there is an error in the code
				if(operation != null) 
				{
					//invalid number of arguments specified for operation
					if(numArgs != operation.getArgumentCount())
					{
						String error = String.format(
								"Invalid number of arguments for operation '%s'.",operation.getMnemonic());
						errorList.push(new Error(error,Severity.SEVERITY_ERROR,lineNumber));
						hasError = true;
					}
					
					//create the argument array if necessary
					int[] arguments = null;
					if(numArgs > 0)
					{
						arguments = new int[numArgs];
						//make sure the arguments are integers
						if(!parseOperationArguments(MIN_TOKENS, tokens[i], arguments, lineNumber, errorList))
						{
							hasError = true;
						}
					}
					
					//generate the instruction
					Instruction instruction = new Instruction(operation, arguments);
					
					//add it to the list
					instructions.add(instruction);
				}
			}
		}
		
		return !hasError;
	}
	
	/**
	 * Parses arguments passed to an operation and makes sure they are integers.
	 * Stores them in the arguments array.
	 * @param startInArray the index of the first argument.
	 * @param tokenArray the array of tokens containing the arguments.
	 * @param arguments the array to store arguments that are valid.
	 * @param lineNumber the line number in the source code.
	 * @param errorList
	 * @return true if all arguments were successfully parsed and are valid.
	 */
	
	private boolean parseOperationArguments(
			int startInArray, String[] tokenArray, int[] arguments, int lineNumber, ErrorList errorList)
	{
		boolean hasError = false;
		//parse through the arguments
		int curArg = 0;
		for(int j = startInArray; j < tokenArray.length; j++)
		{
			String token = tokenArray[j];
			
			//it must be an integer
			if(isInteger(token))
			{
				int argVal = parseInteger(token);
				arguments[curArg] = argVal;
			}
			else //it is not an integer
			{
				String error = String.format("Invalid argument '%s'. Expected integer.",token);
				errorList.push(new Error(error,Severity.SEVERITY_ERROR,lineNumber));
				hasError = true;
			}
			curArg++; //increment which argument we are on
		}
		
		return !hasError;
	}
	
	@Override
	public Instruction[] getInstructions()
	{
		return instructions.toArray(new Instruction[0]);
	}
	
	@Override
	public boolean compile(String input, ErrorList errorList)
	{
		//parse the input string into a 2D token array
		if(!tokenizeSourceCode(input, errorList))
			return false;
	
		//convert the identifiers at the start of each line into array indexes
		if(!convertAndMapIdentifiersToNumbers(errorList))
			return false;
		
		//change the arguments for the operations to match the identifier numbers
		if(!convertArgsToIdentifierNumbers(errorList))
			return false;
		
		//generate the list of instructions
		if(!generateInstructions(errorList))
			return false;
		
		return true;
	}
}
