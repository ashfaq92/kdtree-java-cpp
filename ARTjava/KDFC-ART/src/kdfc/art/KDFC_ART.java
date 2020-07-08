package kdfc.art;

import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;



import auxiliary.model.Node;
import auxiliary.model.Point;


public class KDFC_ART {

	public Node root; // ���ڵ�
	public int size = 0; // ���нڵ����
	public int candidateNum = 10; // ��ѡ��������
	public int[][] inputDomain; // ������Χ

	public KDFC_ART() {

	}

	public KDFC_ART(int[][] bound) {
		super();
		root = new Node();
		inputDomain = bound;
	}

	public Stack<Node> getTreePath(Point p) { // ���p���ڸ����е�����·��

		Stack<Node> path = new Stack<>();
		Node pathNode = this.root;

		while (true) {
			path.push(pathNode);
			if (pathNode.p.coordPoint[pathNode.spilt] > p.coordPoint[pathNode.spilt]) { // ��p��pathNode���
				if (pathNode.left == null) {
					break;
				}
				pathNode = pathNode.left;
			} else { // ��p��pathNode�ұ�
				if (pathNode.right == null) {
					break;
				}
				pathNode = pathNode.right;
			}
		}
		return path;
	}

	public int judgeDirection(Point p, Node node) { // �жϵ�p��node�ڵ����߻����ұ� 0Ϊ��� 1Ϊ�ұ�
		if (p.coordPoint[node.spilt] < node.p.coordPoint[node.spilt]) {
			return 0;
		} else
			return 1;
	}

	public Boolean isCrossSpiltLine(Point p, double distance, Node node) { // �ж��Խڵ�pΪ���ģ�distanceΪ�뾶��Բ�Ƿ���node���ڵı߽����ཻ

		if (Math.abs(node.p.coordPoint[node.spilt] - p.coordPoint[node.spilt]) >= distance) { // �ֽ���
			return false;
		}
		return true;

	}

	public double getMinDisByBacktracking(Point p, int back) { // �����p�������нڵ����̾���-���ƻ���

		int num = 0; // ��¼��ǰ���ݽڵ�ĸ���
		Stack<Node> path = this.getTreePath(p);
		Node pathNode = null;
		double distance = Double.MAX_VALUE;

		while ((!path.isEmpty())) {
			pathNode = path.pop(); // ����·����pop��pathNode�ڵ�
			if (this.isCrossSpiltLine(p, distance, pathNode)) { // ��ǰdistance��PathNode���ڵı߽����ཻʱ���������һ�߲�ѯ
				double d = Point.getDistance(p, pathNode.p);
				if (distance > d) {
					distance = d;
				}
				num++;
				if (num == back) {
					return distance;
				}

				int direction = this.judgeDirection(p, pathNode);// �жϵ�p��pathNode����߻����ұ�
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
					while (!queue.isEmpty()) { // ���������й�����ȱ���
						tempNode = queue.poll();

						direction = this.judgeDirection(p, tempNode);

						if (this.isCrossSpiltLine(p, distance, tempNode)) { // ���p�ڵ�������ڵ�ı߽����ཻ���������ڵ����һ��push������
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

						if (direction == 0) { // ����ߣ����Ƚ���߽ڵ�push������
							if (tempNode.left != null) {
								queue.offer(tempNode.left);
							}

						} else { // ���ұߣ����Ƚ��ұ߽ڵ�push������
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

	public void insertPointByStrategy(Point p) { // ����ƽ�������tree�в����µĽڵ�p
		if (root.p == null) { // ���root�ڵ��е�p��Ϊ��
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
					if (ntemp.left == null) { // ntemp�����Ϊ�գ���n����Ϊ�������,���˳�ѭ��
						ntemp.left = n;
						break;
					}
					ntemp = ntemp.left;
				} else {
					if (ntemp.right == null) { // ntemp���ұ�Ϊ�գ���n����Ϊ�ұ����������˳�ѭ��
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
			if (n.p.coordPoint[ntemp.spilt] < ntemp.p.coordPoint[ntemp.spilt]) { // �ڸ��׽ڵ�����
				n.boundary[ntemp.spilt][1] = ntemp.p.coordPoint[ntemp.spilt];
			} else {
				n.boundary[ntemp.spilt][0] = ntemp.p.coordPoint[ntemp.spilt];
			}
			n.spilt = this.spiltSelect(n.boundary, p);
		}
		size++;
	}

	public void testLimBalKDFC_Efficiency(int pointNum, int[] backNum) throws IOException { // LimBalKDFC����Ч�ʲ���

		Point p = Point.generateRandP(inputDomain); // �������һ������
		this.insertPointByStrategy(p);
		Point finalCase;
		ArrayList<Point> canD;
		for (int i = 1; i < pointNum; i++) {
			canD = new ArrayList<>(); // ����������ѡ��
			for (int j = 0; j < candidateNum; j++) {
				canD.add(Point.generateRandP(inputDomain));
			}
			finalCase = canD.get(0);
			int back = backNum[this.size];
			double distance = this.getMinDisByBacktracking(finalCase, back);
			for (int c = 1; c < candidateNum; c++) {
				double d = this.getMinDisByBacktracking(canD.get(c), back); // �����С����
				if (distance < d) { // �����С���������Ǹ���ѡ��
					distance = d;
					finalCase = canD.get(c);
				}
			}
			this.insertPointByStrategy(finalCase);

		}
	}



}
