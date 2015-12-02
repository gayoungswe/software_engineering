#pragma once
#ifndef __ADMIN_H__
#define __ADMIN_H__

#include "user.h"

class ADMIN : public USER
{
public:
	ADMIN();
	ADMIN(string uid);
	~ADMIN();
};

#endif