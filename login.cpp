//log in.cpp

//select userpw
//from aucuser
//where userid=?

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


//logiin 클래스
class LOGIN
{
private:
	//char id[20];
	string id;
	string pw;
	string nick;
	char sqlpw[20];
	char sqlnick[20];
	int sqlauth;
	
	wstring	query = L"select userpw, nick, authority from aucuser where userid = ? ;";

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
		SQLCHAR isqlnick[20];
		SQLINTEGER isqlauth;
		USER* user=NULL;

		//query 문에 bindparameter 하기위해 char배열로,, string으론 하는법을 알지못함..
		strncpy_s(t_id, id.c_str(), sizeof(t_id));
		t_id[sizeof(t_id) - 1] = 0;

		SQLRETURN retcode;

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

		//for debug
		//cout << "\nSQLExecDirect retcode = " << retcode << endl;	
		
		if (retcode == SQL_SUCCESS)
		{
			//for debug
			//cout << "22222222222222222" << endl;

			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, &sqlpw, 20, (SQLINTEGER *)&isqlpw);			//비밀번호
			retcode = SQLBindCol(hstmt, 2, SQL_C_CHAR, &sqlnick, 20, (SQLINTEGER *)&isqlnick);		//닉네임
			retcode = SQLBindCol(hstmt, 3, SQL_C_SLONG, &sqlauth, 0, (SQLINTEGER *)&isqlauth);		//권한

			//for debug
			/*cout << "retcode = " << retcode << endl;
			cout << "33333333" << endl;*/

			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{	
				//for debug
				/*cout << "44444444444" << endl;
				cout << sqlpw << ", " << sqlnick << ", " << sqlauth << endl;*/
				
				//이부분은 고정크기의 배열로 받아온 sqlpw char배열에 빈문자자리에 쓰레기값이 들어간 것을 없애줌
				int i;
				for (i = 0; (int)sqlpw[i] != 32; i++);
					//cout << "sqlpw[" << i << "] = " << sqlpw[i] << endl;;
				for (i; i < 10; i++)
				{
					sqlpw[i] = '\0';
				}

				int j;
				for (j = 0; (int)sqlnick[j] != 32; j++);
				for (j; j < 10; j++)
				{
					sqlnick[j] = '\0';
				}

				//char to string..
				string pw2 = sqlpw;					//sqlpw -> string
				string nick = sqlnick;				//sqlnick -> string

				/*cout << pw.length() << endl;
				cout << pw2.length() << endl;*/

				if (pw.compare(pw2) == 0)
				{
					cout << "로그인 성공!" << endl;

					if (sqlauth != 1)      //권한이 일반유저일 때
					{
						user = new G_USER(id, nick);	//일반 유저 객체 동적 할당
					}
					else      //권한이 관리자일때
					{
						user = new ADMIN(id, nick);		//관리자 유저 객체 동적할당
					}
				}
				else
				{
					cout << "로그인 실패!" << endl;

					//cout << sqlpw << endl;
					//cout << pw << endl;
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

		SQLFreeStmt(hstmt, SQL_DROP);
		SQLDisconnect(hdbc);
		SQLFreeConnect(hdbc);
		SQLFreeEnv(henv);

		return user;		//로그인 된 유저 객체 포인터 반환
	}
};


int main()
{
	string id, pw;

	//gui에서 입력받을부분
	//id 입력 -> gui text 상자
	cout << "id : ";
	getline(cin, id);

	cin.clear();  // 에러 플래그를 지우기 위해 cin에 있는 clear 함수를 사용
	//cin.ignore(INT_MAX, '\n');

	//pw 입력 -> gui text 상자
	cout << "pw : ";
	getline(cin, pw);

	cin.clear();  // 에러 플래그를 지우기 위해 cin에 있는 clear 함수를 사용
	//cin.ignore(INT_MAX, '\n');


	//로그인
	LOGIN l(id, pw);			//로그인객체생성 (생성자 호출)

	USER * user = l.login();	//login 하고 성공시 유저객체를반환

	cout << "main끝!" << endl;

	delete user;		//유저객체 소멸
}
