//software engineering term project
//8_Auction

#include <iostream>
#include <string>

#include <Windows.h>
//SQL ODBC Conn
#include <sql.h>
#include <sqlext.h>

using namespace std;

SQLHENV		henv;
SQLHDBC		hdbc;
SQLHSTMT	hstmt;

//��ǰ Ŭ����
class goods
{
public:
	int g_ID;			//��ǰ ��ȣ	
	string u_ID;		//����� ��ȣ
	string g_name;		//��ǰ �̸�
	int start_price;	//���� ������
	int right_price;	//��� ���Ű�
	int p_price;		//���� ������
	string bid_user;		//���� �������� ������ ���� ��ȣ
	//�����ð�ó���������

	goods(int gid, string uid, string gname, int start, int right/*, �ð�����*/)		//������
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

//��ǰ ��� Ŭ����
class goods_list
{
public:
	int goods_num;		//��ϵ� ��ǰ ����
	goods list;			//��ǰ��ü�迭

	int print_list()	//��ǰ ��� ���
	{

	}

	int add_goods()		//��ǰ ���
	{

	}

	int del_goods()		//��ǰ ����
	{

	}
};

//���� Ŭ����
class bidding
{
public:
	string whois;
	int howmuch;

	int add_bidding()
	{

	}
};

//���� Ŭ����
class suc_bidding
{
public:
	int whatis;
	int howmuch;

	int add_suc()
	{

	}
};

//userŬ����
class user
{
private:
	string nick_name;
	string user_id;

public:
	//������� Ȥ�� ������� ����Լ� -> �������̵�����
};

//�Ϲ�����Ŭ����
class g_user : user
{
private:
	bidding bidding_list;	//������ü�迭
	suc_bidding suc_list;	//������ü�迭

	/*u_bidding()
	u_add_good()
	u_del_goods()
	show_bidding()
	show_suc()*/
};

//����������Ŭ����
class admin : user
{
	/*goods_delete()
	user_delete()
	user_search()
	noteice_reg()*/
};

//ȸ������
class join
{
private:
	string id;
	string nickname;
	string pwd;

public:
	int user_reg()
	{

	}
};

//�α���
class login
{
private:
	string id;
	string pwd;

public:
	user log_in()
	{

	}
};

int main()
{


	return 0;
}