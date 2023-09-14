#pragma once 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

//结点类型枚举
enum DataType {
	EXTDEFLIST = 1,      //外部定义序列结点
	EXTVARDEF,
	EXTVARTYPE,
	EXTVARLIST,          //外部变量名序列结点
	EXTVAR,              //外部变量名结点
	FUNCDEF,             //函数定义结点
	FUNCRETURNTYPE,      //函数返回值类型结点
	FUNCNAME,            //函数名结点
	FUNCFORMALPARALIST,  //函数形式参数序列结点
	FUNCFORMALPARADEF,   //函数形式参数结点10
	FUNCFORMALPARATYPE,  //函数形参类型结点
	FUNCFORMALPARA,      //函数形参名结点
	FUNCBODY,            //函数体结点
	LOCALVARDEFLIST,     //局部变量定义序列结点
	LOCALVARDEF,         //局部变量定义结点15
	LOCALVARTYPE,        //局部变量类型结点
	LOCALVARNAMELIST,    //局部变量名序列
	LOCALVARNAME,        //局部变量名
	STATELIST,           //语句序列结点
	OPERAND,             //操作数结点20
	OPERATOR,            //运算符结点
	EXPRESSION,          //表达式
	IFPART,              // if语句部分
	ELSEPART,            // else部分
	IFSTATEMENT,         // if语句25
	IFELSESTATEMENT,     // if-else语句
	WHILESTATEMENT,      // while语句结点
	WHILEPART,           // while条件语句结点
	WHILEBODY,           // while语句体
	FORSTATEMENT,        // for语句结点30
	FORPART,             // for条件语句
	FORPART1,            // for语句条件一
	FORPART2,            // for语句条件二
	FORPART3,            // for语句条件三
	FORBODY,             // for语句体35
	RETURNSTATEMENT,     // return语句
	BREAKSTATEMENT,      // break语句
	DOWHILESTATEMENT,    // do-while循环语句
	DOWHILEBODY,         // do-while语句体
	DOWHILECONDITION,    // do-while条件40
	CONTINUESTATEMENT,   // continue语句
	FUNCCLAIM,           //函数声明
	ARRAYDEF,            //数组定义
	ARRAYTYPE,           //数组类型
	ARRAYNAME,           //数组名45
	ARRAYSIZE,           //数组大小
	ARRAYLIST,           //数组定义序列
	ASSIGN_CONST,        //赋值
};

//抽象语法树定义
typedef struct ASTTree {
	struct ASTTree* l;
	struct ASTTree* r;
	int type;//该节点类型
	struct data {
		int type;
		char* text;
	}data;//用于存储关键语法分析信息关键字等
}ASTTree;

//变量定义结点
typedef struct VariableDefNode {
	int size;//存储变量个数
	struct datas {
		char* variable[25];//存储定义的变量名称
		int type[25];//存储返回类型
		bool isFunc[25] = { 0 };//判断是否为函数
	}datas;
	struct VariableDefNode* next;//指针指向下一个定义变量结点
} VDN;

typedef struct FuncDefNode {
	int name[25];
	int size = 0;
	char* fName;
	struct FuncDefNode* next;
}FDN;

void syntaxAnalyze();

void freeTree(ASTTree* root);

ASTTree* program();

ASTTree* ExtDefList();

ASTTree* ExtDef();

ASTTree* ExtVarDef();

ASTTree* FuncDef();

ASTTree* FormParaList();

ASTTree* FormParaDef();

ASTTree* CompState();

ASTTree* LocalVarDefList();

ASTTree* StateList();

ASTTree* Statement();

ASTTree* Expression(int endsym);

char Precede(int c1, int c2);

void returnToken(int w, FILE* fp);

void showType(int type);

void PreTraverse(ASTTree* root, int depth);

void addName(char* token_text);

void addNameF(char* token_text);

void addNameFunc(char* token_text);

int checkname_exist(char* token_text);

ASTTree* ArrayDef();

void Traverse();

int trans(char* token_text);

void transF(char* token_text2);

void arraySolve(char* token_text);