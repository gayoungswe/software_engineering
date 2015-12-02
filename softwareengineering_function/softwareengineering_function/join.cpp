#pragma once

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>	//CString ������ ����

#include "auc_db.h"
#include "all.h"

//#include <sql.h>
//#include <sqlext.h>

using namespace std;

//join Ŭ����
class JOIN
{
private:
	wstring	query_check_id = L"select userid from aucuser where userid = ? ;";
	wstring query_join = L"insert into aucuser values(?, ?, 0);";

public:
	JOIN(){}
	~JOIN(){}

	bool id_check(string id)	//id �ߺ�üũ
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
			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, sqlid, 20, (SQLINTEGER *)&isqlid);			//��й�ȣ

			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{
				// �Է��� ���̵� �̹� ����

				//�˾�â ��� "�̹� ������� ���̵��Դϴ�." �޽��� ���
				cout << "�̹� ������� ���̵� �Դϴ�. \n�ٸ� ���̵� �Է��ϼ���.\n" << endl;
				SQLFreeStmt(hstmt, SQL_CLOSE);		//////!!�߿� �̰Ծ����� ù��° �ߺ��̰� �ι�° ��밡�ɶ� �ȵ�
				return false;
			}
			else if (strncmp(t_id, sqlid, id.length() + 1))
			{
				//sqlid �� id ��
				//�빮�� �ҹ��� ������� ���� ���ڿ��� �ߺ�

				cout << "�̹� ������� ���̵� �Դϴ�. \n�ٸ� ���̵� �Է��ϼ���.\n" << endl;
				SQLFreeStmt(hstmt, SQL_CLOSE);		//////!!�߿� �̰Ծ����� ù��° �ߺ��̰� �ι�° ��밡�ɶ� �ȵ�
				return false;
			}
			else
			{
				//�Է��� ���̵� �ߺ� ����->��� ����

				//�˾�â ��� "��� ������ ���̵��Դϴ�." �޽��� ���
				cout << "��� ������ ���̵��Դϴ�.\n	" << endl;

				//for debug
				//cout << " ��������ϴ°ų�.." << endl;

				SQLFreeStmt(hstmt, SQL_CLOSE);
				return true;
			}
		}
		else
		{
			//for debug
			//cout << "��ü �� ������°�?" << endl;

			return false;
		}
	}

	bool pw_equal_check(string pw1, string pw2)	//pw1, pw2 �ߺ�üũ
	{
		if (pw1.compare(pw2) == 0)	//�� ��й�ȣ ��ġ
		{
			return true;
		}
		else   //�� ��й�ȣ ����ġ
		{
			cout << "��й�ȣ�� ��ġ���� �ʽ��ϴ�." << endl;
			return false;
		}
	}

	int join(string id, string pw)	//ȸ������
	{
		//for debug
		/*cout << "�Է��� id = " << id << endl;
		cout << "�Է��� pw = " << pw << endl;*/

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
			t_id,		//////�̰Թ�����!! &���������� ����ü�ε� �ּ�!!!!!!!!!!!1
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
			t_pw,		//////�̰Թ�����!! &���������� ����ü�ε� �ּ�!!!!!!!!!!!!!!!!!!!1
			pw.length() + 1,
			&len_pw
			);

		//for debug
		//cout << "sqlbindpara 2 retcode = " << retcode << endl;

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query_join.c_str(), SQL_NTS);

		if (retcode == SQL_SUCCESS)
		{
			cout << "insert ����!" << endl;

			SQLFreeStmt(hstmt, SQL_CLOSE);	//

			retcode = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);		//commit;
			if (!SQL_SUCCEEDED(retcode)) {
				printf("Error committing transaction.\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "insert ����!" << endl;
		}

		return 0;
	}
};
