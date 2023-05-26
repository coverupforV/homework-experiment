
#include<iostream>
#include<random>
#include<time.h>
#include<thread>
#include<Windows.h>
#include<mutex>

using namespace std;

class sbuf {
private:
	int buffer[5];  //缓冲区大小为5
	int rear;       //指向数组最后一个元素
	mutex mu;       //互斥信号量
	int empty;      //记录空位
	int full;       //记录占位
public:
	void init();            //初始化
	void insert(int item);  //放入产品
	void remove();          //取出产品
	void display();         //显示缓冲区
};

//初始化
void sbuf::init() {
	rear = -1;
	empty = 5;
	full = 0;
}

//放入产品
void sbuf::insert(int item) {
	while (1) {
		while (!empty) {}
		empty -= 1;             //P(empty)
		mu.lock();              //P(mutex)
		cout << "生产的数为:" << item << endl;
	    buffer[++rear] = item;  //产品放入缓冲区
		display();
	    mu.unlock();            //V(mutex)
	    full += 1;              //V(full)
		break;
	}
}

//取出产品
void sbuf::remove() {
	int item;
	while (1) {
		while (!full) {}
		full -= 1;              //P(full)
		mu.lock();              //P(mutex) 
		item=buffer[rear--];    //从缓冲区取出产品
		cout << "消费的数为:" << item << endl;
		display();
		mu.unlock();            //V(mutex)
		empty += 1;             //V(empty)
		break;
	}
}

//显示缓冲区
void sbuf::display() {
	cout << "缓冲区:";
	if (rear == -1)
		cout << "空" << endl;
	else {
		for (int i = 0; i <= rear; i++)
			cout << buffer[i]<<" ";
		cout << endl;
	}
	cout << endl;
}


//生产者线程
void producer(sbuf* p) {  
    int item;
    uniform_int_distribution<unsigned> k(50, 500);
    static uniform_int_distribution<unsigned> u(1000, 3000);
    static default_random_engine e(time(0)%3000);  //随机数引擎
    for (int i = 0; i < 10; i++) {     //生产20次
        item = u(e);
        p->insert(item);
        Sleep(k(e));
    }
}

//消费者线程
void consumer(sbuf* p) {
    uniform_int_distribution<unsigned> k(100, 1500);
    default_random_engine v(time(0)%2000);    //随机数引擎
    for (int i = 0; i < 10; i++) {     //消费20次
        p->remove();
        Sleep(k(v));
    }
}

int main() {
    sbuf p;
    p.init();

    thread pro(producer, &p);    //创建生产者子线程
    thread con(consumer, &p);    //创建消费者子线程
    pro.join();                  //阻塞主线程
    con.join();                  //阻塞主线程

    cout << "结束！" << endl;
    
    return 0;
}
