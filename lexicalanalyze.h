#pragma once 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

/// <summary>
/// 词法单元枚举类型
/// </summary>
typedef enum token_kind {
	ERROR_TOKEN, // 错误的单词编码
	IDENT,     // 标识符
	INT_CONST,   // 整数常量
	FLOAT_CONST, //浮点数常量
	CHAR_CONST,  //字符常量
	STRING_CONST,//字符串常量
	INT,     // 关键字 int
	FLOAT,     // 关键字 float
	CHAR,    // 关键字 char
	IF,      // 关键字 if
	ELSE,    // 关键字 else
	WHILE,     // 关键字 while
	FOR,     // 关键字 for
	DO,      // 关键字 do
	RETURN,    // 关键字 return
	BREAK,     // 关键字 break
	CONTINUE,  // 关键字 continue
	SWITCH,    // 关键字 switch
	CASE,    // 关键字 case
	DEFAULT,   // 关键字 default
	VOID,    // 关键字 void
	SIZEOF,    // 关键字 sizeof
	STRUCT,    // 关键字 struct
	TYPEDEF,   // 关键字 typedef
	ENUM,    // 关键字 enum
	CONST,     // 关键字 const
	SHORT,     // 关键字 short
	LONG,    // 关键字 long
	DOUBLE,    // 关键字 double
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
	ANNO,//注释
	INCLUDE,//头文件
	ARRAY,//数组
	ERROR,//错误
	MACRO,//宏
}TokenType;

/// <summary>
/// 词法单元
/// </summary>
typedef struct {
	TokenType type;   // 词法单元类型
	char* value;  // 词法单元值
} Token;

void addtoken(char* token, char c);

void isKey(Token* token, FILE* fp);

void isIdent(Token* token, FILE* fp);

void defVarType(Token* token, FILE* fp);

void isSymbol(Token* token, FILE* fp);

Token* gettoken(FILE* fp);

void lexAnalyzeOutput();
