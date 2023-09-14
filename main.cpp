#define _CRT_SECURE_NO_WARNINGS
#include "lexicalanalyze.h"
#include "syntaxanalyze.h"
#include "indent.h"

int main() {
	int op = 1;//选择
	while (op) {
		system("cls");
		cout << "高级语言源程序格式处理工具" << endl;
		cout << "请选择功能：" << endl;
		cout << "1.词法分析" << endl;
		cout << "2.语法分析" << endl;
		cout << "3.代码缩进" << endl;
		cout << "0.退出程序" << endl;
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