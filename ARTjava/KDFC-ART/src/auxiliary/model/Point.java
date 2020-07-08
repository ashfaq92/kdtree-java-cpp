package auxiliary.model;

public class Point {
	public int n; // ά��
	public float[] coordPoint; //�洢�õ����ά�ȵ�����ֵ

	public Point(int n) {
		this.n = n;
		this.coordPoint = new float[n];

	}
	
	public static double getDistance(Point p1, Point p2) {   //����������Ե�֮���ŷʽ����
		double sum = 0;
		for (int i = 0; i <  p1.n; i++) {
			sum=sum+Math.pow(p1.coordPoint[i] - p2.coordPoint[i], 2);
		}
		return Math.sqrt(sum);
		
	}
 
	public static Point generateRandP(int[][] inputDomain) { // ��ָ���ռ����������һ����������
		int n = inputDomain.length;
		Point newPoint = new Point(n);
		for (int i = 0; i < n; i++) {
			newPoint.coordPoint[i] = (float) (inputDomain[i][0] + (inputDomain[i][1] - inputDomain[i][0]) * Math.random());
		}
		return newPoint;
	}

}
