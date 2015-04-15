/*
 * Name: Joshua Larouche
 * Date: April 5th, 2014
 * Description: Assignment 5 L-Systems Drawing
 */
import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.FileNotFoundException;

import javax.swing.*;
import java.util.*;

class L_Gui extends JFrame {

    /**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	/** default constructor  */
    public L_Gui() {
        initComponents();
    }
    
    // all the initializations in one method
    private void initComponents() {
      
        lblGeneration		= new JLabel("Generation");
        lblCode		= new JLabel("Axiom");
        lblProd		= new JLabel("F Rule");
        lblStepAng		= new JLabel("Angle");
        lblStepLength		= new JLabel("Scale");
        lblDrawDirX		= new JLabel("X-coor");
        lblDrawDirY		= new JLabel("Y-coor");
        lblDrawAng		= new JLabel("Direction");
        lblXProd	= new JLabel("X Rule");
        lblYProd	= new JLabel("Y Rule");
        jSpace1		= new JLabel("");
        jSpace2		= new JLabel("");
        jSpace3		= new JLabel("");

        txtGeneration	= new JTextField();
        txtCode	= new JTextField();
        txtProduction	= new JTextField();
        txtStepAngle	= new JTextField();
        txtStepLength	= new JTextField();
        txtDrawDirX	= new JTextField();
        txtDrawDirY	= new JTextField();
        txtDrawAng	= new JTextField();
        txtXProd    = new JTextField();
        txtYProd    = new JTextField();
        
        
        drawButton	= new JButton("Draw");
        loadButton	= new JButton("Load" );
        
        txtGeneration.setText("0");
        txtDrawAng.setText("0");
        txtDrawDirX.setText("0");
        txtDrawDirY.setText("0");
        txtStepLength.setText("15");
        txtStepAngle.setText("60");

        // make a drawing surface
          p = new DrawBot();
          p.setBackground(new Color(255, 255, 255));
        
        // make the panel of buttons
          controlPanel = new JPanel();
          controlPanel.setPreferredSize( new Dimension(100,700));
          controlPanel.setLayout(new FlowLayout());
          controlPanel.setBackground(new Color(225, 225, 123));
          
          setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // set up drawButton
          drawButton.setPreferredSize( new Dimension(80,25));
          drawButton.setBackground(new Color(235, 235, 235));
          drawButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                handler(evt);
            }
          });
          
          txtGeneration.setPreferredSize( new Dimension(80,25));
          txtCode.setPreferredSize( new Dimension(80,25));
          txtProduction.setPreferredSize( new Dimension(80,25));
          txtStepAngle.setPreferredSize( new Dimension(80,25));
          txtStepLength.setPreferredSize( new Dimension(80,25));
          txtDrawDirX.setPreferredSize( new Dimension(80,25));
          txtDrawDirY.setPreferredSize( new Dimension(80,25));
          txtDrawAng.setPreferredSize( new Dimension(80,25));
          txtXProd.setPreferredSize( new Dimension(80,25));
          txtYProd.setPreferredSize( new Dimension(80,25));
          
          txtGeneration.setHorizontalAlignment(JTextField.CENTER);
          txtCode.setHorizontalAlignment(JTextField.CENTER);
          txtProduction.setHorizontalAlignment(JTextField.CENTER);
          txtStepAngle.setHorizontalAlignment(JTextField.CENTER);
          txtStepLength.setHorizontalAlignment(JTextField.CENTER);
          txtDrawDirX.setHorizontalAlignment(JTextField.CENTER);
          txtDrawDirY.setHorizontalAlignment(JTextField.CENTER);
          txtDrawAng.setHorizontalAlignment(JTextField.CENTER);
          txtXProd.setHorizontalAlignment(JTextField.CENTER);
          txtYProd.setHorizontalAlignment(JTextField.CENTER);
          
          
          jSpace1.setPreferredSize( new Dimension(80,20));
          jSpace2.setPreferredSize( new Dimension(80,20));
          jSpace3.setPreferredSize( new Dimension(80,20));

        // Fix up loadButton
          loadButton.setPreferredSize(new Dimension(80,25));
          loadButton.setBackground(new Color(235, 235, 235));
          loadButton.addActionListener(new ActionListener() {
             public void actionPerformed(ActionEvent evt){
                  // *******   put your code to load L-SYSTEM
            	 try {
            		 Scanner in = new Scanner(new File("l-system1.txt"));
            		 
            		txtCode.setText(in.nextLine());
            		txtProduction.setText(in.nextLine());
            		txtStepAngle.setText(in.nextLine());
            		txtStepLength.setText(in.nextLine());
            		txtDrawDirX.setText(in.nextLine());
            		txtDrawDirY.setText(in.nextLine());
            		txtDrawAng.setText(in.nextLine());
            		txtXProd.setText("");
            		txtYProd.setText("");
            		txtGeneration.setText("0");
                	 
            	 }
            	 catch (FileNotFoundException e){
            		 System.out.println("NO File found");
            		 System.exit(0);
            	 }
             }
          });
        
        // Add the components to the control panel
          controlPanel.add(jSpace1);
          controlPanel.add(lblCode); 
          controlPanel.add(txtCode);        
          controlPanel.add(lblProd); 
          controlPanel.add(txtProduction);   
          controlPanel.add(lblXProd); 
          controlPanel.add(txtXProd);  
          controlPanel.add(lblYProd); 
          controlPanel.add(txtYProd);  
          controlPanel.add(lblStepAng);    
          controlPanel.add(txtStepAngle);
          controlPanel.add(lblStepLength);   
          controlPanel.add(txtStepLength);
          controlPanel.add(lblDrawDirX); 
          controlPanel.add(txtDrawDirX);
          controlPanel.add(lblDrawDirY); 
          controlPanel.add(txtDrawDirY);
          controlPanel.add(lblDrawAng); 
          controlPanel.add(txtDrawAng);
          controlPanel.add(jSpace2);
          controlPanel.add(loadButton);
          controlPanel.add(drawButton);
          controlPanel.add(jSpace3);
          controlPanel.add(lblGeneration);
          controlPanel.add(txtGeneration);        
         
        // add the panels to the L_GUI
          add(controlPanel,BorderLayout.WEST);
          getContentPane().add(p,BorderLayout.CENTER);

          pack();
          setVisible(true);
    } // _________  end of InitComponents

    
    /* method that will be called to handle reading in of file
    private void jTextField3ActionPerformed(ActionEvent evt) {
           // **********************    Add your handling code here:
    }*/
    
    // method that is called when a button is pressed
    private void handler(ActionEvent evt) {
      if(evt.getSource()==drawButton) {
    	boolean gotData = p.getData();
          
          if(gotData) {
        	  p.repaint();
        	  	 int generation = 0;
                 try {
               	 generation =  Integer.parseInt(txtGeneration.getText());
                 }
                catch(Exception e) {
                	
                }
                 
             generation++;
             txtGeneration.setText(String.valueOf(generation));  
          }
      }
     
    }

   private class DrawBot extends JPanel {

		private class DrawDirection {
			public int x;
			public int y;
			public double angle;
		}

		private static final long serialVersionUID = 1L;
		private static final int MAX_GEN = 8;

		private int stepLength;

		private double stepAngle;

		private String code; // the drawing instructions
		private String production; // the drawing instructions
		private String prodX;
		private String prodY;
		private DrawDirection drawDirection;
		private int generation;

		Color drawingColor = Color.red;

		public DrawBot() {

			this.code = "";
			this.production = "";
			this.prodX = "";
			this.prodY = "";
			this.drawDirection = new DrawDirection();

			this.setBackground(Color.white);

			this.setPreferredSize(new Dimension(700, 700));
			
			// initial drawing coordinates at the center (i.e. height / 2 and
			// width / 2 )

			drawDirection.x = this.getPreferredSize().width / 2;
			drawDirection.y = this.getPreferredSize().height / 2;

		}
		
	      /**
	       * Gets data from TextFields and loads it into the drawbot
	       */
	      public boolean getData(){
	    	  try {
	    	   	  code = txtCode.getText();
		    	  production = txtProduction.getText();
		    	  prodX = txtXProd.getText();
		    	  prodY = txtYProd.getText();
		    	  stepAngle = Double.parseDouble(txtStepAngle.getText());
		    	  stepLength = (int)Double.parseDouble(txtStepLength.getText());
		    	  drawDirection.x = (int)Double.parseDouble(txtDrawDirX.getText());
		    	  drawDirection.y = (int)Double.parseDouble(txtDrawDirY.getText());
		    	  drawDirection.angle = Double.parseDouble(txtDrawAng.getText()) - 180;
		    	  generation = Integer.parseInt(txtGeneration.getText());
		    	  
		    	  for(int i = 0; i < generation && i <= MAX_GEN; i++) {
		    		  expandCode();
		    	  }
	    	  }
	    		  
	          catch(Exception e)
	          {
	        	  return false;
	    	  }
	    	  
	    	  return true;
	      }
	      

		public void changeCode(String newCode) {

			code = newCode;
			repaint();
		}

		public void expandCode() {
			String temp = code;
			code = "";
			for (int i = 0; i < temp.length(); i++) {

				switch (temp.charAt(i)) {
				case 'F':
				case 'f':
					code += this.production;
					break;
				case 'X':
				case 'x':
					code += this.prodX;
					break;
				case 'Y':
				case 'y':
					code += this.prodY;
					break;
				default:
					code += temp.charAt(i);
				}
			}
			repaint();
		}

		public void paint(Graphics g) {

			// always call super-paint before anything else

			super.paint(g);
			Stack<DrawDirection> directions = new Stack<DrawDirection>();

			// make copies of the DrawBot variables because we don't want

			// to change the instance variables of the Bot itself

			int penX = this.drawDirection.x;

			int penY = this.drawDirection.y;

			int newX = penX;

			int newY = penY;

			double angle = this.drawDirection.angle;

			double radAngle = 0;

			// let's paint

			g.setColor(drawingColor);

			//g.drawString(" TESTING ", 100, 100);

			//g.drawString(" code length : " + code.length(), 100, 140);

			// g.drawString( code, 100 , 150);

			// and now the “drawing” code

			for (int k = 0; k < code.length(); k++) {

				char c = code.charAt(k);

				switch (c) {

				case '-':
					angle += stepAngle;

					break;

				case '+':
					angle -= stepAngle;

					break;

				case 'F':
					radAngle = Math.PI * (angle) / 180.0;

					newX = penX + (int) (stepLength * Math.cos(radAngle));

					newY = penY + (int) (stepLength * Math.sin(radAngle));

					g.drawLine(penX, penY, newX, newY);

					penX = newX;

					penY = newY;

					break;
				case '[':
					DrawDirection d = new DrawDirection();
					d.x = penX;
					d.y = penY;
					d.angle = angle;
					directions.push(d);
					break;
				case ']':
					if(directions.empty()) {
						throw new NullPointerException();
					}
					DrawDirection old = directions.pop();
					penX = old.x;
					penY = old.y;
					angle = old.angle;
					break;
				default:

				}

			}

		}

	}
   
 // Variable declarations
    private JLabel		lblGeneration;
    private JLabel		lblCode;
    private JLabel		lblProd;
    private JLabel		lblStepAng;
    private JLabel		lblStepLength;
    private JLabel		lblDrawDirX;
    private JLabel		lblDrawDirY;
    private JLabel		lblDrawAng;
    private JLabel		lblXProd;
    private JLabel		lblYProd;
    private JLabel		jSpace1;
    private JLabel		jSpace2;
    private JLabel		jSpace3;
    
    private JTextField	txtGeneration;
    private JTextField	txtCode;
    private JTextField	txtProduction;
    private JTextField	txtXProd;
    private JTextField	txtYProd;
    private JTextField	txtStepAngle;
    private JTextField	txtStepLength;
    private JTextField	txtDrawDirX;
    private JTextField	txtDrawDirY;
    private JTextField	txtDrawAng;
    
    private JButton		drawButton;
    private JButton		loadButton;    

    private DrawBot	p;
    private JPanel		controlPanel;

}
