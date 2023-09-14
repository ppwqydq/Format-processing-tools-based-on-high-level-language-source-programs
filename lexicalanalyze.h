#pragma once 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

/// <summary>
/// �ʷ���Ԫö������
/// </summary>
typedef enum token_kind {
	ERROR_TOKEN, // ����ĵ��ʱ���
	IDENT,     // ��ʶ��
	INT_CONST,   // ��������
	FLOAT_CONST, //����������
	CHAR_CONST,  //�ַ�����
	STRING_CONST,//�ַ�������
	INT,     // �ؼ��� int
	FLOAT,     // �ؼ��� float
	CHAR,    // �ؼ��� char
	IF,      // �ؼ��� if
	ELSE,    // �ؼ��� else
	WHILE,     // �ؼ��� while
	FOR,     // �ؼ��� for
	DO,      // �ؼ��� do
	RETURN,    // �ؼ��� return
	BREAK,     // �ؼ��� break
	CONTINUE,  // �ؼ��� continue
	SWITCH,    // �ؼ��� switch
	CASE,    // �ؼ��� case
	DEFAULT,   // �ؼ��� default
	VOID,    // �ؼ��� void
	SIZEOF,    // �ؼ��� sizeof
	STRUCT,    // �ؼ��� struct
	TYPEDEF,   // �ؼ��� typedef
	ENUM,    // �ؼ��� enum
	CONST,     // �ؼ��� const
	SHORT,     // �ؼ��� short
	LONG,    // �ؼ��� long
	DOUBLE,    // �ؼ��� double
	EQ,   // ==
	NEQ,  // !=
	LT,   // <
	LTE,  // <=
	GT,   // >
	GTE,  // >=
	ASSIGN, // =
	ADD,  // +
	SUB,  // -
	MUL,  // *
	DIV,  // /
	MOD,  // %
	INC,  // ++
	DEC,  // --
	AND,  // &&
	OR,   // ||
	NOT,  // !
	BIT_AND,  // &
	BIT_OR,   // |
	BIT_XOR,  // ^
	BIT_NOT,  // ~
	LEFT_SHIFT, // <<
	RIGHT_SHIFT,// >>
	ADD_ASSIGN, // +=
	SUB_ASSIGN, // -=
	MUL_ASSIGN, // *=
	DIV_ASSIGN, // /=
	MOD_ASSIGN, // %=
	AND_ASSIGN, // &=
	OR_ASSIGN,  // |=
	XOR_ASSIGN, // ^=
	LP,  // (
	RP,  // )
	LBRACE, // {
	RBRACE, // }
	LBRACKET, // [
	RBRACKET, // ]
	SEMI,  // ;
	COMMA, // ,
	DOT,   // .
	ARROW, // ->
	COLON, // :
	QUESTION, // ?
	POUND,//#
	ANNO,//ע��
	INCLUDE,//ͷ�ļ�
	ARRAY,//����
	ERROR,//����
	MACRO,//��
}TokenType;

/// <summary>
/// �ʷ���Ԫ
/// </summary>
typedef struct {
	TokenType type;   // �ʷ���Ԫ����
	char* value;  // �ʷ���Ԫֵ
} Token;

void addtoken(char* token, char c);

void isKey(Token* token, FILE* fp);

void isIdent(Token* token, FILE* fp);

void defVarType(Token* token, FILE* fp);

void isSymbol(Token* token, FILE* fp);

Token* gettoken(FILE* fp);

void lexAnalyzeOutput();
