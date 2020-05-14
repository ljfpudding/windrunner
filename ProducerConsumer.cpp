#include "stdafx.h"




#include "ProducerConsumer.h"


Mutex_Condition mc_1;
Mutex_Condition mc_2;
Mutex_Condition mc_3;
std::queue<int> que_1;
std::queue<int> que_2;


class Produce_1 {
public:
	Produce_1(std::queue<int> * que_1, std::queue<int> * que_2, Mutex_Condition * mc_1, Mutex_Condition * mc_2) {
		m_read_que = que_1;
		m_writer_que = que_2;
		m_read_mc = mc_1;
		m_writer_mc = mc_2;
		m_stop = false;

	}
	void runProduce() {
		while (!m_stop) {
			std::this_thread::sleep_for(std::chrono::microseconds(20 * 1000));
			std::lock_guard<std::mutex> lgd(m_writer_mc->mt);
			m_writer_que->push(1);
			m_writer_mc->cv.notify_one();
			//std::cout << "m_writer push" << std::endl;
		}

	}
	void join() {
		m_trd->join();
		m_trd.reset();
	}
	void start() {
		m_trd.reset(new std::thread(std::bind(std::mem_fun(&Produce_1::runProduce), this)));
	}
	void stop() {
		m_stop = true;
	}
private:
	Mutex_Condition * m_read_mc;
	Mutex_Condition * m_writer_mc;
	std::queue<int> * m_read_que;
	std::queue<int> * m_writer_que;
	volatile bool m_stop;
	std::shared_ptr<std::thread> m_trd;
};


class Consume_1 {
public:
	Consume_1(std::queue<int> * que_1, std::queue<int> * que_2, Mutex_Condition * mc_1, Mutex_Condition * mc_2, Mutex_Condition * switch_mc) {
		m_read_que = que_1;
		m_writer_que = que_2;
		m_read_mc = mc_1;
		m_writer_mc = mc_2;
		m_stop = false;
		m_switch_mc = switch_mc;
	}

	void runConsume() {
		while (!m_stop) {
			while (true) {
				std::unique_lock<std::mutex> ulg(m_read_mc->mt);
				while (m_read_que->empty()) {
					m_read_mc->cv.wait(ulg);
				}
				//deal data
				//std::lock_guard<std::mutex> ulg(m_read_mc->mt);
				while (!m_read_que->empty()) {
					m_read_que->pop();
					//std::cout << "m_read_queue pop" << std::endl;
				}
				m_switch_mc->cv.notify_one();
			}
		}
	}
	void join() {
		m_trd->join();
		m_trd.reset();
	}
	void start() {
		m_trd.reset(new std::thread(std::bind(std::mem_fun(&Consume_1::runConsume), this)));
	}
	void stop() {
		m_stop = true;
	}
private:
	Mutex_Condition * m_read_mc;
	Mutex_Condition * m_writer_mc;
	Mutex_Condition * m_switch_mc;
	std::queue<int> * m_read_que;
	std::queue<int> * m_writer_que;
	volatile bool m_stop;
	std::shared_ptr<std::thread> m_trd;
};
void que_switch_trd(std::queue<int> * read_que, std::queue<int> * writer_que, Mutex_Condition * read_mc, Mutex_Condition * writer_mc, Mutex_Condition * switch_mc) {
	while (true) {
		{
			std::unique_lock<std::mutex> ulg(writer_mc->mt);
			while (writer_que->empty()) {
				writer_mc->cv.wait(ulg);
			}
			std::lock_guard<std::mutex> ulg_2(read_mc->mt);
			std::swap(*read_que, *writer_que);
			//std::cout << "switch queue" << std::endl;
			if (!read_que->empty()) {
				read_mc->cv.notify_one();
			}
		}
		std::unique_lock<std::mutex> ulg_2(switch_mc->mt);
		while (!read_que->empty()) {
			switch_mc->cv.wait(ulg_2);
		}
	}
}
/*int main() {

	Mutex_Condition mc_1;
	Mutex_Condition mc_2;
	Mutex_Condition mc_3;
	std::queue<int> que_1;
	std::queue<int> que_2;

	Produce_1 produce_1(&que_1, &que_2, &mc_1, &mc_2);
	Consume_1 consume_1(&que_1, &que_2, &mc_1, &mc_2, &mc_3);

	std::thread trd(std::bind(&que_switch_trd, &que_1, &que_2, &mc_1, &mc_2, &mc_3));
	produce_1.start();
	consume_1.start();

	produce_1.join();
	consume_1.join();
	trd.join();

	return 0;
}
*/









