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

public:
	USER(){}
	USER(string uid)
	{
		user_id = uid;
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
	}
	~ADMIN(){}
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

		//for debug
		//cout << sizeof(t_id) << endl;

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query_check_id.c_str(), SQL_NTS);
		
		//for debug
		//cout << "SQLExecDirect retcode = " << retcode << endl;

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
				
				//for debug
				//cout << " 여기오긴하는거냐.." << endl;
				
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
		//for debug
		/*cout << "입력한 id = " << id << endl;
		cout << "입력한 pw = " << pw << endl;*/

		char *t_id = new char[id.length()+1];
		char *t_pw = new char[pw.length()+1];
		SQLINTEGER len_id = SQL_NTS;
		SQLINTEGER len_pw = SQL_NTS;
		
		t_id = const_cast<char *>(id.c_str());
		t_id[id.length()] = '\0';

		t_pw = const_cast<char *>(pw.c_str());
		t_pw[pw.length()] = '\0';

		//for debug
		/*cout << "id.length() = " << id.length() << endl;
		cout << "pw.length() = " << pw.length() << endl;
		cout << t_id << endl;
		cout << t_pw << endl;*/

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
		//cout << "sqlbindpara 1 retcode = " << retcode << endl;

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
		//cout << "sqlbindpara 2 retcode = " << retcode << endl;

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query_join.c_str(), SQL_NTS);
		
		if (retcode == SQL_SUCCESS)
		{
			cout << "insert 성공!" << endl;

			SQLFreeStmt(hstmt, SQL_CLOSE);	//

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
	bool id_check = false;		//아이디 중복체크 (확인후 1)
	bool pw_check = false;		//두 비밀번호가 같은지 확인 (확인 후1)
	string id, pw1, pw2, nickname;

	JOIN j;
	AUC_DB db;

	db.auc_db_conn();

	///////////////////////////// id ///////////////////////////
	while (!id_check)
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
	//cout << "id_check = " << id_check << endl;

	////////////////////////////// pw /////////////////////////////
	while (!pw_check)
	{
		//pw1 입력 -> gui text 상자
		cout << "pw1 : ";
		getline(cin, pw1);
		cin.clear();

		//for debug
		//cout << "입력한 pw = " << pw1 << endl;

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