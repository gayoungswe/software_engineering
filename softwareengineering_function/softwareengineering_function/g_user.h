#pragma once
#ifndef __G_USER_H__
#define __G_USER_H__

#include "user.h"
#include "bidding.h"
#include "suc_bidding.h"

class G_USER : public USER
{
private:
	BIDDING bidding_list;	//������ü�迭
	SUC_BIDDING suc_list;	//������ü�迭

public:
	G_USER();
	G_USER(string uid);
	~G_USER();
};

#endif