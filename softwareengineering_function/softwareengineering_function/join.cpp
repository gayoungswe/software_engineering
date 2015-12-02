#pragma once

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>	//CString 쓰려면 포함

#include "auc_db.h"
#include "all.h"

//#include <sql.h>
//#include <sqlext.h>

using namespace std;

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

		char *t_id = new char[id.length() + 1];
		char *t_pw = new char[pw.length() + 1];
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
