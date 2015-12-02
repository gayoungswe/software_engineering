#pragma once
#ifndef __USER_H__
#define __USER_H__

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>	//CString 쓰려면 포함

#include <ATLComTime.h>

using namespace std;

class USER
{
protected:
	string user_id;

public:
	USER();
	USER(string uid);
	~USER();

	//입찰목록 혹은 낙찰목록 출력함수 -> 오버라이딩가능
};

#endif