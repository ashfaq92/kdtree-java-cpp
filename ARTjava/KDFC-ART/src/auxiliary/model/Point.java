package auxiliary.model;
import java.util.ArrayList;

public class Point {
	public int n; // ά��
	public float[] coordPoint; //�洢�õ����ά�ȵ�����ֵ

	public Point(int n) {
		this.n = n;
		this.coordPoint = new float[n];

	}
	
	public static double getDistance(Point p1, Point p2) {   //returns euclidean distance between two points
		double sum = 0;
		for (int i = 0; i <  p1.n; i++) {
			sum=sum+Math.pow(p1.coordPoint[i] - p2.coordPoint[i], 2);
		}
		return Math.sqrt(sum);
		
	}


	public void printPoint() {  //to visualize points on https://www.desmos.com/calculator
		System.out.print("(");
		for (int i=0; i < this.coordPoint.length; i++) {
			if (i != (this.coordPoint.length - 1)) {
				System.out.print(this.coordPoint[i]+",");
			}
			else {
				System.out.print(this.coordPoint[i]);
			}
		}
		System.out.print("),");
	}
 
	public static Point generateRandP(int[][] inputDomain) { // generate a random point in given input domain
		int n = inputDomain.length;
		Point newPoint = new Point(n);
		for (int i = 0; i < n; i++) {
			newPoint.coordPoint[i] = (float) (inputDomain[i][0] + (inputDomain[i][1] - inputDomain[i][0]) * Math.random());
		}
		return newPoint;
	}

}
