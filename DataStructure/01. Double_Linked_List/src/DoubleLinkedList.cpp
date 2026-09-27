#include <iostream>
#include "DoubleLinkedList.h"

int main() {
	List<int> list;

	list.insertNode(1);
	list.insertNode(2);
	list.insertNode(3);
	list.insertNode(2);
	list.printList();

	list.deleteNode(2);
	list.printList();

	list.insertNode(2);
	list.printList();

	list.deleteList();
	list.printList();
    
    return 0;
}