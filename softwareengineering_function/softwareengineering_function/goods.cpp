#pragma once
#include <string>

using namespace std;

//��ǰ Ŭ����
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

	GOODS(int gid, string uid, string gname, int start, int right/*, �ð�����*/)		//������
	{
		g_ID = gid;
		u_ID = uid;
		g_name = gname;
		start_price = start;
		right_price = right;
		p_price = start;
		bid_user = "null";
		//�ð�
	}

	int bid(int new_bid_price, string b_user)	//������������
	{
		p_price = new_bid_price;
		bid_user = b_user;
	}

	int suc_bid()	//���� ����
	{

	}

	int fail_bid()	//�ŷ� ����
	{

	}
};
