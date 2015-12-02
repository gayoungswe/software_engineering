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
			retcode = SQLBindCol(hstmt, 2, SQL_C_SLONG, &sqlauth, 0, (SQLINTEGER *)&isqlauth);		//권한

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