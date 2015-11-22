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
	string nick_name;

public:
	USER(){}
	USER(string uid, string nick)
	{
		user_id = uid;
		nick_name = nick;
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
		nick_name = nick;
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
		nick_name = nick;
	}
	~ADMIN(){}
};


//join Ŭ����
class JOIN
{
private:
	/*string id;
	string nickname;
	string pwd;*/

	/*wstring	query_check_id = L"select userid from aucuser where userid = ? ;";
	wstring query_join = L"insert into aucuser values(?, ?,'nickname' , 0);";*/

public:
	JOIN(){}
	~JOIN(){}

	int id_check(string id)	//id �ߺ�üũ
	{
		wstring	query_check_id = L"select userid from aucuser where userid = ? ;";

		SQLINTEGER len_id = SQL_NTS;
		char t_id[20];
		char sqlid[20];
		SQLINTEGER isqlid;

		strncpy_s(t_id, id.c_str(), sizeof(t_id));
		t_id[sizeof(t_id) - 1] = 0;

		retcode = SQLBindParameter
			(
			hstmt,
			1,
			SQL_PARAM_INPUT,
			SQL_C_CHAR,
			SQL_CHAR,
			20,//sizeof(t_id),
			0,
			&t_id,
			20,//sizeof(t_id),
			&len_id
			);

		//for debug
		cout << sizeof(t_id) << endl;

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query_check_id.c_str(), SQL_NTS);

		if (retcode == SQL_SUCCESS)
		{
			retcode = SQLBindCol(hstmt, 1, SQL_C_CHAR, &sqlid, 20, (SQLINTEGER *)&isqlid);			//��й�ȣ
			
			if (SQLFetch(hstmt) == SQL_SUCCESS)
			{
				// �Է��� ���̵� �̹� ����

				//�˾�â ��� "�̹� ������� ���̵��Դϴ�." �޽��� ���
				cout << "�̹� ������� ���̵� �Դϴ�. \n�ٸ� ���̵� �Է��ϼ���.\n" << endl;
				return 0;
			}
			else
			{
				//�Է��� ���̵� �ߺ� ����->��� ����

				//�˾�â ��� "��� ������ ���̵��Դϴ�." �޽��� ���
				cout << "��� ������ ���̵��Դϴ�.\n	" << endl;
				return 1;
			}
		}
	}

	int pw_equal_check(string pw1, string pw2)	//pw1, pw2 �ߺ�üũ
	{
		if (pw1.compare(pw2) == 0)	//�� ��й�ȣ ��ġ
		{
			return 1;
		}
		else   //�� ��й�ȣ ����ġ
		{
			cout << "��й�ȣ�� ��ġ���� �ʽ��ϴ�." << endl;
			return 0;
		}
	}

	int join(string id, string pw)	//ȸ������
	{
		wstring query_join = L"insert into aucuser values(?, ?,'nickname' , 0);";
		//wstring query_join = L"insert into aucuser values('abcd', '1234', 'nickname' , 0);";

		//for debug
		cout << "�Է��� id = " << id << endl;
		cout << "�Է��� pw = " << pw << endl;

		char *t_id = new char[id.length()+1];
		char *t_pw = new char[pw.length()+1];
		SQLINTEGER len_id = SQL_NTS;
		SQLINTEGER len_pw = SQL_NTS;
		
		//strncpy_s(t_id, id.c_str(), id.length() + 1);
		//strncpy_s(t_id, const_cast<char *>(id.c_str()), sizeof(t_id));
		t_id = const_cast<char *>(id.c_str());
		t_id[id.length()] = '\0';

		t_pw = const_cast<char *>(pw.c_str());
		t_pw[pw.length()] = '\0';

		//for debug
		cout << "id.length() = " << id.length() << endl;
		cout << "pw.length() = " << pw.length() << endl;
		cout << t_id << endl;
		cout << t_pw << endl;

		//copy(id.begin(), id.end(), t_id);
		//t_id[id.length()] = '\0'; // don't forget the terminating 0

		//copy(pw.begin(), pw.end(), t_pw);
		//t_id[pw.length()] = '\0'; // don't forget the terminating 0

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
		cout << "sqlbindpara 1 retcode = " << retcode << endl;

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
		cout << "sqlbindpara 2 retcode = " << retcode << endl;

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query_join.c_str(), SQL_NTS);
		if (retcode == SQL_SUCCESS)
		{
			cout << "insert ����!" << endl;
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
	int id_check = 0;		//���̵� �ߺ�üũ (Ȯ���� 1)
	int pw_check = 0;		//�� ��й�ȣ�� ������ Ȯ�� (Ȯ�� ��1)
	string id, pw1, pw2, nickname;

	JOIN j;
	AUC_DB db;

	db.auc_db_conn();

	///////////////////////////// id ///////////////////////////
	while (id_check == 0)
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
	cout << "id_check = " << id_check << endl;

	////////////////////////////// pw /////////////////////////////
	while (pw_check == 0)
	{
		//pw1 �Է� -> gui text ����
		cout << "pw1 : ";
		getline(cin, pw1);
		cin.clear();

		cout << "�Է��� pw = " << pw1 << endl;

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

////////////�ٷ��ߺ����°ų�����ǰ�
/////////ù��°�ߺ����� -> �״��� �ߺ�x �ȵ�