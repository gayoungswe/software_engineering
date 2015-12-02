#pragma once
#ifndef __GOODS_LIST_H__
#define __GOODS_LIST_H__

#include "goods.h"

class GOODS_LIST
{
public:
	int goods_num;		//등록된 물품 개수
	GOODS* list;			//물품객체배열
	int new_fresh_list();	//물품 목록 출력
	int add_goods();		//물품 등록
	int del_goods();	//물품 삭제
};

#endif