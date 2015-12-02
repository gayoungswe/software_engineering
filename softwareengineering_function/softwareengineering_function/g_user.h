#pragma once
#ifndef __G_USER_H__
#define __G_USER_H__

#include "user.h"
#include "bidding.h"
#include "suc_bidding.h"

class G_USER : public USER
{
private:
	BIDDING bidding_list;	//ÀÔÂû°´Ã¼¹è¿­
	SUC_BIDDING suc_list;	//³«Âû°´Ã¼¹è¿­

public:
	G_USER();
	G_USER(string uid);
	~G_USER();
};

#endif