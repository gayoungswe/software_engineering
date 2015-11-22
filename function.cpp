//software engineering term project
//8_Auction

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
//SQL ODBC Conn
#include <sql.h>
#include <sqlext.h>

using namespace std;

SQLHENV		henv;
SQLHDBC		hdbc;
SQLHSTMT	hstmt;

SQLRETURN retcode;

//DB 연결, 해제 클래스
class AUC_DB
{
public:
	int auc_db_conn()
	{
		if ((retcode = SQLAllocEnv(&henv)) != SQL_SUCCESS)
		{
			cout << "SQLAllocEnv: error code = " << retcode << endl;
			return 0;
		}
		if ((retcode = SQLAllocConnect(henv, &hdbc)) != SQL_SUCCESS)
		{
			cout << "SQLAllocConnect: error code = " << retcode << endl;
			return 0;
		}
		if ((retcode = SQLConnect(hdbc, (SQLWCHAR *)TEXT("ODBC_se8"), SQL_NTS, (SQLWCHAR *)TEXT("c##se8"), SQL_NTS, (SQLWCHAR *)TEXT("software"), SQL_NTS)) != SQL_SUCCESS)
		{
			cout << "SQLConnect: error code = " << retcode << endl;
			return 0;
		}

		SQLAllocStmt(hdbc, &hstmt);
		return 1;
	}

	int auc_db_conn_free()
	{
		SQLFreeStmt(hstmt, SQL_DROP);
		SQLDisconnect(hdbc);
		SQLFreeConnect(hdbc);
		SQLFreeEnv(henv);

		return 0;
	}
};

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
class USER
{
protected:
	string user_id;

public:
	USER(){}
	USER(string uid)
	{
		user_id = uid;
	}
	~USER(){}

	//입찰목록 혹은 낙찰목록 출력함수 -> 오버라이딩가능
};

//일반유저클래스
class G_USER : public USER
{
private:
	bidding bidding_list;	//입찰객체배열
	suc_bidding suc_list;	//낙찰객체배열

public:
	G_USER(){}
	G_USER(string uid)
	{
		user_id = uid;
	}
	~G_USER(){}

	/*u_bidding()
	u_add_good()
	u_del_goods()
	show_bidding()
	show_suc()*/
};

//관리자유저클래스
class ADMIN : public USER
{
public:
	ADMIN(){}
	ADMIN(string uid)
	{
		user_id = uid;
	}
	~ADMIN(){}
	/*goods_delete()
	user_delete()
	user_search()
	noteice_reg()*/
};

//join 클래스
class JOIN
{
private:
	wstring	query_check_id = L"select userid from aucuser where userid = ? ;";
	wstring query_join = L"insert into aucuser values(?, ?, 0);";

public:
	JOIN(){}
	~JOIN(){}

	bool id_check(string id)	//id 중복체크
	{
		SQLINTEGER len_id = SQL_NTS;
		SQLINTEGER isqlid;

		char *t_id = new char[id.length() + 1];
		char *sqlid = new char[id.length() + 1];

		t_id = const_cast<char *>(id.c_str());
		t_id[id.length()] = '\0';

		retcode = SQLBindParameter
			(
			hstmt,
			1,
			SQL_PARAM_INPUT,
			SQL_C_CHAR,
			SQL_CHAR,
			id.length() + 1,
			0,
			t_id,
			id.length() + 1,
			&len_id
			);

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query_check_id.c_str(), SQL_NTS);

		if (retcode == SQL_SUCCESS)
		{
			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, sqlid, 20, (SQLINTEGER *)&isqlid);			//비밀번호

			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{
				// 입력한 아이디가 이미 존재

				//팝업창 띄워 "이미 사용중인 아이디입니다." 메시지 출력
				cout << "이미 사용중인 아이디 입니다. \n다른 아이디를 입력하세요.\n" << endl;
				SQLFreeStmt(hstmt, SQL_CLOSE);		//////!!중요 이게없으면 첫번째 중복이고 두번째 사용가능때 안됨
				return false;
			}
			else if (strncmp(t_id, sqlid, id.length() + 1))
			{
				//sqlid 와 id 비교
				//대문자 소문자 상관없이 같은 문자여도 중복
					
				cout << "이미 사용중인 아이디 입니다. \n다른 아이디를 입력하세요.\n" << endl;
				SQLFreeStmt(hstmt, SQL_CLOSE);		//////!!중요 이게없으면 첫번째 중복이고 두번째 사용가능때 안됨
				
				return false;
			}
			else
			{
				//입력한 아이디 중복 없음->사용 가능

				//팝업창 띄워 "사용 가능한 아이디입니다." 메시지 출력
				cout << "사용 가능한 아이디입니다.\n	" << endl;

				SQLFreeStmt(hstmt, SQL_CLOSE);
				return true;
			}
		}
		else
		{
			//for debug
			//cout << "대체 왜 여기오는것?" << endl;

			return false;
		}
	}

	bool pw_equal_check(string pw1, string pw2)	//pw1, pw2 중복체크
	{
		if (pw1.compare(pw2) == 0)	//두 비밀번호 일치
		{
			return true;
		}
		else   //두 비밀번호 불일치
		{
			cout << "비밀번호가 일치하지 않습니다." << endl;
			return false;
		}
	}

	int join(string id, string pw)	//회원가입
	{
		char *t_id = new char[id.length() + 1];
		char *t_pw = new char[pw.length() + 1];
		SQLINTEGER len_id = SQL_NTS;
		SQLINTEGER len_pw = SQL_NTS;

		t_id = const_cast<char *>(id.c_str());
		t_id[id.length()] = '\0';

		t_pw = const_cast<char *>(pw.c_str());
		t_pw[pw.length()] = '\0';

		retcode = SQLBindParameter
			(
			hstmt,
			1,
			SQL_PARAM_INPUT,
			SQL_C_CHAR,
			SQL_CHAR,
			id.length() + 1,
			0,
			t_id,		//////이게문제였!! &붙이지말것 이자체로도 주소!!!!!!!!!!!1
			id.length() + 1,
			&len_id
			);

		retcode = SQLBindParameter
			(
			hstmt,
			2,
			SQL_PARAM_INPUT,
			SQL_C_CHAR,
			SQL_CHAR,
			pw.length() + 1,
			0,
			t_pw,		//////이게문제였!! &붙이지말것 이자체로도 주소!!!!!!!!!!!!!!!!!!!1
			pw.length() + 1,
			&len_pw
			);

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query_join.c_str(), SQL_NTS);

		if (retcode == SQL_SUCCESS)
		{
			cout << "insert 성공!" << endl;
			retcode = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);		//commit;
			if (!SQL_SUCCEEDED(retcode)) {
				printf("Error committing transaction.\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "insert 실패!" << endl;
		}

		return 0;
	}
};

//logiin 클래스
class LOGIN
{
private:
	string id;
	string pw;
	char sqlpw[20];
	int sqlauth;

	wstring	query = L"select userpw, authority from aucuser where userid = ? ;";

public:
	LOGIN(){}

	LOGIN(string id, string pw)
	{
		this->id = id;
		this->pw = pw;
	}

	~LOGIN(){}

	USER* login()
	{
		char t_id[20];
		SQLINTEGER len_id = SQL_NTS;
		SQLCHAR isqlpw[20];
		SQLINTEGER isqlauth;
		USER* user = NULL;

		//query 문에 bindparameter 하기위해 char배열로,, string으론 하는법을 알지못함..
		strncpy_s(t_id, id.c_str(), sizeof(t_id));
		t_id[sizeof(t_id) - 1] = 0;

		AUC_DB db;

		db.auc_db_conn();		//db 연결


		retcode = SQLBindParameter
			(
			hstmt,
			1,
			SQL_PARAM_INPUT,
			SQL_C_CHAR,
			SQL_CHAR,
			sizeof(t_id),
			0,
			&t_id,
			sizeof(t_id),
			&len_id
			);

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query.c_str(), SQL_NTS);

		if (retcode == SQL_SUCCESS)
		{
			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, &sqlpw, 20, (SQLINTEGER *)&isqlpw);			//비밀번호
			retcode = SQLBindCol(hstmt, 3, SQL_C_SLONG, &sqlauth, 0, (SQLINTEGER *)&isqlauth);		//권한

			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{
				//이부분은 고정크기의 배열로 받아온 sqlpw char배열에 빈문자자리에 쓰레기값이 들어간 것을 없애줌
				int i;
				for (i = 0; (int)sqlpw[i] != 32; i++);

				for (i; i < 10; i++)
				{
					sqlpw[i] = '\0';
				}

				//char to string..
				string pw2 = sqlpw;					//sqlpw -> string

				if (pw.compare(pw2) == 0)
				{
					cout << "로그인 성공!" << endl;

					if (sqlauth != 1)      //권한이 일반유저일 때
					{
						user = new G_USER(id);	//일반 유저 객체 동적 할당
					}
					else      //권한이 관리자일때
					{
						user = new ADMIN(id);		//관리자 유저 객체 동적할당
					}
				}
				else
				{
					cout << "로그인 실패!" << endl;
				}
			}
			else          //입력한 아이디가 존재하지 않음
			{
				cout << "존재하지 않는 ID입니다" << endl;
				cout << "SQLFetch = " << SQLFetch(hstmt) << endl;
				cout << "SQL_SUCCESS = " << SQL_SUCCESS << endl;
			}
		}
		else
		{
			cout << "Something wrong in SQLExecDirect. Error code = " << retcode << endl;
		}

		db.auc_db_conn_free();

		return user;		//로그인 된 유저 객체 포인터 반환
	}
};


int main()
{


	return 0;
}