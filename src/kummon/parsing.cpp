//---------------------------------------------------------------------------


#pragma hdrstop

#include "parsing.h"
#include "converter.h"

//---------------------------------------------------------------------------
//разбить строку на подстроки, используя разделитель delim
vector<string> Parsing::split( const string &str, const string &delim )
{
	vector<string> result;
	int n = str.length();
	int nD = delim.length();
	int i = 0;
	while ( i < n ) {
		int j = str.find( delim, i );
		if ( j >= 0 && j < n ) {
			string s = str.substr( i, j - i );
			result.push_back( s );
			i = j + nD;
		}
		else break;
	}
	if (i < n) result.push_back( str.substr( i ) );

	return result;

}

//---------------------------------------------------------------------------
//фильтрует пробелы и табуляции
vector<float> Parsing::splitToFloats( const string &str, const string &delim )
{
	string s = str;
	s = Converter::replaceAll( s, " ", "" );
	s = Converter::replaceAll( s, "\t", "" );
	vector<string> list = split( s, delim );
	vector<float> listF( list.size() );
	for (int i=0; i<listF.size(); i++) {
		listF[i] = Converter::strToDouble( list[i] );
	}
	return listF;
}

//---------------------------------------------------------------------------
vector<string> Parsing::splitFiltered( const string &str, const string &delim )
{
	string s = str;
	s = Converter::replaceAll( s, " ", "" );
	s = Converter::replaceAll( s, "\t", "" );
	return split( s, delim );
}

//---------------------------------------------------------------------------
vector<float> Parsing::splitFilteredFloat( const string &str, const string &delim )
{
	vector<string> list = splitFiltered( str, delim );
	int n = list.size();
	vector<float> list1( n );
	for (int i=0; i<n; i++) {
		list1[i] = Converter::strToDouble( list[i] );
	}
	return list1;
}

//---------------------------------------------------------------------------
vector<int> Parsing::splitFilteredInt( const string &str, const string &delim )
{
	vector<string> list = splitFiltered( str, delim );
	int n = list.size();
	vector<int> list1( n );
	for (int i=0; i<n; i++) {
		list1[i] = Converter::strToInt( list[i] );
	}
	return list1;

}

//---------------------------------------------------------------------------
