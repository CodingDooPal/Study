#pragma once

#include <iostream>
#include <vector>

#ifndef __DB_LINKED_LIST_H__
#define __DB_LINKED_LIST_H__

template<typename T>
class List {
public:
	List() {
		// head와 tail에 더미 노드를 생성
		head = new Node;
		tail = new Node;
		cur = nullptr; // 현재 가리키는 대상이 없으므로 nullptr
		listInit(); // 리스트를 초기 상태로 만든다.
	}

	~List() {
		if (!isEmpty()) {
			deleteList();
		}

		delete head;
		delete tail;

		std::cout << "Executed list destructor!\n";
	}

	bool isEmpty() {
		// 리스트가 비어있으면 (head가 tail을 가리키면) true 반환
		if (head->next == tail) {
			return true;
		}

		return false;
	}

	void insertNode(T data) {
		// Node를 생성하고 데이터를 저장한다.
		Node* node{ createNode() };
		node->data = data;

		// 리스트가 비어있다면
		if (isEmpty()) {
			node->prev = head;
			head->next = node;
		}
		// 데이터가 존재한다면
		else {
			node->prev = tail->prev;
			tail->prev->next = node;
		}
		node->next = tail;
		tail->prev = node;

		++numOfData; // 노드가 삽입되었기 때문에 데이터의 수를 +1
	}

	bool deleteNode(T data) {
		// 리스트가 비어있으면 삭제할 데이터도 없다.
		if (isEmpty()) {
			return false;
		}

		cur = head->next; // 리스트 순회 시작 지점
		Node* deleteNode = nullptr; // 동적 할당 해제 시 사용하는 포인터

		// cur이 tail(리스트의 끝)이 아닐 동안 반복
		while (cur != tail) {
			// 앞으로 한 칸 옮긴다.
			deleteNode = cur;
			cur = cur->next;

			// deleteNode가 가리키는 Node의 데이터가 인자에 들어온
			// 데이터와 동일하다면, 해당 Node를 삭제한다.
			if (deleteNode->data == data) {
				// 삭제되는 노드의 앞뒤에 위치한 노드를 서로 연결한 다음에 삭제한다.
				deleteNode->next->prev = deleteNode->prev;
				deleteNode->prev->next = deleteNode->next;
				delete deleteNode;
				--numOfData; // 노드가 삭제되었기 때문에 데이터의 수를 -1
			}
		}

		return true;
	}

	bool deleteList() {
		if (isEmpty()) {
			return false;
		}

		cur = head->next;
		Node* deleteNode = nullptr;

		// 리스트에 저장된 모든 Node 삭제
		while (cur != tail) {
			deleteNode = cur;
			cur = cur->next;
			delete deleteNode;
		}

		// 리스트를 초기 상태로 만든다.
		listInit();

		return true;
	}

	void printList() {
		cur = head->next;

		for (int i = 1; i <= numOfData; ++i) {
			std::cout << "[" << i << "]번째 데이터: " << cur->data << '\n';
			cur = cur->next;
		}
		std::cout << '\n';
	}

private:
	typedef struct _node {
		T data{}; // 데이터 저장
		struct _node* next{ nullptr }; // 다음 노드
		struct _node* prev{ nullptr }; // 이전 노드
	} Node;

	Node* head;
	Node* tail;
	Node* cur;
	int numOfData;

	void listInit() {
		// head와 tail 포인터를 연결
		head->next = tail;
		tail->prev = head;
		numOfData = 0; // 데이터의 수 = 0
	}

	Node* createNode() {
		// Node 생성 후 반환
		Node* node = new Node;
		return node;
	}
};

void DBLinkedList();

#endif