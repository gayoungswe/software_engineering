#pragma once
#include <string>
#include "user.h"

using namespace std;

//����������Ŭ����
class ADMIN : public USER
{
public:
	ADMIN(){}
	ADMIN(string uid)
	{
		user_id = uid;
	}
	~ADMIN(){}
	/*goods_delete()
	user_delete()
	user_search()
	noteice_reg()*/
};