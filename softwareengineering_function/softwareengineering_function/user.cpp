#pragma once
#include <string>
using namespace std;

//userŬ����
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

	//������� Ȥ�� ������� ����Լ� -> �������̵�����
};
