/*
 * Code completed by George Minnigerode
 * Minor fixes by Sean LaRochelle
 * Comments by George Minnigerode
 */
#include <string>
#include <thread>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"

using namespace std;

//Constructor for Waiter using given id that populates IO object myIO with given filename
Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

//Waiter destructor
Waiter::~Waiter()
{
}

//Gets next Order from file_IO by updating value of given order
//If given order contains new order then return == SUCCESS
//Otherwise return fileIO error (noOrder or FAIL)
int Waiter::getNext(ORDER &anOrder){
	return myIO.getNext(anOrder);
}

//Main function for waiter that populates the order_in_Q wiht orders from filename
//Each time an order is added to the queue all Baker threads are notified so that they can begin working
//When all orders are processed b_WaiterIsFinished is set to true to notify bakers
void Waiter::beWaiter() {
	int ordersLeft = 0;
	//iterates through orders using myIO until there are no orders left
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
	//all orders have been processed
	b_WaiterIsFinished = true;
	this_thread::sleep_for(chrono::milliseconds(100));
	cv_order_inQ.notify_all();
}

