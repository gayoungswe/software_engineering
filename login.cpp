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


//userŬ����
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

//�Ϲ�����Ŭ����
class G_USER : public USER
{
public:
	G_USER(){}
	G_USER(string uid)
	{
		user_id = uid;
	}
	~G_USER(){}
};

//����������Ŭ����
class ADMIN : public USER
{
public:
	ADMIN(){}
	ADMIN(string uid)
	{
		user_id = uid;
	}
	~ADMIN(){}
};


//logiin Ŭ����
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
		USER* user=NULL;

		//query ���� bindparameter �ϱ����� char�迭��,, string���� �ϴ¹��� ��������..
		strncpy_s(t_id, id.c_str(), sizeof(t_id));
		t_id[sizeof(t_id) - 1] = 0;

		AUC_DB db;

		db.auc_db_conn();		//db ����


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

			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, &sqlpw, 20, (SQLINTEGER *)&isqlpw);			//��й�ȣ
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

				//char to string..
				string pw2 = sqlpw;					//sqlpw -> string

				/*cout << pw.length() << endl;
				cout << pw2.length() << endl;*/

				if (pw.compare(pw2) == 0)
				{
					cout << "�α��� ����!" << endl;

					if (sqlauth != 1)      //������ �Ϲ������� ��
					{
						user = new G_USER(id);	//�Ϲ� ���� ��ü ���� �Ҵ�
					}
					else      //������ �������϶�
					{
						user = new ADMIN(id);		//������ ���� ��ü �����Ҵ�
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

		db.auc_db_conn_free();

		return user;		//�α��� �� ���� ��ü ������ ��ȯ
	}
};


int main()
{
	string id, pw;

	//gui���� �Է¹����κ�
	//id �Է� -> gui text ����
	cout << "id : ";
	getline(cin, id);

	cin.clear();  // ���� �÷��׸� ����� ���� cin�� �ִ� clear �Լ��� ���
	//cin.ignore(INT_MAX, '\n');

	//pw �Է� -> gui text ����
	cout << "pw : ";
	getline(cin, pw);

	cin.clear();  // ���� �÷��׸� ����� ���� cin�� �ִ� clear �Լ��� ���
	//cin.ignore(INT_MAX, '\n');


	//�α���
	LOGIN l(id, pw);			//�α��ΰ�ü���� (������ ȣ��)

	USER * user = l.login();	//login �ϰ� ������ ������ü����ȯ

	cout << "main��!" << endl;

	delete user;		//������ü �Ҹ�
}
