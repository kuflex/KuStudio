#ifndef pbFilesH
#define pbFilesH


#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;



class pbFiles {
public:
	static bool fileExists( const string &fileName );
	static int fileSize(  const string &fileName );
	static string getExtension( const string &fileName, bool lowerCase = true );

	static vector<string> readStrings( const string &fileName );
	static vector<string> readStringsFilterAndNotEmpty( const string &fileName );
	static bool writeStrings( const vector<string> &list, const string &fileName );
	static vector<float> readFloats( const string &fileName );
	static bool writeFloats( const vector<float> &list, const string &fileName );
	static vector<int> readInts( const string &fileName );
	static bool writeInts( const vector<int> &list, const string &fileName );
	
	struct PairFloat {
		PairFloat( const string &name0, float value0 ) {
			name = name0;
			value = value0;
		}
		string name;
		float value;
	};

	static vector<PairFloat> readPairs( const string &fileName ); 
	static void writePairs( const vector<PairFloat> &list, const string &fileName  ); 
	static float findValue( const vector<PairFloat> &list, string name, float defValue = 0 );

	//работа с однострочными файлами
	static string readOneString( const string &fileName );
	static void writeOneString( const string &fileName, const string &s );
};

#endif