#ifndef STACK_H
#define STACK_H

#include <iostream>

template<typename T>
class Stack {
public:
	// 생성자와 소멸자
	Stack() {
		head = new Node;
		stackInit();
	}
	~Stack() {
		deleteStack();
		delete head;

		std::cout << "Executed stack destructor!\n";
	}

	// Stack의 ADT
	bool isEmpty() {
		return head->next == nullptr;
	}

	void push(T data) {
		// 노드를 생성하고 데이터를 저장한다.
		Node* node{ createNode() };
		node->data = data;

		// 스택의 top에 데이터를 삽입한다.
		node->next = head->next;
		head->next = node;

		++numOfData; // 노드가 삽입되었기 때문에 데이터의 수를 +1
	}


	T pop() {
		// 스택이 비어있다면 더미 데이터 반환
		if (isEmpty()) {
			return T{};
		}

		// pop 연산 과정
		Node* deleteNode{ head->next };
		T data{ deleteNode->data };
		head->next = deleteNode->next;
		delete deleteNode;
		--numOfData; // 노드가 삭제되었기 때문에 데이터의 수를 -1

		return data; // 삭제된 데이터 반환
	}

	T peek() {
		// 스택이 비어있다면 더미 데이터 반환
		if (isEmpty()) {
			return T{};
		}

		// top에 저장된 데이터 반환
		return head->next->data;
	}

	bool deleteStack() {
		// 스택이 비어있다면 
		if (isEmpty()) {
			return false;
		}

		// 스택에 저장된 모든 Node 삭제
		Node* deleteNode{ nullptr };
		while (head->next != nullptr) {
			deleteNode = head->next;
			head->next = deleteNode->next;
			delete deleteNode;
		}

		// 스택을 초기 상태로 만든다.
		stackInit();

		return true;
	}

	// 결과 확인용 함수
	void printStack() {
		if (isEmpty()) {
			return;
		}

		Node* cur{ head->next };
		for (int i = 1; i <= numOfData; ++i) {
			std::cout << "[" << i << "]번째 데이터: " << cur->data << '\n';
			cur = cur->next;
		}
		std::cout << '\n';
	}

private:
	// Node 구조체
	typedef struct _node {
		T data;
		struct _node* next;
	} Node;

	Node* head; // 스택의 top을 가리키는 더미 노드
	int numOfData; // 스택에 저장된 데이터의 수

	void stackInit() {
		// head가 nullptr을 가리키도록 한다.
		head->next = nullptr;
		numOfData = 0; // 저장된 데이터의 수 = 0
	}

	Node* createNode() {
		// Node 생성 후 반환
		Node* node = new Node;
		return node;
	}
};

#endif