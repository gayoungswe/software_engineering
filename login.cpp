//log in

//select userpw
//from aucuser
//where userid=?

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

class LOGIN
{
private:
	char id[6];
	char pw[8];
	char sqlpw[8];
	wstring	query = L"select userpw from aucuser where userid = ? ;";

public:
	int login()
	{
		SQLINTEGER len_id = SQL_NTS;
		SQLCHAR isqlpw[20];

		//id �Է� -> gui text ����
		cout << "id : ";
		cin >> id;
		cin.clear();  // ���� �÷��׸� ����� ���� cin�� �ִ� clear �Լ��� ���
		cin.ignore(INT_MAX, '\n');

		//cout << id << endl;

		//pw �Է� -> gui text ����
		cout << "pw : ";
		cin >> pw;
		cin.clear();  // ���� �÷��׸� ����� ���� cin�� �ִ� clear �Լ��� ���
		cin.ignore(INT_MAX, '\n');

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
			6,	
			0,	
			id,		
			6,		
			&len_id 
			);

		cout << retcode << endl;

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query.c_str(), SQL_NTS);

		//for debug
		//cout << "\n1111111111111111" << endl;	
		
		if (retcode == SQL_SUCCESS)
		{
			//for debug
			//cout << "22222222222222222" << endl;

			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, &sqlpw, 6, (SQLINTEGER *)&isqlpw);		//��й�ȣ

			cout << retcode << endl;

			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{
				//for debug
				//cout << "333333333" << endl;

				cout << sqlpw << endl;
				cout << pw << endl;
				
				if(strcmp(pw, sqlpw) == 0)
				{
					cout << "�α��� ����!" << endl;

				}
				else
				{
					cout << "�α��� ����!" << endl;

					//cout << sqlpw << endl;
					//cout << pw << endl;
				}
			}
			else          //�Է��� ���̵� �������� ����
			{
				cout << "�������� �ʴ� ID�Դϴ�" << endl;
			}
		}
		else
		{
			printf("Something wrong in SQLExecDirect. Error code = %d\n", retcode);
		}

		SQLFreeStmt(hstmt, SQL_DROP);
		SQLDisconnect(hdbc);
		SQLFreeConnect(hdbc);
		SQLFreeEnv(henv);

		return 0;
	}
};



int main()
{
	LOGIN l;

	l.login();

	cout << "main��!" << endl;
}