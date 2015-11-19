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

//물품 클래스
class goods
{
public:
	int g_ID;			//물품 번호	
	string u_ID;		//등록자 번호
	string g_name;		//물품 이름
	int start_price;	//시작 입찰가
	int right_price;	//즉시 구매가
	int p_price;		//현재 입찰가
	string bid_user;		//현재 입찰가로 입찰한 유저 번호
	//남은시간처리어떻게하지

	goods(int gid, string uid, string gname, int start, int right/*, 시간변수*/)		//생성자
	{
		g_ID = gid;
		u_ID = uid;
		g_name = gname;
		start_price = start;
		right_price = right;
		p_price = start;
		bid_user = "null";
		//시간
	}
	
	int bid(int new_bid_price, string b_user)	//입찰정보갱신
	{
		p_price = new_bid_price;
		bid_user = b_user;
	}

	int suc_bid()	//입찰 성공
	{

	}

	int fail_bid()	//거래 실패
	{

	}
};

//물품 목록 클래스
class goods_list
{
public:
	int goods_num;		//등록된 물품 개수
	goods list;			//물품객체배열

	int print_list()	//물품 목록 출력
	{

	}

	int add_goods()		//물품 등록
	{

	}

	int del_goods()		//물품 삭제
	{

	}
};

//입찰 클래스
class bidding
{
public:
	string whois;
	int howmuch;

	int add_bidding()
	{

	}
};

//낙찰 클래스
class suc_bidding
{
public:
	int whatis;
	int howmuch;

	int add_suc()
	{

	}
};

//user클래스
class user
{
private:
	string nick_name;
	string user_id;

public:
	//입찰목록 혹은 낙찰목록 출력함수 -> 오버라이딩가능
};

//일반유저클래스
class g_user : user
{
private:
	bidding bidding_list;	//입찰객체배열
	suc_bidding suc_list;	//낙찰객체배열

	/*u_bidding()
	u_add_good()
	u_del_goods()
	show_bidding()
	show_suc()*/
};

//관리자유저클래스
class admin : user
{
	/*goods_delete()
	user_delete()
	user_search()
	noteice_reg()*/
};

//회원가입
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

//로그인
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