#include <stdio.h>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include<vector>

using namespace std;
using std::vector;

vector<int> Origin_jnum;
vector<char> Patchar;
vector<int> No_overlap_jnum;
vector<int> Jump(122 + 1);	//���ڰ� �ٷ� index�� ���Ǵ� ���� jump�迭.

//�Լ�����
void computeJump(const char* P);
void BoyerMoor(string A, const char* P);

void main() {

	const char* P = "Further Reading about Standards";
	//const char* P = "17650    20000818";
	ifstream in("test.txt"); //test1�� �������� test5�� ū����
	string A;

	if (in.is_open()) {

		//��ġ �����ڸ� ������ ������ �ű�
		in.seekg(0, ios::end);

		//������ �� ��ġ�� �д´�. (��, ������ ũ��)
		int size = in.tellg();

		//���� ũ�⸸ŭ�� ���ڿ��� �Ҵ��Ѵ�.
		A.resize(size);

		// ��ġ �����ڸ� ������ �������� �ű�
		in.seekg(0, ios::beg);

		// ���� ��ü ������ �о ���ڿ��� �����Ѵ�.
		in.read(&A[0], size);

	}
	else {
		printf("������ ã�� �� �����ϴ�!");

	}
	printf("<�⺻����>\n\n");
	BoyerMoor(A, P);	//BoyerMoor�Լ� ȣ��.

}

void computeJump(const char* P) {

	int len = strlen(P);

	//vector<int> Origin_jnum�� ���� �ִ� �κ�.
	for (int i = 0; i < len; i++) {

		if (i == len - 1) {
			Origin_jnum.push_back(len);
			break;
		}
		else {
			Origin_jnum.push_back(len - 1 - i);
		}
	}

	const int origin_jumpsize = Origin_jnum.size();		//vector�� esrase�� size�� �����ϹǷ� ó�� vector�� ũ�⸦ �������� ����.

	//vector<char> Patchar�� ���� �ִ� �κ�.(Patchar==���Ϲ��ڿ�)
	for (int i = 0; i < len; i++) {

		Patchar.push_back(P[i]);

	}

	//vector<int> Origin_jnum �� �����ƴ��� Ȯ���ϴ� �κ�.
	printf("\tOrigin_jnum[]={");
	for (int i = 0; i < Origin_jnum.size(); i++) {

		printf("%d, ", Origin_jnum[i]);
	}
	printf("}\n\n");

	//vector<char> Patchar �� �����ƴ��� Ȯ���ϴ� �κ�.
	printf("\tPatcharr[]={");
	for (int i = 0; i < Patchar.size(); i++) {

		printf("%c", Patchar[i]);
	}
	printf("}\n\n");

	/*vector<int> No_overlap_jnum�� ���� �ִ� �κ�. 
	���Ϲ��ڿ����� �ߺ��� ���ڴ� minimum jump������ overwrite.*/
	printf("\tNo_overlap_jnum[]={");
	int i = 0;
	while (i < Origin_jnum.size()) {

		int min = Origin_jnum[i];	//�̰ͺ��� ���� ������ ������Ʈ���� ���� default val.

		int j = i + 1;		//i���� �������ڿ� ���ؾ��ϹǷ�.
		while (j < Origin_jnum.size()) {
			if (Patchar[i] == Patchar[j]) {		//�ߺ��Ǵ� ���� �߰�
				if (Origin_jnum[i] > Origin_jnum[j]) {	//������(�ڿ� ������) ������ jump num�� �� �۴ٸ�
					min = Origin_jnum[j];		//��������� ���� ������ overwrite
				}
				//�ߺ��Ǵ� ���� �߰� ���� ������ ���� ������ �ߺ����ε� �˻簡��.
				Patchar.erase(Patchar.begin() + j);
				Origin_jnum.erase(Origin_jnum.begin() + j); 
			}
			j++;
		}
		No_overlap_jnum.push_back(min); printf(" %d,", No_overlap_jnum[i]);
		i++;

	}
	printf("}\n\n");

	//�ߺ��Ǵ� ���ڿ��� �����Ǿ����� Ȯ���ϱ� ���� �κ�.
	printf("\t(after erase) Patcharr[]={");
	for (unsigned int i = 0; i < Patchar.size(); i++) {

		printf("%c", Patchar[i]);
	}
	printf("}\n\n");

	//vector<int> Jump�� ���� �ִ� �κ�.
	for (int i = 0; i < No_overlap_jnum.size(); i++) {		//���ڸ� �ƽ�Ű�ڵ�(����)�� index��ü�� �ǵ�����.

		Jump[Patchar[i]] = No_overlap_jnum[i];

	}
	for (int i = 0; i < Jump.size(); i++) {		//��Ÿ���ڿ��� ���� ���Ϲ��ڿ� size��ŭ jump num ����.

		if (Jump[i] == 0) Jump[i] = origin_jumpsize;	
	}

	//vector<int> Jump �� �����ƴ��� Ȯ���ϴ� �κ�.
	printf("\tJump[]={");
	for (unsigned int i = 0; i < Jump.size(); i++) {

		printf("%d, ", Jump[i]);
	}
	printf("}\n\n");

}

void BoyerMoor(string A, const char* P) {

	computeJump(P);		//jump�迭 �����Ϸ�.

	double bm_elapsed_time;
	clock_t before = clock(); //���� �ð� ����

	int n = A.length(); 
	int m = strlen(P); 
	printf("\t�ؽ�Ʈ���ڿ��� ���̴� %d, ���Ϲ��ڿ��� ���̴� %d \n\n", n, m);

	printf("<Booyer-Moor Matching Algorithm ���� ���>\n\n");
	int i = 0; int count = 0; 
	while (i <= n - m + 1) {

		//���Ϲ��ڿ� �����ʳ����� ����.
		int j = m - 1;
		int k = i + m - 1;

		while ((j >= 0) && (P[j] == A[k])) {	//matching�߻��ϸ�
			j--; k--;
		}

		if (j == -1) {		//j=m-1�̹Ƿ� j�� -1�϶� ���Ϲ��ڿ� ��ü�� matchig�Ǿ��ٴ� �Ҹ�.

			printf("\t������ [%d] ��ġ���� ��Ī�Ǿ����ϴ�! \n\n", i);
			i++; 
			count++;	//matching�� �Ͼ ��

		}

		/*��ȿ���� ���� �ƽ�Ű�ڵ� ���� ó���ϴ� �κ��Դϴ�.*/
		if ((A[i + m - 1] >= 0) && (A[i + m - 1] <= 122)) {
			i = i + Jump[A[i + m - 1]];
		}
		else {
			i = i + strlen(P);
		}

	}
	if (count == 0) printf("\n\n\t���Ϲ��ڿ��� ã�� ���߽��ϴ�.");

	bm_elapsed_time = (double)(clock() - before) / CLOCKS_PER_SEC;  // �� ������ ��� �ð��� ������
	fprintf(stdout, "\tBooyer-Moor Matching���� ���Ϲ��ڿ��� ã�µ� �ɸ� �ð�(��): %10.2f\n", bm_elapsed_time);

}