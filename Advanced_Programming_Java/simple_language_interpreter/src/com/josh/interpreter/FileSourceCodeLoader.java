/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import com.josh.interpreter.Error.Severity;

public class FileSourceCodeLoader implements SourceCodeLoader
{
	private String filename;
	private StringBuffer sourceCode;
	
	public FileSourceCodeLoader()
	{
		filename = "";
		sourceCode = new StringBuffer();
	}
	
	public FileSourceCodeLoader(String filename)
	{
		this();
		setFilename(filename);
	}
	
	@Override
	public boolean load(ErrorList errorList) 
	{
		/*
		 * Load the file at filename.
		 * Read it line by line and store it into a String.
		 */
		
		BufferedReader reader;
		
		//open the file
		try
		{
			reader = new BufferedReader(new FileReader(getFilename()));
		} 
		catch (FileNotFoundException e) //file was not found
		{
			String error = String.format("The source file '%s' was not found.", getFilename());
			errorList.push(new Error(error,Severity.SEVERITY_ERROR));
			return false;
		}
		 
		sourceCode = new StringBuffer();
		String line = null;
		 
		//read line by line
		try 
		{
			while((line = reader.readLine())!=null)
			{
			   sourceCode.append(line);
			   
			   //append a new line until at last line
			   if(reader.ready())
			   {
				   sourceCode.append("\n");
			   }
			}
			reader.close();
			return true;
		}
		catch (IOException e) 
		{
			String error = "Problem reading the source code file.";
			errorList.push(new Error(error,Severity.SEVERITY_ERROR));
			return false;
		}
	}

	@Override
	public String get() 
	{
		return sourceCode.toString();
	}
	
	@Override
	public String getFilename() 
	{
		return filename;
	}
	
	/**
	 * sets the filename to be used in the load method.
	 * @param filename the full filename of the file containing the source code.
	 */
	
	public void setFilename(String filename) 
	{
		this.filename = filename;
	}

}
