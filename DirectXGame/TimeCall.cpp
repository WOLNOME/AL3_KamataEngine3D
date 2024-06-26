#include "TimeCall.h"

TimeCall::TimeCall(std::function<void(void)> callBack, uint32_t time) { 
	//初期化
	FireReset_ = callBack; 
	time_ = time;
}

void TimeCall::Update() {

	if (isFinish_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinish_ = true;
		//コールバック関数呼び出し
		FireReset_();
	}

}
