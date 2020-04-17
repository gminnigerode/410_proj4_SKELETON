#include <mutex>
#include <thread>
#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"
using namespace std;

//Baker constructor using given id
Baker::Baker(int id):id(id)
{
}

//Baker destructor
Baker::~Baker()
{
}

//Bakes appropriate number of donuts and adds them to a box for given order
//When finished box is added to the box vector associated with the order (anOrder.boxes)
void Baker::bake_and_box(ORDER &anOrder) {
	//keeps adding donuts into a box until appropriate amount are added
	while(anOrder.number_donuts != 0){
		Box boo;
		while (boo.size() != 12 && anOrder.number_donuts != 0){
			DONUT doo;
			boo.addDonut(doo);
			anOrder.number_donuts -= 1;
		}
		//add box to boxes vector
		anOrder.boxes.push_back(boo);
	}
	for(Box b: anOrder.boxes){
		anOrder.number_donuts += b.size();
	}
}

//Iterates through each order, creates and fills boxes of donuts using bake_and_box function,
// then places orders into the order_outvector once they are complete.
//If waiter is finished (indicated by b_WaiterIsFinished) then the rest of the orders in order_in_Q
// are processed and then stops
void Baker::beBaker() {
	while(true){
		unique_lock<mutex> lck(mutex_order_inQ);
		//waits for orders
		while (order_in_Q.size() == 0 && !b_WaiterIsFinished){
			cv_order_inQ.wait(lck);
		}
		//processes orders
		if(!order_in_Q.empty()){
			ORDER nextOrder;
			nextOrder = order_in_Q.front();
			order_in_Q.pop();
			bake_and_box(nextOrder);
			lock_guard<mutex> lg(mutex_order_outQ);
			order_out_Vector.push_back(nextOrder);
		}
		//No more orders
		else if(b_WaiterIsFinished){
			break;
		}
	}
}
