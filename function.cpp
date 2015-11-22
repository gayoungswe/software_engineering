//software engineering term project
//8_Auction

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

//DB ����, ���� Ŭ����
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

//��ǰ Ŭ����
class goods
{
public:
	int g_ID;			//��ǰ ��ȣ	
	string u_ID;		//����� ��ȣ
	string g_name;		//��ǰ �̸�
	int start_price;	//���� ������
	int right_price;	//��� ���Ű�
	int p_price;		//���� ������
	string bid_user;		//���� �������� ������ ���� ��ȣ
	//�����ð�ó���������

	goods(int gid, string uid, string gname, int start, int right/*, �ð�����*/)		//������
	{
		g_ID = gid;
		u_ID = uid;
		g_name = gname;
		start_price = start;
		right_price = right;
		p_price = start;
		bid_user = "null";
		//�ð�
	}
	
	int bid(int new_bid_price, string b_user)	//������������
	{
		p_price = new_bid_price;
		bid_user = b_user;
	}

	int suc_bid()	//���� ����
	{

	}

	int fail_bid()	//�ŷ� ����
	{

	}
};

//��ǰ ��� Ŭ����
class goods_list
{
public:
	int goods_num;		//��ϵ� ��ǰ ����
	goods list;			//��ǰ��ü�迭

	int print_list()	//��ǰ ��� ���
	{

	}

	int add_goods()		//��ǰ ���
	{

	}

	int del_goods()		//��ǰ ����
	{

	}
};

//���� Ŭ����
class bidding
{
public:
	string whois;
	int howmuch;

	int add_bidding()
	{

	}
};

//���� Ŭ����
class suc_bidding
{
public:
	int whatis;
	int howmuch;

	int add_suc()
	{

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

	//������� Ȥ�� ������� ����Լ� -> �������̵�����
};

//�Ϲ�����Ŭ����
class G_USER : public USER
{
private:
	bidding bidding_list;	//������ü�迭
	suc_bidding suc_list;	//������ü�迭

public:
	G_USER(){}
	G_USER(string uid)
	{
		user_id = uid;
	}
	~G_USER(){}

	/*u_bidding()
	u_add_good()
	u_del_goods()
	show_bidding()
	show_suc()*/
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
	/*goods_delete()
	user_delete()
	user_search()
	noteice_reg()*/
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

		retcode = SQLExecDirect(hstmt, (SQLWCHAR *)query_check_id.c_str(), SQL_NTS);

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
			else
			{
				//�Է��� ���̵� �ߺ� ����->��� ����

				//�˾�â ��� "��� ������ ���̵��Դϴ�." �޽��� ���
				cout << "��� ������ ���̵��Դϴ�.\n	" << endl;

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
		char *t_id = new char[id.length() + 1];
		char *t_pw = new char[pw.length() + 1];
		SQLINTEGER len_id = SQL_NTS;
		SQLINTEGER len_pw = SQL_NTS;

		t_id = const_cast<char *>(id.c_str());
		t_id[id.length()] = '\0';

		t_pw = const_cast<char *>(pw.c_str());
		t_pw[pw.length()] = '\0';

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

//�α���
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
				//�̺κ��� ����ũ���� �迭�� �޾ƿ� sqlpw char�迭�� �����ڸ��� �����Ⱚ�� �� ���� ������
				int i;
				for (i = 0; (int)sqlpw[i] != 32; i++);

				for (i; i < 10; i++)
				{
					sqlpw[i] = '\0';
				}

				//char to string..
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


int main()
{


	return 0;
}