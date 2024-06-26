#pragma once
#include<functional>

class TimeCall {
public:
	TimeCall(std::function<void(void)> callBack, uint32_t time);

	void Update();

	bool IsFinished() { return isFinish_; }

private:
	//コールバック
	std::function<void(void)> FireReset_;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool isFinish_ = false;


};
