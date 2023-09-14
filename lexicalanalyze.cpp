#define _CRT_SECURE_NO_WARNINGS
#include "lexicalanalyze.h"

char c;
char token_text[100];//���ڴ洢�ʷ���Ԫ��ȫ�ֱ���
char string_num[100];
char FileName[50];//�ļ���
FILE* fp;
int ncount;//���ڼ�¼������������

/// <summary>
/// ��һ���ַ����뵽�ʷ���Ԫ��
/// </summary>
/// <param name="token">�ʷ���Ԫ</param>
/// <param name="c">�ַ�</param>
void addtoken(char* token, char c) {
	int i = 0;
	while (*(token + i) != '\0') {
		i++;
	}
	*(token + i) = c;
	*(token + i + 1) = '\0';
}

/// <summary>
/// �жϱ�ʶ���Ƿ�Ϊ�ؼ���/����
/// </summary>
/// <param name="token">��ʶ��Ĵʷ���Ԫ</param>
/// <param name="fp">�ļ�ָ��</param>
void isKey(Token* token, FILE* fp) {
	//�������Ͷ���
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
	//�ؼ���
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
	//����
	else {
		c = fgetc(fp);
		if (c == '[') {
			//ʶ������
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
/// ��ʶ��ʶ��
/// </summary>
/// <param name="token">��ʶ��Ĵʷ���Ԫ</param>
/// <param name="fp">�ļ�ָ��</param>
void isIdent(Token* token, FILE* fp) {
	//���c����ĸ����_
	int i = 0;
	do {
		token_text[i++] = c;
	} while (isalpha(c = fgetc(fp)) || isdigit(c));//ƴ��ʶ����
	ungetc(c, fp);//�˻ض�����ַ����ļ����뻺����
	token_text[i] = '\0';
	//ʶ��ؼ���
	isKey(token, fp);
	token->value = new char[strlen(token_text) + 1];
	strcpy(token->value, token_text);
}

/// <summary>
/// ʶ���������
/// </summary>
/// <param name="token">��ʶ��Ĵʷ���Ԫ</param>
/// <param name="fp">�ļ�ָ��</param>
void defVarType(Token* token, FILE* fp) {
	//���0��ͷ�����ǰ˽���ʮ������
	if (c == '0') {
		addtoken(token_text, c);
		c = fgetc(fp);
		//ʮ������
		if (c == 'x') {
			addtoken(token_text, c);
			c = fgetc(fp);
			while (isxdigit(c)) {
				addtoken(token_text, c);
				c = fgetc(fp);
			}
			ungetc(c, fp);   // �˻ض�����ַ�
			token->type = INT_CONST;
		}
		else if (c == '.') {
			c = fgetc(fp);
			//.���治Ϊ���֣�����
			if (!isdigit(c)) token->type = ERROR_TOKEN;
			//��ȷ������
			else {
				ungetc(c, fp);
				addtoken(token_text, '.');
				c = fgetc(fp);
				do {
					addtoken(token_text, c);
				} while (isdigit(c = fgetc(fp)));
				//���Ǻ���f�ĸ�����
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
		//�˽���
		else {
			while (c >= '0' && c <= '7') {
				addtoken(token_text, c);
				c = fgetc(fp);
			}
			ungetc(c, fp);   // �˻ض�����ַ�
			token->type = INT_CONST;
		}
	}
	//����0��ͷ
	else {
		//ƴ���ִ�
		do {
			addtoken(token_text, c);
		} while (isdigit(c = fgetc(fp)));
		//���Ǹ��������
		if (c != '.' && c != 'u' && c != 'l') {
			//�жϴ���
			/*if (c != ' ' && c != ';' && c != ')' && c != '+' && c != '-' && c != '*' && c != '/') {
				token->type = ERROR_TOKEN;
			}*/
			//��֮Ϊ���ͱ���
			//else {
			ungetc(c, fp);
			token->type = INT_CONST;
			//}
		}
		//������
		else if (c == '.') {
			c = fgetc(fp);
			//.���治Ϊ���֣�����
			if (!isdigit(c)) token->type = ERROR_TOKEN;
			//��ȷ������
			else {
				ungetc(c, fp);
				addtoken(token_text, '.');
				c = fgetc(fp);
				do {
					addtoken(token_text, c);
				} while (isdigit(c = fgetc(fp)));
				//���Ǻ���f�ĸ�����
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
		//�޷�������
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
				//����
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
		//������
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
		//��ͨ����
		else {
			token->type = INT_CONST;
		}

	}
	token->value = new char[strlen(token_text) + 1];
	strcpy(token->value, token_text);
}

/// <summary>
/// ���ſ�ͷ��ʶ��
/// </summary>
/// <param name="token">��ʶ��Ĵʷ���Ԫ</param>
/// <param name="fp">�ļ�ָ��</param>
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
		//.��ͷ�ĸ�����
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
			//��ȡע��
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
					// ����ȱ��ע�ͽ�����ǵ����
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
			//�˽���
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
		//�ļ�������ȡ
		if (feof(fp)) {
			token->type = ERROR;
		}
		else {
			token->type = ERROR_TOKEN;
		}
	}
}

/// <summary>
/// ʶ��ʷ���Ԫ
/// </summary>
/// <param name="fp"></param>
/// <returns></returns>
Token* gettoken(FILE* fp) {
	Token* token = new Token;
	token_text[0] = '\0';

	//�����հ׷��ţ�ͬʱ��¼����
	do {
		c = fgetc(fp);
		if (c == '\n') ncount++;
	} while (c == ' ' || c == '\n');

	//ʶ���ʶ�����ؼ���
	if (isalpha(c) || c == '_') isIdent(token, fp);

	//ʶ�������������
	else if (isdigit(c)) defVarType(token, fp);

	//ʶ����ŵ�
	else isSymbol(token, fp);

	return token;
}

/// <summary>
/// �ʷ�����������
/// </summary>
void lexAnalyzeOutput() {
	char ch = 0;
	cout << "�������ļ���:" << endl;
	cin >> FileName;
	fp = fopen(FileName, "r");
	if (fp == NULL) {//��ʧ��
		cout << "�ļ���ʧ��";
		return;
	}
	while (ch != EOF) {
		Token* res = gettoken(fp);
		if (res->type == INT_CONST) {
			cout << "���ͳ���" << "     " << token_text << endl;
		}
		else if (res->type == FLOAT_CONST) {
			cout << "�����ͳ���" << "   " << token_text << endl;
		}
		else if (res->type == CHAR_CONST) {
			cout << "�ַ�����" << "     " << token_text << endl;
		}
		else if (res->type == STRING_CONST) {
			cout << "�ַ�������" << "   " << token_text << endl;
		}
		else if (res->type == IDENT) {
			cout << "��ʶ��" << "       " << token_text << endl;
		}
		else if (res->type == EQ) {
			cout << "���ں�" << "       " << "==" << endl;
		}
		else if (res->type == ASSIGN) {
			cout << "��ֵ��" << "       " << "=" << endl;
		}
		else if (res->type == NEQ) {
			cout << "���Ⱥ�" << "       " << "!=" << endl;
		}
		else if (res->type == NOT) {
			cout << "��" << "           " << "��" << endl;
		}
		else if (res->type == LEFT_SHIFT) {
			cout << "����" << "         " << "<<" << endl;
		}
		else if (res->type == LTE) {
			cout << "С�ڵ��ں�" << "   " << "<=" << endl;
		}
		else if (res->type == LT) {
			cout << "С�ں�" << "       " << "<" << endl;
		}
		else if (res->type == RIGHT_SHIFT) {
			cout << "����" << "         " << ">>" << endl;
		}
		else if (res->type == GTE) {
			cout << "���ڵ��ں�" << "   " << ">=" << endl;
		}
		else if (res->type == GT) {
			cout << "���ں�" << "       " << ">" << endl;
		}
		else if (res->type == INC) {
			cout << "������" << "       " << "++" << endl;
		}
		else if (res->type == ADD_ASSIGN) {
			cout << "����������ֵ" << " " << "+=" << endl;
		}
		else if (res->type == SUB_ASSIGN) {
			cout << "����������ֵ" << " " << "-=" << endl;
		}
		else if (res->type == MUL_ASSIGN) {
			cout << "����������ֵ" << " " << "*=" << endl;
		}
		else if (res->type == DIV_ASSIGN) {
			cout << "����������ֵ" << " " << "/=" << endl;
		}
		else if (res->type == MOD_ASSIGN) {
			cout << "����������ֵ" << " " << "%=" << endl;
		}
		else if (res->type == ADD) {
			cout << "�Ӻ�" << "         " << "+" << endl;
		}
		else if (res->type == SUB) {
			cout << "����" << "         " << "-" << endl;
		}
		else if (res->type == MUL) {
			cout << "�˺�" << "         " << "*" << endl;
		}
		else if (res->type == DIV) {
			cout << "����" << "         " << "/" << endl;
		}
		else if (res->type == MOD) {
			cout << "����" << "         " << "%" << endl;
		}
		else if (res->type == DEC) {
			cout << "�Լ���" << "       " << "--" << endl;
		}
		else if (res->type == ARROW) {
			cout << "��Ա�����" << "   " << "->" << endl;
		}
		else if (res->type == BIT_AND) {
			cout << "λ��" << "         " << "&" << endl;
		}
		else if (res->type == BIT_OR) {
			cout << "λ��" << "         " << "|" << endl;
		}
		else if (res->type == BIT_XOR) {
			cout << "λ���" << "       " << "^" << endl;
		}
		else if (res->type == BIT_NOT) {
			cout << "λ��" << "         " << "~" << endl;
		}
		else if (res->type == AND_ASSIGN) {
			cout << "����λ���㸳ֵ" << "  " << "&=" << endl;
		}
		else if (res->type == OR_ASSIGN) {
			cout << "����λ���㸳ֵ" << "  " << "|=" << endl;
		}
		else if (res->type == XOR_ASSIGN) {
			cout << "����λ���㸳ֵ" << "  " << "^=" << endl;
		}
		else if (res->type == AND_ASSIGN) {
			cout << "����λ���㸳ֵ" << "  " << "&=" << endl;
		}
		else if (res->type == LP) {
			cout << "��С����" << "     " << "(" << endl;
		}
		else if (res->type == RP) {
			cout << "��С����" << "     " << ")" << endl;
		}
		else if (res->type == LBRACE) {
			cout << "�������" << "     " << "{" << endl;
		}
		else if (res->type == RBRACE) {
			cout << "�Ҵ�����" << "     " << "}" << endl;
		}
		else if (res->type == LBRACKET) {
			cout << "��������" << "     " << "[" << endl;
		}
		else if (res->type == RBRACKET) {
			cout << "��������" << "     " << "]" << endl;
		}
		else if (res->type == SEMI) {
			cout << "�ֺ�" << "         " << "��" << endl;
		}
		else if (res->type == COMMA) {
			cout << "����" << "         " << "��" << endl;
		}
		else if (res->type == DOT) {
			cout << "��Ա�����" << "       " << "." << endl;
		}
		else if (res->type == COLON) {
			cout << "��Ŀ�����" << "   " << "��" << endl;
		}
		else if (res->type == QUESTION) {
			cout << "��Ŀ�����" << "   " << "��" << endl;
		}
		else if (res->type == ARRAY) {
			cout << "����" << "         " << token_text << endl;
		}
		else if (res->type == ERROR_TOKEN) {
			cout << "����ı�ʾ" << "   " << token_text << endl;
			cout << "�������ڵ�" << ncount << "��" << endl;
		}
		else if (res->type == ANNO) {
			cout << "ע��" << "         " << token_text << endl;
		}
		else if (res->type == INCLUDE) {
			cout << "ͷ�ļ�" << "       " << token_text << endl;
		}
		else if (res->type == MACRO) {
			cout << "��" << "           " << token_text << endl;
		}
		else if (res->type == ERROR) break;
		else {
			cout << "�ؼ���" << "       " << token_text << endl;
		}
		ch = fgetc(fp);
		ungetc(ch, fp);
	}
	fclose(fp);//�ر��ļ�
}