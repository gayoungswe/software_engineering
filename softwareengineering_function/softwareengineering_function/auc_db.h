#pragma once
#ifndef __AUC_DB_H__
#define __AUC_DB_H__

class AUC_DB
{
public:
	int auc_db_conn();
	int auc_db_conn_free();
};

#endif