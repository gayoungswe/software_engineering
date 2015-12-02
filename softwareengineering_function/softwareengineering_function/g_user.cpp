#pragma once
#include "bidding.h"
#include "suc_bidding.h"
#include "user.h"

//일반유저클래스
class G_USER : public USER
{
private:
	BIDDING bidding_list;	//입찰객체배열
	SUC_BIDDING suc_list;	//낙찰객체배열

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