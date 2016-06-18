
#include "ParamMap.h"
#include "pbFiles.h"


//---------------------------------------------------------------------------
vector<string> ParamMap0::keys() {
    vector<string> _keys;
    for (StringStringMap::const_iterator iter = _values.begin();
         iter != _values.end(); iter++) {
        string key = iter->first;
        _keys.push_back( key );
    }
    return _keys;
}


//---------------------------------------------------------------------------
void ParamMap0::deleteKey( string key )	{
    _values.erase( key );
}

//---------------------------------------------------------------------------
void ParamMap0::clear() {
    _values.clear();
}

//---------------------------------------------------------------------------
bool ParamMap0::keyExists( const string &key ) {
    StringStringMap::iterator p = _values.find( key );
    return ( p != _values.end() );
}

//---------------------------------------------------------------------------
void ParamMap0::getStringValue( const string &key, string &var ) {
    var = getString( key, var );
}

//---------------------------------------------------------------------------
void ParamMap0::getIntValue( const string &key, int &var ) {
    var = getInt( key, var );
}

//---------------------------------------------------------------------------
void ParamMap0::getDoubleValue( const string &key, double &var ) {
    var = getDouble( key, var );
}

//---------------------------------------------------------------------------
void ParamMap0::getFloatValue( const string &key, float &var ) {
    var = getFloat( key, var );
}

//---------------------------------------------------------------------------
string ParamMap0::getString( const string &key, string def ) {
    StringStringMap::iterator p = _values.find( key );
    if (p != _values.end()) {
        return p->second;
    }
    else {
        return def;
    }
}

//---------------------------------------------------------------------------
int ParamMap0::getInt( const string &key, int def ) {
    string value = getString( key );
    if ( value != "" ) return Converter::strToInt( value);
    else return def;
}

//---------------------------------------------------------------------------
double ParamMap0::getDouble( const string &key, double def ) {
    string value = getString( key );
    if ( value != "" ) return Converter::strToDouble( value );
    else return def;
}

//---------------------------------------------------------------------------
float ParamMap0::getFloat( const string &key, float def ) {
    return getDouble( key, def );
}

//---------------------------------------------------------------------------
bool ParamMap0::getBool( const string &key, bool def ) {
    string value = getString( key );
    if ( value != "" ) {
        if ( value == "1" ) return true;
        else return false;
    }
    else return def;
}

//---------------------------------------------------------------------------
//x=y
void ParamMap0::setPair( const string &data ) {
    vector<string> list = ofSplitString( data, "=" );
    if ( list.size() >= 2 ) {
        setString( list[0], list[1] );
    }
}

//---------------------------------------------------------------------------
void ParamMap0::setString( const string &key, const string &value ) {
    //_values.insert( StringStringMapValueType( key, value) );
    _values[key] = value;
}

//---------------------------------------------------------------------------
void ParamMap0::setDouble( const string &key, double dValue )
{
    setString( key, Converter::doubleToStr( dValue ) );
}

//---------------------------------------------------------------------------
void ParamMap0::setFloat( const string &key, float value )
{
    setDouble( key, value );
}

//---------------------------------------------------------------------------
void ParamMap0::setInt( const string &key, int iValue )
{
    setString( key, Converter::intToStr( iValue ) );
}

//---------------------------------------------------------------------------
void ParamMap0::setBool( const string &key, bool bValue )
{
    string value;
    if (bValue) value = "1";
    else value = "0";
    setString( key, value );
}

//---------------------------------------------------------------------------
void ParamMap0::setPoint( const string &key, ofPoint p ) {
    setFloat( key + "_x", p.x );
    setFloat( key + "_y", p.y );
    setFloat( key + "_z", p.z );
}

//---------------------------------------------------------------------------
ofPoint ParamMap0::getPoint( const string &key, ofPoint defValue ) {
    defValue.x = getFloat( key + "_x", defValue.x );
    defValue.y = getFloat( key + "_y", defValue.y );
    defValue.z = getFloat( key + "_z", defValue.z );
    return defValue;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void Var::saveIni( ParamMap &ini ) {
    if ( type == Var_Float ) { ini.setFloat(name, var_float() ); }
    if ( type == Var_Int ) { ini.setInt(name, var_int() ); }
    if ( type == Var_String ) { ini.setString(name, var_string() ); }
}

//---------------------------------------------------------------------------
void Var::loadIni( ParamMap &ini ) {
    if ( type == Var_Float ) {
        var_float() = ini.getFloat(name, var_float() ); }
    if ( type == Var_Int ) { var_int() = ini.getInt(name, var_int() ); }
    if ( type == Var_String ) { var_string() = ini.getString(name, var_string() ); }
}

//---------------------------------------------------------------------------
void Var::fromString( const string &value ) {
    if ( type == Var_Float ) var_float() = ofToFloat( value );
    if ( type == Var_Int ) var_int() = ofToInt( value );
    if ( type == Var_String ) var_string() = value;
}

//---------------------------------------------------------------------------
string Var::toString() {
    if ( type == Var_Float ) return ofToString(var_float());
    if ( type == Var_Int ) return ofToString(var_float());
    if ( type == Var_String ) return var_string();
    return "";
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ParamMap::loadIni( string fileName ) {
    _fileName = fileName;
    vector<string> file = pbFiles::readStrings( ofToDataPath( fileName ) );
    loadFromStrings( file );
}

//---------------------------------------------------------------------------
void ParamMap::loadFromStrings( vector<string> &lines ) {
    clear();

    for (int i=0; i<lines.size(); i++) {
        setPair( lines[i] );
    }
    
    iniToVars();
}

//---------------------------------------------------------------------------
void ParamMap::varsUpdated() {      //копирует значения переменных в map
    for (int i=0; i<vars.size(); i++) {
        vars[i].saveIni( *this );
    }
}

//---------------------------------------------------------------------------
void ParamMap::iniToVars() {         //установить значения из ini в привязанные переменные
    for (int i=0; i<vars.size(); i++) {
        vars[i].loadIni( *this );
    }
}

//---------------------------------------------------------------------------
void ParamMap::saveIni( string fileName ) {
    varsUpdated();
    
    vector<string> file;
    for (StringStringMap::const_iterator iter = _values.begin();
         iter != _values.end(); iter++) {
        string key = iter->first;
        string value = iter->second;
        file.push_back( key + "=" + value );
    }
    pbFiles::writeStrings( file, ofToDataPath( fileName ) );
    _fileName = fileName;

}

//---------------------------------------------------------------------------
//Установка значения из OSC, не в ini, а только в vars
void ParamMap::setValue( const string &name, const string &value ) {
    int i = vars_index.getInt( name, -1 );
    if ( i >= 0 ) {
        vars[i].fromString( value );
        //cout << "set value " << vars[i].name << " " << value << endl;
    }
}

//---------------------------------------------------------------------------
string ParamMap::getValue( const string &name, const string &def ) {
    int i = vars_index.getInt( name, -1 );
    if ( i >= 0 ) {
        return vars[i].toString();
        //cout << "set value " << vars[i].name << " " << value << endl;
    }
    else return def;
}

//---------------------------------------------------------------------------
//Есть ли такая переменная
bool ParamMap::hasVar( const string &name ) {
    return ( vars_index.getInt( name, -1 ) >= -1 );
}

//---------------------------------------------------------------------------
void ParamMap::addFloat( float &value, string name, float value0 ) {
    check_new_var( &value, name );
    value = value0;
    Var var;
    var.VarFloat( value, name );
    vars.push_back( var );
    vars_index.setInt( name, vars.size()-1 );
    vars[vars.size()-1].loadIni( *this );
}

//---------------------------------------------------------------------------
void ParamMap::addInt( int &value, string name, int value0 ) {
    check_new_var( &value, name );
    value = value0;
    Var var;
    var.VarInt( value, name );
    vars.push_back( var );
    vars_index.setInt( name, vars.size()-1 );
    vars[vars.size()-1].loadIni( *this );

}

//---------------------------------------------------------------------------
void ParamMap::addString( string &value, string name, string value0 ) {
    check_new_var( &value, name );
    value = value0;
    Var var;
    var.VarString( value, name );
    vars.push_back( var );
    vars_index.setInt( name, vars.size()-1 );
    vars[vars.size()-1].loadIni( *this );

}

//---------------------------------------------------------------------------
void ParamMap::check_new_var( void *value, string name ) {  //проверка, что нет такой переменной
    for (int i=0; i<vars.size(); i++) {
        if ( value == vars[i].pointer ) {
            cout << "ERROR: duplicated var pointer in ParamMap: " << name << endl;
            OF_EXIT_APP(0);
        }
        if ( name == vars[i].name ) {
            cout << "ERROR: duplicated var name in ParamMap: " << name << endl;
            OF_EXIT_APP(0);
        }
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

