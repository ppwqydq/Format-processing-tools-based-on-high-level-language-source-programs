#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include "lexicalanalyze.h"
#include "indent.h"

extern FILE* fp;
extern char token_text[100];
extern char FileName[50];

void indent() {
	cout << "�������ļ���:" << endl;
	cin >> FileName; // ��ȡ�û�������ļ���
	cout << "������Ĵ������£�" << endl;
	fp = fopen(FileName, "r"); // ���ļ�
	if (fp == NULL) { // ��ʧ��
		cout << "�ļ���ʧ��";
		return;
	}
	int tabs = 0; // ��������
	int flag = 0; // ���ڱ���޴����ŵ���Ҫ���������
	token* root, * tail, * p;
	while (1) {
		root = readline(); // ���ļ��ж�ȡһ��token
		if (root == NULL) {
			break; // �ļ���ȡ����������ѭ��
		}
		tail = root;
		while (tail->next != NULL) {
			tail = tail->next; // �ҵ���������һ���ڵ�
		}
		if ((root == tail) && (strcmp(tail->str, "}") == 0)) {
			tabs--; // ���ֻ��һ���ڵ���Ϊ"}"������������
		}
		if ((root != tail) && (strcmp(root->str, "}") == 0)) {
			tabs--; // �����һ���ڵ�Ϊ"}"������������
		}
		for (int i = 0; i < tabs; i++) {
			printf("  "); // �������
		}
		if (flag == 1) {
			tabs--;
			flag = 0; // ������Ϊ1������������
		}
		p = root;
		while (p != NULL) {
			cout << p->str << " "; // ���token
			p = p->next;
		}
		cout << endl;
		if (strcmp(tail->str, "{") == 0) {
			tabs++; // ������һ���ڵ�Ϊ"{"������������
		}
		p = root;
		while (p != NULL) {
			if ((strcmp(p->str, "if") == 0) || (strcmp(p->str, "for") == 0) ||
				(strcmp(p->str, "while") == 0)) {
				if (strcmp(tail->str, "{") != 0 &&
					strcmp(tail->str, ";") != 0) {
					tabs++;
					flag = 1; // ����ڵ�Ϊ"if"��"for"��"while"�������һ���ڵ㲻��"{"��";"������������
				}
			}
			p = p->next;
		}
	}
	fclose(fp); // �ر��ļ�
	return;
}


token* readline() {
	int word = gettoken(fp)->type; // ��ȡ��һ��token������
	// ��������ļ�ĩβ
	if (word == -1) {
		return NULL; // ���ؿ�ָ��
	}
	token* root = NULL, * tail = NULL;
	root = new token; // ����һ���µ�token�ڵ�
	char* token_text1 = new char[25];
	strcpy(token_text1, token_text);
	root->str = token_text1; // ��token�ı����Ƶ��ڵ���
	root->next = NULL;
	tail = root;
	char c;
	// ��ȡ�ļ�ֱ�����з�
	while ((c = fgetc(fp)) != '\n') {
		ungetc(c, fp); // ���ַ��Ż��ļ�����
		gettoken(fp); // ��ȡ��һ��token
		tail->next = new token; // ����һ���µ�token�ڵ�
		tail = tail->next;
		token_text1 = new char[25];
		strcpy(token_text1, token_text);
		tail->str = token_text1; // ��token�ı����Ƶ��ڵ���
		tail->next = NULL;
	}
	return root; // ����token����ĸ��ڵ�
}
