#pragma once
#include <string>

#include "goods.h"

using namespace std;


//물품 목록 클래스
class GOODS_LIST
{
public:
	int goods_num;		//등록된 물품 개수
	GOODS* list;			//물품객체배열

	int new_fresh_list()	//물품 목록 출력
	{

	}

	int add_goods()		//물품 등록
	{
		string p_name;
		string p_info;
		//사진
		int p_start;
		int p_instant;
		int p_present;
		CString end_time;	//등록시간 + 24
		int left_minute;
	}

	int del_goods()		//물품 삭제
	{

	}
};