
//Created by Joshua Larouche on 1/14/14
//CS321 Assignment 1
import java.util.*;
public class Larojos_assign01 {

	public enum BagBall
	{
		RED_BALL,
		BLUE_BALL
	}
	
	/**prints the winning ball
	 * 
	 * @param winner
	 */
	public static void bagBallPrint(BagBall winner)
	{
		String winColor;
		winColor = winner == BagBall.RED_BALL ? "Red Wins!" : "Blue Wins!";
		System.out.println( winColor);
	}
	
	/**runs a game of bagball
	 * 
	 */
	public static void bagBallRun()
	{
		Random r = new Random();
		
		//create between 60 and 80 balls
		final int numBalls = 60 + r.nextInt(21);
		
		ArrayList<BagBall> balls = new ArrayList<BagBall>();
		
		//fill the array with random red and blue balls
		for(int i = 0; i < numBalls; i++)
		{
			BagBall b = r.nextBoolean() ? BagBall.RED_BALL : BagBall.BLUE_BALL;
			
			balls.add(b);
		}
		
		while(balls.size() > 1)
		{
			BagBall a = balls.remove(r.nextInt(balls.size()));
			BagBall b = balls.remove(r.nextInt(balls.size()));
			
			if(a == b)
			{
				balls.add(BagBall.BLUE_BALL);
			}
			else
			{
				balls.add(BagBall.RED_BALL);
			}
		}
		
		//winner is the last ball remaining
		BagBall winner = balls.get(0);
		
		bagBallPrint(winner);
	}

	/**returns true if the specified index is in the bounds of the image array
	 * 
	 * @param image
	 * @param r
	 * @param c
	 * @return true if in range
	 */
	public static boolean smoothIndexInRange(byte[][] image, int r, int c)
	{
		//images have consistent column lengths
		final int numRows = image.length;
		final int numCols = image[0].length;
		
		return r >= 0 && r < numRows &&
			   c >= 0 && c < numCols;
	}
	
	/**get the average of the neighbors that exist
	 * 
	 * @param image
	 * @param r
	 * @param c
	 * @return average of pixel's neighbors
	 */
	public static byte smoothComputeAverage(byte[][] image, int r, int c)
	{
		int sum = 0;
		int divisor = 0;
		
		for(int i = r - 1; i <= r + 1; i++)
		{
			for(int j = c - 1; j <= c + 1; j++)
			{
				if(smoothIndexInRange(image, i, j))
				{
					sum += image[i][j];
					divisor++;
				}
			}
		}
		
		return (byte)(sum / divisor);
	}
	
	/**applies nearest neighbor linear filtering to the image
	 * 
	 * @param image
	 * @return 2d byte array representing the smoothed image
	 */
	public static byte[][] smoothSmoothed(byte[][] image)
	{
		//images have consistent column lengths
		final int numRows = image.length;
		final int numCols = image[0].length;
		
		byte[][] newImage = new byte[numRows][numCols];
		
		for(int i = 0; i < numRows; i++)
		{
			for(int j = 0; j < numCols; j++)
			{
				newImage[i][j] = smoothComputeAverage(image, i, j);
			}
		}
		
		return newImage;
	}
	
	/**prints an image using its numeric representation
	 * 
	 * @param image
	 */
	public static void smoothPrintImageNumbers(byte[][] image)
	{
		final int numRows = image.length;
		final int numCols = image[0].length;

		for(int i = 0; i < numRows; i++)
		{
			for(int j = 0; j < numCols; j++)
			{
				System.out.printf("%2d ", image[i][j]);
			}
			
			System.out.println();
		}
	}
	
	/**prints the image using ascii characters
	 * 
	 * @param image
	 */
	public static void smoothPrintImageCharacters(byte[][] image)
	{
		final int numRows = image.length;
		final int numCols = image[0].length;
		
	    final char[] values = {' ','.',',','-','+','o','O','X'};

		for(int i = 0; i < numRows; i++)
		{
			for(int j = 0; j < numCols; j++)
			{
				int index =  Math.min(image[i][j] / values.length,7);
				System.out.print(values[index]);
			}
			
			System.out.println();
		}
		
	}
	
	/**generates a random image
	 * 
	 * @return 2d byte array representing the random image
	 */
	public static byte[][] smoothGenerateRandomImage()
	{
		final int MAX_BRIGHTNESS = 64;
		Random r = new Random();
		
		final int numRows = 10 + r.nextInt(5);
		final int numCols = numRows;
		
		byte[][] image = new byte[numRows][numCols];
		
		for(int i = 0; i < numRows; i++)
		{
			for(int j = 0; j < numCols; j++)
			{
				image[i][j] = (byte)r.nextInt(MAX_BRIGHTNESS);
			}
		}
		
		return image;
	}
	
	public static void main(String[] args)
	{
		//run the bagball game, also prints the result
		System.out.println("Running BagBall...");
		bagBallRun();
		
		byte[][] image = {{0,0,0,0,0,0,0,0,0,0,0,0},
						  {0,0,0,0,0,0,0,0,0,0,0,0},
						  {0,0,5,5,5,5,5,5,5,5,0,0},
						  {0,0,5,5,5,5,5,5,5,5,0,0},
						  {0,0,5,5,5,5,5,5,5,5,0,0},
						  {0,0,5,5,5,5,5,5,5,5,0,0},
						  {0,0,5,5,5,5,5,5,5,5,0,0},
						  {0,0,5,5,5,5,5,5,5,5,0,0},
						  {0,0,5,5,5,5,5,5,5,5,0,0},
						  {0,0,5,5,5,5,5,5,5,5,0,0},
						  {0,0,0,0,0,0,0,0,0,0,0,0},
						  {0,0,0,0,0,0,0,0,0,0,0,0}};
		
		byte[][] newImage = smoothSmoothed(image);
		
		byte[][] detailedImage = smoothGenerateRandomImage();
		byte[][] smoothDetailImage = smoothSmoothed(detailedImage);
	
		System.out.println("Initial image numeric:");
		System.out.println("Input:");
		smoothPrintImageNumbers(image);
		System.out.println("Output:");
		smoothPrintImageNumbers(newImage);
		System.out.println("Random image numeric:");
		System.out.println("Input:");
		smoothPrintImageNumbers(detailedImage);
		System.out.println("Output:");
		smoothPrintImageNumbers(smoothDetailImage);
		
		System.out.println("Initial image ascii:");
		System.out.println("Input:");
		smoothPrintImageCharacters(image);
		System.out.println("Output:");
		smoothPrintImageCharacters(newImage);
		System.out.println("Random image ascii:");
		System.out.println("Input:");
		smoothPrintImageCharacters(detailedImage);
		System.out.println("Output:");
		smoothPrintImageCharacters(smoothDetailImage);
	}
}
