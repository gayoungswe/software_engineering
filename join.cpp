//join.cpp

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
	G_USER(string uid, string nick)
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
	ADMIN(string uid, string nick)
	{
		user_id = uid;
	}
	~ADMIN(){}
};


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

		char *t_id = new char[id.length()+1];
		char *t_pw = new char[pw.length()+1];
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


int main(void)
{
	bool id_check = false;		//���̵� �ߺ�üũ (Ȯ���� 1)
	bool pw_check = false;		//�� ��й�ȣ�� ������ Ȯ�� (Ȯ�� ��1)
	string id, pw1, pw2, nickname;

	JOIN j;
	AUC_DB db;

	db.auc_db_conn();

	///////////////////////////// id ///////////////////////////
	while (!id_check)
	{
		//gui���� �Է¹����κ�
		//id �Է� -> gui text ����
		cout << "id : ";
		getline(cin, id);
		cin.clear();  // ���� �÷��׸� ����� ���� cin�� �ִ� clear �Լ��� ���

		cout << "�Է��� id = " << id << endl;

		//id�ߺ�üũ ��ư�� ���� ��
		id_check = j.id_check(id);
	}

	//for debug
	//cout << "id_check = " << id_check << endl;

	////////////////////////////// pw /////////////////////////////
	while (!pw_check)
	{
		//pw1 �Է� -> gui text ����
		cout << "pw1 : ";
		getline(cin, pw1);
		cin.clear();

		//for debug
		//cout << "�Է��� pw = " << pw1 << endl;

		//pw2 �Է� -> gui text ����
		cout << "pw2 : ";
		getline(cin, pw2);
		cin.clear();  // ���� �÷��׸� ����� ���� cin�� �ִ� clear �Լ��� ���

		//pw1�� pw2�� ������ Ȯ��
		pw_check = j.pw_equal_check(pw1, pw2);
	}

	///////////////// join //////////////
	j.join(id, pw1);

	db.auc_db_conn_free();

	return 0;

}