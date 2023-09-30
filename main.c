
#include <stdio.h>
#include "INIParser.h"



//#ifdef _DEBUG
//void* operator new(size_t size, const char* file, int line)
//{
//	std::string fullpath = file;
//	std::string filename = fullpath.substr(fullpath.find_last_not_of("/\\") + 1 );
//	printf("new F:%s L:%d  (consider using modern pointers) \n", filename.c_str(), line);
//	return malloc(size);
//}
//
//void log_delete(const char* file, int line)
//{
//	std::string fullpath = file;
//	std::string filename = fullpath.substr(fullpath.find_last_not_of("/\\") + 1);
//	printf("delete F:%s L:%d \n", filename.c_str(), line);
//	
//}
//
//void operator delete(void* p, const char* file, int line)
//{
//	free(p);
//}
//#define new new(__FILE__, __LINE__)
//#define delete log_delete(__FILE__,__LINE__); delete
//#endif

int main()

{
	

	 ini_parseINIFromFile("./Config.ini");



}

