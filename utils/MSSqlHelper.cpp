#include "MSSqlHelper.h"
#include <iostream>

CMSSqlHelper::CMSSqlHelper(void)
{
	
}


CMSSqlHelper::~CMSSqlHelper(void)
{
	// Free The Connection Handle
	if (_sqlconnectionhandle != NULL)
		SQLFreeHandle(SQL_HANDLE_DBC, _sqlconnectionhandle);
	// Free The Environment Handle
	if (_sqlenvhandle != NULL)
		SQLFreeHandle(SQL_HANDLE_ENV, _sqlenvhandle);
}
 bool CMSSqlHelper::init()
{
	// Initialize The Return Code Variable
	_retcode = SQL_SUCCESS;
	// Allocate An Environment Handle
	_retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_sqlenvhandle);
	// Set The ODBC Application Version To 3.x
	if (_retcode != SQL_SUCCESS)
		return false;
	_retcode = SQLSetEnvAttr(_sqlenvhandle, SQL_ATTR_ODBC_VERSION,
			(SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER);
	
	// Allocate A Connection Handle
	if (_retcode != SQL_SUCCESS)
		return false;
	_retcode = SQLAllocHandle(SQL_HANDLE_DBC, _sqlenvhandle, &_sqlconnectionhandle);
	if (_retcode != SQL_SUCCESS)
		return false;
	return true;
}


void  CMSSqlHelper::show_error(unsigned int handletype, const SQLHANDLE& handle){
		SQLCHAR sqlstate[1024];
		SQLCHAR message[1024];
		if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
			std::cout<<"Message: "<<message<<"\nSQLSTATE: "<<sqlstate<< std::endl;
}




// Get the data for one column and return the info in the form
// of a std::string.  The ODBC driver will make all necessary
// data conversions from whatever type the data is in the database
// to SQL_CHAR.  You could make this function more comples by 
// getting the return type as it appears in the database then constructing
// a VARIANT object to hold the data.
SQLRETURN CMSSqlHelper::GetColData(int colnum, std::string& str)
{
	unsigned int bufSize = 1024 * 1024 * 5;
	SQLCHAR* buf = new SQLCHAR[bufSize];
	if( (_retcode = SQLGetData(_sqlstatementhandle, colnum, SQL_CHAR, buf, bufSize, NULL)) == SQL_SUCCESS)
		str = reinterpret_cast<char*>(buf);
	delete buf;
	return _retcode;
}

//
// Define The ShowResults() Member Function
SQLRETURN CMSSqlHelper::GetResultset()
{
	// Get all column description
	DescribeColumns();
	// erase anything that's in the colData vector
	colData.clear();
	// fetch a row from the resultset
	while( SQLFetch(_sqlstatementhandle) == SQL_SUCCESS)
	{
		// vector of strings to hold the column data
		vector<string> col;
		string data;
		// column counter
		int i = 1;
		// get the data for each column and add it to 
		// the col vector
		while( GetColData(i, data) == SQL_SUCCESS)
		{
			col.push_back(data);
			++i; // increment the column number
		}
		// add column data to the colData vector
		colData.push_back(col);
	}
	return SQL_SUCCESS;
}

// Get the description for one column in the resultset.
// This was made a seprate function to simplify the coding
SQLRETURN  CMSSqlHelper::Describe(ColDescription& c)
{
	return SQLDescribeCol(_sqlstatementhandle,c.colNumber,
		c.colName, sizeof(c.colName), &c.nameLen,
		&c.dataType, &c.colSize, &c.decimalDigits, &c.nullable);
}

// Get the description for all the columns in the resultset.
void CMSSqlHelper::DescribeColumns()
{
	ColDescription c;
	c.colNumber = 1;
	cols.clear();
	while( Describe(c) == SQL_SUCCESS)
	{
		cols.push_back(c);
		++c.colNumber;
	}
		
}

bool CMSSqlHelper::insert(const string& hphm, const string& hpzl, const string& regDateTime, const string& regPhoto,const string& regSign, const string& regState){
	if(_sqlconnectionhandle != NULL){
		_retcode = SQLAllocHandle(SQL_HANDLE_STMT, _sqlconnectionhandle, &_sqlstatementhandle);
		if (_retcode == SQL_SUCCESS)
		{
			// Define A SELECT SQL Statement
			//strcpy((char *) SQLStmt, "SELECT * FROM PDARegister");
			string sqlStament =  "INSERT INTO PDARegister (hphm, hpzl, RegDateTime, RegSign, RegState) VALUES (\'" +hphm +"\', \'"+ hpzl
														+"\', \'" + regDateTime +"\', \'"+ regSign +"\', \'" + regState +"\')";
			// Prepare And Execute The SQL Statement
			_retcode = SQLExecDirect(_sqlstatementhandle, (SQLCHAR*)sqlStament.c_str(), SQL_NTS);
			if(_retcode == SQL_SUCCESS){
				return true;
			}
		}
	}
		
}

bool CMSSqlHelper::connect(){
	SQLSMALLINT bufsize=0;
	string connStatement = "DRIVER=SQL Server;SERVER=" + _host+ ","+ _port +";DATABASE=" + _dbName +";UID=" + _userName + ";PWD=" + _pwd+";";
	_retcode = SQLDriverConnect(_sqlconnectionhandle, NULL, (SQLCHAR*)TEXT(connStatement.c_str()), 
				SQL_NTS, NULL, 256, &bufsize, SQL_DRIVER_NOPROMPT); 
	if(bufsize == 0)
		return false;
	return true;
}