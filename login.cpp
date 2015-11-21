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
	char user_id[20];
	char nick_name[20];
public:
	USER(){}
	USER(char uid[20], char nick[20])
	{
		strcpy_s(user_id, sizeof(user_id), uid);
		strcpy_s(nick_name, sizeof(nick_name), nick);
	}
	~USER(){}
};

//일반유저클래스
class G_USER : public USER
{
public:
	G_USER(){}
	G_USER(char uid[20], char nick[20])
	{
		strcpy_s(user_id, sizeof(user_id), uid);
		strcpy_s(nick_name, sizeof(nick_name), nick);
	}
	~G_USER(){}
};

//관리자유저클래스
class ADMIN : public USER
{
public:
	ADMIN(){}
	ADMIN(char uid[20], char nick[20])
	{
		strcpy_s(user_id, sizeof(user_id), uid);
		strcpy_s(nick_name, sizeof(nick_name), nick);
	}
	~ADMIN(){}
};


//logiin 클래스
class LOGIN
{
private:
	char id[20];
	string pw;
	string nick;
	char sqlpw[20];
	char sqlnick[20];
	int sqlauth;
	
	wstring	query = L"select userpw, nick, authority from aucuser where userid = ? ;";

public:
	LOGIN(){}
	~LOGIN(){}
	USER* login()
	{
		SQLINTEGER len_id = SQL_NTS;
		SQLCHAR isqlpw[20];
		SQLCHAR isqlnick[20];
		SQLINTEGER isqlauth;
		USER* user=NULL;

		//id 입력 -> gui text 상자
		cout << "id : ";
		cin >> id;

		cin.clear();  // 에러 플래그를 지우기 위해 cin에 있는 clear 함수를 사용
		cin.ignore(INT_MAX, '\n');

		//cout << id << endl;

		//pw 입력 -> gui text 상자
		cout << "pw : ";
		getline(cin, pw	);

		cin.clear();  // 에러 플래그를 지우기 위해 cin에 있는 clear 함수를 사용
		//cin.ignore(INT_MAX, '\n');

		//cout << pw << endl;

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
			20,	
			0,	
			id,		
			20,		
			&len_id 
			);
		
		//for debug
		//cout << retcode << endl;

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query.c_str(), SQL_NTS);

		//for debug
		//cout << "\n1111111111111111" << endl;	
		
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

				string pw2 = sqlpw;					//sqlpw -> string
				string nick = sqlnick;				//sqlnick -> string

				/*cout << pw.length() << endl;
				cout << pw2.length() << endl;*/

				if (pw.compare(pw2) == 0)
				{
					cout << "로그인 성공!" << endl;

					if (sqlauth != 1)      //권한이 일반유저일 때
					{
						user = new G_USER(id, sqlnick);	//일반 유저 객체 동적 할당
					}
					else      //권한이 관리자일때
					{
						user = new ADMIN(id, sqlnick);		//관리자 유저 객체 동적할당
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
	LOGIN l;

	USER * user=l.login();

	cout << "main끝!" << endl;
}
