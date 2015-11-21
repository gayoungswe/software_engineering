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

//userŬ����
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

//�Ϲ�����Ŭ����
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

//����������Ŭ����
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


//logiin Ŭ����
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

		//id �Է� -> gui text ����
		cout << "id : ";
		cin >> id;

		cin.clear();  // ���� �÷��׸� ����� ���� cin�� �ִ� clear �Լ��� ���
		cin.ignore(INT_MAX, '\n');

		//cout << id << endl;

		//pw �Է� -> gui text ����
		cout << "pw : ";
		getline(cin, pw	);

		cin.clear();  // ���� �÷��׸� ����� ���� cin�� �ִ� clear �Լ��� ���
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

			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, &sqlpw, 20, (SQLINTEGER *)&isqlpw);			//��й�ȣ
			retcode = SQLBindCol(hstmt, 2, SQL_C_CHAR, &sqlnick, 20, (SQLINTEGER *)&isqlnick);		//�г���
			retcode = SQLBindCol(hstmt, 3, SQL_C_SLONG, &sqlauth, 0, (SQLINTEGER *)&isqlauth);		//����

			//for debug
			/*cout << "retcode = " << retcode << endl;
			cout << "33333333" << endl;*/

			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{	
				//for debug
				/*cout << "44444444444" << endl;
				cout << sqlpw << ", " << sqlnick << ", " << sqlauth << endl;*/
				
				//�̺κ��� ����ũ���� �迭�� �޾ƿ� sqlpw char�迭�� �����ڸ��� �����Ⱚ�� �� ���� ������
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
					cout << "�α��� ����!" << endl;

					if (sqlauth != 1)      //������ �Ϲ������� ��
					{
						user = new G_USER(id, sqlnick);	//�Ϲ� ���� ��ü ���� �Ҵ�
					}
					else      //������ �������϶�
					{
						user = new ADMIN(id, sqlnick);		//������ ���� ��ü �����Ҵ�
					}
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

		return user;		//�α��� �� ���� ��ü ������ ��ȯ
	}
};


int main()
{
	LOGIN l;

	USER * user=l.login();

	cout << "main��!" << endl;
}
