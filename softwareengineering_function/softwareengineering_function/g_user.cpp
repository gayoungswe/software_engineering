#pragma once
#include "bidding.h"
#include "suc_bidding.h"
#include "user.h"

//�Ϲ�����Ŭ����
class G_USER : public USER
{
private:
	BIDDING bidding_list;	//������ü�迭
	SUC_BIDDING suc_list;	//������ü�迭

public:
	G_USER(){}
	G_USER(string uid)
	{
		user_id = uid;
	}
	~G_USER(){}

	/*u_bidding()
	u_add_good()
	u_del_goods()
	show_bidding()
	show_suc()*/
};