#pragma once
#ifndef __GOODS_LIST_H__
#define __GOODS_LIST_H__

#include "goods.h"

class GOODS_LIST
{
public:
	int goods_num;		//��ϵ� ��ǰ ����
	GOODS* list;			//��ǰ��ü�迭
	int new_fresh_list();	//��ǰ ��� ���
	int add_goods();		//��ǰ ���
	int del_goods();	//��ǰ ����
};

#endif