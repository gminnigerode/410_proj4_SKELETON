#include <string>
#include <thread>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
//if return == SUCCESS then anOrder
//contains new order
//otherwise return contains fileIO error
int Waiter::getNext(ORDER &anOrder){
	return myIO.getNext(anOrder);
}

//contains a loop that will get orders from filename one at a time
//then puts them in order_in_Q then signals baker(s) using cv_order_inQ
//so they can be consumed by baker(s)
//when finished exits loop and signals baker(s) using cv_order_inQ that
//it is done using b_WaiterIsFinished
void Waiter::beWaiter() {
	int ordersLeft = 0;
	while(ordersLeft != NO_ORDERS && ordersLeft != FAIL){
		ORDER currentOrder;
		ordersLeft = getNext(currentOrder);
		if(ordersLeft != NO_ORDERS){
			{
				lock_guard<mutex> lg(mutex_order_inQ);
				order_in_Q.push(currentOrder);
			}
			this_thread::sleep_for(chrono::milliseconds(100));
			cv_order_inQ.notify_all();
		}
	}
	b_WaiterIsFinished = true;
	this_thread::sleep_for(chrono::milliseconds(100));
	cv_order_inQ.notify_all();
}

