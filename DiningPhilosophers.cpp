#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <windows.h>
#include <time.h>
#include <mutex>

using namespace std;

#define THINKING_TIME 100;
#define EATING_TIME 50;
#define HUNGER_TIME 50;

class Philosopher {
	public:

		Philosopher(int m_id, string m_name) {
			id = m_id;
			name = m_name;
			state = 0;
		}

		/*
		States:
			0 == Thinking
			1 == Eating
			2 == Waiting
		*/
		int state;
		int id;
		string name;
		int msThink = 0;
		int msHunger = 0;
		int msEat = 0;
};

class Main {
public:

	Main() {
		p1 = new Philosopher(0, "philosopher_1");
		p2 = new Philosopher(1, "philosopher_2");
		p3 = new Philosopher(2, "philosopher_3");
		p4 = new Philosopher(3, "philosopher_4");
		p5 = new Philosopher(4, "philosopher_5");
	}

	Philosopher* p1;
	Philosopher* p2;
	Philosopher* p3;
	Philosopher* p4;
	Philosopher* p5;

	void Test(int c) {
		int l = getLeft(c);
		int r = getRight(c);

		if (getP(l)->state != 1 && getP(r)->state != 1 && getP(c)->state != 1) { // Check if both neighbors are not eating
			getP(c)->state = 1;
		}
	}

	Philosopher* getP(int id) {
		switch (id) {
		case 0:
			return p1;
		case 1:
			return p2;
		case 2:
			return p3;
		case 3:
			return p4;
		case 4:
			return p5;
		}
	}

	int getLeft(int i) {
		int l = i - 1;
		if (l == -1) l = 4;
		return l;
	}

	int getRight(int i) {
		int r = i + 1;
		if (r == 5) r = 0;
		return r;
	}

	void releaseChopstick(int i) {
		getP(i)->state = 0; // Change this philosopher to thinking
		Test(getRight(i));
		Test(getLeft(i));
		
	};

	void takeChopstick(int i) {
		getP(i)->state = 2;
		Test(i);
		if (getP(i)->state != 1) {
			Sleep(500);
			cout << "Philosopher " << getP(i)->id << " is starving." << endl;
			getP(i)->msHunger += HUNGER_TIME;
			//bool change = false;
			//while (!change) {
			//	Test(i);
			//	if (getP(i)->state == 1) change = true;
			//}
		}
		else {
			Sleep(500);
			cout << "Philosopher " << getP(i)->id << " is eating." << endl;
			getP(i)->msEat += EATING_TIME;
		}
	}

	void Loop(int ref) {
		int c = 0;
		while (c < 10) {
			Sleep(1000);
			cout << "Philosopher " << ref << " is thinking." << endl;
			getP(ref)->msThink += THINKING_TIME;
			takeChopstick(ref);
			releaseChopstick(ref);
			c++;
		}
		for (int i = 0; i < ref * 10; i++) {
			Sleep(10 * i);
		}
		cout << "Phil " << ref << ":" << endl;
		cout << "H: " << getP(ref)->msHunger << endl;
		cout << "E: " << getP(ref)->msEat << endl;
		cout << "T: " << getP(ref)->msThink << endl;
	}

	void Run() {
		// Create the threads for each philosopher
		thread first_thread([this] {this->Loop(0);});
		thread second_thread([this] {this->Loop(1);});
		thread third_thread([this] {this->Loop(2);});
		thread fourth_thread([this] {this->Loop(3);});
		thread fifth_thread([this] {this->Loop(4);});


		// Join all of the threads
		first_thread.join();
		second_thread.join();
		third_thread.join();
		fourth_thread.join();
		fifth_thread.join();
	}
};


int main()
{
	srand(time(NULL));
	Main* main = new Main();
	main->Run();
    std::cout << "Finished!\n"; 
}