#pragma once
#ifndef __BIDDING_H__
#define __BIDDING_H__

#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>	//CString ������ ����

using namespace std;

class BIDDING
{
public:
	string whois;
	int howmuch;

	int add_bidding();
};

#endif