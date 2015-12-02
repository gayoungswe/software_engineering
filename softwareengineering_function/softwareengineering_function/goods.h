#pragma once
#ifndef __GOODS_H__
#define __GOODS_H__

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>	//CString 쓰려면 포함

using namespace std;

class GOODS
{
public:
	int g_ID;			//물품 번호	
	string u_ID;		//등록자 번호
	string g_name;		//물품 이름
	int start_price;	//시작 입찰가
	int right_price;	//즉시 구매가
	int p_price;		//현재 입찰가
	string bid_user;		//현재 입찰가로 입찰한 유저 번호
	GOODS *next;
	//남은시간처리어떻게하지

	GOODS(int gid, string uid, string gname, int start, int right/*, 시간변수*/);		//생성자
	int bid(int new_bid_price, string b_user);	//입찰정보갱신
	int suc_bid();	//입찰 성공
	int fail_bid();	//거래 실패
};

#endif