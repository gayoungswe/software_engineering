#pragma once
#ifndef __GOODS_H__
#define __GOODS_H__

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>	//CString ������ ����

using namespace std;

class GOODS
{
public:
	int g_ID;			//��ǰ ��ȣ	
	string u_ID;		//����� ��ȣ
	string g_name;		//��ǰ �̸�
	int start_price;	//���� ������
	int right_price;	//��� ���Ű�
	int p_price;		//���� ������
	string bid_user;		//���� �������� ������ ���� ��ȣ
	GOODS *next;
	//�����ð�ó���������

	GOODS(int gid, string uid, string gname, int start, int right/*, �ð�����*/);		//������
	int bid(int new_bid_price, string b_user);	//������������
	int suc_bid();	//���� ����
	int fail_bid();	//�ŷ� ����
};

#endif