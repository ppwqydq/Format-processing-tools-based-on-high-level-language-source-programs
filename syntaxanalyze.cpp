#define _CRT_SECURE_NO_WARNINGS
#include <stack>
#include <string.h>
#include "syntaxanalyze.h"
#include "lexicalanalyze.h"

using namespace std;

extern char c;
extern char token_text[100];//用于存储词法单元的全局变量
extern char string_num[100];
extern char FileName[50];//文件名
extern FILE* fp;
extern int ncount;//用于记录代码所在行数


int type;//存储变量类型
VDN* Vroot;//链表头节点
int isVoid, hasReturn, isInrecycle;
int w;//存储当前读入的单词种类编码
FDN* Froot;//函数链表头节点

/// <summary>
/// 用于调试过程中检查变量作用域
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
/// 生成输出语法树
/// </summary>
void syntaxAnalyze() {
	cout << "请输入文件名:" << endl;
	cin >> FileName;
	fp = fopen(FileName, "r");
	if (fp == NULL) {//打开失败
		cout << "文件打开失败";
		return;
	}
	ASTTree* root = program();
	if (root == NULL) {
		cout << "有语法错误";
		fclose(fp);//关闭文件
		return;
	}
	else {
		PreTraverse(root, 0);
		//Traverse();
		freeTree(root);
		fclose(fp);//关闭文件
		return;
	}
}

/// <summary>
/// 释放语法树内存
/// </summary>
/// <param name="root">语法树根节点</param>
void freeTree(ASTTree* root) {
	if (root) {
		freeTree(root->l);
		freeTree(root->r);
		delete(root);
	}
}

/// <summary>
/// 读取下一个词法单元，同时跳过头文件，注释，宏
/// </summary>
void skip() {
	w = gettoken(fp)->type;
	while (w == ANNO || w == INCLUDE || w == MACRO) w = gettoken(fp)->type;
	if (w == ERROR_TOKEN) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：错误的词法单元" << endl;
		exit(0);
	}
}

/// <summary>
/// 先序遍历输出抽象语法树
/// </summary>
/// <param name="root">程序根节点</param>
/// <param name="depth">语句深度</param>
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
/// 输出各个语法单元类型
/// </summary>
/// <param name="type">结点类型</param>
void showType(int type) {
	switch (type) {
	case 1:
		//cout << "外部定义序列" << endl;
		break;
	case 2:
		cout << "外部变量定义" << endl;
		break;
	case 3:
		cout << "外部变量定义类型：";
		break;
	case 4:
		//cout << "外部变量名称序列" << endl;
		break;
	case 5:
		cout << "外部变量名称：";
		break;
	case 6:
		cout << "函数定义" << endl;
		break;
	case 7:
		cout << "函数返回类型：";
		break;
	case 8:
		cout << "函数名称：";
		break;
	case 9:
		cout << "函数参数序列" << endl;
		break;
	case 10:
		//cout << "函数参数定义" << endl;
		break;
	case 11:
		cout << "函数参数定义类型：";
		break;
	case 12:
		cout << "函数参数名称：";
		break;
	case 13:
		cout << "函数体" << endl;
		break;
	case 14:
		//cout << "局部变量定义序列" << endl;
		break;
	case 15:
		cout << "局部变量定义" << endl;
		break;
	case 16:
		cout << "局部变量定义类型：";
		break;
	case 17:
		cout << "局部变量名称序列" << endl;
		break;
	case 18:
		cout << "局部变量名称：";
		break;
	case 19:
		//cout << "语句序列" << endl;
		break;
	case 20:
		cout << "操作数：";
		break;
	case 21:
		cout << "运算符：";
		break;
	case 22:
		cout << "表达式" << endl;
		break;
	case 23:
		cout << "if语句条件部分" << endl;
		break;
	case 24:
		cout << "else语句部分" << endl;
		break;
	case 25:
		cout << "if语句" << endl;
		break;
	case 26:
		cout << "if-else语句" << endl;
		break;
	case 27:
		cout << "while语句" << endl;
		break;
	case 28:
		cout << "while语句条件部分" << endl;
		break;
	case 29:
		cout << "while语句体" << endl;
		break;
	case 30:
		cout << "for语句" << endl;
		break;
	case 31:
		cout << "for语句条件" << endl;
		break;
	case 32:
		cout << "for语句条件1" << endl;
		break;
	case 33:
		cout << "for语句条件2" << endl;
		break;
	case 34:
		cout << "for语句条件3" << endl;
		break;
	case 35:
		cout << "for语句体" << endl;
		break;
	case 36:
		cout << "return语句" << endl;
		break;
	case 37:
		cout << "break语句" << endl;
		break;
	case 38:
		cout << "do-while语句" << endl;
		break;
	case 39:
		cout << "do-while语句体" << endl;
		break;
	case 40:
		cout << "do-while语句条件部分" << endl;
		break;
	case 41:
		cout << "continue语句" << endl;
		break;
	case 42:
		cout << "函数声明" << endl;
		break;
	case 43:
		cout << "数组定义" << endl;
		break;
	case 44:
		cout << "数组类型：";
		break;
	case 45:
		cout << "数组名称：";
		break;
	case 46:
		cout << "数组大小：";
		break;
	case 47:
		cout << "数组定义序列" << endl;
		break;
	case 48:
		cout << "赋值为：";
	}
}

/// <summary>
/// 生成程序根结点
/// </summary>
/// <returns>程序根结点</returns>
ASTTree* program() {
	skip();
	//初始化节点
	Vroot = new VDN;
	//变量个数为0
	Vroot->size = 0;
	Vroot->next = NULL;
	Froot = new FDN;
	Froot->next = NULL;
	Froot->size = 0;
	//生成外部定义序列结点
	ASTTree* p = ExtDefList();
	if (p != NULL) {
		//新建根节点，将其设为根结点
		ASTTree* root = new ASTTree;
		root->l = NULL;
		root->r = NULL;
		root = p;
		root->type = EXTDEFLIST;//结点类型
		return root;
	}
	else return NULL;
}

/// <summary>
/// 生成外部定义序列结点
/// </summary>
/// <returns>外部定义序列结点</returns>
ASTTree* ExtDefList() {
	// 判断是否到达文件结束
	if (w == ERROR)
		return NULL;
	//创建根结点
	ASTTree* node = new ASTTree;
	node->type = EXTDEFLIST;
	node->data.text = NULL;
	//左子树解析一个外部定义
	node->l = ExtDef();
	skip();
	//递归调用，继续解析后续外部定义
	node->r = ExtDefList();
	return node;
}

/// <summary>
/// 生成外部定义结点
/// </summary>
/// <returns>外部定义结点</returns>
ASTTree* ExtDef() {
	int a;
	//不是变量定义类型
	if (w != INT && w != DOUBLE && w != CHAR && w != LONG && w != SHORT && w != FLOAT && w != VOID) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：错误的外部定义";
		exit(0);
	}
	//存储变量定义类型，便于后续保存使用
	type = w;
	skip();

	//既不是标识符也不是数组
	if (w != IDENT && w != ARRAY) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：错误的外部定义";
		exit(0);
	}
	//存储变量类型，便于后续保存使用
	a = w;
	//外部定义结点
	ASTTree* p = new ASTTree;
	p->l = NULL;
	p->r = NULL;
	char* token_text0 = new char[25];
	strcpy(token_text0, token_text);
	skip();
	strcpy(token_text, token_text0);
	if (w == LP) {
		p = FuncDef();//左括号，函数定义
		hasReturn = 0;
	}
	else if (a == ARRAY) p = ArrayDef();//变量名为数组，数组定义
	else p = ExtVarDef();//外部变量定义
	return p;
}

/// <summary>
/// 处理数组名称
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
/// 生成数组定义结点
/// </summary>
/// <returns>数组定义结点</returns>
ASTTree* ArrayDef() {
	//void不能定义数组，报错
	if (type == VOID) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：不能定义void类型的数组";
		exit(0);
	}
	arraySolve(token_text);
	//将变量加入链表
	addNameF(token_text);
	//数组定义节点初始化
	ASTTree* node = new ASTTree;
	node->type = ARRAYDEF;
	node->l = NULL;
	node->r = NULL;
	node->data.text = NULL;
	//数组类型，是定义结点左子树
	ASTTree* p = new ASTTree;
	p->type = ARRAYTYPE;
	p->l = NULL;
	p->r = NULL;
	//存储数组类型
	char* token_text4 = new char[25];
	if (type == INT) strcpy(token_text4, "int");
	else if (type == DOUBLE) strcpy(token_text4, "double");
	else if (type == CHAR) strcpy(token_text4, "char");
	else if (type == FLOAT) strcpy(token_text4, "float");
	else if (type == LONG) strcpy(token_text4, "long");
	else if (type == SHORT) strcpy(token_text4, "short");
	p->data.text = token_text4;
	node->l = p;
	//数组名称，是定义结点右子树
	ASTTree* q = new ASTTree;
	q->type = ARRAYNAME;
	q->l = NULL;
	q->r = NULL;
	char* token_text3 = new char[25];
	strcpy(token_text3, token_text);
	q->data.text = token_text3;
	//继续存储数组大小
	node->r = q;
	ASTTree* m = new ASTTree;
	m->type = ARRAYSIZE;
	m->l = NULL;
	m->r = NULL;
	char* token_text5 = new char[25];
	strcpy(token_text5, string_num);
	//数组大小值存储
	m->data.text = token_text5;
	//是名称结点左孩子结点
	q->l = m;
	return node;
}

/// <summary>
/// 将变量名称加入到变量定义链表中
/// </summary>
/// <param name="token_text"></param>
/// <returns></returns>
void addName(char* token_text) {
	int i, flag = 0;
	//链表头节点
	VDN* p = Vroot;
	//到链表末尾
	while (p->next != NULL) p = p->next;
	for (i = 0; i < (p->size); i++) {
		//变量名称重复
		if (strcmp(token_text, p->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	if (flag == 1) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：重复的变量定义";
		exit(0);
	}
	//保存变量名
	char* savename = new char[25];
	strcpy(savename, token_text);
	p->datas.variable[p->size] = savename;
	p->datas.type[p->size] = type;
	p->size++;
	return;
}

/// <summary>
/// 将变量名称加入到全局变量定义链表中
/// </summary>
/// <param name="token_text"></param>
/// <returns></returns>
void addNameF(char* token_text) {
	int i, flag = 0;
	//链表头节点
	VDN* p = Vroot;
	//到链表末尾
	for (i = 0; i < (p->size); i++) {
		//变量名称重复
		if (strcmp(token_text, p->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	if (flag == 1) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：重复的变量定义";
		exit(0);
	}
	//保存变量名
	char* savename = new char[25];
	strcpy(savename, token_text);
	p->datas.variable[p->size] = savename;
	p->datas.type[p->size] = type;
	p->size++;
	return;
}

/// <summary>
/// 将函数名称加入到全局变量定义链表中
/// </summary>
/// <param name="token_text"></param>
/// <returns></returns>
void addNameFunc(char* token_text) {
	int i, flag = 0;
	//链表头节点
	VDN* p = Vroot;
	//到链表末尾
	for (i = 0; i < (p->size); i++) {
		//变量名称重复
		if (strcmp(token_text, p->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	if (flag == 1) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：重复的变量定义";
		exit(0);
	}
	//保存变量名
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
/// 用于转化局部变量类型
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
/// 处理函数调用
/// </summary>
/// <param name="token_text2">可能调用的函数名</param>
void transF(char* token_text2) {
	char token_text3[25];
	strcpy(token_text3, token_text2);
	FDN* k = Froot;
	while (k->next != NULL) {
		if (strcmp(k->fName, token_text3) == 0) break;
	}
	int i = 0;
	int j = 0;//控制循环
	skip();
	if (w != LP) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：错误的表达式";
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
				cout << "第" << ncount << "行出现错误" << endl;
				cout << "错误原因：错误的表达式";
				exit(0);
			}
			else {
				w = trans(token_text);
				if (w != k->name[i]) {
					cout << "第" << ncount << "行出现错误" << endl;
					cout << "错误原因：与函数参数类型不匹配";
					exit(0);
				}
				strcat(token_text3, token_text);
				i++;
				j++;
			}
		}
		skip();
		if (w != RP) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：错误的表达式";
			exit(0);
		}
		else {
			addtoken(token_text3, ')');
			strcpy(token_text2, token_text3);
		}
	}
}

/// <summary>
/// 生成外部变量定义节点
/// </summary>
/// <returns>外部变量定义节点</returns>
ASTTree* ExtVarDef() {
	int cnt = 0;
	//void不能定义变量
	if (type == VOID) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：不能定义void类型的变量";
		exit(0);
	}
	//将变量加入链表
	addNameF(token_text);
	//新建外部变量定义结点
	ASTTree* root = new ASTTree;
	root->type = EXTVARDEF;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	//存储外部变量类型，是定义节点左子树
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
	//外部变量定义序列，是右子树
	p = new ASTTree;
	p->type = EXTVARLIST;
	p->l = NULL;
	p->r = NULL;
	p->data.text = NULL;
	root->r = p;
	//存储变量名称,是外部变量定义（第二个）左子树
	p->l = new ASTTree;
	p->l->l = NULL;
	p->l->r = NULL;
	p->l->type = EXTVAR;
	char* token_text1 = new char[25];
	strcpy(token_text1, token_text);
	p->l->data.text = token_text1;
	while (1) {
		//不是逗号也不是分号
		if (w != COMMA && w != SEMI && w != ASSIGN) {
			if (ncount > cnt) ncount--;
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：错误的外部定义";
			exit(0);
		}
		//是分号，定义完成
		else if (w == SEMI) return root;
		else if (w == ASSIGN) {
			skip();
			w = trans(token_text);
			if (w == VOID) {
				cout << "第" << ncount << "行出现错误" << endl;
				cout << "错误原因：错误的函数调用";
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
				cout << "第" << ncount << "行出现错误" << endl;
				cout << "错误原因：定义变量与类型不匹配或者所赋的值不存在";
				exit(0);
			}
			skip();
			continue;
		}
		skip();
		//是逗号但后面不是标识符报错
		if (w != IDENT) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：错误的外部定义";
			exit(0);
		}
		addNameF(token_text);
		//是逗号的情况，变量定义类型都是一样的
		ASTTree* q = new ASTTree;
		q->l = q->r = NULL;
		q->data.text = NULL;
		q->type = EXTVARLIST;
		p->r = q;
		p = q;
		//存储变量名称
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
/// 生成函数定义节点
/// </summary>
/// <returns>函数定义节点</returns>
ASTTree* FuncDef() {
	//函数定义根节点
	ASTTree* root = new ASTTree;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	root->type = FUNCDEF;
	//函数返回类型节点，是根节点的左子树
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
		isVoid = 1;//标记函数不返回值
	}
	p->data.text = token_text6;
	root->l = p;
	//函数名字节点，是根节点的右子树
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
	//构建函数参数列表，是函数名称的左子树
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
	//后面为分号，是函数声明
	if (w == SEMI) {
		//函数名称结点右子树为空
		root->r->r = NULL;
		//函数定义结点标记为函数声明
		root->type = FUNCCLAIM;
	}
	//后面为左大括号，为函数体
	else if (w == LBRACE) {
		q->r = CompState();
		q->r->type = FUNCBODY;
		if (isVoid == 0 && hasReturn == 0) {
			cout << "程序出现错误" << endl;
			cout << "错误原因：函数缺少返回值";
			exit(0);
		}
	}
	else {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：错误的函数定义或者声明";
		exit(0);
	}
	return root;
}

/// <summary>
/// 生成函数参数序列节点
/// </summary>
/// <returns>函数参数序列节点</returns>
ASTTree* FormParaList() {
	skip();
	//如果为右小括号，不含参数
	if (w == RP) return NULL;
	//为逗号，跳过
	if (w == COMMA) skip();
	//新建函数参数列表根节点
	ASTTree* root = new ASTTree;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	root->type = FUNCFORMALPARALIST;
	//左子树存定义，右子树为形参序列
	root->l = FormParaDef();
	root->r = FormParaList();
	return root;
}

/// <summary>
/// 生成函数参数定义
/// </summary>
/// <returns>函数参数定义</returns>
ASTTree* FormParaDef() {
	if (w != INT && w != DOUBLE && w != CHAR && w != LONG && w != SHORT && w != FLOAT && w != STRING_CONST) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：错误的函数参数定义";
		exit(0);
	}
	//存函数参数类型
	type = w;
	skip();
	//不是标识符，报错
	if (w != IDENT) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：错误的函数参数定义";
		exit(0);
	}
	//新建函数参数定义根节点
	ASTTree* root = new ASTTree;
	root->type = FUNCFORMALPARADEF;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	//函数参数类型节点，是根节点的左子树
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
	//函数参数名称结点，是根节点的右子树
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
/// 生成复合语句结点
/// </summary>
/// <returns>复合语句结点</returns>
ASTTree* CompState() {
	//新建函数体结点，类型在FuncDef函数里面
	ASTTree* root = new ASTTree;
	root->data.text = NULL;
	root->l = NULL;
	root->r = NULL;
	skip();
	//函数体内部定义
	if (w == INT || w == DOUBLE || w == CHAR || w == LONG || w == SHORT || w == FLOAT) root->l = LocalVarDefList();
	else root->l = NULL;
	root->r = StateList();
	//是右大括号，调用完毕
	if (w == RBRACE) return root;
	else {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：复合语句部分出错";
		exit(0);
	}
}

/// <summary>
/// 生成局部变量定义结点
/// </summary>
/// <returns>局部变量定义结点</returns>
ASTTree* LocalVarDefList() {
	//创建局部变量定义序列根节点
	ASTTree* root = new ASTTree;
	root->type = LOCALVARDEFLIST;
	root->data.text = NULL;
	root->l = NULL;
	root->r = NULL;
	//局部变量定义结点，是根节点的左子树
	ASTTree* p = new ASTTree;
	p->data.text = NULL;
	p->type = LOCALVARDEF;
	p->l = NULL;
	p->r = NULL;
	root->l = p;
	//局部变量类型结点，是局部变量定义结点的左子树
	p->l = new ASTTree;
	p->l->l = NULL;
	p->l->r = NULL;
	p->l->type = LOCALVARTYPE;
	type = w;
	char* token_text1 = new char[25];
	strcpy(token_text1, token_text);
	p->l->data.text = token_text1;

	skip();
	//局部变量名称序列结点，是局部变量定义结点的右子树
	ASTTree* q = new ASTTree;
	q->data.text = NULL;
	q->l = NULL;
	q->r = NULL;
	p->r = q;
	q->type = LOCALVARNAMELIST;
	//局部变量名称结点
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
		//如果是分号，定义完成
		if (w == SEMI) {
			q->r = NULL;
			skip();
			break;
		}
		//如果是逗号，类似外部变量定义，类型一样
		else if (w == COMMA) {
			skip();
			if (w != IDENT) {
				cout << "第" << ncount << "行出现错误" << endl;
				cout << "错误原因：错误的局部变量定义";
				exit(0);
			}
			//局部变量名称序列结点，是局部变量名称序列结点右子树
			ASTTree* s = new ASTTree;
			s->l = NULL;
			s->r = NULL;
			s->data.text = NULL;
			q->r = s;
			q = q->r;
			q->type = LOCALVARNAMELIST;
			//局部变量名称结点，是前一个序列结点的左子树
			q->l = new ASTTree;
			q->l->l = NULL;
			q->l->r = NULL;
			q->l->type = LOCALVARNAME;
			char* token_text1 = new char[25];
			strcpy(token_text1, token_text);
			q->l->data.text = token_text1;
			//加入变量链表
			addName(token_text);
		}
		else if (w == ASSIGN) {
			skip();
			w = trans(token_text);
			if (w == VOID) {
				cout << "第" << ncount << "行出现错误" << endl;
				cout << "错误原因：错误的函数调用";
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
				cout << "第" << ncount << "行出现错误" << endl;
				cout << "错误原因：定义变量与类型不匹配";
				exit(0);
			}
			continue;
		}
		else {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：错误的局部变量定义";
			exit(0);
		}
	}
	//继续调用局部变量定义序列函数，继续定义
	if (w == INT || w == DOUBLE || w == CHAR || w == LONG || w == SHORT || w == FLOAT) root->r = LocalVarDefList();
	else root->r = NULL;
	return root;
}

/// <summary>
/// 生成语句序列定义结点
/// </summary>
/// <returns>语句序列定义结点</returns>
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
		//创建语句序列根结点
		root = new ASTTree;
		root->data.text = NULL;
		root->type = STATELIST;
		root->l = r1;
		root->r = NULL;
		skip();
		//不是右大括号，函数体未结束继续调用
		if (w != RBRACE) {
			//语句序列结点调用，是根节点的右子树
			root->r = StateList();
			return root;
		}
		else return root;
	}
}

/// <summary>
/// 生成各类语句结点
/// </summary>
/// <returns>各类语句结点</returns>
ASTTree* Statement() {
	//新建语句结点，类型定义在后面
	ASTTree* root = new ASTTree;
	root->l = NULL;
	root->r = NULL;
	root->data.text = NULL;
	//if语句处理
	switch (w) {
	case IF: {
		skip();
		//if后面不是(，错误
		if (w != LP) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：错误的if语句表达";
			exit(0);
		}
		skip();
		//if语句条件结点，是语句结点的左子树
		ASTTree* p1 = new ASTTree;
		p1->r = NULL;
		p1->data.text = NULL;
		p1->type = IFPART;
		p1->l = Expression(RP);
		if (p1->l == NULL) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：if语句条件部分出错";
			exit(0);
		}
		skip();
		//为左大括号，进入if语句后续内容
		if (w == LBRACE) {
			skip();
			//语句序列，是if语句部分的右子树
			p1->r = StateList();
		}
		//不是左大括号，后面只有一句
		else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT || w == INT || w == FLOAT || w == CHAR || w == RETURN || w == BREAK || w == CONTINUE || w == SHORT || w == LONG || w == DOUBLE || w == IF || w == WHILE || w == ELSE || w == FOR || w == DO) {
			//继续调用语句部分，是if语句部分的右子树
			p1->r = Statement();
		}
		else {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：if语句内出错";
			exit(0);
		}
		root->l = p1;
		skip();
		//为else，生成ifelse判断结构结点
		if (w == ELSE) {
			root->type = IFELSESTATEMENT;
			//else语句部分，是语句根节点的右子树
			ASTTree* p2 = new ASTTree;
			p2->l = NULL;
			p2->r = NULL;
			p2->data.text = NULL;
			p2->type = ELSEPART;
			root->r = p2;
			skip();
			//为左大括号，进入else语句内
			if (w == LBRACE) {
				//继续调用语句序列，是else结点的右子树
				skip();
				p2->r = StateList();
			}
			else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT || w == INT || w == FLOAT || w == CHAR || w == RETURN || w == BREAK || w == CONTINUE || w == SHORT || w == LONG || w == DOUBLE || w == IF || w == WHILE || w == ELSE || w == FOR || w == DO) {
				//忽略大括号，直接进入语句部分结点，是else结点的右子树
				p2->r = Statement();
			}
			else if (w == IF) {
				//为else if结构，是else结点左子树
				p2->l = Statement();
			}
			else {
				cout << "第" << ncount << "行出现错误" << endl;
				cout << "错误原因：else语句内出错";
				exit(0);
			}
		}
		//不是else，退回w到文件中
		else {
			root->type = IFSTATEMENT;
			returnToken(w, fp);
		}
		return root;
	}
		   //while语句处理
	case WHILE: {
		isInrecycle = 1;
		skip();
		//不是左小括号
		if (w != LP) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：while语句内出错";
			exit(0);
		}
		skip();
		//创建while表达式部分结点，是语句结点的左子树
		ASTTree* p1 = new ASTTree;
		p1->data.text = NULL;
		p1->type = WHILEPART;
		p1->r = NULL;
		p1->l = Expression(RP);
		if (p1->l == NULL) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：while语句条件部分出错";
			exit(0);
		}
		//while语句主体，是语句结点的右子树
		ASTTree* p2 = new ASTTree;
		p2->data.text = NULL;
		p2->type = WHILEBODY;
		p2->l = NULL;
		p2->r = NULL;
		skip();
		//是左大括号
		if (w == LBRACE) {
			skip();
			//跳转到语句序列结点，是while语句体结点的右子树
			p2->r = StateList();
		}
		//不是左大括号，同理if语句
		else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT || w == INT || w == FLOAT || w == CHAR || w == RETURN || w == BREAK || w == CONTINUE || w == VOID || w == SHORT || w == LONG || w == DOUBLE || w == IF || w == WHILE || w == ELSE || w == FOR || w == DO) {
			//跳转到语句结点，是while语句体结点的右子树
			p2->r = Statement();
		}
		else {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：while语句内出错";
			exit(0);
		}
		root->type = WHILESTATEMENT;
		root->l = p1;
		root->r = p2;
		isInrecycle = 0;
		return root;
	}
			  //for语句处理
	case FOR: {
		isInrecycle = 1;
		skip();
		if (w != LP) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：for语句内出错";
			exit(0);
		}
		skip();
		//for语句部分结点，是语句结点左子树
		ASTTree* p1 = new ASTTree;
		p1->l = NULL;
		p1->r = NULL;
		p1->data.text = NULL;
		p1->type = FORPART;
		//for语句第一部分结点，是for语句结点左子树，同时做表达式处理
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
		//for语句第二部分结点，是for语句第一部分结点右子树，同时做表达式处理
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
		//for语句第三部分结点，是for语句第二部分结点右子树，同时做表达式处理
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
		//for语句体结点创建，是for语句部分结点右子树
		ASTTree* p2 = new ASTTree;
		p2->l = NULL;
		p2->r = NULL;
		p2->type = FORBODY;
		p2->data.text = NULL;
		skip();
		//为左大括号，跳转到语句序列，还是同理
		if (w == LBRACE) {
			skip();
			p2->r = StateList();
		}
		else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT || w == INT || w == FLOAT || w == CHAR || w == RETURN || w == BREAK || w == CONTINUE || w == VOID || w == SHORT || w == LONG || w == DOUBLE || w == IF || w == WHILE || w == ELSE || w == FOR || w == DO) {
			//为关键字类，跳转到语句部分
			p2->r = Statement();
		}
		else {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：for语句内出错";
			exit(0);
		}
		//标记for语句结点
		root->type = FORSTATEMENT;
		root->l = p1;
		root->r = p2;
		isInrecycle = 0;
		return root;
	}
			//return语句处理
	case RETURN: {
		//语句结点类型标记为return
		root->type = RETURNSTATEMENT;
		root->l = NULL;
		root->data.text = NULL;
		skip();
		//做表达式处理
		root->r = Expression(SEMI);
		if (root->r == NULL && isVoid == 0) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：缺少返回值";
			exit(0);
		}
		else if (root->r != NULL && isVoid == 1) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：此处不应有返回值";
			exit(0);
		}
		else hasReturn = 1;
		return root;
	}
			   //do-while语句
	case DO: {
		isInrecycle = 1;
		skip();
		//缺少左大括号
		if (w != LBRACE) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：do-while语句可能缺少必要的括号";
			exit(0);
		}
		//创建do-while语句体结点，是语句结点的左子树
		ASTTree* p1 = new ASTTree;
		p1->type = DOWHILEBODY;
		p1->l = NULL;
		p1->r = NULL;
		//do-while条件语句结点，是语句结点的右子树
		ASTTree* p2 = new ASTTree;
		p2->type = DOWHILECONDITION;
		p2->r = NULL;
		root->l = p1;
		root->r = p2;
		p1->data.text = p2->data.text = NULL;
		//调用语句序列，是do-while语句体结点的左子树
		skip();
		p1->l = StateList();
		skip();
		if (w != WHILE) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：do-while语句可能缺失while关键字";
			exit(0);
		}
		//do-while语句部分类型赋值
		root->type = DOWHILESTATEMENT;
		skip();
		//没有左小括号
		if (w != LP) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：do-while语句可能缺少必要的条件";
			exit(0);
		}
		//有左小括号，做表达式处理
		skip();
		p2->l = Expression(RP);
		if (p2->l == NULL) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：do-while语句可能缺少必要的条件";
			exit(0);
		}
		skip();
		if (w != SEMI) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：do-while语句可能缺少必要的分号";
			exit(0);
		}
		isInrecycle = 0;
		return root;
	}
		   //break语句
	case BREAK: {
		skip();
		if (w != SEMI) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：break语句缺少分号";
			exit(0);
		}
		//未出现循环语句等
		if (isInrecycle == 0) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：不需要的break语句";
			exit(0);
		}
		//标记语句结点类型
		root->type = BREAKSTATEMENT;
		return root;
	}
			  //continue语句
	case CONTINUE: {
		skip();
		if (w != SEMI) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：continue语句缺少分号";
			exit(0);
		}
		//同理break
		/*if (isInrecycle == 0) {
			cout << "第" << ncount << "行出现错误" << endl;
			cout << "错误原因：不需要的continue语句";
			exit(0);
		}*/
		//continue语句部分
		root->type = CONTINUESTATEMENT;
		return root;
	}
				 //表达式处理
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
/// 生成表达式运算结点
/// </summary>
/// <param name="endsym">表达式结束符号</param>
/// <returns>表达式运算结点</returns>
ASTTree* Expression(int endsym) {
	//到结束语句
	if (w == endsym) return NULL;
	int flag = 0;
	int error = 0;
	//运算符栈
	stack<ASTTree*> op;
	ASTTree* p = new ASTTree;
	p->l = NULL;
	p->r = NULL;
	p->data.text = NULL;
	p->type = OPERATOR;//运算符结点
	p->data.type = POUND;//#，作为终止符
	//将p加入栈，作为终止符号
	op.push(p);
	stack<ASTTree*> opn;//操作数栈
	ASTTree* t = NULL;
	ASTTree* t1 = NULL;
	ASTTree* t2 = NULL;
	ASTTree* root = NULL;
	while (((w != endsym) || (op.top()->data.type != POUND)) && !error) {
		//没到结束符号或者栈没到终止符，同时无报错
		//栈顶元素为右小括号，栈顶没到终止符
		if (op.top()->data.type == RP) {
			//栈的大小小于3，右小括号无法找到匹配的左小括号和操作数
			if (op.size() < 3) {
				error++;
				break;
			}
			//弹出栈中所有元素
			op.pop();
			op.pop();
		}
		//判断是否有未声明的变量
		if (w == IDENT) {
			int m = checkname_exist(token_text);
			w = trans(token_text);
		}
		//将正确的操作数加入到栈中
		if (w == IDENT || w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == ARRAY || w == STRING_CONST) {
			//操作数结点
			p = new ASTTree;
			p->type = OPERAND;
			p->l = NULL;
			p->r = NULL;
			char* token_text1 = new char[25];
			strcpy(token_text1, token_text);
			//char* x = p->data.text;
			p->data.text = token_text1;
			//x = p->data.text;
			//加入操作数栈
			opn.push(p);
			//继续取下一个词法符号
			skip();
		}
		else if (w == endsym) {
			//到结束符号，然后开始从操作堆栈中弹出运算符
			while (op.top()->data.type != POUND) {
				//t2取操作数栈顶元素
				t2 = opn.top();
				//运算符不是自增自减符号
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
				cout << "第" << ncount << "行出现错误" << endl;
				cout << "错误原因：未知的运算符";
				exit(0);
			}
		}
		else error = 1;
	}
	//接到表达式节点上
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
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：错误的表达式";
		exit(0);
	}
}

/// <summary>
/// 检查变量是否存在
/// </summary>
/// <param name="token_text"></param>
/// <returns></returns>
int checkname_exist(char* token_text) {
	int i;
	int flag = 0;
	VDN* p = Vroot;
	while (p->next != NULL) p = p->next;
	//查局部变量存在
	for (i = 0; i < (p->size); i++) {
		if (strcmp(token_text, p->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	//查全局变量存在
	for (i = 0; i < (Vroot->size); i++) {
		if (strcmp(token_text, Vroot->datas.variable[i]) == 0) {
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		cout << "第" << ncount << "行出现错误" << endl;
		cout << "错误原因：使用了未声明的变量";
		exit(0);
	}
	return flag;
}

/// <summary>
/// 运算符优先级判断
/// </summary>
/// <param name="c1">待比较的运算符</param>
/// <param name="c2">待比较的运算符</param>
/// <returns>两者优先级关系</returns>
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
/// 将已经读取的词法单元w退回到文件中
/// </summary>
/// <param name="w">已经读取的词法单元</param>
/// <param name="fp">文件指针</param>
void returnToken(int w, FILE* fp) {
	int n = strlen(token_text);
	int i;
	for (i = 0; i < n; i++) {
		ungetc(token_text[n - 1 - i], fp);
	}
}