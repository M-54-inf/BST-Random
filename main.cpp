#include <iostream>
//#include<ctime>
#include <time.h>
#include <math.h>
#include "Tree.h"

using namespace std;
typedef int DataType;
typedef int KeyType;

typedef unsigned long long INT_64;
//��������� ��������� ����� ������� �����������
//���������� � ��������� ���������� LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;
//������� ��������� ������� ���������� ����� �� ����� ����������
void sRand() { srand(time(0)); RRand = (INT_64)rand(); }
//������� ��������� ���������� �����
//�������� ������������ ��������� Xi+1=(a*Xi+c)%m
//habr.com/ru/post/208178
INT_64 LineRand()
{
	INT_64 y1, y2;
	y1 = (aRand * RRand + cRand) % mRand;
	y2 = (aRand * y1 + cRand) % mRand;
	RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;
	return RRand;
}

void test_rand(int n)
{
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	MyTree< INT_64, int > tree;
	MyTree< INT_64, int > tree1;

	//������ ��� ������, ������� ������������ � ������
	INT_64* m = new INT_64[n];
	//��������� ������� ���������� �����
	sRand();
	//���������� ������ � ������� ���������� �� ���������� �������
	int nn = 1;
	int nn1 = 1;
	bool b = true;
	for (int i = 0; i < n; i++)
	{
		m[i] = LineRand();
		tree.RND_Insert(m[i], 1, nn);
		tree.BST_Insert(m[i], 1, nn1);
	}
	//����� ������� ������ �� �����
	cout << "items count:" << tree.AskForSize() << endl;
	cout << "items count:" << tree1.AskForSize() << endl;
	//��������� ��������� ����������� �������, �������� � ������
	double I = 0;
	double D = 0;
	double S = 0;

	double I1 = 0;
	double D1 = 0;
	double S1 = 0;
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0) //10% ��������
		{
			nn = 1;
			tree.RND_Delete(LineRand(), nn);
			tree1.BST_Delete(LineRand(), nn1);
			D += nn;
			nn = 1;
			D1 += nn1;
			nn1 = 1;
			tree.RND_Insert(m[rand() % n], 1, nn);
			tree1.BST_Insert(m[rand() % n], 1, nn1);
			I += nn;
			I1 += nn1;
			try {
				nn = 1;
				tree.RND_Read(LineRand(), nn);
				S += nn;
			}
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S += nn; }
			try {
				nn1 = 1;
				tree1.RND_Read(LineRand(), nn1);
				S1 += nn1;
			}
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S1 += nn1; }
		}
		else //90% �������� ��������
		{
			int ind = rand() % n;
			nn = 1;
			nn1 = 1;
			tree.RND_Delete(m[ind], nn);
			tree1.BST_Delete(m[ind], nn1);
			D += nn;
			D1 += nn1;
			INT_64 key = LineRand();
			nn = 1;
			nn1 = 1;
			tree.RND_Insert(key, 1, nn);
			tree1.BST_Insert(key, 1, nn1);
			I += nn;
			I1 += nn1;
			m[ind] = key;
			try {
				nn = 1;
				tree.RND_Read(m[rand() % n], nn);
				S += nn;
			}
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S += nn; }
			try {
				nn1 = 1;
				tree1.RND_Read(m[rand() % n], nn1);
				S1 += nn1;
			}
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S1 += nn1; }
		} //����� �����
		//����� �����������:
		//����� ������� ������ ����� �����
	cout << "items count:" << tree.AskForSize() << endl;
	//������������� ������ ����������� �������� BST
	cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << endl;
	//����������������� ������ ����������� �������
	cout << "Count insert: " << I / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "Count delete: " << D / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "Count search: " << S / (n / 2) << endl;
	//������������ ������ ������� m[]
	
	cout << "items count:" << tree1.AskForSize() << endl;
	//������������� ������ ����������� �������� BST
	cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << endl;
	//����������������� ������ ����������� �������
	cout << "Count insert: " << I1 / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "Count delete: " << D1 / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "Count search: " << S1 / (n / 2) << endl;
	//������������ ������ ������� m[]
	
	delete[] m;
}

void test_ord(int n)
{
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	MyTree< INT_64, int > tree;
	//������ ��� ������, ������� ������������ � ������
	INT_64* m = new INT_64[n];
	//���������� ������ � ������� ���������� � ������������� ������� //������� �� ��������� [0, 10000, 20000, ... ,10000*n]
	int nn = 1;
	bool b = true;
	for (INT_64 i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.RND_Insert(m[i], 1, nn);
	}
	//����� ������� ������ �� �����
	cout << "items count:" << tree.AskForSize() << endl;
	//��������� ��������� ����������� �������, �������� � ������
	double I = 0;
	double D = 0;
	double S = 0;
	//��������� ������� ���������� �����
	sRand();
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0) //10% ��������
		{
			INT_64 k = LineRand() % (10000 * n);
			k = k + k % 2; //��������� �������� ����
			nn = 1;
			tree.RND_Delete(k, nn);
			D += nn;
			nn = 1;
			tree.RND_Insert(m[rand() % n], 1, nn);
			I += nn;
			k = LineRand() % (10000 * n);
			k = k + !(k % 2); // ��������� �������� ����
			try {
				nn = 1;
				tree.RND_Read(k, nn);
				S += nn;
			}
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S += nn; }
		}
		else //90% �������� ��������
		{
			int ind = rand() % n;
			nn = 1;
			tree.RND_Delete(m[ind], nn);
			D += nn;
			INT_64 k = LineRand() % (10000 * n);
			k = k + k % 2; // ��������� ������ ����
			nn = 1;
			tree.RND_Insert(k, 1, nn);
			I += nn;
			m[ind] = k;
			try {
				nn = 1;
				tree.RND_Read(m[rand() % n], nn);
				S += nn;
			}
			//��������� ���������� ��� ������ �������� ������
			catch (int) { S += nn; }
		}
	}
	//����� �����������:
	// ����� ������� ������ ����� �����
	cout << "items count:" << tree.AskForSize() << endl;
	//������������� ������ ����������� �������� BST
	cout << "n/2 =" << n / 2 << endl;
	//����������������� ������ ����������� �������
	cout << "Count insert: " << I / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "Count delete: " << D / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "Count search: " << S / (n / 2) << endl;
	//������������ ������ ������� m[]
	delete[] m;
} //����� �����

int main()
{
	setlocale(LC_ALL, "Russian");

	MyTree<int, int> NewTree;
	MyTree<int, int>::Iterator it(NewTree);
	MyTree<int, int>::ReverseIterator rit(NewTree);
	int command;
	int value;
	int key;
	int nn = 0;

	cout << "0 - ��������� ������ ���������" << endl;
	cout << "1 - ����� ������� ������" << endl;
	cout << "2 - ������� ������" << endl;
	cout << "3 - �������� ������ �� �������" << endl;
	cout << "4 - ������ �������� � �������� ������" << endl;
	cout << "5 - �������� ����� �������� � �������� ������" << endl;
	cout << "6 - �������� ������ � �������� ������" << endl;
	cout << "7 - ������������ ������ ������ � ������ � ������� ������ ����� �� t->Lt->Rt" << endl;
	cout << "8 - ������ ��������� ������" << endl;
	cout << "��������" << endl;
	cout << "9 - ��������� �� ������ ���� � ������ � ����������� ������" << endl;
	cout << "10 - ��������� �� ��������� �� �����" << endl;
	cout << "11 - �������� ��������� ���������" << endl;
	cout << "12 - ������ �� ������ � ������ �������� ���� � ������" << endl;
	cout << "13 - ������ �� ������ � ������ �������� ���� � ������" << endl;
	cout << "�������� ��������" << endl;
	cout << "14 - ��������� �� ��������� ���� � ������ � ������������ ������" << endl;
	cout << "15 - ��������� �� ���������� �� �����" << endl;
	cout << "16 - �������� ��������� ���������" << endl;
	cout << "17 - ������ �� ������ � ������ �������� ���� � ������" << endl;
	cout << "18 - ������ �� ������ � ������ �������� ���� � ������" << endl;
	cout << "19 - ���� ���������� ������" << endl;
	cout << "20 - ���� ������������ BST-������" << endl;
	cout << "21 - �������� ������� ������ ������" << endl << endl;


	/*NewTree.RND_Insert(10, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(20, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(30, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(40, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(50, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(60, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(70, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(80, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(90, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(100, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(110, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(120, 1, nn);
	//NewTree.PrintTree();
	NewTree.RND_Insert(130, 1, nn);
	NewTree.PrintTree();*/
	/*NewTree.RND_Insert(140, 1, nn);
	NewTree.RND_Insert(150, 1, nn);
	NewTree.RND_Insert(160, 1, nn);
	NewTree.RND_Insert(170, 1, nn);
	NewTree.RND_Insert(180, 1, nn);
	NewTree.RND_Insert(190, 1, nn);*/

	do {
		cout << "������� ����� �������: ";
		cin >> command;
		switch (command) {

		case 0:	//��������� ������ ���������
			cout << "������ ��������� ���������" << endl;
			break;

		case 1:	//����� ������� ������
			cout << "������ ������ ���������� " << NewTree.AskForSize() << " ���������" << endl << endl;
			break;

		case 2:	//������� ������
			NewTree.Delete();
			cout << "������ ������" << endl << endl;
			break;

		case 3:	//�������� ������ �� �������
			if (NewTree.EmptyCheck() == 1) {
				cout << "C����� ����" << endl << endl;
			}
			else {
				cout << "������ �� ����" << endl << endl;
			}
			break;

		case 4:	//������ �������� � �������� ������
			cout << "������� �������� �����: ";
			cin >> key;
			try {
				cout << "�������� � �������� ������ = " << NewTree.RND_Read(key, nn) << endl << endl;
			}
			catch (int) {
				cout << "������ ���� � ������ �����������" << endl << endl;
			}
			break;

		case 5:	//�������� ����� �������� � �������� ������
			cout << "������� ����: ";
			cin >> key;
			if (NewTree.Iteration_Key_Check(key) == true) {
				cout << "�������� � ������ ������ ��� ����������" << endl << endl;
			}
			else{
				cout << "������� ����� ��������: ";
				cin >> value;
				if (NewTree.RND_Insert(key, value, nn) == true) {
					cout << "�������� ��������" << endl << endl;
				}
				else {
					cout << "�������� �� ��������" << endl << endl;
				}
			}
			break;

		case 6:	//�������� ������ � �������� ������
			cout << "������� ����: ";
			cin >> key;
			if (NewTree.RND_Delete(key, nn) == true) {
				cout << "������� � ������ ������ ������" << endl << endl;
			}
			else {
				cout << "������ ���� � ������ �����������" << endl << endl;
			}
			break;

		case 7:	//������������ ������ ������ � ������ � ������� ������ ����� �� tLR
			NewTree.StartPrint();
			cout << endl;
			break;

		case 8://������ ��������� ������
			NewTree.PrintTree();
			break;

		case 9://��������� �� ������ ���� � ������ 
			try {
				it.begin();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 10://��������� �� ���������
			try {
				it.next();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ���������� " << endl << endl;
			}
			break;

		case 11://�������� ��������� ���������
			if (it.is_off() == true) {
				cout << "�������� �� ����������" << endl << endl;
			}
			else {
				cout << "�������� ����������" << endl << endl;
			}
			break;

		case 12://������ �� ������
			try {
				cout << *it << endl << endl;
			}
			catch (int) {
				cout << "�������� �� ��������� ������" << endl << endl;
			}
			break;

		case 13://������ �� ������
			try {
				cout << "������� ����� ��������: ";
				cin >> *it;
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ��������� ������" << endl << endl;
			}
			break;

		case 14://��������� �� ��������� ���� � ������ 
			try {
				rit.rbegin();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 15://��������� �� ����������
			try {
				rit.prev();
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ����������" << endl << endl;
			}
			break;

		case 16://�������� ��������� ���������
			if (rit.is_off() == true) {
				cout << "�������� �� ����������" << endl << endl;
			}
			else {
				cout << "�������� ����������" << endl << endl;
			}
			break;

		case 17://������ �� ������
			try {
				cout << *rit;
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ��������� ������" << endl << endl;
			}
			break;

		case 18://������ �� ������
			try {
				cout << "������� ����� ��������: ";
				cin >> *rit;
				cout << endl;
			}
			catch (int) {
				cout << "�������� �� ��������� ������" << endl << endl;
			}
			break;

		case 19://���� ���������� ������
			cout << "������� ��������� ���������" << endl;
			cin >> key;
			test_rand(key);
			break;

		case 20://���� ������������ ������
			cout << "������� ��������� ���������" << endl;
			cin >> key;
			test_ord(key);
			break;

		case 21://������� ������ ������
			cout << "0 - ��������� ������ ���������" << endl;
			cout << "1 - ����� ������� ������" << endl;
			cout << "2 - ������� ������" << endl;
			cout << "3 - �������� ������ �� �������" << endl;
			cout << "4 - ������ �������� � �������� ������" << endl;
			cout << "5 - �������� ����� �������� � �������� ������" << endl;
			cout << "6 - �������� ������ � �������� ������" << endl;
			cout << "7 - ������������ ������ ������ � ������ � ������� ������ ����� �� t->Lt->Rt" << endl;
			cout << "8 - ������ ��������� ������" << endl;
			cout << "��������" << endl;
			cout << "9 - ��������� �� ������ ���� � ������ � ����������� ������" << endl;
			cout << "10 - ��������� �� ��������� �� �����" << endl;
			cout << "11 - �������� ��������� ���������" << endl;
			cout << "12 - ������ �� ������ � ������ �������� ���� � ������" << endl;
			cout << "13 - ������ �� ������ � ������ �������� ���� � ������" << endl;
			cout << "�������� ��������" << endl;
			cout << "14 - ��������� �� ��������� ���� � ������ � ������������ ������" << endl;
			cout << "15 - ��������� �� ���������� �� �����" << endl;
			cout << "16 - �������� ��������� ���������" << endl;
			cout << "17 - ������ �� ������ � ������ �������� ���� � ������" << endl;
			cout << "18 - ������ �� ������ � ������ �������� ���� � ������" << endl;
			cout << "19 - ���� ���������� ������" << endl;
			cout << "20 - ���� ������������ BST-������" << endl;
			cout << "21 - �������� ������� ������ ������" << endl << endl;
			break;

		default:
			cout << "������� � ����� ������� �� ����������" << endl << endl;
		}
	} while (command != 0);
}

