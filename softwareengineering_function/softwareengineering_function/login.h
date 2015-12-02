#pragma once
#ifndef __LOGIN_H__
#define __LOGIN_H__

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>	//CString 쓰려면 포함

#include "user.h"

using namespace std;

class LOGIN
{
private:
	string id;
	string pw;
	char sqlpw[20];
	int sqlauth;

	wstring	query = L"select userpw, authority from aucuser where userid = ? ;";

public:
	LOGIN();
	LOGIN(string id, string pw);
	~LOGIN();
	USER* login();
};


#endif