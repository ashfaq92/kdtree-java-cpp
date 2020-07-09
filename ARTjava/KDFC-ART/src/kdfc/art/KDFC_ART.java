package kdfc.art;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;



import auxiliary.model.Node;
import auxiliary.model.Point;


public class KDFC_ART {

	public Node root; //Root node
	public int size = 0; // Number of nodes in the tree
	public int[][] inputDomain; // Enter the range of the domain

	public KDFC_ART() {

	}

	public KDFC_ART(int[][] bound) {
		super();
		root = new Node();
		inputDomain = bound;
	}

	public Stack<Node> getTreePath(Point p) { // Get the search path of the p-point in the tree

		Stack<Node> path = new Stack<>();
		Node pathNode = this.root;

		while (true) {
			path.push(pathNode);
			if (pathNode.p.coordPoint[pathNode.spilt] > p.coordPoint[pathNode.spilt]) { //Point p on the left side of the pathNode
				if (pathNode.left == null) {
					break;
				}
				pathNode = pathNode.left;
			} else { // Point p on the right side of the pathNode
				if (pathNode.right == null) {
					break;
				}
				pathNode = pathNode.right;
			}
		}
		return path;
	}

	public int judgeDirection(Point p, Node node) { // The judgment point p is on the left or right of the node node 0 is the left 1 is the right
		if (p.coordPoint[node.spilt] < node.p.coordPoint[node.spilt]) {
			return 0;
		} else
			return 1;
	}

	public Boolean isCrossSpiltLine(Point p, double distance, Node node) { //Determines whether a circle centered on node p and distance radius intersects the boundary line where the node is located

		if (Math.abs(node.p.coordPoint[node.spilt] - p.coordPoint[node.spilt]) >= distance) { // �ֽ���
			return false;
		}
		return true;

	}

	public double getMinDisByBacktracking(Point p, int back) { //  Find the shortest distance from point p to the node in the tree - limit inglisal

		int num = 0; // The number of current backtracking nodes recorded
		Stack<Node> path = this.getTreePath(p);
		Node pathNode = null;
		double distance = Double.MAX_VALUE;

		while ((!path.isEmpty())) {
			pathNode = path.pop(); // Pop out of the pathNode node from the main path
			if (this.isCrossSpiltLine(p, distance, pathNode)) { // When the current boundary line where the distance and PathNode are located intersect, you need to go to the other side of the query
				double d = Point.getDistance(p, pathNode.p);
				if (distance > d) {
					distance = d;
				}
				num++;
				if (num == back) {
					return distance;
				}

				int direction = this.judgeDirection(p, pathNode);  //The judgment point p is on the left or right of the pathNode
				Node tempNode = null;

				if (direction == 0) {
					if (pathNode.right != null) {
						tempNode = pathNode.right;
					}
				} else {
					if (pathNode.left != null) {
						tempNode = pathNode.left;
					}
				}

				if (tempNode != null) {
					Queue<Node> queue = new LinkedList<>();
					queue.offer(tempNode);
					while (!queue.isEmpty()) { //  Breadth-first traversal of subtrees
						tempNode = queue.poll();

						direction = this.judgeDirection(p, tempNode);

						if (this.isCrossSpiltLine(p, distance, tempNode)) { // If the boundary line of the p-node and the subtree node intersects, push the other side of the child tree node into the queue
							d = Point.getDistance(p, tempNode.p);
							if (distance > d) {
								distance = d;
							}
							num++;
							if (num == back) {
								return distance;
							}
							if (direction == 1) {
								if (tempNode.left != null) {
									queue.offer(tempNode.left);
								}
							} else {
								if (tempNode.right != null) {
									queue.offer(tempNode.right);
								}
							}
						} else {
							num++;
							if (num == back) {
								return distance;
							}
						}

						if (direction == 0) { //  On the left, first push the left node into the queue
							if (tempNode.left != null) {
								queue.offer(tempNode.left);
							}

						} else { //  On the right, first push the right node into the queue�
							if (tempNode.right != null) {
								queue.offer(tempNode.right);
							}
						}
					}
				}
			} else {
				num++;
				if (num == back) {
					return distance;
				}
			}

		}
		return distance;
	}

	public int spiltSelect(float[][] boundary, Point p) { // ����ƽ����� ѡ����ѷָ�ά��
		double rate = 0;
		int spilt = 0;
		for (int i = 0; i < p.n; i++) {
			double length = boundary[i][1] - boundary[i][0]; // �߳�
			double lx1 = boundary[i][1] - p.coordPoint[i];
			double lx2 = p.coordPoint[i] - boundary[i][0];
			double spread = length * (1 - (lx1 / length) * (lx1 / length) - (lx2 / length) * (lx2 / length));
			if (rate < spread) {
				rate = spread;
				spilt = i;
			}
		}
		return spilt;
	}

	public void insertPointByStrategy(Point p) { // Insert a new node p into the tree using a balancing strategy
		if (root.p == null) { //   If the p-point in the root node is empty
			root.deep = 1;
			root.p = p;
			root.boundary = new float[p.n][2];
			for (int i = 0; i < p.n; i++) {
				root.boundary[i][0] = this.inputDomain[i][0];
				root.boundary[i][1] = this.inputDomain[i][1];
			}
			root.spilt = this.spiltSelect(root.boundary, p);
		} else {
			Node ntemp = root;
			Node n = new Node();
			while (true) {
				if (ntemp.p.coordPoint[ntemp.spilt] > p.coordPoint[ntemp.spilt]) {
					if (ntemp.left == null) { //The left side of ntemp is empty, sets n to the left subtree, and exits the loop
						ntemp.left = n;
						break;
					}
					ntemp = ntemp.left;
				} else {
					if (ntemp.right == null) { // The right side of ntemp is empty, sets n to the right subtree, and exits the loop
						ntemp.right = n;
						break;
					}
					ntemp = ntemp.right;
				}

			}
			n.p = p;
			n.boundary = new float[p.n][2];
			n.deep = ntemp.deep + 1;
			for (int i = 0; i < p.n; i++) {
				n.boundary[i][0] = ntemp.boundary[i][0];
				n.boundary[i][1] = ntemp.boundary[i][1];
			}
			if (n.p.coordPoint[ntemp.spilt] < ntemp.p.coordPoint[ntemp.spilt]) { // On the left side of the father node
				n.boundary[ntemp.spilt][1] = ntemp.p.coordPoint[ntemp.spilt];
			} else {
				n.boundary[ntemp.spilt][0] = ntemp.p.coordPoint[ntemp.spilt];
			}
			n.spilt = this.spiltSelect(n.boundary, p);
		}
		size++;
	}

	// public void testLimBalKDFC_Efficiency(int pointNum, int[] backNum) throws IOException { // LimBalKDFC����Ч�ʲ���

	// 	Point p = Point.generateRandP(inputDomain); // �������һ������
	// 	this.insertPointByStrategy(p);
	// 	Point finalCase;
	// 	ArrayList<Point> canD;
	// 	for (int i = 1; i < pointNum; i++) {
	// 		canD = new ArrayList<>(); // ����������ѡ��
	// 		for (int j = 0; j < candidateNum; j++) {
	// 			canD.add(Point.generateRandP(inputDomain));
	// 		}
	// 		finalCase = canD.get(0);
	// 		int back = backNum[this.size];
	// 		double distance = this.getMinDisByBacktracking(finalCase, back);
	// 		for (int c = 1; c < candidateNum; c++) {
	// 			double d = this.getMinDisByBacktracking(canD.get(c), back); // �����С����
	// 			if (distance < d) { // �����С���������Ǹ���ѡ��
	// 				distance = d;
	// 				finalCase = canD.get(c);
	// 			}
	// 		}
	// 		this.insertPointByStrategy(finalCase);
	// 	}
	// }

	public int buildIndex(ArrayList<Point> dataSet, boolean debug) {  //returns KDTree of  data-set
		double d = this.inputDomain.length;  // dimensionality of input domain
		int back = 0;

		int[] backNum = new int[dataSet.size()];
		backNum[1] = 1;
		for (int i = 2; i < dataSet.size(); i++) {
			backNum[i] = (int) Math.ceil(1 / 2.0 * Math.pow((d + 1 / d), 2) * (Math.log(i) / Math.log(2)));
		}
		for (Point p : dataSet) {
			back = backNum[this.size];
			this.insertPointByStrategy(p);
			if (debug){p.printPoint();}   // if debug is true, print the dataset point to visualize them on www.desmos.com/calculator
		}
		return back;
	}



	public static void main(String[] args) {
		System.out.println("Hello World");
		// input domain. also called input space. 
		//In this case 2 dimensional input domain is created. each dimension represents axis on coordinate plane 
		int[][] D = {{-5000, 5000}, {-5000, 5000}};  
		ArrayList<Point> randData = new ArrayList<>();  // generate 100 vectors/data points in input domain
		for (int i = 0; i < 500; i++) {
			Point x = Point.generateRandP(D);
			randData.add(x);
		}

		KDFC_ART myKdTree = new KDFC_ART(D);
		int back = myKdTree.buildIndex(randData, true);

		Point q = Point.generateRandP(D);   // generate a query point for neares neighbor search
		double minDist = myKdTree.getMinDisByBacktracking(q, back);  //get the distance of nearest neighbor to q
		System.out.println("\n\t"+minDist);
		q.printPoint();

		System.out.println("END");
		
	}



}
