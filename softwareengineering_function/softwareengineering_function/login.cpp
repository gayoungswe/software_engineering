#pragma once
//#include <iostream>
//#include <string>
//#include <string.h>
//#include <Windows.h>

//SQL ODBC Conn
//#include <sql.h>
//#include <sqlext.h>

#include "auc_db.h"
#include "user.h"
#include "g_user.h"
#include "admin.h"
#include "all.h"

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
		USER* user = NULL;

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

		if (retcode == SQL_SUCCESS)
		{
			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, &sqlpw, 20, (SQLINTEGER *)&isqlpw);			//��й�ȣ
			retcode = SQLBindCol(hstmt, 3, SQL_C_SLONG, &sqlauth, 0, (SQLINTEGER *)&isqlauth);		//����

			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{
				int i;
				for (i = 0; (int)sqlpw[i] != 32; i++);
			
				for (i; i < 10; i++)
				{
					sqlpw[i] = '\0';
				}

				string pw2 = sqlpw;					//sqlpw -> string

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