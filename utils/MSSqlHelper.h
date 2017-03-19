#pragma once
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

class CMSSqlHelper
{
public:
	CMSSqlHelper(string host, string port, string dbName, string user, string pwd): 
			_host(host), _port(port), _dbName(dbName), _userName(user), _pwd(pwd){
		init();
	}
	bool connect();
	SQLRETURN GetResultset();
	void DescribeColumns();
	bool insert(const string& hphm, const string& hpzl, const string& regDateTime, const string& regPhoto,const string& regSign, const string& regState);
	CMSSqlHelper(void);
	~CMSSqlHelper(void);

private:
	struct ColDescription
	{
		SQLSMALLINT colNumber;
		SQLCHAR colName[80];
		SQLSMALLINT nameLen;
		SQLSMALLINT dataType;
		SQLULEN colSize;
		SQLSMALLINT decimalDigits;
		SQLSMALLINT nullable;
	};
	bool init();
	void show_error(unsigned int handletype, const SQLHANDLE& handle);
	SQLHANDLE _sqlenvhandle;
    SQLHANDLE _sqlconnectionhandle;
    SQLHANDLE _sqlstatementhandle;
    SQLRETURN _retcode;

	vector<ColDescription> cols;
	vector< vector<string> > colData;

	_inline SQLRETURN Describe(ColDescription& c);
	SQLRETURN GetColData(int colnum, std::string& str);
	string _host;
	string _port;
	string _dbName;
	string _userName;
	string _pwd;
	
};

class ODBC_Class
{
	struct ColDescription
	{
		SQLSMALLINT colNumber;
		SQLCHAR colName[80];
		SQLSMALLINT nameLen;
		SQLSMALLINT dataType;
		SQLULEN colSize;
		SQLSMALLINT decimalDigits;
		SQLSMALLINT nullable;
	};
// Attributes
public:
	SQLHANDLE EnvHandle;
	SQLHANDLE ConHandle;
	SQLHANDLE StmtHandle;
	SQLRETURN rc;
	vector<ColDescription> cols;
	vector< vector<string> > colData;
// Operations
public:
	ODBC_Class(); // Constructor
	~ODBC_Class(); // Destructor
	SQLRETURN GetResultset();
	void DescribeColumns();
private:
	_inline SQLRETURN Describe(ColDescription& c);
	SQLRETURN GetColData(int colnum, string& str);
};