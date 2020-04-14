#include <mutex>
#include <thread>
#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"
using namespace std;

mutex mtx;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {
	while(anOrder.number_donuts != 0){
		Box boo;
		while (boo.size() != 12 && anOrder.number_donuts != 0){
			DONUT doo;
			boo.addDonut(doo);
			anOrder.number_donuts -= 1;
		}
		anOrder.boxes.push_back(boo);
	}
	for(Box b: anOrder.boxes){
		anOrder.number_donuts += b.size();
	}
	{
		lock_guard<mutex> lg(mtx);
		PRINT2("Order completed by ", id);
		order_out_Vector.push_back(anOrder);
	}
}

//as long as there are orders in order_in_Q then
//for each order:
//	create box(es) filled with number of donuts in the order
//  then place finished order on order_outvector
//  if waiter is finished (b_WaiterIsFinished) then
//  finish up remaining orders in order_in_Q and exit
//
//You will use cv_order_inQ to be notified by waiter
//when either order_in_Q.size() > 0 or b_WaiterIsFinished == true
//hint: wait for something to be in order_in_Q or b_WaiterIsFinished == true
void Baker::beBaker() {
	while(!b_WaiterIsFinished){
		{
			unique_lock<mutex> lck(mtx);
			while (order_in_Q.size() == 0 && !b_WaiterIsFinished){
				cv_order_inQ.wait(lck);
			}
		}
		while(!order_in_Q.empty()){
			ORDER nextOrder;
			nextOrder.order_number = -7;
			{
				lock_guard<mutex> lg(mtx);
				if(order_in_Q.size() != 0){
					nextOrder = order_in_Q.front();
					order_in_Q.pop();
				}
			}
			if(nextOrder.order_number != -7){
				bake_and_box(nextOrder);
			}
			this_thread::sleep_for(chrono::milliseconds(50));
		}
	}
}
//I hate my life
