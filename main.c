#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define now_is_root 2
#define root_is_root 1
#define MAX_SIZE 1024 // �ڵ� ���� �ִ� �迭���� �ִ�ũ��
#define false 0
#define true  1

typedef enum TOKENTYPE_ {
	NUL, NUM, LPARN, RPARN, ADD, SUB, MUL, DIV
} TOKENTYPE; // ���������� ������ �Ǻ�

typedef struct TOKEN_ {
	TOKENTYPE tokenType;
	int value;
} TOKEN; // TOKENTYPE���� �����Ͽ� �����ڸ� oper, ���̸� value�� ���� ����

typedef struct Node_ {
	TOKEN node;
	struct Node_ *left;
	struct Node_ *right;
} Node; // Ʈ���� ���

typedef struct Q_ {
	TOKEN queue[MAX_SIZE];
} Q; // ��ū���� ���� ť

typedef struct Tree_ {
	Node *root;
} Tree; // Parsing tree

Q q;
int front;
int rear;
q_index;
q_max;

Tree TREE;

char input[MAX_SIZE]; // ������ �Է¹��� �迭
int input_index;

TOKEN token[MAX_SIZE];
int token_index;
int token_max;

// dequeue �۾��� �ʿ����� �ʾ� enqueue�� �������
void enqueue(TOKEN t) {
	q.queue[front++] = t;
	rear = (rear + 1) % MAX_SIZE;
}

// ���� ��ū�� �������� Ȯ���� �뵵�� ����ϴ� �Լ�
TOKEN check_queue(void) {
	if (q_index < q_max) {
		return q.queue[q_index++];
	}
	else {
		TOKEN Null;
		Null.tokenType = NUL;
		return Null;
	}
}

// �ϳ��� ��ū�� ������ �Լ�
void make_token(TOKEN tok) {
	token[token_index++] = tok;
}

// ���꿡 ���Ǵ� ���� �����Ͽ� token����ü�� �����ϴ� �Լ�
void get_num(void) {
	int start_index = input_index;
	int end_index;
	int weighted_value = 1;
	int sum = 0;

	TOKEN num;

	while (true) { // input���� ������ ���� ���ϱ�
		if (!isdigit(input[input_index])) {
			end_index = input_index - 1;
			break;
		}
		input_index++;
	}

	for (end_index; end_index >= start_index; end_index--) { // �� �ڸ����� 10�� n���� ���� �� ����
		sum += (input[end_index] - 48) * weighted_value;
		weighted_value *= 10;
	}
	num.tokenType = NUM;
	num.value = sum;

	make_token(num);
	return;
}

// �������� ������ �Ǻ��Ͽ� token����ü�� �����ϴ� �Լ�
void get_oper(void) {
	TOKEN operator;
	operator.value = NUL;

	switch (input[input_index]) {
		case '+': {
			operator.tokenType = ADD;
			break;
		}
		case '-': {
			operator.tokenType = SUB;
			break;
		}
		case '*': {
			operator.tokenType = MUL;
			break;
		}
		case '/': {
			operator.tokenType = DIV;
			break;
		}
		case '(': {
			operator.tokenType = LPARN;
			break;
		}
		case ')': {

			operator.tokenType = RPARN;
			break;
		}
	}
	make_token(operator);
	input_index++;
}

// ������ ���� �� ������ ��ŵ���ִ� �Լ�
void skip_whitespace(void) {
	while (input[input_index] == ' ') {
		input_index++;
	}
}

// �Է� ������ ��ūȭ �����ִ� �Լ�
void Lexer(void) {
	while (input_index < (int)strlen(input)) {
		switch (input[input_index]) {
			case ' ':
			case '\t': {
				skip_whitespace();
				break;
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': {
				get_num();
				break;
			}
			case '+':
			case '-':
			case '*':
			case '/':
			case '(':
			case ')': {
				get_oper();
				break;
			}
			default: {
				printf("���ڿ� �����ڸ� �Է��ϼ���.\n");
				exit(1);
				break;
			}
		}
	}
	token_max = token_index;
	return;
}

void SynAnalyzer(void) {
	while (input_index < (int)strlen(input)) {
		if (input_index == 0 && input[input_index] != '(') {
			if (input_index == 0 && isdigit(input[input_index]) == false) {
				printf("ù ��° �Է��� �׻� �ް�ȣ �Ǵ� �ǿ����ڿ��� �մϴ�.\n");
				exit(1);
				return;
			}
		}

		switch (input[input_index]) {
			case '+':
			case '-':
			case '*':
			case '/':
			case '(': {
				if (input[input_index] == '(' && isdigit(input[input_index - 1]) != false && isspace(input[input_index - 1]) == false) {
					printf("�ް�ȣ �տ��� �׻� �����ڸ� �Է��ϼ���.\n");
					exit(1);
					return;
				}
				
				if (input[input_index+1] != '(' && isdigit(input[input_index + 1]) == false && isspace(input[input_index + 1]) == false) {
					printf("������, �ް�ȣ �ڿ��� �׻� �ǿ����ڸ� �Է��ϼ���.\n");
					exit(1);
					return;
				}
				break;
			}
		}
		input_index++;
	}
	return;
}

void token_input_queue(void) {
	while (token_index < token_max) {
		enqueue(token[token_index++]);
	}
	q_max = token_index;
}

Node *value(TOKEN tok) {
	Node *val = (Node *)malloc(sizeof(Node));
	val->node = tok;

	return val;
}

Node *oper(TOKEN *tok, Node *le, Node *ri) {
	Node *parent = (Node *)malloc(sizeof(Node));
	parent->node = *tok;
	parent->left = le;
	parent->right = ri;

	return parent;
}

TOKEN peek;

void check_error(TOKENTYPE);

Node *expr(void);

Node *term(void);

Node *factor(void);

Node *parse(void) {
	peek = q.queue[q_index++];
	Node *node = expr();
	return node;
}

int get_result(Node *node) {
	int a, b;
	if (node->node.tokenType != NUM) {
		switch (node->node.tokenType) {
			case ADD: {
				if (node->left->node.tokenType != NUM) {
					a = get_result(node->left);
				}
				else {
					a = get_result(node->left);
				}
			
				if (node->right->node.tokenType != NUM) {
					b = get_result(node->right);
				}
				else {
					b = get_result(node->right);
				}

				return a + b;
			}
			case SUB: {
				if (node->left->node.tokenType != NUM) {
					a = get_result(node->left);
				}
				else {
					a = get_result(node->left);
				}

				if (node->right->node.tokenType != NUM) {
					b = get_result(node->right);
				}
				else {
					b = get_result(node->right);
				}
	
				return a - b;
			}
			case MUL: {
				if (node->left->node.tokenType != NUM) {
					a = get_result(node->left);
				}
				else {
					a = get_result(node->left);
				}
	
				if (node->right->node.tokenType != NUM) {
					b = get_result(node->right);
				}
				else {
					b = get_result(node->right);
				}
	
				return a * b;
			}
			case DIV: {
				if (node->left->node.tokenType != NUM) {
					a = get_result(node->left);
				}
				else {
					a = get_result(node->left);
				}
	
				if (node->right->node.tokenType != NUM) {
					b = get_result(node->right);
				}
				else {
					b = get_result(node->right);
				}
	
				return a / b;
			}
		}
	}
	else {
		return node->node.value;
	}
}

int main(int argc, char *argv[]) {
	int result = 0, mode = 0;
	printf("������ �Է��ϼ���\n");
	printf("�Է� : ");
	scanf("%[^\n]s", input);

	SynAnalyzer();

	input_index = 0;
	Lexer();

	token_index = 0;
	token_input_queue();

	/*
	<expr> -> <term> | <term> + | - <term>
	<term> -> <factor> | <factor> * | / <factor>
	<factor> -> <number> | ( <expr> )
	*/
	TREE.root = parse();

	result = get_result(TREE.root);
	printf("���� ��� : %d\n", result);

	return 0;
}

void check_error(TOKENTYPE toktype) {
	if (peek.tokenType != toktype) {
		printf("���Ŀ� ������ �ֽ��ϴ�.\n");
		exit(1);
		return;
	}
	else {
		peek = q.queue[q_index++];
		return;
	}
}

Node *expr(void) {
	Node *node = term();
	while (peek.tokenType == ADD || peek.tokenType == SUB) {
		TOKEN tok = peek;
		if (peek.tokenType == ADD) {
			check_error(ADD);
		}
		else {
			check_error(SUB);
		}
		node = oper(&tok, node, term());
	}
	return node;
}

Node *term(void) {
	Node *node = factor();
	while (peek.tokenType == MUL || peek.tokenType == DIV) {
		TOKEN tok = peek;
		if (peek.tokenType == MUL) {
			check_error(MUL);
		}
		else {
			check_error(DIV);
		}
		node = oper(&tok, node, factor());
	}
	return node;
}

Node *factor(void) {
	TOKEN tok = peek;

	switch (tok.tokenType) {
		case NUM: {
			check_error(NUM);
			return value(tok);
		}
		case LPARN: {
			check_error(LPARN);
			Node *node = expr();
			check_error(RPARN);
			return node;
		}
	}
}