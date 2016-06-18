//---------------------------------------------------------------------------

#ifndef parsingH
#define parsingH

#include <vector>
#include <string>
using namespace std;

#include "converter.h"

//Модуль парсинга строк 

class Parsing {
public:

	//разбить строку на подстроки, используя разделитель delim
	static vector<string> split( const string &str, const string &delim );
	
	//фильтрует пробелы и табуляции
	static vector<string> splitFiltered( const string &str, const string &delim );
	static vector<float> splitFilteredFloat( const string &str, const string &delim );
	static vector<int> splitFilteredInt( const string &str, const string &delim );

	//фильтрует пробелы и табуляции
	static vector<float> splitToFloats( const string &str, const string &delim );

};

//---------------------------------------------------------------------------
#endif
