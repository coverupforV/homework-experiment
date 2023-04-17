#include"sbuf.h"
#include<iostream>
#include<random>
#include <time.h>
#include<thread>
#include <Windows.h>

using namespace std;

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
