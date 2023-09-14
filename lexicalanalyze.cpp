#define _CRT_SECURE_NO_WARNINGS
#include "lexicalanalyze.h"

char c;
char token_text[100];//用于存储词法单元的全局变量
char string_num[100];
char FileName[50];//文件名
FILE* fp;
int ncount;//用于记录代码所在行数

/// <summary>
/// 将一个字符加入到词法单元中
/// </summary>
/// <param name="token">词法单元</param>
/// <param name="c">字符</param>
void addtoken(char* token, char c) {
	int i = 0;
	while (*(token + i) != '\0') {
		i++;
	}
	*(token + i) = c;
	*(token + i + 1) = '\0';
}

/// <summary>
/// 判断标识符是否为关键字/数组
/// </summary>
/// <param name="token">待识别的词法单元</param>
/// <param name="fp">文件指针</param>
void isKey(Token* token, FILE* fp) {
	//变量类型定义
	if (strcmp(token_text, "int") == 0) {
		token->type = INT;
	}
	else if (strcmp(token_text, "float") == 0) {
		token->type = FLOAT;
	}
	else if (strcmp(token_text, "char") == 0) {
		token->type = CHAR;
	}
	else if (strcmp(token_text, "short") == 0) {
		token->type = SHORT;
	}
	else if (strcmp(token_text, "long") == 0) {
		token->type = LONG;
	}
	else if (strcmp(token_text, "double") == 0) {
		token->type = DOUBLE;
	}
	//关键字
	else if (strcmp(token_text, "if") == 0) {
		token->type = IF;
	}
	else if (strcmp(token_text, "else") == 0) {
		token->type = ELSE;
	}
	else if (strcmp(token_text, "while") == 0) {
		token->type = WHILE;
	}
	else if (strcmp(token_text, "for") == 0) {
		token->type = FOR;
	}
	else if (strcmp(token_text, "do") == 0) {
		token->type = DO;
	}
	else if (strcmp(token_text, "return") == 0) {
		token->type = RETURN;
	}
	else if (strcmp(token_text, "break") == 0) {
		token->type = BREAK;
	}
	else if (strcmp(token_text, "continue") == 0) {
		token->type = CONTINUE;
	}
	else if (strcmp(token_text, "switch") == 0) {
		token->type = SWITCH;
	}
	else if (strcmp(token_text, "case") == 0) {
		token->type = CASE;
	}
	else if (strcmp(token_text, "default") == 0) {
		token->type = DEFAULT;
	}
	else if (strcmp(token_text, "void") == 0) {
		token->type = VOID;
	}
	else if (strcmp(token_text, "const") == 0) {
		token->type = CONST;
	}
	//数组
	else {
		c = fgetc(fp);
		if (c == '[') {
			//识别数组
			addtoken(token_text, c);
			c = fgetc(fp);
			while (isdigit(c)) {
				addtoken(token_text, c);
				addtoken(string_num, c);
				c = fgetc(fp);
			}
			if (c != ']') {
				token->type = ERROR_TOKEN;
			}
			addtoken(token_text, c);
			token->type = ARRAY;
		}
		else {
			ungetc(c, fp);
			token->type = IDENT;
		}
	}
}

/// <summary>
/// 标识符识别
/// </summary>
/// <param name="token">待识别的词法单元</param>
/// <param name="fp">文件指针</param>
void isIdent(Token* token, FILE* fp) {
	//如果c是字母或者_
	int i = 0;
	do {
		token_text[i++] = c;
	} while (isalpha(c = fgetc(fp)) || isdigit(c));//拼标识符串
	ungetc(c, fp);//退回多读的字符到文件输入缓冲区
	token_text[i] = '\0';
	//识别关键字
	isKey(token, fp);
	token->value = new char[strlen(token_text) + 1];
	strcpy(token->value, token_text);
}

/// <summary>
/// 识别变量类型
/// </summary>
/// <param name="token">待识别的词法单元</param>
/// <param name="fp">文件指针</param>
void defVarType(Token* token, FILE* fp) {
	//如果0开头，考虑八进制十六进制
	if (c == '0') {
		addtoken(token_text, c);
		c = fgetc(fp);
		//十六进制
		if (c == 'x') {
			addtoken(token_text, c);
			c = fgetc(fp);
			while (isxdigit(c)) {
				addtoken(token_text, c);
				c = fgetc(fp);
			}
			ungetc(c, fp);   // 退回多读的字符
			token->type = INT_CONST;
		}
		else if (c == '.') {
			c = fgetc(fp);
			//.后面不为数字，错误
			if (!isdigit(c)) token->type = ERROR_TOKEN;
			//正确浮点型
			else {
				ungetc(c, fp);
				addtoken(token_text, '.');
				c = fgetc(fp);
				do {
					addtoken(token_text, c);
				} while (isdigit(c = fgetc(fp)));
				//考虑含有f的浮点型
				if (c == 'f') {
					addtoken(token_text, c);
					token->type = FLOAT_CONST;
				}
				else {
					ungetc(c, fp);
					token->type = FLOAT_CONST;
				}
			}
		}
		//八进制
		else {
			while (c >= '0' && c <= '7') {
				addtoken(token_text, c);
				c = fgetc(fp);
			}
			ungetc(c, fp);   // 退回多读的字符
			token->type = INT_CONST;
		}
	}
	//不是0开头
	else {
		//拼数字串
		do {
			addtoken(token_text, c);
		} while (isdigit(c = fgetc(fp)));
		//不是浮点数情况
		if (c != '.' && c != 'u' && c != 'l') {
			//判断错误
			/*if (c != ' ' && c != ';' && c != ')' && c != '+' && c != '-' && c != '*' && c != '/') {
				token->type = ERROR_TOKEN;
			}*/
			//反之为整型变量
			//else {
			ungetc(c, fp);
			token->type = INT_CONST;
			//}
		}
		//浮点型
		else if (c == '.') {
			c = fgetc(fp);
			//.后面不为数字，错误
			if (!isdigit(c)) token->type = ERROR_TOKEN;
			//正确浮点型
			else {
				ungetc(c, fp);
				addtoken(token_text, '.');
				c = fgetc(fp);
				do {
					addtoken(token_text, c);
				} while (isdigit(c = fgetc(fp)));
				//考虑含有f的浮点型
				if (c == 'f') {
					addtoken(token_text, c);
					token->type = FLOAT_CONST;
				}
				else {
					ungetc(c, fp);
					token->type = FLOAT_CONST;
				}
			}
		}
		//无符号类型
		else if (c == 'u') {
			addtoken(token_text, c);
			c = fgetc(fp);
			if (c == 'l') {
				addtoken(token_text, c);
				c = fgetc(fp);
				//unsigned long long
				if (c == 'l') {
					addtoken(token_text, c);
					token->type = INT_CONST;
				}
				//错误
				/*else if (c != ' ' && c != ';' && c != ')' && c != '+' && c != '-' && c != '*' && c != '/') {
					token->type = ERROR_TOKEN;
				}*/
				//unsigned long
				else {
					ungetc(c, fp);
					token->type = INT_CONST;
				}
			}
			//unsigned
			else {
				ungetc(c, fp);
				token->type = INT_CONST;
			}
		}
		//长整型
		else if (c == 'l') {
			addtoken(token_text, c);
			c = fgetc(fp);
			//long long
			if (c == 'l') {
				addtoken(token_text, c);
				token->type = INT_CONST;
			}
			//long
			else {
				ungetc(c, fp);
				token->type = INT_CONST;
			}
		}
		//普通整型
		else {
			token->type = INT_CONST;
		}

	}
	token->value = new char[strlen(token_text) + 1];
	strcpy(token->value, token_text);
}

/// <summary>
/// 符号开头的识别
/// </summary>
/// <param name="token">待识别的词法单元</param>
/// <param name="fp">文件指针</param>
void isSymbol(Token* token, FILE* fp) {
	switch (c) {
	case '=':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '=') {
			addtoken(token_text, c);
			token->type = EQ;
		}
		else {
			ungetc(c, fp);
			token->type = ASSIGN;
		}
		break;
	case '!':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '=') {
			addtoken(token_text, c);
			token->type = NEQ;
		}
		else {
			ungetc(c, fp);
			token->type = NOT;
		}
		break;
	case '<':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '<') {
			addtoken(token_text, c);
			token->type = LEFT_SHIFT;
		}
		else if (c == '=') {
			addtoken(token_text, c);
			token->type = LTE;
		}
		else {
			ungetc(c, fp);
			token->type = LT;
		}
		break;
	case '>':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '>') {
			addtoken(token_text, c);
			token->type = RIGHT_SHIFT;
		}
		else if (c == '=') {
			addtoken(token_text, c);
			token->type = GTE;
		}
		else {
			ungetc(c, fp);
			token->type = GT;
		}
		break;
	case '+':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '+') {
			addtoken(token_text, c);
			token->type = INC;
		}
		else if (c == '=') {
			addtoken(token_text, c);
			token->type = ADD_ASSIGN;
		}
		else {
			ungetc(c, fp);
			token->type = ADD;
		}
		break;
	case '-':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '-') {
			addtoken(token_text, c);
			token->type = DEC;
		}
		else if (c == '=') {
			addtoken(token_text, c);
			token->type = SUB_ASSIGN;
		}
		else if (c == '>') {
			addtoken(token_text, c);
			token->type = ARROW;
		}
		else {
			ungetc(c, fp);
			token->type = SUB;
		}
		break;
	case '*':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '=') {
			addtoken(token_text, c);
			token->type = MUL_ASSIGN;
		}
		else {
			ungetc(c, fp);
			token->type = MUL;
		}
		break;
	case '%':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '=') {
			addtoken(token_text, c);
			token->type = MOD_ASSIGN;
		}
		else {
			ungetc(c, fp);
			token->type = MOD;
		}
		break;
	case '&':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '&') {
			addtoken(token_text, c);
			token->type = AND;
		}
		else if (c == '=') {
			addtoken(token_text, c);
			token->type = AND_ASSIGN;
		}
		else {
			ungetc(c, fp);
			token->type = BIT_AND;
		}
		break;
	case '|':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '|') {
			addtoken(token_text, c);
			token->type = OR;
		}
		else if (c == '=') {
			addtoken(token_text, c);
			token->type = OR_ASSIGN;
		}
		else {
			ungetc(c, fp);
			token->type = BIT_OR;
		}
		break;
	case '^':
		addtoken(token_text, c);
		c = fgetc(fp);
		if (c == '=') {
			addtoken(token_text, c);
			token->type = XOR_ASSIGN;
		}
		else {
			ungetc(c, fp);
			token->type = BIT_XOR;
		}
		break;
	case '.':
		//.开头的浮点型
		do {
			addtoken(token_text, c);
		} while (isdigit(c = fgetc(fp)));
		ungetc(c, fp);
		if (strlen(token_text) > 1) token->type = FLOAT_CONST;
		else token->type = DOT;
		break;
	case '~':
		addtoken(token_text, c);
		token->type = BIT_NOT;
		break;
	case '(':
		addtoken(token_text, c);
		token->type = LP;
		break;
	case ')':
		addtoken(token_text, c);
		token->type = RP;
		break;
	case '{':
		addtoken(token_text, c);
		token->type = LBRACE;
		break;
	case '}':
		addtoken(token_text, c);
		token->type = RBRACE;
		break;
	case '[':
		addtoken(token_text, c);
		token->type = LBRACKET;
		break;
	case ']':
		addtoken(token_text, c);
		token->type = RBRACKET;
		break;
	case ';':
		addtoken(token_text, c);
		token->type = SEMI;
		break;
	case ',':
		addtoken(token_text, c);
		token->type = COMMA;
		break;
	case ':':
		addtoken(token_text, c);
		token->type = COLON;
		break;
	case '\?':
		addtoken(token_text, c);
		token->type = QUESTION;
		break;
	case '/':
		addtoken(token_text, c);
		if ((c = fgetc(fp)) == '/') {
			//读取注释
			do {
				addtoken(token_text, c);
			} while ((c = fgetc(fp)) != '\n');
			ncount++;
			ungetc(c, fp);
			token->type = ANNO;
			break;
		}
		else if (c == '*') {
			while (1) {
				addtoken(token_text, c);
				c = fgetc(fp);
				if (c == '\n') {
					ncount++;
				}
				if (c == '*') {
					addtoken(token_text, c);
					if ((c = fgetc(fp)) == '/') {
						addtoken(token_text, c);
						token->type = ANNO;
						break;
					}
					else {
						ungetc(c, fp);
					}
				}
				else if (c == EOF) {
					// 处理缺少注释结束标记的情况
					token->type = ERROR_TOKEN;
					break;
				}
			}
			break;
		}
		else {
			c = fgetc(fp);
			if (c == '=') {
				addtoken(token_text, c);
				token->type = DIV_ASSIGN;
				break;
			}
			else {
				ungetc(c, fp);
				token->type = DIV;
			}
			break;
		}
	case '#':
		addtoken(token_text, c);
		if (isalpha(c = fgetc(fp))) {
			do {
				addtoken(token_text, c);
			} while (isalpha(c = fgetc(fp)));
			if (strcmp(token_text, "#include") == 0) {
				do {
					addtoken(token_text, c);
				} while ((c = fgetc(fp)) != '\n');
				ncount++;
				ungetc(c, fp);
				token->type = INCLUDE;
			}
			else if (strcmp(token_text, "#define") == 0) {
				do {
					addtoken(token_text, c);
				} while ((c = fgetc(fp)) != '\n');
				ncount++;
				token->type = MACRO;
			}
			else {
				token->type = ERROR_TOKEN;
			}
		}
		else {
			token->type = ERROR_TOKEN;
		}
		break;
	case '\'':
		addtoken(token_text, '\'');
		if ((c = fgetc(fp)) != '\\') {
			addtoken(token_text, c);
			if ((c = fgetc(fp)) == '\'') {
				addtoken(token_text, c);
				token->type = CHAR_CONST;
			}
			else {
				token->type = ERROR_TOKEN;
			}
		}
		else {
			//  \\ 
			addtoken(token_text, '\\');
			c = fgetc(fp);
			if (c == 'n' || c == 't' || c == '\\' || c == '\'' ||
				c == '\"') {
				addtoken(token_text, c);
				if ((c = fgetc(fp)) == '\'') {
					addtoken(token_text, c);
					token->type = CHAR_CONST;
				}
				else {
					token->type = ERROR_TOKEN;
				}
			}
			/*else if (c == 'x') {
				addtoken(token_text, c);
				if ((isdigit(c)) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
					addtoken(token_text, c);
					if ((isdigit(c)) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
						addtoken(token_text, c);
					}
					else {
						ungetc(c, fp);
					}
					if ((c = fgetc(fp)) == '\'') {
						addtoken(token_text, '\'');
						token->type = CHAR_CONST;
					}
					else {
						token->type = ERROR_TOKEN;
					}
				}
				else {
					token->type = ERROR_TOKEN;
				}
			}*/
			//八进制
			/*else if (c >= '0' && c <= '7') {
				addtoken(token_text, c);
				if ((c = fgetc(fp)) >= '0' && c <= '7') {
					addtoken(token_text, c);
					if ((c = fgetc(fp)) >= '0' && c <= '7') {
						addtoken(token_text, c);
						if ((c = fgetc(fp)) == '\'') {
							addtoken(token_text, '\'');
							token->type = CHAR_CONST;
						}
						else {
							token->type = ERROR_TOKEN;
						}
					}
					else if (c == '\'') {
						addtoken(token_text, '\'');
						token->type = CHAR_CONST;
					}
					else {
						token->type = ERROR_TOKEN;
					}
				}
				else {
					if (c == '\'') {
						addtoken(token_text, c);
						token->type = CHAR_CONST;
					}
					else {
						ungetc(c, fp);
						token->type = ERROR_TOKEN;
					}
				}
			}*/
			else {
				token->type = ERROR_TOKEN;
			}
		}
		break;
	case '"':
		do {
			if (c != '\\')
				addtoken(token_text, c);
			if (c == '\\') {
				c = fgetc(fp);
				addtoken(token_text, c);
			}
		} while ((c = fgetc(fp)) != '"' && c != '\n');
		if (c == '"') {
			addtoken(token_text, '"');
			token->type = STRING_CONST;
		}
		else {
			token->type = ERROR_TOKEN;
		}
		break;
	default:
		//文件结束读取
		if (feof(fp)) {
			token->type = ERROR;
		}
		else {
			token->type = ERROR_TOKEN;
		}
	}
}

/// <summary>
/// 识别词法单元
/// </summary>
/// <param name="fp"></param>
/// <returns></returns>
Token* gettoken(FILE* fp) {
	Token* token = new Token;
	token_text[0] = '\0';

	//消除空白符号，同时记录行数
	do {
		c = fgetc(fp);
		if (c == '\n') ncount++;
	} while (c == ' ' || c == '\n');

	//识别标识符，关键字
	if (isalpha(c) || c == '_') isIdent(token, fp);

	//识别变量定义类型
	else if (isdigit(c)) defVarType(token, fp);

	//识别符号等
	else isSymbol(token, fp);

	return token;
}

/// <summary>
/// 词法分析结果输出
/// </summary>
void lexAnalyzeOutput() {
	char ch = 0;
	cout << "请输入文件名:" << endl;
	cin >> FileName;
	fp = fopen(FileName, "r");
	if (fp == NULL) {//打开失败
		cout << "文件打开失败";
		return;
	}
	while (ch != EOF) {
		Token* res = gettoken(fp);
		if (res->type == INT_CONST) {
			cout << "整型常量" << "     " << token_text << endl;
		}
		else if (res->type == FLOAT_CONST) {
			cout << "浮点型常量" << "   " << token_text << endl;
		}
		else if (res->type == CHAR_CONST) {
			cout << "字符常量" << "     " << token_text << endl;
		}
		else if (res->type == STRING_CONST) {
			cout << "字符串常量" << "   " << token_text << endl;
		}
		else if (res->type == IDENT) {
			cout << "标识符" << "       " << token_text << endl;
		}
		else if (res->type == EQ) {
			cout << "等于号" << "       " << "==" << endl;
		}
		else if (res->type == ASSIGN) {
			cout << "赋值号" << "       " << "=" << endl;
		}
		else if (res->type == NEQ) {
			cout << "不等号" << "       " << "!=" << endl;
		}
		else if (res->type == NOT) {
			cout << "非" << "           " << "！" << endl;
		}
		else if (res->type == LEFT_SHIFT) {
			cout << "左移" << "         " << "<<" << endl;
		}
		else if (res->type == LTE) {
			cout << "小于等于号" << "   " << "<=" << endl;
		}
		else if (res->type == LT) {
			cout << "小于号" << "       " << "<" << endl;
		}
		else if (res->type == RIGHT_SHIFT) {
			cout << "右移" << "         " << ">>" << endl;
		}
		else if (res->type == GTE) {
			cout << "大于等于号" << "   " << ">=" << endl;
		}
		else if (res->type == GT) {
			cout << "大于号" << "       " << ">" << endl;
		}
		else if (res->type == INC) {
			cout << "自增符" << "       " << "++" << endl;
		}
		else if (res->type == ADD_ASSIGN) {
			cout << "复合算术赋值" << " " << "+=" << endl;
		}
		else if (res->type == SUB_ASSIGN) {
			cout << "复合算术赋值" << " " << "-=" << endl;
		}
		else if (res->type == MUL_ASSIGN) {
			cout << "复合算术赋值" << " " << "*=" << endl;
		}
		else if (res->type == DIV_ASSIGN) {
			cout << "复合算术赋值" << " " << "/=" << endl;
		}
		else if (res->type == MOD_ASSIGN) {
			cout << "复合算术赋值" << " " << "%=" << endl;
		}
		else if (res->type == ADD) {
			cout << "加号" << "         " << "+" << endl;
		}
		else if (res->type == SUB) {
			cout << "减号" << "         " << "-" << endl;
		}
		else if (res->type == MUL) {
			cout << "乘号" << "         " << "*" << endl;
		}
		else if (res->type == DIV) {
			cout << "除号" << "         " << "/" << endl;
		}
		else if (res->type == MOD) {
			cout << "求余" << "         " << "%" << endl;
		}
		else if (res->type == DEC) {
			cout << "自减符" << "       " << "--" << endl;
		}
		else if (res->type == ARROW) {
			cout << "成员运算符" << "   " << "->" << endl;
		}
		else if (res->type == BIT_AND) {
			cout << "位与" << "         " << "&" << endl;
		}
		else if (res->type == BIT_OR) {
			cout << "位或" << "         " << "|" << endl;
		}
		else if (res->type == BIT_XOR) {
			cout << "位异或" << "       " << "^" << endl;
		}
		else if (res->type == BIT_NOT) {
			cout << "位非" << "         " << "~" << endl;
		}
		else if (res->type == AND_ASSIGN) {
			cout << "复合位运算赋值" << "  " << "&=" << endl;
		}
		else if (res->type == OR_ASSIGN) {
			cout << "复合位运算赋值" << "  " << "|=" << endl;
		}
		else if (res->type == XOR_ASSIGN) {
			cout << "复合位运算赋值" << "  " << "^=" << endl;
		}
		else if (res->type == AND_ASSIGN) {
			cout << "复合位运算赋值" << "  " << "&=" << endl;
		}
		else if (res->type == LP) {
			cout << "左小括号" << "     " << "(" << endl;
		}
		else if (res->type == RP) {
			cout << "右小括号" << "     " << ")" << endl;
		}
		else if (res->type == LBRACE) {
			cout << "左大括号" << "     " << "{" << endl;
		}
		else if (res->type == RBRACE) {
			cout << "右大括号" << "     " << "}" << endl;
		}
		else if (res->type == LBRACKET) {
			cout << "左中括号" << "     " << "[" << endl;
		}
		else if (res->type == RBRACKET) {
			cout << "右中括号" << "     " << "]" << endl;
		}
		else if (res->type == SEMI) {
			cout << "分号" << "         " << "；" << endl;
		}
		else if (res->type == COMMA) {
			cout << "逗号" << "         " << "，" << endl;
		}
		else if (res->type == DOT) {
			cout << "成员运算符" << "       " << "." << endl;
		}
		else if (res->type == COLON) {
			cout << "三目运算符" << "   " << "：" << endl;
		}
		else if (res->type == QUESTION) {
			cout << "三目运算符" << "   " << "？" << endl;
		}
		else if (res->type == ARRAY) {
			cout << "数组" << "         " << token_text << endl;
		}
		else if (res->type == ERROR_TOKEN) {
			cout << "错误的表示" << "   " << token_text << endl;
			cout << "错误发生在第" << ncount << "行" << endl;
		}
		else if (res->type == ANNO) {
			cout << "注释" << "         " << token_text << endl;
		}
		else if (res->type == INCLUDE) {
			cout << "头文件" << "       " << token_text << endl;
		}
		else if (res->type == MACRO) {
			cout << "宏" << "           " << token_text << endl;
		}
		else if (res->type == ERROR) break;
		else {
			cout << "关键字" << "       " << token_text << endl;
		}
		ch = fgetc(fp);
		ungetc(ch, fp);
	}
	fclose(fp);//关闭文件
}