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
vector<int> Jump(122 + 1);	//문자가 바로 index로 사용되는 최종 jump배열.

//함수선언
void computeJump(const char* P);
void BoyerMoor(string A, const char* P);

void main() {

	const char* P = "Further Reading about Standards";
	//const char* P = "17650    20000818";
	ifstream in("test.txt"); //test1은 작은샘플 test5는 큰샘플
	string A;

	if (in.is_open()) {

		//위치 지정자를 파일의 끝으로 옮김
		in.seekg(0, ios::end);

		//파일의 끝 위치를 읽는다. (즉, 파일의 크기)
		int size = in.tellg();

		//파일 크기만큼의 문자열을 할당한다.
		A.resize(size);

		// 위치 지정자를 파일의 시작으로 옮김
		in.seekg(0, ios::beg);

		// 파일 전체 내용을 읽어서 문자열에 저장한다.
		in.read(&A[0], size);

	}
	else {
		printf("파일을 찾을 수 없습니다!");

	}
	printf("<기본정보>\n\n");
	BoyerMoor(A, P);	//BoyerMoor함수 호출.

}

void computeJump(const char* P) {

	int len = strlen(P);

	//vector<int> Origin_jnum에 값을 넣는 부분.
	for (int i = 0; i < len; i++) {

		if (i == len - 1) {
			Origin_jnum.push_back(len);
			break;
		}
		else {
			Origin_jnum.push_back(len - 1 - i);
		}
	}

	const int origin_jumpsize = Origin_jnum.size();		//vector는 esrase후 size가 감소하므로 처음 vector의 크기를 고정으로 저장.

	//vector<char> Patchar에 값을 넣는 부분.(Patchar==패턴문자열)
	for (int i = 0; i < len; i++) {

		Patchar.push_back(P[i]);

	}

	//vector<int> Origin_jnum 잘 생성됐는지 확인하는 부분.
	printf("\tOrigin_jnum[]={");
	for (int i = 0; i < Origin_jnum.size(); i++) {

		printf("%d, ", Origin_jnum[i]);
	}
	printf("}\n\n");

	//vector<char> Patchar 잘 생성됐는지 확인하는 부분.
	printf("\tPatcharr[]={");
	for (int i = 0; i < Patchar.size(); i++) {

		printf("%c", Patchar[i]);
	}
	printf("}\n\n");

	/*vector<int> No_overlap_jnum에 값을 넣는 부분. 
	패턴문자열에서 중복된 문자는 minimum jump값으로 overwrite.*/
	printf("\tNo_overlap_jnum[]={");
	int i = 0;
	while (i < Origin_jnum.size()) {

		int min = Origin_jnum[i];	//이것보다 작지 않으면 업데이트되지 않을 default val.

		int j = i + 1;		//i문자 다음문자와 비교해야하므로.
		while (j < Origin_jnum.size()) {
			if (Patchar[i] == Patchar[j]) {		//중복되는 문자 발견
				if (Origin_jnum[i] > Origin_jnum[j]) {	//오른쪽(뒤에 나오는) 문자의 jump num이 더 작다면
					min = Origin_jnum[j];		//억울하지만 작은 값으로 overwrite
				}
				//중복되는 문자 발견 이후 지워야 다음 문자의 중복여부도 검사가능.
				Patchar.erase(Patchar.begin() + j);
				Origin_jnum.erase(Origin_jnum.begin() + j); 
			}
			j++;
		}
		No_overlap_jnum.push_back(min); printf(" %d,", No_overlap_jnum[i]);
		i++;

	}
	printf("}\n\n");

	//중복되는 문자열은 삭제되었는지 확인하기 위한 부분.
	printf("\t(after erase) Patcharr[]={");
	for (unsigned int i = 0; i < Patchar.size(); i++) {

		printf("%c", Patchar[i]);
	}
	printf("}\n\n");

	//vector<int> Jump에 값을 넣는 부분.
	for (int i = 0; i < No_overlap_jnum.size(); i++) {		//문자를 아스키코드(숫자)로 index자체가 되도록함.

		Jump[Patchar[i]] = No_overlap_jnum[i];

	}
	for (int i = 0; i < Jump.size(); i++) {		//기타문자열은 원래 패턴문자열 size만큼 jump num 설정.

		if (Jump[i] == 0) Jump[i] = origin_jumpsize;	
	}

	//vector<int> Jump 잘 생성됐는지 확인하는 부분.
	printf("\tJump[]={");
	for (unsigned int i = 0; i < Jump.size(); i++) {

		printf("%d, ", Jump[i]);
	}
	printf("}\n\n");

}

void BoyerMoor(string A, const char* P) {

	computeJump(P);		//jump배열 생성완료.

	double bm_elapsed_time;
	clock_t before = clock(); //현재 시간 저장

	int n = A.length(); 
	int m = strlen(P); 
	printf("\t텍스트문자열의 길이는 %d, 패턴문자열의 길이는 %d \n\n", n, m);

	printf("<Booyer-Moor Matching Algorithm 실행 결과>\n\n");
	int i = 0; int count = 0; 
	while (i <= n - m + 1) {

		//패턴문자열 오른쪽끝부터 비교함.
		int j = m - 1;
		int k = i + m - 1;

		while ((j >= 0) && (P[j] == A[k])) {	//matching발생하면
			j--; k--;
		}

		if (j == -1) {		//j=m-1이므로 j가 -1일때 패턴문자열 전체가 matchig되었다는 소리.

			printf("\t패턴이 [%d] 위치에서 매칭되었습니다! \n\n", i);
			i++; 
			count++;	//matching이 일어난 빈도

		}

		/*유효하지 않은 아스키코드 값을 처리하는 부분입니다.*/
		if ((A[i + m - 1] >= 0) && (A[i + m - 1] <= 122)) {
			i = i + Jump[A[i + m - 1]];
		}
		else {
			i = i + strlen(P);
		}

	}
	if (count == 0) printf("\n\n\t패턴문자열을 찾지 못했습니다.");

	bm_elapsed_time = (double)(clock() - before) / CLOCKS_PER_SEC;  // 초 단위로 경과 시간을 측정함
	fprintf(stdout, "\tBooyer-Moor Matching으로 패턴문자열을 찾는데 걸린 시간(초): %10.2f\n", bm_elapsed_time);

}