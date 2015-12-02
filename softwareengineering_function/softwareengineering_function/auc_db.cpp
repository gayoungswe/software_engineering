#pragma once
#include <iostream>
#include <Windows.h>

#include "all.h"

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
		////if ((retcode = SQLConnect(hdbc, (SQLWCHAR *)TEXT("softwareengineering"), SQL_NTS, (SQLWCHAR *)TEXT("software"), SQL_NTS, (SQLWCHAR *)TEXT("software"), SQL_NTS)) != SQL_SUCCESS)
			if ((retcode = SQLConnect(hdbc, (SQLWCHAR *)TEXT("softwareengineering2"), SQL_NTS, (SQLWCHAR *)TEXT("software"), SQL_NTS, (SQLWCHAR *)TEXT("software"), SQL_NTS)) != SQL_SUCCESS)
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
