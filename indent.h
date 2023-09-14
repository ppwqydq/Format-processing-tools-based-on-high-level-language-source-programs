#pragma once
#define _CRT_SECURE_NO_WARNINGS

typedef struct token {
	char* str;
	struct token* next;
} token;

void indent();

token* readline();
