//---------------------------------------------------------------------------

#ifndef mapsH
#define mapsH

#include <string>
#include <map>

using namespace std;

typedef map<string, string, less<string> > StringStringMap;
typedef StringStringMap::value_type StringStringMapValueType;

typedef map<string, int, less<string> > StringIntMap;
typedef StringIntMap::value_type StringStringIntValueType;

//typedef map<string, (void *), less<(void *)> > StringPointerMap;	//тут нужен не void*, а итератор в списке
//typedef StringPointerMap::value_type StringPointerMapValueType;


//---------------------------------------------------------------------------
#endif
