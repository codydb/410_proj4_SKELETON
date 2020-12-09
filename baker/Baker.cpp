#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"

using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {
	Box aBox;
	int numDonutInBox = 0;
	for (int i = 0; i < anOrder.number_donuts; i++) {
		numDonutInBox++;
		DONUT donut;
		aBox.addDonut(donut);

		// > 12 donuts, push a box into vector, then clear.
		if (numDonutInBox % DOZEN == 0) {
			anOrder.boxes.push_back(aBox);
			aBox.clear();
		}
	}

	//else use 1 box.
	if(aBox.size() != 0){
		anOrder.boxes.push_back(aBox);
		aBox.clear();
	}
}

void Baker::beBaker() {
	std::unique_lock<mutex> lck(mutex_order_inQ);
	while(!b_WaiterIsFinished && order_in_Q.empty()) {
		cv_order_inQ.wait(lck);
	}

	while(true) {
		if(!order_in_Q.empty()) {
			ORDER order = order_in_Q.front();
			{
				lock_guard<mutex> lck(mutex_order_outQ);
				order_in_Q.pop();
			}
			bake_and_box(order);
			order_out_Vector.push_back(order);
		}

		if (b_WaiterIsFinished && order_in_Q.empty()) {
			break;
		}
	}
}
