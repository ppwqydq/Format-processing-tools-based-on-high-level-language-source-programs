#define _CRT_SECURE_NO_WARNINGS
#include "lexicalanalyze.h"
#include "syntaxanalyze.h"
#include "indent.h"

int main() {
	int op = 1;//ѡ��
	while (op) {
		system("cls");
		cout << "�߼�����Դ�����ʽ������" << endl;
		cout << "��ѡ���ܣ�" << endl;
		cout << "1.�ʷ�����" << endl;
		cout << "2.�﷨����" << endl;
		cout << "3.��������" << endl;
		cout << "0.�˳�����" << endl;
		cin >> op;
		switch (op) {
		case 1:
			lexAnalyzeOutput();
			getchar();
			getchar();
			break;
		case 2:
			syntaxAnalyze();
			getchar();
			getchar();
			break;
		case 3:
			indent();
			getchar();
			getchar();
			break;
		}
	}


}