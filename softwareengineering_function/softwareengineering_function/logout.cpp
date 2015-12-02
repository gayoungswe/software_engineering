#pragma once
#include "user.h"

class LOGOUT
{
public:
	int logout(USER* user)
	{
		delete user;

		return 0;
	}
};
