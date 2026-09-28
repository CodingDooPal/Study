#include "Stack.h"

int main() {
	Stack<int> st;

	st.push(1);
	st.push(2);
	st.push(3);
	st.printStack();

	st.pop();
	st.printStack();
	int data{ st.peek() };
	std::cout << data << '\n';

	st.deleteStack();
	data = st.peek();
	std::cout << data << '\n';

    return 0;
}