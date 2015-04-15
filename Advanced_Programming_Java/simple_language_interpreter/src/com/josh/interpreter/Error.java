/*
 * Author: Joshua Larouche
 * Date: February 2, 2014
 */
package com.josh.interpreter;

public class Error 
{
	public enum Severity
	{
		SEVERITY_WARNING,
		SEVERITY_ERROR
	}
	
	public static final int NO_LINE_NUMBER = -1;
	
	private String error;
	private Severity severity;
	private int lineNumber;
	
	public Error()
	{
		this.error = "";
		this.severity = Severity.SEVERITY_ERROR;
		this.lineNumber = NO_LINE_NUMBER;
	}
	
	public Error(String error, Severity severity, int lineNumber)
	{
		this();
		this.error = error;
		this.severity = severity;
		this.lineNumber = lineNumber;
	}
	
	public Error(String error, Severity severity)
	{
		this(error,severity,NO_LINE_NUMBER);
	}
	
	/**
	 * @return the line number of the error or the constant NO_LINE_NUMBER.
	 */
	
	public int getLineNumber()
	{
		return lineNumber;
	}
	
	/** 
	 * @return the severity of the error (Warning or Error).
	 */
	
	public Severity getSeverity()
	{
		return severity;
	}
	
	/**
	 * @return the description of the error.
	 */
	
	public String get()
	{
		return error;
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if(obj instanceof Error)
		{
			Error e = (Error)obj;
			return e.get().equals(get())
			&&     e.getLineNumber() == getLineNumber()
			&&     e.getSeverity() == getSeverity();
		}
		
		return false;
	}
	
	@Override
	public String toString()
	{
		String isError = "";
		String onLine = getLineNumber() == Error.NO_LINE_NUMBER ?
				"" :  " on line " + String.valueOf(getLineNumber());
		
		switch(getSeverity())
		{
		case SEVERITY_ERROR:
			isError = "Error";
			break;
		case SEVERITY_WARNING:
			isError = "Warning";
			break;
		default:
			break;
		}
		
		return String.format("%s%s: %s", isError,onLine,get());
	}
}
