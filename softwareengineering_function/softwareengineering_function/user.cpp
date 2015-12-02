#pragma once
#include <string>
using namespace std;

//user클래스
class USER
{
protected:
	string user_id;

public:
	USER(){}
	USER(string uid)
	{
		user_id = uid;
	}
	~USER(){}

	//입찰목록 혹은 낙찰목록 출력함수 -> 오버라이딩가능
};
