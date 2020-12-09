#include <string>
#include "stdlib.h"

#include "../includes/externs.h"
#include "../includes/waiter.h"
#include "../includes/PRINT.h"
using namespace std;

mutex waiterMutex;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	//lock guard so no two waiters grab the same order
	//lock_guard<mutex> lock(waiterMutex);
	return myIO.getNext(anOrder);;
}

void Waiter::beWaiter() {

	int next = SUCCESS;
	//gets new orders until getNext runs out, then signals b_WaiterIsFinished
	while (next == SUCCESS){
		ORDER temp;
		next = getNext(temp);
		if (next == SUCCESS){
			lock_guard<mutex> lck(mutex_order_outQ);
			PRINT2("Bewaiter pushing order: ", temp.order_number);
			order_in_Q.push(temp);
		}else {
			b_WaiterIsFinished = true;
		}
		//signals Bakers that a new order is in queue or waiter is done
		PRINT1("notify baker");
		cv_order_inQ.notify_all();

	}

}

