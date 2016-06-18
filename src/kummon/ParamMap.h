//---------------------------------------------------------------------------

#ifndef ParamMapH
#define ParamMapH
//---------------------------------------------------------------------------

#include <string>
#include <map>
#include <vector>

#include "ofMain.h"
#include "maps.h"

#include "converter.h"


using namespace std;

const int Var_No = -1;
const int Var_Float = 0;
const int Var_Int = 1;
const int Var_String = 2;

class ParamMap;

//регистрация переменных
struct Var {
    string name;
    void *pointer;
    int type;
    float &var_float() { return *((float*)(pointer)); }
    int &var_int() { return *((int*)(pointer)); }
    string &var_string() { return *((string*)(pointer)); }
    
    Var() { pointer = 0; type = Var_No; }
    void VarFloat( float &value0, string name0 ) {
        name = name0;
        pointer = &value0;
        type = Var_Float;
    }
    void VarInt( int &value0, string name0 ) {
        name = name0;
        pointer = &value0;
        type = Var_Int;
    }
    void VarString( string &value0, string name0 ) {
        name = name0;
        pointer = &value0;
        type = Var_String;
    }
    void saveIni( ParamMap &ini );
    void loadIni( ParamMap &ini );
    void fromString( const string &value );
    string toString();
};


//Список параметров, без записи в ini
class ParamMap0 {
public:
    vector<string> keys();
	void deleteKey( string key );
	void clear();
	bool keyExists( const string &key );
    void getStringValue( const string &key, string &var );
    void getIntValue( const string &key, int &var );
    void getDoubleValue( const string &key, double &var );
    void getFloatValue( const string &key, float &var );
	string getString( const string &key, string def = "" );
    
	int getInt( const string &key, int def = 0 );
    
	double getDouble( const string &key, double def = 0 );
	float getFloat( const string &key, float def = 0 );
	bool getBool( const string &key, bool def = false );
    //x=y
    void setPair( const string &data );
    
	void setString( const string &key, const string &value );
    
	void setDouble( const string &key, double dValue );
	void setFloat( const string &key, float value );
    
	void setInt( const string &key, int iValue );
	void setBool( const string &key, bool bValue );
    void setPoint( const string &key, ofPoint p );
    ofPoint getPoint( const string &key, ofPoint defValue = ofPoint() );
    
protected:
	StringStringMap _values;
};

//Работа с ini, а также с переменными
class ParamMap: public ParamMap0 {
public:
    //регистрация переменных - они будут автоматически отслеживаться
    //внимание, переменные должны сохраняться всё время работы программы,
    //так как на них хранится указатель
    void addFloat( float &value, string name, float value0 );
    void addInt( int &value, string name, int value0 );
    void addString( string &value, string name, string value0 );

    void varsUpdated();       //копирует значения переменных в map
    void iniToVars();         //установить значения из ini в привязанные переменные
    
    void loadIni( string fileName );
    void saveIni( string fileName );
    void saveIni() { saveIni( _fileName ); }
    void loadFromStrings( vector<string> &lines );
    
    //получить значение переменной
    string getValue( const string &name, const string &def = "0" );
    //Установка значения в переменную, не в ini, а только в vars
    void setValue( const string &name, const string &value );
    //Есть ли такая переменная
    bool hasVar( const string &name );

	string fileName() { return _fileName; };
	void setFileName( string fileName ) { _fileName = fileName; }


protected:
	string _fileName;

private:
    vector<Var> vars; //хранение зарегистрированных переменных
    ParamMap0 vars_index;    //хранение соответствия имени номеру
    void check_new_var( void *value, string name );  //проверка, что нет такой переменной

};



#endif
