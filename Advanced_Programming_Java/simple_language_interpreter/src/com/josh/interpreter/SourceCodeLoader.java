/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public interface SourceCodeLoader 
{
	/**
	 * Attempts to load the file at the specified filename or data if not using a file handle.
	 * @param errorList the error list used to provide more detailed error messages.
	 * @return true if the data was successfully loaded.
	 */
	
	public boolean load(ErrorList errorList);

	/**
	 * @return the filename if this loader uses a file handle.
	 * Can return null if a file handle is not used.
	 */
	
	public String getFilename();
	
	/**
	 * @return the String representation after loading has successfully completed.
	 * Can return null if loading has not succeeded.
	 */
	
	public String get();
}
