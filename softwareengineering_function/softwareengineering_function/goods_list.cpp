#pragma once
#include <string>

#include "goods.h"

using namespace std;


//��ǰ ��� Ŭ����
class GOODS_LIST
{
public:
	int goods_num;		//��ϵ� ��ǰ ����
	GOODS* list;			//��ǰ��ü�迭

	int new_fresh_list()	//��ǰ ��� ���
	{

	}

	int add_goods()		//��ǰ ���
	{
		string p_name;
		string p_info;
		//����
		int p_start;
		int p_instant;
		int p_present;
		CString end_time;	//��Ͻð� + 24
		int left_minute;
	}

	int del_goods()		//��ǰ ����
	{

	}
};