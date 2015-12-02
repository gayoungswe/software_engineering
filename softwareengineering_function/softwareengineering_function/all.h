#pragma once
#ifndef __ALL_H__
#define __ALL_H__

//SQL ODBC Conn
#include <sql.h>
#include <sqlext.h>

extern SQLHENV	henv;
extern SQLHDBC	hdbc;
extern SQLHSTMT	hstmt;
extern SQLRETURN retcode;

#endif;