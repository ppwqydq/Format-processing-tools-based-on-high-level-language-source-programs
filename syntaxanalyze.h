#pragma once 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

//�������ö��
enum DataType {
	EXTDEFLIST = 1,      //�ⲿ�������н��
	EXTVARDEF,
	EXTVARTYPE,
	EXTVARLIST,          //�ⲿ���������н��
	EXTVAR,              //�ⲿ���������
	FUNCDEF,             //����������
	FUNCRETURNTYPE,      //��������ֵ���ͽ��
	FUNCNAME,            //���������
	FUNCFORMALPARALIST,  //������ʽ�������н��
	FUNCFORMALPARADEF,   //������ʽ�������10
	FUNCFORMALPARATYPE,  //�����β����ͽ��
	FUNCFORMALPARA,      //�����β������
	FUNCBODY,            //��������
	LOCALVARDEFLIST,     //�ֲ������������н��
	LOCALVARDEF,         //�ֲ�����������15
	LOCALVARTYPE,        //�ֲ��������ͽ��
	LOCALVARNAMELIST,    //�ֲ�����������
	LOCALVARNAME,        //�ֲ�������
	STATELIST,           //������н��
	OPERAND,             //���������20
	OPERATOR,            //��������
	EXPRESSION,          //���ʽ
	IFPART,              // if��䲿��
	ELSEPART,            // else����
	IFSTATEMENT,         // if���25
	IFELSESTATEMENT,     // if-else���
	WHILESTATEMENT,      // while�����
	WHILEPART,           // while���������
	WHILEBODY,           // while�����
	FORSTATEMENT,        // for�����30
	FORPART,             // for�������
	FORPART1,            // for�������һ
	FORPART2,            // for���������
	FORPART3,            // for���������
	FORBODY,             // for�����35
	RETURNSTATEMENT,     // return���
	BREAKSTATEMENT,      // break���
	DOWHILESTATEMENT,    // do-whileѭ�����
	DOWHILEBODY,         // do-while�����
	DOWHILECONDITION,    // do-while����40
	CONTINUESTATEMENT,   // continue���
	FUNCCLAIM,           //��������
	ARRAYDEF,            //���鶨��
	ARRAYTYPE,           //��������
	ARRAYNAME,           //������45
	ARRAYSIZE,           //�����С
	ARRAYLIST,           //���鶨������
	ASSIGN_CONST,        //��ֵ
};

//�����﷨������
typedef struct ASTTree {
	struct ASTTree* l;
	struct ASTTree* r;
	int type;//�ýڵ�����
	struct data {
		int type;
		char* text;
	}data;//���ڴ洢�ؼ��﷨������Ϣ�ؼ��ֵ�
}ASTTree;

//����������
typedef struct VariableDefNode {
	int size;//�洢��������
	struct datas {
		char* variable[25];//�洢����ı�������
		int type[25];//�洢��������
		bool isFunc[25] = { 0 };//�ж��Ƿ�Ϊ����
	}datas;
	struct VariableDefNode* next;//ָ��ָ����һ������������
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