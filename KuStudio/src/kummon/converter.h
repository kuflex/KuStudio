//---------------------------------------------------------------------------

#ifndef converterH
#define converterH

#include <string>
#include <sstream>
#include <vector>
//#include <windows.h>		//Не кросс-платформенно!

using namespace std;

class Converter {
public:
	static int strToInt(const string& s)
	{
	   istringstream i(s);
	   int x;
	   if (!(i >> x)) {
		   x = 0;
	   }
	   return x;
	}

	static string intToStr(int x, int len = 0)
	{
	   ostringstream o;
	   if (!(o << x)) {
	   }
	   string s = o.str();
	   if (len > 0) {
		int d = len - s.length();		//DUMMY это можно написать короче
		for (int i=0; i<d; i++) {
				s = '0' + s;
		}
	   }
	   return s;
	}

    static double strToDouble(const string& s)
	{
	   	istringstream i(s);
		double x;
		if (!(i >> x)) {
			x = 0;
		}
		return x;
	}

	static string doubleToStr(double x, int len = 0)
	 {
	   ostringstream o;
	   if (!(o << x)) {
	   }
	   string s = o.str();
	   return s;
	 }

	static vector<string> vectorFloatToString( const vector<float> &v ){
		vector<string> sv( v.size() );
		for (int i=0; i<v.size(); i++) {
			sv[ i ] = doubleToStr( v[ i ] );
		}
		return sv;
	}
	static vector<float> vectorStringToFloat( const vector<string> &sv ){
		vector<float> v( sv.size() );
		for (int i=0; i<sv.size(); i++) {
			v[ i ] = strToDouble( sv[ i ] );
		}
		return v;
	}
	static vector<string> vectorIntToString( const vector<int> &v ){
		vector<string> sv( v.size() );
		for (int i=0; i<v.size(); i++) {
			sv[ i ] = intToStr( v[ i ] );
		}
		return sv;
	}
	static vector<int> vectorStringToInt( const vector<string> &sv ){
		vector<int> v( sv.size() );
		for (int i=0; i<sv.size(); i++) {
			v[ i ] = strToInt( sv[ i ] );
		}
		return v;
	}
	//конвертация строки в wstr, тут - используется кодировка по умолчанию
/*	static wstring strToWstr( const string &text )
	{
				// буфер для получения результата
		WCHAR buffer[256];

		// преобразование строки в Unicode
		MultiByteToWideChar(CP_ACP, 0, text.c_str(), strlen(text.c_str()) + 1, buffer, sizeof(buffer)/sizeof(WCHAR));
		return buffer;
	}*/

	//замена всех вхождений строки на другую
	static string replaceAll(const string& context0, const string& from, const string& to)
	{
		string context = context0;
		size_t lookHere = 0;
		size_t foundHere;
		while((foundHere = context.find(from, lookHere)) != string::npos)
		{
			context.replace(foundHere, from.size(), to);
			lookHere = foundHere + to.size();
		}
		return context;
	}
    
    static string filterNewlines( string s ) {
        s    = Converter::replaceAll( s, "\n", "" );
        s    = Converter::replaceAll( s, "\r", "" );
        return s;
    }

	//преобразовать \t в пробелы, удалить дубликаты пробелов и удалить их из начала и конца
	static string filterString( string s )
	{
		s = replaceAll( s, "\t", " ");
		s = replaceAll( s, "\n", " ");
		//s = replaceAll( s, "\l", " ");
		s = replaceAll( s, "\r", " ");
		string res = "";
		int i = 0;
		while ( i < s.length() ) {
			if ( s[i] != ' ' || (s[i] == ' ' && i > 0 && s[i-1] != ' ') ) {
				res += s[i];
			}
			i++;
		}
		//удаляем пробел в начале и конце
		if ( res.length() > 0 && res[0] == ' ' ) res = res.substr( 1 );
		if ( res.length() > 0 && res[res.length() - 1] == ' ' ) res = res.substr( 0, res.length() - 1 );
		return res;
	}


};

//---------------------------------------------------------------------------
#endif
