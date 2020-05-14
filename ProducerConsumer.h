#pragma once

#include <queue>
#include <mutex>
#include <list>







typedef struct Mutex_Condition {
	std::mutex mt;
	std::condition_variable cv;
}Mutex_Condition;

extern Mutex_Condition mc_1;
extern Mutex_Condition mc_2;
extern Mutex_Condition mc_3;
extern std::queue<int> que_1;
extern std::queue<int> que_2;


class Produce_1;
class Consume_1;




template<typename T>
class DoubleArray {
	struct NODE {
		T t;
		NODE* next;
	};
	int size_a;
	int size_b;
	NODE* header_a;
	NODE* header_a_cur;
	NODE* header_b;
	NODE* header_b_cur;
	int trigger;



public:
	DoubleArray() : size_a(0), size_b(0), trigger(0), header_a(0), header_a_cur(0), header_b(0), header_b_cur(0) {
	}
	int push(T t);
	std::list<T>& fetch(std::list<T>& list);
};
template<typename T>
int DoubleArray<T>::push(T t) {
	NODE *n = new NODE;
	n->t = t;
	n->next = 0;
	if (size_a == 0 && trigger == 0) {
		header_a = n;
		header_a_cur = n;
		size_a++;
		return size_a;
	}
	else if (size_b == 0 && trigger == 1) {
		header_b = n;
		header_b_cur = n;
		size_b++;
		return size_b;
	}
	else {
		switch (trigger) {
		case 0:
			header_a_cur->next = n;
			header_a_cur = n;
			size_a++;
			return size_a;
			//break;
		case 1:
			header_b_cur->next = n;
			header_b_cur = n;
			size_b++;
			return size_b;
			//break;
		}


	}

	return -1;
}
template<typename T>
std::list<T>& DoubleArray<T>::fetch(std::list<T>& list) {
	switch (trigger) {
	case 0:
		if (header_a != 0) {
			// change b
			trigger = 1;
			// fetch a
			NODE* temp = header_a;
			while (temp) {
				list.push_back(temp->t);
				temp = temp->next;
			}
			// delete a
			temp = header_a;
			for (int i = 0; i < size_a; ++i) {
				NODE* p = temp;
				temp = temp->next;
				delete p;
			}
			size_a = 0;
			header_a = 0;
			header_a_cur = 0;
		}
		break;
	case 1:
		if (header_b != 0) {
			// change a
			trigger = 0;
			// fetch b
			NODE* temp = header_b;
			// delete b
			while (temp) {
				list.push_back(temp->t);
				temp = temp->next;
			}
			temp = header_b;
			for (int i = 0; i < size_b; ++i) {
				NODE* p = temp;
				temp = temp->next;
				delete p;
			}
			size_b = 0;
			header_b = 0;
			header_b_cur = 0;
		}
		break;
	}
	return list;
}








