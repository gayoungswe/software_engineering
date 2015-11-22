//join.cpp

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


//user클래스
class USER
{
protected:
	string user_id;
	string nick_name;

public:
	USER(){}
	USER(string uid, string nick)
	{
		user_id = uid;
		nick_name = nick;
	}
	~USER(){}
};

//일반유저클래스
class G_USER : public USER
{
public:
	G_USER(){}
	G_USER(string uid, string nick)
	{
		user_id = uid;
		nick_name = nick;
	}
	~G_USER(){}
};

//관리자유저클래스
class ADMIN : public USER
{
public:
	ADMIN(){}
	ADMIN(string uid, string nick)
	{
		user_id = uid;
		nick_name = nick;
	}
	~ADMIN(){}
};


//join 클래스
class JOIN
{
private:
	/*string id;
	string nickname;
	string pwd;*/

	/*wstring	query_check_id = L"select userid from aucuser where userid = ? ;";
	wstring query_join = L"insert into aucuser values(?, ?,'nickname' , 0);";*/

public:
	JOIN(){}
	~JOIN(){}

	int id_check(string id)	//id 중복체크
	{
		wstring	query_check_id = L"select userid from aucuser where userid = ? ;";

		SQLINTEGER len_id = SQL_NTS;
		char t_id[20];
		char sqlid[20];
		SQLINTEGER isqlid;

		strncpy_s(t_id, id.c_str(), sizeof(t_id));
		t_id[sizeof(t_id) - 1] = 0;

		retcode = SQLBindParameter
			(
			hstmt,
			1,
			SQL_PARAM_INPUT,
			SQL_C_CHAR,
			SQL_CHAR,
			20,//sizeof(t_id),
			0,
			&t_id,
			20,//sizeof(t_id),
			&len_id
			);

		//for debug
		cout << sizeof(t_id) << endl;

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query_check_id.c_str(), SQL_NTS);

		if (retcode == SQL_SUCCESS)
		{
			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, &sqlid, 20, (SQLINTEGER *)&isqlid);			//비밀번호
			
			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{
				// 입력한 아이디가 이미 존재

				//팝업창 띄워 "이미 사용중인 아이디입니다." 메시지 출력
				cout << "이미 사용중인 아이디 입니다. \n다른 아이디를 입력하세요.\n" << endl;
				return 0;
			}
			else
			{
				//입력한 아이디 중복 없음->사용 가능

				//팝업창 띄워 "사용 가능한 아이디입니다." 메시지 출력
				cout << "사용 가능한 아이디입니다.\n	" << endl;
				return 1;
			}
		}
	}

	int pw_equal_check(string pw1, string pw2)	//pw1, pw2 중복체크
	{
		if (pw1.compare(pw2) == 0)	//두 비밀번호 일치
		{
			return 1;
		}
		else   //두 비밀번호 불일치
		{
			cout << "비밀번호가 일치하지 않습니다." << endl;
			return 0;
		}
	}

	int join(string id, string pw)	//회원가입
	{
		wstring query_join = L"insert into aucuser values(?, ?,'nickname' , 0);";
		//wstring query_join = L"insert into aucuser values('abcd', '1234', 'nickname' , 0);";

		//for debug
		cout << "입력한 id = " << id << endl;
		cout << "입력한 pw = " << pw << endl;

		char *t_id = new char[id.length()+1];
		char *t_pw = new char[pw.length()+1];
		SQLINTEGER len_id = SQL_NTS;
		SQLINTEGER len_pw = SQL_NTS;
		
		//strncpy_s(t_id, id.c_str(), id.length() + 1);
		//strncpy_s(t_id, const_cast<char *>(id.c_str()), sizeof(t_id));
		t_id = const_cast<char *>(id.c_str());
		t_id[id.length()] = '\0';

		t_pw = const_cast<char *>(pw.c_str());
		t_pw[pw.length()] = '\0';

		//for debug
		cout << "id.length() = " << id.length() << endl;
		cout << "pw.length() = " << pw.length() << endl;
		cout << t_id << endl;
		cout << t_pw << endl;

		//copy(id.begin(), id.end(), t_id);
		//t_id[id.length()] = '\0'; // don't forget the terminating 0

		//copy(pw.begin(), pw.end(), t_pw);
		//t_id[pw.length()] = '\0'; // don't forget the terminating 0

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

		//for debug
		cout << "sqlbindpara 1 retcode = " << retcode << endl;

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

		//for debug
		cout << "sqlbindpara 2 retcode = " << retcode << endl;

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


int main(void)
{
	int id_check = 0;		//아이디 중복체크 (확인후 1)
	int pw_check = 0;		//두 비밀번호가 같은지 확인 (확인 후1)
	string id, pw1, pw2, nickname;

	JOIN j;
	AUC_DB db;

	db.auc_db_conn();

	///////////////////////////// id ///////////////////////////
	while (id_check == 0)
	{
		//gui에서 입력받을부분
		//id 입력 -> gui text 상자
		cout << "id : ";
		getline(cin, id);
		cin.clear();  // 에러 플래그를 지우기 위해 cin에 있는 clear 함수를 사용

		cout << "입력한 id = " << id << endl;

		//id중복체크 버튼을 누를 때
		id_check = j.id_check(id);
	}

	//for debug
	cout << "id_check = " << id_check << endl;

	////////////////////////////// pw /////////////////////////////
	while (pw_check == 0)
	{
		//pw1 입력 -> gui text 상자
		cout << "pw1 : ";
		getline(cin, pw1);
		cin.clear();

		cout << "입력한 pw = " << pw1 << endl;

		//pw2 입력 -> gui text 상자
		cout << "pw2 : ";
		getline(cin, pw2);
		cin.clear();  // 에러 플래그를 지우기 위해 cin에 있는 clear 함수를 사용

		//pw1과 pw2가 같은지 확인
		pw_check = j.pw_equal_check(pw1, pw2);
	}

	///////////////// join //////////////
	j.join(id, pw1);

	db.auc_db_conn_free();

	return 0;

}

////////////바로중복없는거넣으면되고
/////////첫번째중복판정 -> 그다음 중복x 안됨