#include <stdio.h>
#include <time.h>
#include <stdlib.h> 
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define MAX_STACK_SIZE 100

typedef int element;
typedef struct StackType {
	// element data[];
	element* data;
	int capacity;
	int top;
} StackType;


// create : 이미 만들었음, StackType의 변수를 선언하면 만들어짐
// delete : 할수 없음

// init 
void init(StackType* sptr, int ofs) {
	sptr->data = (element*)malloc(sizeof(StackType) * ofs);
	sptr->top = -1;
	sptr->capacity = ofs;
}

// is_full
int is_full(StackType* sptr) {
	// printf("[is_full] top = %d, capacity = %d\n", sptr->top, sptr->capacity);
	if (sptr->top == sptr->capacity - 1) {
		sptr->capacity = sptr->capacity * 2;
		//sptr->data = (element*)realloc(sptr->data, sptr->capacity * sizeof(element));

	}
	return 0;
}

int is_empty(StackType* sptr) {
	return (sptr->top == -1);
}

// push
void push(StackType* sptr, element item) {

	if (is_full(sptr)) {
		fprintf(stderr, "Stack is full\n");
		return;
	}
	else {
		sptr->top = sptr->top + 1;
		sptr->data[sptr->top] = item;
	}
}

element pop(StackType* sptr) {
	element r;
	if (is_empty(sptr)) {
		fprintf(stderr, "stack is empty\n");
		return -1;
	}
	else {
		// r = sptr->stack[sptr->top];
		// sptr->top = sptr->top - 1;
		// return r;
		return (sptr->data[(sptr->top)--]);
	}
}

element peek(StackType* sptr) {

	element r;
	if (is_empty(sptr)) {
		fprintf(stderr, "stack is empty\n");
		return -1;
	}
	else {
		// r = stack[top];
		// top = top - 1;
		// return r;
		return (sptr->data[sptr->top]);
	}
}

void stack_print(StackType* sptr) {
	for (int i = sptr->top; i >= 0; i--) {
		printf("[%d]", sptr->data[i]);
	}
}

int eval(char expr[]) {
	int len;
	StackType s;
	init(&s, 20);
	len = strlen(expr);
	for (int i = 0; i < len; i++) {
		char ch = expr[i];
		int value;
		int op1, op2;

		//printf("\nStep [%d] %c ", i, ch);
		if (ch >= '0' && ch <= '9') {
			push(&s, ch - '0');
		}
		else if ((ch == '+') || (ch == '-')
			|| (ch == '*') || (ch == '/')) {
			op1 = pop(&s);
			op2 = pop(&s);
			/* if (ch == '+') {
				value = op1 + op2;
				push(&s, value);
			} else if (ch == '-') {
				value = op1 - op2;
				push(&s, value);
			} else if (ch == '*') {
				value = op1 * op2;
				push(&s, value);
			} else {
				value = op1 / op2;
				push(&s, value);
			}
			*/
			switch (ch) {
			case '+':push(&s, op2 + op1); break;
			case '-':push(&s, op2 - op1); break;
			case '*':push(&s, op2 * op1); break;
			case '/':push(&s, op2 / op1); break;
			default:;
			}
		}
		else if ((ch >= '0') && (ch <= '9')) {
			value = ch - '0';
			push(&s, value);
		}
		/*else {
			printf(" Abnormal character in expr\n");
		}*/
		//printf(" Stack : ");
		//stack_print(&s);
	}
	return (pop(&s));
}

int prec(char op) {
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
}

void infix_to_postfix(char exp[], char cexp[]) {
	int i = 0, j = 0;
	char ch, top_op;
	int len = strlen(exp);

	StackType s;
	init(&s, -1); // 스택 초기화

	for (i = 0; i < len; i++) {
		ch = exp[i];
		switch (ch) {
			push(&s, ch);
			break;
			// 연산자일 경우
		case '+': case '-': case '*': case '/':
			while (!is_empty(&s) && (peek(&s) != '(') && ((ch == '*' || ch == '/') ? (peek(&s) == '*' || peek(&s) == '/') : 1)) {
				cexp[j++] = pop(&s);
				cexp[j++] = ' ';
			}
			push(&s, ch);
			break;
			// 괄호의 경우
		case '(':
			push(&s, ch);
			break;
		case ')':
			while (!is_empty(&s)) {
				top_op = pop(&s);
				if (top_op == '(') break;
				cexp[j++] = top_op;
				cexp[j++] = ' ';
			}
			break;
			// 피연산자일 경우
		default:
			// 숫자인 경우 뒤에 있는 숫자까지 하나의 피연산자로 처리
			while (isdigit(ch)) {
				cexp[j++] = ch;
				ch = exp[++i];
			}
			cexp[j++] = ' ';
			i--; // 다음 반복에서 현재 숫자를 다시 처리할 필요가 있으므로 i를 감소시킴
			break;
		}
	}

	while (!is_empty(&s)) {
		cexp[j++] = pop(&s);
		cexp[j++] = ' ';
	}
	cexp[j] = '\0';
}

//fgets(char exp[], int size, char get[]) {
//	for (int i = 0; i <= size; i--) {
//		scanf_s("%s", exp[i]);
//
//		}
//}


int main(int argc, char* argv[]) {
	char infixExpression[MAX_STACK_SIZE];
	char postfixExpression[MAX_STACK_SIZE];
	int choice;


	while (1) {
		printf("다음과 같은 메뉴로 동작하는 프로그램입니다:\n");
		printf("1. 중위식을 입력 받음\n");
		printf("2. 중위식을 후위식으로 변환\n");
		printf("3. 후위식을 계산\n");
		printf("4. 종료\n");
		printf("메뉴를 선택하세요: ");

		int result = 0;
		scanf_s("%d", &choice);

		if (choice >= 5 && choice <= 0) {
			printf("잘못된 메뉴 선택입니다. 다시 선택하세요.\n");
			while (getchar() != '\n');
			break;
		}
		switch (choice) {
		case 1:
			printf("중위식을 입력하세요: ");
			getchar();
			fgets(infixExpression, sizeof(infixExpression), stdin);
			printf("입력된 중위식 : %s\n", infixExpression);
			break;
		case 2:
			printf("중위식을 후위식으로 변환합니다.\n");
			infix_to_postfix(infixExpression, postfixExpression);
			printf("후위식 : %s\n", postfixExpression);
			break;

		case 3:
			printf("계산 결과 : %d", eval(postfixExpression));
			break;

		case 4:
			printf("프로그램을 종료합니다.\n");
			exit(0);

		default:
			printf("잘못된 메뉴 선택입니다. 다시 선택하세요.\n");
			break;
		}

	}

	return 0;
	/*char expression[MAX_STACK_SIZE];
	int result;
	*/

	////printf(" 중위식 표기: 1+2*3-4/(1+1)+3");
	//printf("중위식 표기: 8/2-3+3*2\n");
	////infix_to_postfix("1+2*3-4/2");
	//infix_to_postfix("8/2-3+3*2");
	//printf("\n후위식 수식: 82/3-32\n");
	//result = eval("82/3-32*+");
	//printf("계산 결과: %d\n", result);
	//
}