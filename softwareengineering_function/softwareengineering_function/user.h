#pragma once
#ifndef __USER_H__
#define __USER_H__

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>	//CString ������ ����

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

	//������� Ȥ�� ������� ����Լ� -> �������̵�����
};

#endif