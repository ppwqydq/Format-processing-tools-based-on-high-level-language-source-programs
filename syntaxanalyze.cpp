#define _CRT_SECURE_NO_WARNINGS
#include <stack>
#include <string.h>
#include "syntaxanalyze.h"
#include "lexicalanalyze.h"

using namespace std;

extern char c;
extern char token_text[100];//���ڴ洢�ʷ���Ԫ��ȫ�ֱ���
extern char string_num[100];
extern char FileName[50];//�ļ���
extern FILE* fp;
extern int ncount;//���ڼ�¼������������


int type;//�洢��������
VDN* Vroot;//����ͷ�ڵ�
int isVoid, hasReturn, isInrecycle;
int w;//�洢��ǰ����ĵ����������
FDN* Froot;//��������ͷ�ڵ�

/// <summary>
/// ���ڵ��Թ����м�����������
/// </summary>
void Traverse() {
	VDN* p = Vroot;
	while (p != NULL) {
		for (int i = 0; i < (p->size); i++) {
			cout << p->datas.variable[i] << " " << p->datas.type[i] << " ";
		}
		p = p->next;
		cout << endl;
	}
}

/// <summary>
/// ��������﷨��
/// </summary>
void syntaxAnalyze() {
	cout << "�������ļ���:" << endl;
	cin >> FileName;
	fp = fopen(FileName, "r");
	if (fp == NULL) {//��ʧ��
		cout << "�ļ���ʧ��";
		return;
	}
	ASTTree* root = program();
	if (root == NULL) {
		cout << "���﷨����";
		fclose(fp);//�ر��ļ�
		return;
	}
	else {
		PreTraverse(root, 0);
		//Traverse();
		freeTree(root);
		fclose(fp);//�ر��ļ�
		return;
	}
}

/// <summary>
/// �ͷ��﷨���ڴ�
/// </summary>
/// <param name="root">�﷨�����ڵ�</param>
void freeTree(ASTTree* root) {
	if (root) {
		freeTree(root->l);
		freeTree(root->r);
		delete(root);
	}
}

/// <summary>
/// ��ȡ��һ���ʷ���Ԫ��ͬʱ����ͷ�ļ���ע�ͣ���
/// </summary>
void skip() {
	w = gettoken(fp)->type;
	while (w == ANNO || w == INCLUDE || w == MACRO) w = gettoken(fp)->type;
	if (w == ERROR_TOKEN) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺴���Ĵʷ���Ԫ" << endl;
		exit(0);
	}
}

/// <summary>
/// ���������������﷨��
/// </summary>
/// <param name="root">������ڵ�</param>
/// <param name="depth">������</param>
void PreTraverse(ASTTree* root, int depth) {
	if (root == NULL) {
		cout << " ";
		return;
	}
	else {
		int i = 0;
		//for (i = 0; i < depth; i++) cout << " ";
		showType(root->type);
		if (root->data.text != NULL) {
			cout << root->data.text << endl;
		}
		PreTraverse(root->l, depth + 1);
		PreTraverse(root->r, depth + 1);
	}
}

/// <summary>
/// ��������﷨��Ԫ����
/// </summary>
/// <param name="type">�������</param>
void showType(int type) {
	switch (type) {
	case 1:
		//cout << "�ⲿ��������" << endl;
		break;
	case 2:
		cout << "�ⲿ��������" << endl;
		break;
	case 3:
		cout << "�ⲿ�����������ͣ�";
		break;
	case 4:
		//cout << "�ⲿ������������" << endl;
		break;
	case 5:
		cout << "�ⲿ�������ƣ�";
		break;
	case 6:
		cout << "��������" << endl;
		break;
	case 7:
		cout << "�����������ͣ�";
		break;
	case 8:
		cout << "�������ƣ�";
		break;
	case 9:
		cout << "������������" << endl;
		break;
	case 10:
		//cout << "������������" << endl;
		break;
	case 11:
		cout << "���������������ͣ�";
		break;
	case 12:
		cout << "�����������ƣ�";
		break;
	case 13:
		cout << "������" << endl;
		break;
	case 14:
		//cout << "�ֲ�������������" << endl;
		break;
	case 15:
		cout << "�ֲ���������" << endl;
		break;
	case 16:
		cout << "�ֲ������������ͣ�";
		break;
	case 17:
		cout << "�ֲ�������������" << endl;
		break;
	case 18:
		cout << "�ֲ��������ƣ�";
		break;
	case 19:
		//cout << "�������" << endl;
		break;
	case 20:
		cout << "��������";
		break;
	case 21:
		cout << "�������";
		break;
	case 22:
		cout << "���ʽ" << endl;
		break;
	case 23:
		cout << "if�����������" << endl;
		break;
	case 24:
		cout << "else��䲿��" << endl;
		break;
	case 25:
		cout << "if���" << endl;
		break;
	case 26:
		cout << "if-else���" << endl;
		break;
	case 27:
		cout << "while���" << endl;
		break;
	case 28:
		cout << "while�����������" << endl;
		break;
	case 29:
		cout << "while�����" << endl;
		break;
	case 30:
		cout << "for���" << endl;
		break;
	case 31:
		cout << "for�������" << endl;
		break;
	case 32:
		cout << "for�������1" << endl;
		break;
	case 33:
		cout << "for�������2" << endl;
		break;
	case 34:
		cout << "for�������3" << endl;
		break;
	case 35:
		cout << "for�����" << endl;
		break;
	case 36:
		cout << "return���" << endl;
		break;
	case 37:
		cout << "break���" << endl;
		break;
	case 38:
		cout << "do-while���" << endl;
		break;
	case 39:
		cout << "do-while�����" << endl;
		break;
	case 40:
		cout << "do-while�����������" << endl;
		break;
	case 41:
		cout << "continue���" << endl;
		break;
	case 42:
		cout << "��������" << endl;
		break;
	case 43:
		cout << "���鶨��" << endl;
		break;
	case 44:
		cout << "�������ͣ�";
		break;
	case 45:
		cout << "�������ƣ�";
		break;
	case 46:
		cout << "�����С��";
		break;
	case 47:
		cout << "���鶨������" << endl;
		break;
	case 48:
		cout << "��ֵΪ��";
	}
}

/// <summary>
/// ���ɳ�������
/// </summary>
/// <returns>��������</returns>
ASTTree* program() {
	skip();
	//��ʼ���ڵ�
	Vroot = new VDN;
	//��������Ϊ0
	Vroot->size = 0;
	Vroot->next = NULL;
	Froot = new FDN;
	Froot->next = NULL;
	Froot->size = 0;
	//�����ⲿ�������н��
	ASTTree* p = ExtDefList();
	if (p != NULL) {
		//�½����ڵ㣬������Ϊ�����
		ASTTree* root = new ASTTree;
		root->l = NULL;
		root->r = NULL;
		root = p;
		root->type = EXTDEFLIST;//�������
		return root;
	}
	else return NULL;
}

/// <summary>
/// �����ⲿ�������н��
/// </summary>
/// <returns>�ⲿ�������н��</returns>
ASTTree* ExtDefList() {
	// �ж��Ƿ񵽴��ļ�����
	if (w == ERROR)
		return NULL;
	//���������
	ASTTree* node = new ASTTree;
	node->type = EXTDEFLIST;
	node->data.text = NULL;
	//����������һ���ⲿ����
	node->l = ExtDef();
	skip();
	//�ݹ���ã��������������ⲿ����
	node->r = ExtDefList();
	return node;
}

/// <summary>
/// �����ⲿ������
/// </summary>
/// <returns>�ⲿ������</returns>
ASTTree* ExtDef() {
	int a;
	//���Ǳ�����������
	if (w != INT && w != DOUBLE && w != CHAR && w != LONG && w != SHORT && w != FLOAT && w != VOID) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺴�����ⲿ����";
		exit(0);
	}
	//�洢�����������ͣ����ں�������ʹ��
	type = w;
	skip();

	//�Ȳ��Ǳ�ʶ��Ҳ��������
	if (w != IDENT && w != ARRAY) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺴�����ⲿ����";
		exit(0);
	}
	//�洢�������ͣ����ں�������ʹ��
	a = w;
	//�ⲿ������
	ASTTree* p = new ASTTree;
	p->l = NULL;
	p->r = NULL;
	char* token_text0 = new char[25];
	strcpy(token_text0, token_text);
	skip();
	strcpy(token_text, token_text0);
	if (w == LP) {
		p = FuncDef();//�����ţ���������
		hasReturn = 0;
	}
	else if (a == ARRAY) p = ArrayDef();//������Ϊ���飬���鶨��
	else p = ExtVarDef();//�ⲿ��������
	return p;
}

/// <summary>
/// ������������
/// </summary>
void arraySolve(char* token_text) {
	string s1;
	for (int i = 0;; i++) {
		if (token_text[i] == '[') break;
		else s1[i] = token_text[i];
	}
	strcpy(token_text, s1.c_str());
}

/// <summary>
/// �������鶨����
/// </summary>
/// <returns>���鶨����</returns>
ASTTree* ArrayDef() {
	//void���ܶ������飬����
	if (type == VOID) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺲��ܶ���void���͵�����";
		exit(0);
	}
	arraySolve(token_text);
	//��������������
	addNameF(token_text);
	//���鶨��ڵ��ʼ��
	ASTTree* node = new ASTTree;
	node->type = ARRAYDEF;
	node->l = NULL;
	node->r = NULL;
	node->data.text = NULL;
	//�������ͣ��Ƕ�����������
	ASTTree* p = new ASTTree;
	p->type = ARRAYTYPE;
	p->l = NULL;
	p->r = NULL;
	//�洢��������
	char* token_text4 = new char[25];
	if (type == INT) strcpy(token_text4, "int");
	else if (type == DOUBLE) strcpy(token_text4, "double");
	else if (type == CHAR) strcpy(token_text4, "char");
	else if (type == FLOAT) strcpy(token_text4, "float");
	else if (type == LONG) strcpy(token_text4, "long");
	else if (type == SHORT) strcpy(token_text4, "short");
	p->data.text = token_text4;
	node->l = p;
	//�������ƣ��Ƕ�����������
	ASTTree* q = new ASTTree;
	q->type = ARRAYNAME;
	q->l = NULL;
	q->r = NULL;
	char* token_text3 = new char[25];
	strcpy(token_text3, token_text);
	q->data.text = token_text3;
	//�����洢�����С
	node->r = q;
	ASTTree* m = new ASTTree;
	m->type = ARRAYSIZE;
	m->l = NULL;
	m->r = NULL;
	char* token_text5 = new char[25];
	strcpy(token_text5, string_num);
	//�����Сֵ�洢
	m->data.text = token_text5;
	//�����ƽ�����ӽ��
	q->l = m;
	return node;
}

/// <summary>
/// ���������Ƽ��뵽��������������
/// </summary>
/// <param name="token_text"></param>
/// <returns></returns>
void addName(char* token_text) {
	int i, flag = 0;
	//����ͷ�ڵ�
	VDN* p = Vroot;
	//������ĩβ
	while (p->next != NULL) p = p->next;
	for (i = 0; i < (p->size); i++) {
		//���������ظ�
		if (strcmp(token_text, p->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	if (flag == 1) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ���ظ��ı�������";
		exit(0);
	}
	//���������
	char* savename = new char[25];
	strcpy(savename, token_text);
	p->datas.variable[p->size] = savename;
	p->datas.type[p->size] = type;
	p->size++;
	return;
}

/// <summary>
/// ���������Ƽ��뵽ȫ�ֱ�������������
/// </summary>
/// <param name="token_text"></param>
/// <returns></returns>
void addNameF(char* token_text) {
	int i, flag = 0;
	//����ͷ�ڵ�
	VDN* p = Vroot;
	//������ĩβ
	for (i = 0; i < (p->size); i++) {
		//���������ظ�
		if (strcmp(token_text, p->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	if (flag == 1) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ���ظ��ı�������";
		exit(0);
	}
	//���������
	char* savename = new char[25];
	strcpy(savename, token_text);
	p->datas.variable[p->size] = savename;
	p->datas.type[p->size] = type;
	p->size++;
	return;
}

/// <summary>
/// ���������Ƽ��뵽ȫ�ֱ�������������
/// </summary>
/// <param name="token_text"></param>
/// <returns></returns>
void addNameFunc(char* token_text) {
	int i, flag = 0;
	//����ͷ�ڵ�
	VDN* p = Vroot;
	//������ĩβ
	for (i = 0; i < (p->size); i++) {
		//���������ظ�
		if (strcmp(token_text, p->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	if (flag == 1) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ���ظ��ı�������";
		exit(0);
	}
	//���������
	char* savename = new char[25];
	strcpy(savename, token_text);
	p->datas.variable[p->size] = savename;
	FDN* q = Froot;
	while (q->next != NULL) q = q->next;
	char* savenames = new char[25];
	strcpy(savenames, token_text);
	q->fName = savenames;
	p->datas.type[p->size] = type;
	p->datas.isFunc[p->size++] = 1;
	FDN* last = Froot;
	while (last->next != NULL) {
		last = last->next;
	}
	last->next = new FDN;
	last = last->next;
	last->next = NULL;
	last->size = 0;
	return;
}

/// <summary>
/// ����ת���ֲ���������
/// </summary>
/// <param name="token_text"></param>
/// <returns></returns>
int trans(char* token_text) {
	VDN* p = Vroot;
	while (p->next != NULL) p = p->next;
	int i = 0;
	int flag = 0;
	int flags = 0;
	for (i = 0; i < (p->size); i++) {
		if (strcmp(token_text, p->datas.variable[i]) == 0) {
			flag = 1;
			flags = 1;
			break;
		}
	}
	if (flags == 0) {
		for (i = 0; i < (Vroot->size); i++) {
			if (strcmp(token_text, Vroot->datas.variable[i]) == 0) {
				p = Vroot;
				flag = 1;
				break;
			}
		}
	}
	if (flag == 1 && p->datas.isFunc[i] == 0) {
		if (p->datas.type[i] == INT) return INT_CONST;
		else if (p->datas.type[i] == DOUBLE) return FLOAT_CONST;
		else if (p->datas.type[i] == CHAR) return CHAR_CONST;
		else if (p->datas.type[i] == FLOAT) return FLOAT_CONST;
		else if (p->datas.type[i] == LONG) return INT_CONST;
		else if (p->datas.type[i] == SHORT) return INT_CONST;
		else return w;
	}
	else if (flag == 1 && p->datas.isFunc[i] == 1) {
		transF(token_text);
		if (p->datas.type[i] == INT) return INT_CONST;
		else if (p->datas.type[i] == DOUBLE) return FLOAT_CONST;
		else if (p->datas.type[i] == CHAR) return  CHAR_CONST;
		else if (p->datas.type[i] == FLOAT) return  FLOAT_CONST;
		else if (p->datas.type[i] == LONG) return  INT_CONST;
		else if (p->datas.type[i] == SHORT) return  INT_CONST;
		else if (p->datas.type[i] == VOID) {
			return VOID;
		}
	}
	else return w;
}

/// <summary>
/// ����������
/// </summary>
/// <param name="token_text2">���ܵ��õĺ�����</param>
void transF(char* token_text2) {
	char token_text3[25];
	strcpy(token_text3, token_text2);
	FDN* k = Froot;
	while (k->next != NULL) {
		if (strcmp(k->fName, token_text3) == 0) break;
	}
	int i = 0;
	int j = 0;//����ѭ��
	skip();
	if (w != LP) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺴���ı��ʽ";
		exit(0);
	}
	else {
		addtoken(token_text3, '(');
		while (i < k->size) {
			skip();
			if (j % 2 != 0 && w == COMMA) {
				addtoken(token_text3, ',');
				j++;
				continue;
			}
			else if (j % 2 != 0 && w != COMMA) {
				cout << "��" << ncount << "�г��ִ���" << endl;
				cout << "����ԭ�򣺴���ı��ʽ";
				exit(0);
			}
			else {
				w = trans(token_text);
				if (w != k->name[i]) {
					cout << "��" << ncount << "�г��ִ���" << endl;
					cout << "����ԭ���뺯���������Ͳ�ƥ��";
					exit(0);
				}
				strcat(token_text3, token_text);
				i++;
				j++;
			}
		}
		skip();
		if (w != RP) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ�򣺴���ı��ʽ";
			exit(0);
		}
		else {
			addtoken(token_text3, ')');
			strcpy(token_text2, token_text3);
		}
	}
}

/// <summary>
/// �����ⲿ��������ڵ�
/// </summary>
/// <returns>�ⲿ��������ڵ�</returns>
ASTTree* ExtVarDef() {
	int cnt = 0;
	//void���ܶ������
	if (type == VOID) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺲��ܶ���void���͵ı���";
		exit(0);
	}
	//��������������
	addNameF(token_text);
	//�½��ⲿ����������
	ASTTree* root = new ASTTree;
	root->type = EXTVARDEF;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	//�洢�ⲿ�������ͣ��Ƕ���ڵ�������
	ASTTree* p = new ASTTree;
	p->type = EXTVARTYPE;
	p->l = NULL;
	p->r = NULL;
	p->data.type = type;
	char* token_text5 = new char[25];
	if (type == INT) strcpy(token_text5, "int");
	else if (type == DOUBLE) strcpy(token_text5, "double");
	else if (type == CHAR) strcpy(token_text5, "char");
	else if (type == FLOAT) strcpy(token_text5, "float");
	else if (type == LONG) strcpy(token_text5, "long");
	else if (type == SHORT) strcpy(token_text5, "short");
	p->data.text = token_text5;
	root->l = p;
	//�ⲿ�����������У���������
	p = new ASTTree;
	p->type = EXTVARLIST;
	p->l = NULL;
	p->r = NULL;
	p->data.text = NULL;
	root->r = p;
	//�洢��������,���ⲿ�������壨�ڶ�����������
	p->l = new ASTTree;
	p->l->l = NULL;
	p->l->r = NULL;
	p->l->type = EXTVAR;
	char* token_text1 = new char[25];
	strcpy(token_text1, token_text);
	p->l->data.text = token_text1;
	while (1) {
		//���Ƕ���Ҳ���Ƿֺ�
		if (w != COMMA && w != SEMI && w != ASSIGN) {
			if (ncount > cnt) ncount--;
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ�򣺴�����ⲿ����";
			exit(0);
		}
		//�Ƿֺţ��������
		else if (w == SEMI) return root;
		else if (w == ASSIGN) {
			skip();
			w = trans(token_text);
			if (w == VOID) {
				cout << "��" << ncount << "�г��ִ���" << endl;
				cout << "����ԭ�򣺴���ĺ�������";
				exit(0);
			}
			if (((type == INT || type == LONG || type == SHORT) && w == INT_CONST) || ((type == DOUBLE || type == FLOAT) && w == FLOAT_CONST) || (type == CHAR && w == CHAR_CONST)) {
				ASTTree* m = new ASTTree;
				m->l = NULL;
				m->r = NULL;
				m->type = ASSIGN_CONST;
				char* token_text12 = new char[25];
				strcpy(token_text12, token_text);
				m->data.text = token_text12;
				p->l->l = m;
			}
			else {
				cout << "��" << ncount << "�г��ִ���" << endl;
				cout << "����ԭ�򣺶�����������Ͳ�ƥ�����������ֵ������";
				exit(0);
			}
			skip();
			continue;
		}
		skip();
		//�Ƕ��ŵ����治�Ǳ�ʶ������
		if (w != IDENT) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ�򣺴�����ⲿ����";
			exit(0);
		}
		addNameF(token_text);
		//�Ƕ��ŵ�����������������Ͷ���һ����
		ASTTree* q = new ASTTree;
		q->l = q->r = NULL;
		q->data.text = NULL;
		q->type = EXTVARLIST;
		p->r = q;
		p = q;
		//�洢��������
		p->l = new ASTTree;
		p->l->l = p->l->r = NULL;
		p->l->type = EXTVAR;
		char* token_text1 = new char[25];
		strcpy(token_text1, token_text);
		p->l->data.text = token_text1;
		cnt = ncount;
		skip();
	}
}

/// <summary>
/// ���ɺ�������ڵ�
/// </summary>
/// <returns>��������ڵ�</returns>
ASTTree* FuncDef() {
	//����������ڵ�
	ASTTree* root = new ASTTree;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	root->type = FUNCDEF;
	//�����������ͽڵ㣬�Ǹ��ڵ��������
	ASTTree* p = new ASTTree;
	p->type = FUNCRETURNTYPE;
	p->l = NULL;
	p->r = NULL;
	p->data.type = type;
	char* token_text6 = new char[25];
	if (type == INT) {
		strcpy(token_text6, "int");
		isVoid = 0;
	}
	if (type == DOUBLE) {
		strcpy(token_text6, "double");
		isVoid = 0;
	}
	if (type == CHAR) {
		strcpy(token_text6, "char");
		isVoid = 0;
	}
	if (type == FLOAT) {
		strcpy(token_text6, "float");
		isVoid = 0;
	}
	if (type == LONG) {
		strcpy(token_text6, "long");
		isVoid = 0;
	}
	if (type == SHORT) {
		strcpy(token_text6, "short");
		isVoid = 0;
	}
	if (type == VOID) {
		strcpy(token_text6, "void");
		isVoid = 1;//��Ǻ���������ֵ
	}
	p->data.text = token_text6;
	root->l = p;
	//�������ֽڵ㣬�Ǹ��ڵ��������
	ASTTree* q = new ASTTree;
	q->type = FUNCNAME;
	q->l = NULL;
	q->r = NULL;
	addNameFunc(token_text);
	char* token_text1 = new char[25];
	strcpy(token_text1, token_text);
	q->data.text = token_text1;

	VDN* last = Vroot;
	while (last->next != NULL) {
		last = last->next;
	}
	last->next = new VDN;
	last = last->next;
	last->next = NULL;
	last->size = 0;

	root->r = q;
	//�������������б��Ǻ������Ƶ�������
	q->l = FormParaList();

	FDN* m = Froot;
	while (m->next->next != NULL) m = m->next;
	VDN* n = Vroot;
	while (n->next != NULL) n = n->next;
	int i = 0;
	for (i = 0; i < (n->size); i++) {
		if (n->datas.type[i] == INT) m->name[i] = INT_CONST;
		else if (n->datas.type[i] == DOUBLE) m->name[i] = FLOAT_CONST;
		else if (n->datas.type[i] == CHAR) m->name[i] = CHAR_CONST;
		else if (n->datas.type[i] == FLOAT) m->name[i] = FLOAT_CONST;
		else if (n->datas.type[i] == LONG) m->name[i] = INT_CONST;
		else if (n->datas.type[i] == SHORT) m->name[i] = INT_CONST;
		m->size++;
	}


	skip();
	//����Ϊ�ֺţ��Ǻ�������
	if (w == SEMI) {
		//�������ƽ��������Ϊ��
		root->r->r = NULL;
		//������������Ϊ��������
		root->type = FUNCCLAIM;
	}
	//����Ϊ������ţ�Ϊ������
	else if (w == LBRACE) {
		q->r = CompState();
		q->r->type = FUNCBODY;
		if (isVoid == 0 && hasReturn == 0) {
			cout << "������ִ���" << endl;
			cout << "����ԭ�򣺺���ȱ�ٷ���ֵ";
			exit(0);
		}
	}
	else {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺴���ĺ��������������";
		exit(0);
	}
	return root;
}

/// <summary>
/// ���ɺ����������нڵ�
/// </summary>
/// <returns>�����������нڵ�</returns>
ASTTree* FormParaList() {
	skip();
	//���Ϊ��С���ţ���������
	if (w == RP) return NULL;
	//Ϊ���ţ�����
	if (w == COMMA) skip();
	//�½����������б���ڵ�
	ASTTree* root = new ASTTree;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	root->type = FUNCFORMALPARALIST;
	//�������涨�壬������Ϊ�β�����
	root->l = FormParaDef();
	root->r = FormParaList();
	return root;
}

/// <summary>
/// ���ɺ�����������
/// </summary>
/// <returns>������������</returns>
ASTTree* FormParaDef() {
	if (w != INT && w != DOUBLE && w != CHAR && w != LONG && w != SHORT && w != FLOAT && w != STRING_CONST) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺴���ĺ�����������";
		exit(0);
	}
	//�溯����������
	type = w;
	skip();
	//���Ǳ�ʶ��������
	if (w != IDENT) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺴���ĺ�����������";
		exit(0);
	}
	//�½���������������ڵ�
	ASTTree* root = new ASTTree;
	root->type = FUNCFORMALPARADEF;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	//�����������ͽڵ㣬�Ǹ��ڵ��������
	ASTTree* p = new ASTTree;
	p->type = FUNCFORMALPARATYPE;
	p->l = NULL;
	p->r = NULL;
	p->data.type = type;
	char* token_text7 = new char[25];
	if (type == INT) strcpy(token_text7, "int");
	if (type == DOUBLE) strcpy(token_text7, "double");
	if (type == CHAR) strcpy(token_text7, "char");
	if (type == FLOAT) strcpy(token_text7, "float");
	if (type == LONG) strcpy(token_text7, "long");
	if (type == SHORT) strcpy(token_text7, "short");
	p->data.text = token_text7;
	root->l = p;
	//�����������ƽ�㣬�Ǹ��ڵ��������
	p = new ASTTree;
	p->type = FUNCFORMALPARA;
	p->l = NULL;
	p->r = NULL;
	char* token_text1 = new char[25];
	strcpy(token_text1, token_text);
	p->data.text = token_text1;
	addName(token_text1);
	root->r = p;
	return root;
}

/// <summary>
/// ���ɸ��������
/// </summary>
/// <returns>���������</returns>
ASTTree* CompState() {
	//�½��������㣬������FuncDef��������
	ASTTree* root = new ASTTree;
	root->data.text = NULL;
	root->l = NULL;
	root->r = NULL;
	skip();
	//�������ڲ�����
	if (w == INT || w == DOUBLE || w == CHAR || w == LONG || w == SHORT || w == FLOAT) root->l = LocalVarDefList();
	else root->l = NULL;
	root->r = StateList();
	//���Ҵ����ţ��������
	if (w == RBRACE) return root;
	else {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺸�����䲿�ֳ���";
		exit(0);
	}
}

/// <summary>
/// ���ɾֲ�����������
/// </summary>
/// <returns>�ֲ�����������</returns>
ASTTree* LocalVarDefList() {
	//�����ֲ������������и��ڵ�
	ASTTree* root = new ASTTree;
	root->type = LOCALVARDEFLIST;
	root->data.text = NULL;
	root->l = NULL;
	root->r = NULL;
	//�ֲ����������㣬�Ǹ��ڵ��������
	ASTTree* p = new ASTTree;
	p->data.text = NULL;
	p->type = LOCALVARDEF;
	p->l = NULL;
	p->r = NULL;
	root->l = p;
	//�ֲ��������ͽ�㣬�Ǿֲ������������������
	p->l = new ASTTree;
	p->l->l = NULL;
	p->l->r = NULL;
	p->l->type = LOCALVARTYPE;
	type = w;
	char* token_text1 = new char[25];
	strcpy(token_text1, token_text);
	p->l->data.text = token_text1;

	skip();
	//�ֲ������������н�㣬�Ǿֲ������������������
	ASTTree* q = new ASTTree;
	q->data.text = NULL;
	q->l = NULL;
	q->r = NULL;
	p->r = q;
	q->type = LOCALVARNAMELIST;
	//�ֲ��������ƽ��
	q->l = new ASTTree;
	q->l->type = LOCALVARNAME;
	q->l->l = NULL;
	q->l->r = NULL;
	char* token_text2 = new char[25];
	strcpy(token_text2, token_text);
	q->l->data.text = token_text2;
	addName(token_text);
	while (1) {
		skip();
		//����Ƿֺţ��������
		if (w == SEMI) {
			q->r = NULL;
			skip();
			break;
		}
		//����Ƕ��ţ������ⲿ�������壬����һ��
		else if (w == COMMA) {
			skip();
			if (w != IDENT) {
				cout << "��" << ncount << "�г��ִ���" << endl;
				cout << "����ԭ�򣺴���ľֲ���������";
				exit(0);
			}
			//�ֲ������������н�㣬�Ǿֲ������������н��������
			ASTTree* s = new ASTTree;
			s->l = NULL;
			s->r = NULL;
			s->data.text = NULL;
			q->r = s;
			q = q->r;
			q->type = LOCALVARNAMELIST;
			//�ֲ��������ƽ�㣬��ǰһ�����н���������
			q->l = new ASTTree;
			q->l->l = NULL;
			q->l->r = NULL;
			q->l->type = LOCALVARNAME;
			char* token_text1 = new char[25];
			strcpy(token_text1, token_text);
			q->l->data.text = token_text1;
			//�����������
			addName(token_text);
		}
		else if (w == ASSIGN) {
			skip();
			w = trans(token_text);
			if (w == VOID) {
				cout << "��" << ncount << "�г��ִ���" << endl;
				cout << "����ԭ�򣺴���ĺ�������";
				exit(0);
			}
			if (((type == INT || type == LONG || type == SHORT) && w == INT_CONST) || ((type == DOUBLE || type == FLOAT) && w == FLOAT_CONST) || (type == CHAR && w == CHAR_CONST)) {
				ASTTree* m = new ASTTree;
				m->l = NULL;
				m->r = NULL;
				m->type = ASSIGN_CONST;
				char* token_text12 = new char[25];
				strcpy(token_text12, token_text);
				m->data.text = token_text12;
				q->l->l = m;
			}
			else {
				cout << "��" << ncount << "�г��ִ���" << endl;
				cout << "����ԭ�򣺶�����������Ͳ�ƥ��";
				exit(0);
			}
			continue;
		}
		else {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ�򣺴���ľֲ���������";
			exit(0);
		}
	}
	//�������þֲ������������к�������������
	if (w == INT || w == DOUBLE || w == CHAR || w == LONG || w == SHORT || w == FLOAT) root->r = LocalVarDefList();
	else root->r = NULL;
	return root;
}

/// <summary>
/// ����������ж�����
/// </summary>
/// <returns>������ж�����</returns>
ASTTree* StateList() {
	ASTTree* root = new ASTTree;
	root->l = NULL;
	root->r = NULL;
	ASTTree* r1 = new ASTTree;
	r1->l = NULL;
	r1->r = NULL;
	r1 = Statement();
	if (r1 == NULL) return NULL;
	else {
		//����������и����
		root = new ASTTree;
		root->data.text = NULL;
		root->type = STATELIST;
		root->l = r1;
		root->r = NULL;
		skip();
		//�����Ҵ����ţ�������δ������������
		if (w != RBRACE) {
			//������н����ã��Ǹ��ڵ��������
			root->r = StateList();
			return root;
		}
		else return root;
	}
}

/// <summary>
/// ���ɸ��������
/// </summary>
/// <returns>���������</returns>
ASTTree* Statement() {
	//�½�����㣬���Ͷ����ں���
	ASTTree* root = new ASTTree;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	//if��䴦��
	switch (w) {
	case IF: {
		skip();
		//if���治��(������
		if (w != LP) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ�򣺴����if�����";
			exit(0);
		}
		skip();
		//if���������㣬��������������
		ASTTree* p1 = new ASTTree;
		p1->r = NULL;
		p1->data.text = NULL;
		p1->type = IFPART;
		p1->l = Expression(RP);
		if (p1->l == NULL) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��if����������ֳ���";
			exit(0);
		}
		skip();
		//Ϊ������ţ�����if����������
		if (w == LBRACE) {
			skip();
			//������У���if��䲿�ֵ�������
			p1->r = StateList();
		}
		//����������ţ�����ֻ��һ��
		else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT || w == INT || w == FLOAT || w == CHAR || w == RETURN || w == BREAK || w == CONTINUE || w == SHORT || w == LONG || w == DOUBLE || w == IF || w == WHILE || w == ELSE || w == FOR || w == DO) {
			//����������䲿�֣���if��䲿�ֵ�������
			p1->r = Statement();
		}
		else {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��if����ڳ���";
			exit(0);
		}
		root->l = p1;
		skip();
		//Ϊelse������ifelse�жϽṹ���
		if (w == ELSE) {
			root->type = IFELSESTATEMENT;
			//else��䲿�֣��������ڵ��������
			ASTTree* p2 = new ASTTree;
			p2->l = NULL;
			p2->r = NULL;
			p2->data.text = NULL;
			p2->type = ELSEPART;
			root->r = p2;
			skip();
			//Ϊ������ţ�����else�����
			if (w == LBRACE) {
				//��������������У���else����������
				skip();
				p2->r = StateList();
			}
			else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT || w == INT || w == FLOAT || w == CHAR || w == RETURN || w == BREAK || w == CONTINUE || w == SHORT || w == LONG || w == DOUBLE || w == IF || w == WHILE || w == ELSE || w == FOR || w == DO) {
				//���Դ����ţ�ֱ�ӽ�����䲿�ֽ�㣬��else����������
				p2->r = Statement();
			}
			else if (w == IF) {
				//Ϊelse if�ṹ����else���������
				p2->l = Statement();
			}
			else {
				cout << "��" << ncount << "�г��ִ���" << endl;
				cout << "����ԭ��else����ڳ���";
				exit(0);
			}
		}
		//����else���˻�w���ļ���
		else {
			root->type = IFSTATEMENT;
			returnToken(w, fp);
		}
		return root;
	}
		   //while��䴦��
	case WHILE: {
		isInrecycle = 1;
		skip();
		//������С����
		if (w != LP) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��while����ڳ���";
			exit(0);
		}
		skip();
		//����while���ʽ���ֽ�㣬��������������
		ASTTree* p1 = new ASTTree;
		p1->data.text = NULL;
		p1->type = WHILEPART;
		p1->r = NULL;
		p1->l = Expression(RP);
		if (p1->l == NULL) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��while����������ֳ���";
			exit(0);
		}
		//while������壬��������������
		ASTTree* p2 = new ASTTree;
		p2->data.text = NULL;
		p2->type = WHILEBODY;
		p2->l = NULL;
		p2->r = NULL;
		skip();
		//���������
		if (w == LBRACE) {
			skip();
			//��ת��������н�㣬��while��������������
			p2->r = StateList();
		}
		//����������ţ�ͬ��if���
		else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT || w == INT || w == FLOAT || w == CHAR || w == RETURN || w == BREAK || w == CONTINUE || w == VOID || w == SHORT || w == LONG || w == DOUBLE || w == IF || w == WHILE || w == ELSE || w == FOR || w == DO) {
			//��ת������㣬��while��������������
			p2->r = Statement();
		}
		else {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��while����ڳ���";
			exit(0);
		}
		root->type = WHILESTATEMENT;
		root->l = p1;
		root->r = p2;
		isInrecycle = 0;
		return root;
	}
			  //for��䴦��
	case FOR: {
		isInrecycle = 1;
		skip();
		if (w != LP) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��for����ڳ���";
			exit(0);
		}
		skip();
		//for��䲿�ֽ�㣬�������������
		ASTTree* p1 = new ASTTree;
		p1->l = NULL;
		p1->r = NULL;
		p1->data.text = NULL;
		p1->type = FORPART;
		//for����һ���ֽ�㣬��for�������������ͬʱ�����ʽ����
		ASTTree* q = new ASTTree;
		q->r = NULL;
		p1->l = q;
		q->type = FORPART1;
		q->l = Expression(SEMI);
		if (q->l == NULL) {
			char* token_text8 = new char[25];
			strcpy(token_text8, "None");
			q->data.text = token_text8;
		}
		else q->data.text = NULL;
		skip();
		//for���ڶ����ֽ�㣬��for����һ���ֽ����������ͬʱ�����ʽ����
		q->r = new ASTTree;
		q = q->r;
		q->type = FORPART2;
		q->l = Expression(SEMI);
		if (q->l == NULL) {
			char* token_text9 = new char[25];
			strcpy(token_text9, "None");
			q->data.text = token_text9;
		}
		else q->data.text = NULL;
		skip();
		//for���������ֽ�㣬��for���ڶ����ֽ����������ͬʱ�����ʽ����
		q->r = new ASTTree;
		q = q->r;
		q->r = NULL;
		q->type = FORPART3;
		q->l = Expression(RP);
		if (q->l == NULL) {
			char* token_text10 = new char[25];
			strcpy(token_text10, "None");
			q->data.text = token_text10;
		}
		else q->data.text = NULL;
		//for������㴴������for��䲿�ֽ��������
		ASTTree* p2 = new ASTTree;
		p2->l = NULL;
		p2->r = NULL;
		p2->type = FORBODY;
		p2->data.text = NULL;
		skip();
		//Ϊ������ţ���ת��������У�����ͬ��
		if (w == LBRACE) {
			skip();
			p2->r = StateList();
		}
		else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT || w == INT || w == FLOAT || w == CHAR || w == RETURN || w == BREAK || w == CONTINUE || w == VOID || w == SHORT || w == LONG || w == DOUBLE || w == IF || w == WHILE || w == ELSE || w == FOR || w == DO) {
			//Ϊ�ؼ����࣬��ת����䲿��
			p2->r = Statement();
		}
		else {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��for����ڳ���";
			exit(0);
		}
		//���for�����
		root->type = FORSTATEMENT;
		root->l = p1;
		root->r = p2;
		isInrecycle = 0;
		return root;
	}
			//return��䴦��
	case RETURN: {
		//��������ͱ��Ϊreturn
		root->type = RETURNSTATEMENT;
		root->l = NULL;
		root->data.text = NULL;
		skip();
		//�����ʽ����
		root->r = Expression(SEMI);
		if (root->r == NULL && isVoid == 0) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��ȱ�ٷ���ֵ";
			exit(0);
		}
		else if (root->r != NULL && isVoid == 1) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ�򣺴˴���Ӧ�з���ֵ";
			exit(0);
		}
		else hasReturn = 1;
		return root;
	}
			   //do-while���
	case DO: {
		isInrecycle = 1;
		skip();
		//ȱ���������
		if (w != LBRACE) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��do-while������ȱ�ٱ�Ҫ������";
			exit(0);
		}
		//����do-while������㣬��������������
		ASTTree* p1 = new ASTTree;
		p1->type = DOWHILEBODY;
		p1->l = NULL;
		p1->r = NULL;
		//do-while��������㣬��������������
		ASTTree* p2 = new ASTTree;
		p2->type = DOWHILECONDITION;
		p2->r = NULL;
		root->l = p1;
		root->r = p2;
		p1->data.text = p2->data.text = NULL;
		//����������У���do-while��������������
		skip();
		p1->l = StateList();
		skip();
		if (w != WHILE) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��do-while������ȱʧwhile�ؼ���";
			exit(0);
		}
		//do-while��䲿�����͸�ֵ
		root->type = DOWHILESTATEMENT;
		skip();
		//û����С����
		if (w != LP) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��do-while������ȱ�ٱ�Ҫ������";
			exit(0);
		}
		//����С���ţ������ʽ����
		skip();
		p2->l = Expression(RP);
		if (p2->l == NULL) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��do-while������ȱ�ٱ�Ҫ������";
			exit(0);
		}
		skip();
		if (w != SEMI) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��do-while������ȱ�ٱ�Ҫ�ķֺ�";
			exit(0);
		}
		isInrecycle = 0;
		return root;
	}
		   //break���
	case BREAK: {
		skip();
		if (w != SEMI) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��break���ȱ�ٷֺ�";
			exit(0);
		}
		//δ����ѭ������
		if (isInrecycle == 0) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ�򣺲���Ҫ��break���";
			exit(0);
		}
		//������������
		root->type = BREAKSTATEMENT;
		return root;
	}
			  //continue���
	case CONTINUE: {
		skip();
		if (w != SEMI) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ��continue���ȱ�ٷֺ�";
			exit(0);
		}
		//ͬ��break
		/*if (isInrecycle == 0) {
			cout << "��" << ncount << "�г��ִ���" << endl;
			cout << "����ԭ�򣺲���Ҫ��continue���";
			exit(0);
		}*/
		//continue��䲿��
		root->type = CONTINUESTATEMENT;
		return root;
	}
				 //���ʽ����
	case INT_CONST:
	case FLOAT_CONST:
	case CHAR_CONST:
	case STRING_CONST:
	case IDENT:
	case ARRAY:
		return Expression(SEMI);
	case INT:
	case DOUBLE:
	case LONG:
	case CHAR:
	case SHORT:
	case FLOAT:
		return LocalVarDefList();
	default:
		return NULL;
	}
	return root;
}

/// <summary>
/// ���ɱ��ʽ������
/// </summary>
/// <param name="endsym">���ʽ��������</param>
/// <returns>���ʽ������</returns>
ASTTree* Expression(int endsym) {
	//���������
	if (w == endsym) return NULL;
	int flag = 0;
	int error = 0;
	//�����ջ
	stack<ASTTree*> op;
	ASTTree* p = new ASTTree;
	p->l = NULL;
	p->r = NULL;
	p->data.text = NULL;
	p->type = OPERATOR;//��������
	p->data.type = POUND;//#����Ϊ��ֹ��
	//��p����ջ����Ϊ��ֹ����
	op.push(p);
	stack<ASTTree*> opn;//������ջ
	ASTTree* t = NULL;
	ASTTree* t1 = NULL;
	ASTTree* t2 = NULL;
	ASTTree* root = NULL;
	while (((w != endsym) || (op.top()->data.type != POUND)) && !error) {
		//û���������Ż���ջû����ֹ����ͬʱ�ޱ���
		//ջ��Ԫ��Ϊ��С���ţ�ջ��û����ֹ��
		if (op.top()->data.type == RP) {
			//ջ�Ĵ�СС��3����С�����޷��ҵ�ƥ�����С���źͲ�����
			if (op.size() < 3) {
				error++;
				break;
			}
			//����ջ������Ԫ��
			op.pop();
			op.pop();
		}
		//�ж��Ƿ���δ�����ı���
		if (w == IDENT) {
			int m = checkname_exist(token_text);
			w = trans(token_text);
		}
		//����ȷ�Ĳ��������뵽ջ��
		if (w == IDENT || w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == ARRAY || w == STRING_CONST) {
			//���������
			p = new ASTTree;
			p->type = OPERAND;
			p->l = NULL;
			p->r = NULL;
			char* token_text1 = new char[25];
			strcpy(token_text1, token_text);
			//char* x = p->data.text;
			p->data.text = token_text1;
			//x = p->data.text;
			//���������ջ
			opn.push(p);
			//����ȡ��һ���ʷ�����
			skip();
		}
		else if (w == endsym) {
			//���������ţ�Ȼ��ʼ�Ӳ�����ջ�е��������
			while (op.top()->data.type != POUND) {
				//t2ȡ������ջ��Ԫ��
				t2 = opn.top();
				//��������������Լ�����
				if (!t2 && (op.top()->data.type) != INC &&
					(op.top()->data.type) != DEC) {
					error++;
					break;
				}
				if (t2 != NULL) opn.pop();
				if (opn.size() == 0) t1 = NULL;
				else t1 = opn.top();
				if (!t1 && (op.top()->data.type) != INC &&
					(op.top()->data.type) != DEC) {
					error++;
					break;
				}
				if (t1 != NULL) opn.pop();
				t = op.top();
				if (!t) {
					error++;
					break;
				}
				op.pop();
				t->l = t1;
				t->r = t2;
				flag++;
				opn.push(t);
			}
			if (opn.size() != 1)
				error++;
		}
		else if (w >= EQ && w <= DEC) {
			char* token_text1 = new char[25];
			switch (Precede(op.top()->data.type, w)) {
			case '<':
				p = new ASTTree;
				p->l = NULL;
				p->r = NULL;
				p->type = OPERATOR;
				p->data.type = w;
				strcpy(token_text1, token_text);
				p->data.text = token_text1;
				op.push(p);
				skip();
				break;
			case '=':
				t = op.top();
				if (!t) {
					error++;
					op.pop();
				}
				skip();
				break;
			case '>':
				t2 = opn.top();
				if (!t2 && (op.top()->data.type) != INC &&
					(op.top()->data.type) != DEC) {
					error++;
					break;
				}
				if (t2 != NULL) opn.pop();
				if (opn.size() == 0) t1 = NULL;
				else t1 = opn.top();
				if (!t1 && (op.top()->data.type) != INC &&
					(op.top()->data.type) != DEC) {
					error++;
					break;
				}
				if (t1 != NULL) opn.pop();
				t = op.top();
				if (!t) {
					error++;
					break;
				}
				op.pop();
				t->l = t1;
				t->r = t2;
				opn.push(t);
				p = new ASTTree;
				p->l = NULL;
				p->r = NULL;
				p->type = OPERATOR;
				p->data.type = w;
				strcpy(token_text1, token_text);
				p->data.text = token_text1;
				op.push(p);
				skip();
				break;
			case '\0':
				cout << "��" << ncount << "�г��ִ���" << endl;
				cout << "����ԭ��δ֪�������";
				exit(0);
			}
		}
		else error = 1;
	}
	//�ӵ����ʽ�ڵ���
	if ((opn.size() == 1) && (op.top()->data.type == POUND) && error == 0) {
		t = opn.top();
		opn.pop();
		root = new ASTTree;
		root->data.text = NULL;
		root->r = NULL;
		root->type = EXPRESSION;
		root->l = t;
		return root;
	}
	else {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ�򣺴���ı��ʽ";
		exit(0);
	}
}

/// <summary>
/// �������Ƿ����
/// </summary>
/// <param name="token_text"></param>
/// <returns></returns>
int checkname_exist(char* token_text) {
	int i;
	int flag = 0;
	VDN* p = Vroot;
	while (p->next != NULL) p = p->next;
	//��ֲ���������
	for (i = 0; i < (p->size); i++) {
		if (strcmp(token_text, p->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	//��ȫ�ֱ�������
	for (i = 0; i < (Vroot->size); i++) {
		if (strcmp(token_text, Vroot->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		cout << "��" << ncount << "�г��ִ���" << endl;
		cout << "����ԭ��ʹ����δ�����ı���";
		exit(0);
	}
	return flag;
}

/// <summary>
/// ��������ȼ��ж�
/// </summary>
/// <param name="c1">���Ƚϵ������</param>
/// <param name="c2">���Ƚϵ������</param>
/// <returns>�������ȼ���ϵ</returns>
char Precede(int c1, int c2) {
	if (c1 == ADD || c1 == SUB) {
		switch (c2) {
		case ADD:
		case SUB:
		case RP:
		case POUND:
		case GT:
		case LT:
		case GTE:
		case LTE:
		case EQ:
		case NEQ:
		case ASSIGN:
			return '>';
		case MUL:
		case DIV:
		case LP:
		case INC:
		case DEC:
			return '<';
		default:
			return '\0';
			break;
		}
	}
	else if (c1 == MUL || c1 == DIV) {
		switch (c2) {
		case ADD:
		case SUB:
		case RP:
		case POUND:
		case MUL:
		case DIV:
		case GT:
		case LT:
		case GTE:
		case LTE:
		case EQ:
		case NEQ:
			return '>';
		case LP:
		case INC:
		case DEC:
			return '<';
		default:
			return '\0';
		}
	}
	else if (c1 == LP) {
		switch (c2) {
		case ADD:
		case SUB:
		case MUL:
		case DIV:
		case LP:
		case INC:
		case DEC:
			return '<';
		case RP:
			return '=';
		case GT:
		case LT:
		case GTE:
		case LTE:
		case EQ:
		case NEQ:
		case POUND:
			return '>';
		default:
			return '\0';
		}
	}
	else if (c1 == RP) {
		switch (c2) {
		case ADD:
		case SUB:
		case MUL:
		case DIV:
		case LP:
		case GT:
		case LT:
		case GTE:
		case LTE:
		case EQ:
		case NEQ:
		case INC:
		case DEC:
		case POUND:
			return '>';
		default:
			return '\0';
		}
	}
	else if (c1 == ASSIGN) {
		switch (c2) {
		case ADD:
		case SUB:
		case MUL:
		case DIV:
		case LP:
		case GT:
		case LT:
		case GTE:
		case LTE:
		case EQ:
		case NEQ:
		case INC:
		case DEC:
			return '<';
		case POUND:
			return '>';
		default:
			return '\0';
		}
	}
	else if (c1 == GT || c1 == LT || c1 == GTE || c1 == LTE) {
		switch (c2) {
		case ADD:
		case SUB:
		case MUL:
		case DIV:
		case LP:
		case INC:
		case DEC:
			return '<';
		case RP:
		case GT:
		case LT:
		case GTE:
		case LTE:
		case EQ:
		case NEQ:
		case POUND:
			return '>';
		default:
			return '\0';
		}
	}
	else if (c1 == EQ || c1 == NEQ) {
		switch (c2) {
		case ADD:
		case SUB:
		case MUL:
		case DIV:
		case LP:
		case GT:
		case LT:
		case GTE:
		case LTE:
		case INC:
		case DEC:
			return '<';
		case RP:
		case EQ:
		case NEQ:
		case POUND:
			return '>';
		default:
			return '\0';
		}
	}
	else if (c1 == POUND) {
		switch (c2) {
		case ADD:
		case SUB:
		case MUL:
		case DIV:
		case LP:
		case GT:
		case LT:
		case GTE:
		case LTE:
		case RP:
		case EQ:
		case NEQ:
		case ASSIGN:
		case INC:
		case DEC:
			return '<';
		case POUND:
			return '=';
		default:
			return '\0';
		}
	}
	else if (c1 == INC || c1 == DEC) {
		switch (c2) {
		case ADD:
		case SUB:
		case MUL:
		case DIV:
		case LP:
		case GT:
		case LT:
		case GTE:
		case LTE:
		case EQ:
		case NEQ:
		case ASSIGN:
		case POUND:
			return '>';
		case RP:
			return '<';
		default:
			return '\0';
		}
	}
	else return '\0';
}

/// <summary>
/// ���Ѿ���ȡ�Ĵʷ���Ԫw�˻ص��ļ���
/// </summary>
/// <param name="w">�Ѿ���ȡ�Ĵʷ���Ԫ</param>
/// <param name="fp">�ļ�ָ��</param>
void returnToken(int w, FILE* fp) {
	int n = strlen(token_text);
	int i;
	for (i = 0; i < n; i++) {
		ungetc(token_text[n - 1 - i], fp);
	}
}