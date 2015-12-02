#pragma once
#ifndef __JOIN_H__
#define __JOIN_H__

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>	//CString ������ ����

using namespace std;

class JOIN
{
private:
	wstring	query_check_id = L"select userid from aucuser where userid = ? ;";
	wstring query_join = L"insert into aucuser values(?, ?, 0);";

public:
	JOIN(){}
	~JOIN(){}
	bool id_check(string id);	//id �ߺ�üũ
	bool pw_equal_check(string pw1, string pw2);	//pw1, pw2 �ߺ�üũ
	int join(string id, string pw);	//ȸ������
};

#endif