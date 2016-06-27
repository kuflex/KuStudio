#include "pbFiles.h"
#include "converter.h"
#include "parsing.h"
#include <algorithm>

//--------------------------------------------------------------------------------
bool pbFiles::fileExists( const string &fileName )
{
	ifstream inp;
	inp.open(fileName.c_str(), ifstream::in);
	inp.close();
	return !inp.fail();
}

//--------------------------------------------------------------------------------
int pbFiles::fileSize( const string &fileName )
{
	FILE *file = fopen( fileName.c_str(), "rb" );
	fseek( file, 0, SEEK_END );
	int size = ftell( file );	//TODO size_t вместо int
	fclose( file );
	return size;
}

//--------------------------------------------------------------------------------
char cmStringLastSymbol( const string &s ) {
    if ( !s.empty() ) return s[ s.length()-1 ];
    else return 0;
}

//--------------------------------------------------------------------------------
vector<string> pbFiles::readStrings( const string &fileName )
{
    vector<string> lines;
    //cmAssert( cmFileExists( fileName ), "cmFileReadStrings no file " + fileName );
    ifstream f(fileName.c_str(),ios::in| ios::binary);
    string line;
    while (getline(f,line)) {
        if (line=="") continue;
        else {
            //убираем в конце '\r' для правильного считывания windows-файлов в linux
            //и в windows также сейчас такие есть
            while ( cmStringLastSymbol( line ) == '\r' ) {
                line = line.substr( 0, line.length() - 1 );
            }
            lines.push_back( line );
        }
    }
    return lines;

    
    
//	vector<string> list;
//	if ( fileExists( fileName ) ) {
//		ifstream f(fileName.c_str(),ios::in);
//		string line;
//		while (getline(f,line)) {
//			list.push_back( line );
//		}
//	}                              
//	return list;                   
}

//--------------------------------------------------------------------------------
vector<string> pbFiles::readStringsFilterAndNotEmpty( const string &fileName )
{
	vector<string> list = readStrings( fileName );
	vector<string> res;
	for (int i=0; i<list.size(); i++) {
		string s = Converter::filterString( list[i] );
		if ( s != "" ) {
			res.push_back( s ); 
		}	
	}
	return res;
}

//--------------------------------------------------------------------------------
bool pbFiles::writeStrings( const vector<string> &list, const string &fileName )
{
	ofstream f(fileName.c_str(),ios::out);
	for ( int i=0; i<list.size(); i++ ) {
		f << list[i] << endl;
	}
	return true;
}

//--------------------------------------------------------------------------------
vector<float> pbFiles::readFloats( const string &fileName )
{
	vector<string> sv = readStrings( fileName );
	return Converter::vectorStringToFloat( sv );
}

//--------------------------------------------------------------------------------
bool pbFiles::writeFloats( const vector<float> &list, const string &fileName )
{
	vector<string> sv = Converter::vectorFloatToString( list );
	return writeStrings( sv, fileName );
}

//--------------------------------------------------------------------------------
vector<int> pbFiles::readInts( const string &fileName )
{
	vector<string> sv = readStrings( fileName );
	return Converter::vectorStringToInt( sv );
}

//--------------------------------------------------------------------------------
bool pbFiles::writeInts( const vector<int> &list, const string &fileName )
{
	vector<string> sv = Converter::vectorIntToString( list );
	return writeStrings( sv, fileName );
}

//--------------------------------------------------------------------------------
string pbFiles::readOneString( const string &fileName )
{
	string res = "";
	if ( fileExists( fileName ) ) {
		vector<string> list;
		list = readStrings( fileName );
		if ( list.size() >= 1 ) {
			res = list[0];
		}
	}
	return res;
}

//--------------------------------------------------------------------------------
void pbFiles::writeOneString( const string &fileName, const string &s )
{
	vector<string> list;
	list.push_back( s );
	writeStrings( list, fileName );
}

//--------------------------------------------------------------------------------
string pbFiles::getExtension( const string &fileName, bool lowerCase )
{
	string result = "";
	std::string::size_type idx;
	idx = fileName.rfind('.');
    if(idx != std::string::npos){
        result = fileName.substr(idx+1);
    }
	if ( lowerCase ) {
		 transform( result.begin(), result.end(), result.begin(), ::tolower );
	}
	return result;
}

//--------------------------------------------------------------------------------
vector<pbFiles::PairFloat> pbFiles::readPairs( const string &fileName )
{
	vector<PairFloat>  res;
	vector<string> sv = readStrings( fileName );
	for (int i=0; i<sv.size(); i++) {
		vector<string> line = Parsing::split( Converter::filterString( sv[i] ), "=" );
		if ( line.size() >= 2 ) {
			res.push_back( pbFiles::PairFloat( line[0], Converter::strToDouble( line[1] ) ) );
		}		
	}
	return res;
}

//--------------------------------------------------------------------------------
void pbFiles::writePairs( const vector<pbFiles::PairFloat> &list, const string &fileName )
{
	vector<string> sv;
	for (int i=0; i<list.size(); i++) {
		string s = list[i].name + "=" + Converter::doubleToStr( list[i].value );
		sv.push_back( s );
	}
	writeStrings( sv, fileName );

}

//--------------------------------------------------------------------------------
float pbFiles::findValue( const vector<PairFloat> &list, string name, float defValue )
{
	for (int i=0; i<list.size(); i++) {
		if ( list[i].name == name ) return list[i].value;
	}
	return defValue;
}

//--------------------------------------------------------------------------------
