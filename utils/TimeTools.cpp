#include "TimeTools.h"
#include <ctime>

string TimeTools::GetNowDateTime(string format){
	using namespace std;
	time_t timer;
	time(&timer);
	struct tm* timeinfo;
	timeinfo = localtime(&timer);
	char buf[64] = {0};
	strftime(buf, 64, format.c_str(), timeinfo);
	return string(buf);
}