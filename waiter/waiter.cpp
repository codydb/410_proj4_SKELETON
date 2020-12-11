#include <string>
#include "stdlib.h"

#include "../includes/externs.h"
#include "../includes/waiter.h"
#include "../includes/PRINT.h"
using namespace std;


Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	return myIO.getNext(anOrder);;
}

void Waiter::beWaiter() {

	int next = SUCCESS;
	//gets new orders until getNext runs out, then signals b_WaiterIsFinished
	while (next == SUCCESS){
		ORDER temp;
		next = getNext(temp);
		if (next == SUCCESS){
			std::unique_lock<mutex> lck(mutex_order_inQ);
			order_in_Q.push(temp);
			cv_order_inQ.notify_all();
		} else {
			std::unique_lock<mutex> lck(mutex_order_inQ);
			b_WaiterIsFinished = true;
			cv_order_inQ.notify_all();
		}
	}
}

