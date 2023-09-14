#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include "lexicalanalyze.h"
#include "indent.h"

extern FILE* fp;
extern char token_text[100];
extern char FileName[50];

void indent() {
	cout << "请输入文件名:" << endl;
	cin >> FileName; // 获取用户输入的文件名
	cout << "缩进后的代码如下：" << endl;
	fp = fopen(FileName, "r"); // 打开文件
	if (fp == NULL) { // 打开失败
		cout << "文件打开失败";
		return;
	}
	int tabs = 0; // 控制缩进
	int flag = 0; // 用于标记无大括号但需要缩进的情况
	token* root, * tail, * p;
	while (1) {
		root = readline(); // 从文件中读取一行token
		if (root == NULL) {
			break; // 文件读取结束，跳出循环
		}
		tail = root;
		while (tail->next != NULL) {
			tail = tail->next; // 找到链表的最后一个节点
		}
		if ((root == tail) && (strcmp(tail->str, "}") == 0)) {
			tabs--; // 如果只有一个节点且为"}"，则缩进减少
		}
		if ((root != tail) && (strcmp(root->str, "}") == 0)) {
			tabs--; // 如果第一个节点为"}"，则缩进减少
		}
		for (int i = 0; i < tabs; i++) {
			printf("  "); // 输出缩进
		}
		if (flag == 1) {
			tabs--;
			flag = 0; // 如果标记为1，则缩进减少
		}
		p = root;
		while (p != NULL) {
			cout << p->str << " "; // 输出token
			p = p->next;
		}
		cout << endl;
		if (strcmp(tail->str, "{") == 0) {
			tabs++; // 如果最后一个节点为"{"，则缩进增加
		}
		p = root;
		while (p != NULL) {
			if ((strcmp(p->str, "if") == 0) || (strcmp(p->str, "for") == 0) ||
				(strcmp(p->str, "while") == 0)) {
				if (strcmp(tail->str, "{") != 0 &&
					strcmp(tail->str, ";") != 0) {
					tabs++;
					flag = 1; // 如果节点为"if"、"for"或"while"，且最后一个节点不是"{"或";"，则缩进增加
				}
			}
			p = p->next;
		}
	}
	fclose(fp); // 关闭文件
	return;
}


token* readline() {
	int word = gettoken(fp)->type; // 获取下一个token的类型
	// 如果到达文件末尾
	if (word == -1) {
		return NULL; // 返回空指针
	}
	token* root = NULL, * tail = NULL;
	root = new token; // 创建一个新的token节点
	char* token_text1 = new char[25];
	strcpy(token_text1, token_text);
	root->str = token_text1; // 将token文本复制到节点中
	root->next = NULL;
	tail = root;
	char c;
	// 读取文件直到换行符
	while ((c = fgetc(fp)) != '\n') {
		ungetc(c, fp); // 将字符放回文件流中
		gettoken(fp); // 获取下一个token
		tail->next = new token; // 创建一个新的token节点
		tail = tail->next;
		token_text1 = new char[25];
		strcpy(token_text1, token_text);
		tail->str = token_text1; // 将token文本复制到节点中
		tail->next = NULL;
	}
	return root; // 返回token链表的根节点
}
