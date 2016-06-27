#include "cmUtils.h"
#include "cmLog.h"

#ifdef QT
#include <QString>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QEventLoop>
#include <QDirIterator>
#include <qdatetime.h>
#endif

#include <iomanip>
//#include <windows.h>
#include <fstream>
#include <iostream>
#include "cmMath.h"

#ifndef OS_DEFINED
    ERROR! - нужно определить OS_NAME, OS_NAME_SHORT, OS_DEFINED
#endif

#ifdef OS_WINDOWS
#include <Windows.h>
#endif


//----------------------------------------
int cmCompareU8_pointers(const void * a, const void * b) {
    return ( *(u8*)a - *(u8*)b );
}

//----------------------------------------
int cmCompareInt_pointers(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

//----------------------------------------
int cmCompareFloat_pointers(const void * a, const void * b) {
    return ( *(float*)a - *(float*)b );
}

//----------------------------------------
int cmCompareInt(int a, int b ) {
    return ( a - b );
}

//----------------------------------------
int cmCompareFloat( float a, float b ) {
    return ( a - b );
}

//----------------------------------------
int cmCompareDouble( float a, float b ) {
    return ( a - b );
}

//----------------------------------------
void cmSortInt( vector<int>& v ) {
    cmSort( v, cmCompareInt );
}

//----------------------------------------
void cmSortFloat( vector<float>& v ) {
    cmSort( v, cmCompareFloat );
}

//----------------------------------------
int cm_compare_double (const void * a, const void * b)
{
  return ( *(double*)a - *(double*)b );
}

//----------------------------------------
void cmSortDouble( vector<double>& values ) {
    qsort( &values[0], values.size(), sizeof(values[0]), cm_compare_double );
    //cmSort( v, cmCompareDouble );
}

//----------------------------------------
int cmToInt(const string& intString) {
	int x = 0;
	istringstream cur(intString);
	cur >> x;
	return x;
}

//----------------------------------------
u32 cmToUInt32(const string& intString) {
    u32 x = 0;
    istringstream cur(intString);
    cur >> x;
    return x;
}
//---------------------------------------
u8 cmToU8(const string& intString) {
    u8 x = 0;
    istringstream cur(intString);
    cur >> x;
    return x;
}

//----------------------------------------
float cmToFloat(const string& floatString) {
	float x = 0;
	istringstream cur(floatString);
	cur >> x;
	return x;
}

//----------------------------------------
double cmToDouble(const string& doubleString) {
	double x = 0;
	istringstream cur(doubleString);
	cur >> x;
	return x;
}

//----------------------------------------
ldouble cmToLDouble( const string& ldoubleString ) {
    ldouble x = 0;
    istringstream cur(ldoubleString);
    cur >> x;
    return x;
}


//----------------------------------------
char cmToChar(const string& charString) {
	char x = '\0';
	istringstream cur(charString);
	cur >> x;
	return x;
}

//----------------------------------------
int cmToInt(const string& intString, int defValue ) {
    return (intString.empty()) ? defValue : cmToInt( intString );
}

//----------------------------------------
char cmToChar(const string& charString, char defValue) {
    return (charString.empty()) ? defValue : cmToChar( charString );
}

//----------------------------------------
float cmToFloat(const string& floatString, float defValue) {
    return (floatString.empty()) ? defValue : cmToFloat( floatString );
}

//----------------------------------------
double cmToDouble(const string& doubleString, double defValue) {
    return (doubleString.empty()) ? defValue : cmToDouble( doubleString );
}

//--------------------------------------------------
ldouble cmToLDouble(const string& ldoubleString, ldouble defValue) {
    return (ldoubleString.empty()) ? defValue : cmToLDouble( ldoubleString );
}

//--------------------------------------------------
vector <string> cmSplitString(const string &source, const string &delimiter, bool ignoreEmpty) {
	vector<string> result;
	if (delimiter.empty()) {
		result.push_back(source);
		return result;
	}
	string::const_iterator substart = source.begin(), subend;
	while (true) {
		subend = search(substart, source.end(), delimiter.begin(), delimiter.end());
		string sub(substart, subend);
		if (!ignoreEmpty || !sub.empty()) {
			result.push_back(sub);
		}
		if (subend == source.end()) {
			break;
		}
		substart = subend + delimiter.size();
	}
	return result;
}

//--------------------------------------------------
vector <float> cmSplitStringf (const string & source, const string & delimiter, bool ignoreEmpty) {
    vector<string> list = cmSplitString( source, delimiter, ignoreEmpty );
    int n = list.size();
    vector<float> res(n);
    for (int i=0; i<n; i++) {
        res[i] = cmToFloat( list[i] );
    }
    return res;
}

//--------------------------------------------------
vector <double> cmSplitStringd (const string & source, const string & delimiter, bool ignoreEmpty) {
    vector<string> list = cmSplitString( source, delimiter, ignoreEmpty );
    int n = list.size();
    vector<double> res(n);
    for (int i=0; i<n; i++) {
        res[i] = cmToDouble( list[i] );
    }
    return res;
}

//--------------------------------------------------
vector <int> cmSplitStringi (const string & source, const string & delimiter, bool ignoreEmpty ) {
    vector<string> list = cmSplitString( source, delimiter, ignoreEmpty );
    int n = list.size();
    vector<int> res(n);
    for (int i=0; i<n; i++) {
        res[i] = cmToInt( list[i] );
    }
    return res;
}

//--------------------------------------------------
string cmJoinString(vector <string> stringElements, const string & delimiter){
	string resultString = "";
	int numElements = stringElements.size();

	for(int k = 0; k < numElements; k++){
		if( k < numElements-1 ){
			resultString += stringElements[k] + delimiter;
		} else {
			resultString += stringElements[k];
		}
	}

	return resultString;
}

//--------------------------------------------------
bool cmStringReplace(string& input, string searchStr, string replaceStr){
	size_t uPos = 0; 
	size_t uFindLen = searchStr.length(); 
	size_t uReplaceLen = replaceStr.length();
		
    bool found = false;
	if( uFindLen == 0 ){
        return found;
	}

	for( ;(uPos = input.find( searchStr, uPos )) != std::string::npos; ){
		input.replace( uPos, uFindLen, replaceStr );
		uPos += uReplaceLen;
        found = true;
	}	
    return found;
}

//--------------------------------------------------
void cmStringTrim(string& input, bool trimSpace, bool trimTab, bool trimBSlashN ) {
    cmStringTrimLeft( input, trimSpace, trimTab, trimBSlashN );
    cmStringTrimRight( input, trimSpace, trimTab, trimBSlashN );
}

//--------------------------------------------------
void cmStringTrimLeft(string& input, bool trimSpace, bool trimTab, bool trimBSlashN ) {
    while (!input.empty()) {
        char c = input[0];
        if ( (trimSpace && c == ' ') || (trimTab && c == '\t') || (trimBSlashN && (c == '\n' || c == '\r') ) ) {
            input = input.substr(1);
        }
        else break;
    }
}

//--------------------------------------------------
void cmStringTrimRight(string& input, bool trimSpace, bool trimTab, bool trimBSlashN ) {
     while (!input.empty()) {
        int i=int(input.length()-1);
        char c = input[i];
        if ( (trimSpace && c == ' ') || (trimTab && c == '\t') || (trimBSlashN && (c == '\n' || c == '\r') ) ) {
            input = input.substr(0, input.length()-1);
        }
        else break;
    }
}

//--------------------------------------------------
void cmStringRemovePathSymbols( string& input )    //удаление из строки символов : . \\ / ? *
{
    cmStringReplace( input, ":", "" );
    cmStringReplace( input, ".", "" );
    cmStringReplace( input, "\\", "" );
    cmStringReplace( input, "/", "" );
    cmStringReplace( input, "?", "" );
    cmStringReplace( input, "*", "" );

}

//--------------------------------------------------
bool cmIsStringInString(string haystack, string needle){
	return ( strstr(haystack.c_str(), needle.c_str() ) != NULL );
}

//--------------------------------------------------
string cmToLower(const string & src){
	string dst(src);
	transform(src.begin(),src.end(),dst.begin(),::tolower);
	return dst;
}

//--------------------------------------------------
string cmToUpper(const string & src){
	string dst(src);
	transform(src.begin(),src.end(),dst.begin(),::toupper);
	return dst;
}

//--------------------------------------------------
bool cmStringBeginsWith( const string &s, const string &prefix ) {
    return ( s.substr(0, prefix.length()) == prefix );
}

//--------------------------------------------------
bool cmStringEndsWith( const string &s, const string &suffix ) {
    return ( s.length() >= suffix.length() && s.substr(s.length()-suffix.length(), suffix.length()) == suffix );
}

//--------------------------------------------------
char cmStringLastSymbol( const string &s ) {
    if ( !s.empty() ) return s[ s.length()-1 ];
    else return 0;
}

//--------------------------------------------------
bool cmStringIsInt( const string &s ) {  //является ли строка целым числом
    string s1 = s;
    cmStringTrim( s1 );
    for (size_t i=0; i<s1.length(); i++) {
        char c = s1[i];
        if ( !((c >= '0' && c <= '9') || (i==0 && (c == '+' || c == '-')) ) ) {
            return false;
        }
    }
    return true;
}

//--------------------------------------------------
bool cmFileExists( string filename ) {
	ifstream inp;
	inp.open(filename.c_str(), ifstream::in);
    //inp.close();
	return !inp.fail();
}

//--------------------------------------------------
u64 cmFileSize( string fileName ) {
    //struct stat stat_buf;
    //int rc = stat( fileName.c_str(), &stat_buf );
    //return rc == 0 ? stat_buf.st_size : 0; //-1;

    u64 size = 0;
    FILE *file = fopen( fileName.c_str(),"rb" );
    if ( file ) {
        fseeko64( file, 0, SEEK_END );
        //TODO контроль того что ошибка
        size = ftello64( file );
        fclose( file );
    }
    return size;
}

//--------------------------------------------------
string cmFileRemoveExtension( string fileName )    //имя файла без расширения
{
    if ( !fileName.empty() ) {
        int n = fileName.length();
        int k = cmFileGetExtension( fileName ).length();
        return fileName.substr( 0, n-k-1 );
    }
    return "";
}

//--------------------------------------------------
string cmFileGetExtension( string fileName )         //расширение файла
{
    if ( !fileName.empty() ) {
        int n = fileName.size();
        for (int i=n-1; i>=0; i--) {
            unsigned char c = fileName[i];
            if ( c == '.' ) {
                return fileName.substr(i+1);
            }
            if ( c == '/' || c == '\\' ) break;
        }
    }
    return "";
}

//--------------------------------------------------
string cmFileGetName( string file ) {   //убирает из имени файла путь
    QFileInfo fileInfo( file.c_str() );
    QString filename(fileInfo.fileName());
    return filename.toUtf8().constData();
}

//--------------------------------------------------
string cmFileGetPath(string file ) {   //возвращает путь к файлу, без имени
    QFileInfo fileInfo( file.c_str() );
    QString path(fileInfo.path());
    return path.toUtf8().constData();
}

//--------------------------------------------------
string cmFileGetAbsolutePath( string file ) {   //возвращает полный путь к файлу
    QFileInfo fileInfo( file.c_str() );
    QString path(fileInfo.absolutePath());
    return path.toUtf8().constData();
}

//--------------------------------------------------
string cmDirGetAbsolutePath( string dir0 ) {   //возвращает полный путь к папке, без '~'
    QDir dir( dir0.c_str() );
    QString path(dir.absolutePath());//canonicalPath());
    return path.toUtf8().constData();
}

//--------------------------------------------------
u64 cmDirSize( string path ) {   //Место, занимаемое файлами папки
    //vector<string> files = cmFilesFind( path, "*");

    u64 size = 0;

    QDirIterator it(path.c_str(), QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        size += it.fileInfo().size();
        it.next();
    }
    return size;
}

//--------------------------------------------------
vector<string> cmFilesFind(string path, string fileMask) {  //поиск файлов по маске
    path = cmPathToQt( path );

    QDir currentDir(path.c_str());
    QString fileMaskqs = fileMask.c_str();
    if (fileMaskqs.isEmpty()) fileMaskqs = "*";
    QStringList files = currentDir.entryList(QStringList(fileMaskqs), QDir::Files | QDir::NoSymLinks);
    size_t n = files.size();
    vector<string> list(n);
    for (size_t i=0; i<n; i++) {
        list[i] = files[i].toUtf8().constData();
    }
    return list;
}

//--------------------------------------------------
FILE *cmFileAppendBinary( string fileName )  //открыть для дописывания
{
    FILE *file = fopen( fileName.c_str(),"a+b" );
    fseeko64( file, 0, SEEK_END );
    return file;
}

//----------------------------------------
void cmFileAppendText( const string &fileName, const string &text ) {
    ofstream file(fileName.c_str(), ios::out | ios::app);
    file << text <<endl;
    file.close();
}

//--------------------------------------------------
FILE *cmFileReadPos( string fileName, u64 pos )  //открыть для чтения
{
	FILE *file = fopen( fileName.c_str(),"rb" );
    fseeko64( file, pos, SEEK_SET );
    return file;
}

//--------------------------------------------------
FILE *cmFileUpdatePos( string fileName, u64 pos )  //открыть для обновления части файла
{
    FILE *file = fopen( fileName.c_str(),"r+b" );
    fseeko64( file, pos, SEEK_SET );
    return file;
}


//--------------------------------------------------
void cmFileSaveArray( FILE *file, void *arr, size_t size )  { //запись массива в файл, блоками
    u8 *data = (u8 *)arr;
    //fwrite( arr, 1, size, file );
    size_t block = 50 * 1000 * 1000;   //размер блока
    bool print = ( size > 2*block );
    size_t k = 0;
    while ( k < size ) {
        size_t k1 = min( k + block, size );
        fwrite( data + k, 1, k1 - k, file );
        cmAssert( !ferror(file), "cmFileSaveArray - error saving to file, no memory on disk? (attempt to write "
                  + cmToString( size ) + " bytes" );

        k = k1;
        if ( print ) {
            cmLog << " " << int(double(k) / size * 100)<< "%" << endl;
        }
    }

}

//--------------------------------------------------
void cmFileLoadArray( FILE *file, void *arr, size_t size ) {  //считывание массива из файла, блоками
    u8 *data = (u8 *)arr;
    //fwrite( arr, 1, size, file );
    size_t block = 100 * 1000 * 1000;   //размер блока
    bool print = ( size > 2*block );
    size_t k = 0;
    while ( k < size ) {
        size_t k1 = min( k + block, size );
        fread( data + k, 1, k1 - k, file );
        cmAssert( !ferror(file), "cmFileLoadArray - error loading from file" );

        k = k1;
        if ( print ) {
            cmLog << " " << int(double(k) / size * 100) << "%" << endl;
        }
    }
    //if ( print ) {
    //    cmLog << endl;
    //}
}

//--------------------------------------------------
bool cmFileCopyProgress( string source, string dest, float progress0, float progress1 ) { // копирование файлов с PROGRESS
    if (!cmFileExists(source)) return false;

    u64 size = cmFileSize( source );
    //cout << "MESSAGE_DLG: " << " file size " << size << endl;

    FILE *inFile = fopen(source.c_str(), "rb");
    cmAssert(inFile, "Cannot open " + source);

    FILE *outFile = fopen(dest.c_str(), "wb");
    if(outFile == 0)
        fclose(inFile);

    cmAssert(outFile, "Cannot open " + dest);

    u64 block = 100 * 1000 * 1000;
    vector<u8> data( block );
    u64 k = 0;

    bool res = true;
    while (k < size) {
        u64 k1 = min(k + block, size);
        fread(&data[0], 1, k1 - k, inFile);
        fwrite(&data[0], 1, k1 - k, outFile);
        //cmAssert(!ferror(inFile), "cmCopyFileProgress - error loading from file");
        if ( ferror(inFile) ) { res = false; break; }

        int progress = int( cmMap( double(k1)/double(size), 0, 1, progress0, progress1 ) + 0.5 );
        cmLogProgress << progress << endl;
        k = k1;
    }

    fclose(inFile);
    fclose(outFile);
    return res;
}

//--------------------------------------------------
vector<string> cmFileReadStrings( string fileName )    //считать текстовый файл в строки
{
    vector<string> lines;
    cmAssert( cmFileExists( fileName ), "cmFileReadStrings no file " + fileName );
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
}

//--------------------------------------------------
string cmFileReadString( string fileName )    //считать текстовый файл в строку, с контролем \r для Linux
{

    cmAssert( cmFileExists( fileName ), "cmFileReadString - file '" + fileName + "' not found" );

    //TODO на linux последний символ "\r" отрезает всё строку, поэтому такое считывание сделал
    string value = "";
    FILE *f = fopen( fileName.c_str(), "rb" );
    if ( f ) {
        while (!feof(f)) {
            char c;
            fread( &c, 1, 1, f );
            if (feof(f)) break;
            if ( c == '\r' || c == '\n' ) break;
            value += c;
        }
        fclose( f );
    }
    return value;
}

//--------------------------------------------------
void cmFileWriteStrings( const vector<string> &list, string fileName )    //записать строки в текстовый файл
{
    ofstream f(fileName.c_str(),ios::out);
    cmAssert( !f.fail(), "cmFileWriteStrings - error creating file " + fileName );
    for ( size_t i=0; i<list.size(); i++ ) {
        f << list[i] << endl;
    }
    cmAssert( !f.fail(), "cmFileWriteStrings - error writing file " + fileName );
    f.close();
}

//--------------------------------------------------
void cmFileWriteString( const string &line, const string &fileName )    //записать строку в текстовый файл
{
    vector<string> file;
    file.push_back( line );
    cmFileWriteStrings( file, fileName );
}

//--------------------------------------------------
int cmFileReadInt( string fileName ) {
    //vector<string> file = cmFileReadStrings( fileName );
    //cmAssert( file.size() > 0, "cmFileReadInt, bad file " + fileName );
    string file = cmFileReadString( fileName );
    cmAssert( !file.empty(), "cmFileReadInt, bad file " + fileName );
    return cmToInt( file );
}

//--------------------------------------------------
void cmFileWriteInt( int value, string fileName ) {
    vector<string> file;
    file.push_back( cmToString(value) );
    cmFileWriteStrings( file, fileName );

}

//--------------------------------------------------
string cmFetchExePath() //путь к исполняемому файлу
{
#ifdef QT
    QString qs;
    QFileInfo fi(QCoreApplication::applicationFilePath());
    qs = fi.absolutePath();
    string s = qs.toUtf8().constData();
    return s;
#else
	cmAssert( false, "cmGetExePath not implemented" );
	return "";
#endif

}

//--------------------------------------------------
string cmPathToWindows( string path ) {
    cmStringReplace( path, "/", "\\" );
    return path;
}

//--------------------------------------------------
string cmPathToLinux( string path ) {
    cmStringReplace( path, "\\", "/" );
    return path;
}

//--------------------------------------------------
string cmPathToQt( string path ) { //сконвертировать путь для Qt
    return cmPathToLinux( path );
}

//--------------------------------------------------
string cmPathToOS( string path ) //сконвертировать путь по ОС (\ в Windows, / в Linux)
{
#ifdef OS_WINDOWS
    return cmPathToWindows( path );
#endif
#ifdef OS_LINUX
    return cmPathToLinux( path );
#endif
    return path;
}

//--------------------------------------------------
bool cmMakeDir( string path )  //создать папку
{
    int n = path.length();
    if ( n > 0 ) {
       while ( path[n-1] == '/' || path[n-1] == '\\' ) {
           path = path.substr( 0, n-1 );
       }
       //Версия Windows и без проверки
       //string command = "mkdir " + path;
       //cmLog << "Make dir by command " << command << endl;
       //system( command.c_str() );
       //return true;

       //Версия Qt
       path = cmPathToQt( path );
       QDir dir( path.c_str() );
       if (!dir.exists()) {
           dir.mkpath(".");
       }
       return dir.exists();

       //cmAssert( QDir( path.c_str() ).exists(), "cmMakeDir - can not create directory " + path );
    }
    return true;    //TODO следует поставить false, но нужно всё протестировать!
}

//--------------------------------------------------
bool cmDirRename( string from, string to ) { //переименование папки

    from = cmPathToQt( from );
    to = cmPathToQt( to );
    //QDir dir;//( from.c_str() );
    return QDir().rename( from.c_str(), to.c_str() );
}

//--------------------------------------------------
bool cmDirDelete( string path ) {   //удаление папки
    path = cmPathToQt( path );
    QDir dir( path.c_str() );
    return dir.removeRecursively();
}

//--------------------------------------------------
bool cmDirExists( string path )  //существует ли папка
{
    path = cmPathToQt( path );
    QDir dir( path.c_str() );
    return dir.exists();
}

//--------------------------------------------------
bool cmCd( string path ) {       //перейти в каталог
    path = cmPathToQt( path );
    cout << "cmCd " << path << endl;
    return QDir::setCurrent( path.c_str() );
}

//--------------------------------------------------
bool cmFileCopy( string from, string to ) {  //копирование файла
    from = cmPathToQt( from );
    to = cmPathToQt( to );
    //cout << "------------ from " << from << " to " << to << endl;

    if ( !cmFileExists(from) ) return false;

    bool res = true;
    //нужно удалить файл, так как QFile не может сам перезатереть
    if ( cmFileExists(to) ) {
        res = cmFileDelete(to);
    }
    if ( res ) res = QFile().copy( from.c_str(), to.c_str() );
    //cmLog << "result: " << res << endl;
    return res;
}

//--------------------------------------------------
bool cmFileDelete( string path ) {  //удаление файла
    path = cmPathToQt( path );
    bool res = true;
    if ( cmFileExists( path ) ) {
        res = QFile().remove( path.c_str() );
    }
    if ( res ) res = !cmFileExists( path );
    return res;
}

//--------------------------------------------------
//Cвободное место на диске, из которого запущена программа

#ifdef OS_LINUX
    #include <sys/statvfs.h>
    #include <QDir>
#endif

u64 cmDiskFreeSpace() {
#ifdef OS_WINDOWS
    __int64 FreeBytesAvailable;
    __int64 TotalNumberOfBytes;
    __int64 TotalNumberOfFreeBytes;

    BOOL GetDiskFreeSpaceFlag = GetDiskFreeSpaceEx(NULL, // по умолчанию: откуда запущен проект
    (PULARGE_INTEGER)&FreeBytesAvailable, // доступно для использования(байт)
    (PULARGE_INTEGER)&TotalNumberOfBytes, // максимальный объём(в байтах)
    (PULARGE_INTEGER)&TotalNumberOfFreeBytes); // свободно на диске(в байтах)

    cmAssert(GetDiskFreeSpaceFlag != 0, "Function GetDiskFreeSpaceEx is called incorrectly.");

    return FreeBytesAvailable;
#endif

#ifdef OS_LINUX
//    cmExit( "cmDiskFreeSpace is not implemented in Linux!" );
//    return 0;
    struct statvfs stat;
    const char *path = QDir::currentPath().toUtf8().data();
    cmAssert(statvfs(path, &stat) == 0, "Function statvfs is called incorrectly.");
    return stat.f_bsize * stat.f_bavail;
#endif
}

//--------------------------------------------------
//------------------------------------BinC--------------
//--------------------------------------------------
void cmStringToBuffer( string s, u8 *buffer, int bufferSize )
{
    s = s.substr( 0, bufferSize );
    memset( buffer, 0, bufferSize );
    for ( unsigned int i=0; i<s.length(); i++) {
        buffer[i] = s[i];
    }
}

//--------------------------------------------------
string cmBufferToString( u8 *buffer, int bufferSize )
{
    string s;
    for (int i=0; i<bufferSize; i++) {
        if ( buffer[i] == 0 ) { break; }
        s += buffer[i];
    }
    return s;
}

//--------------------------------------------------
time_t cm_utils_time0 = 0;

void cmSetup() {
    time( &cm_utils_time0 );
}

//--------------------------------------------------
int cmElapsedSecondsi() {
    cmAssert( cm_utils_time0 > 0, "cmElapsedSecondsi not inited, please call cmSetup" );
    time_t t;
    time( &t );
    return difftime( t, cm_utils_time0 );
}

//--------------------------------------------------
void cmSleepMs(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}

//--------------------------------------------------
string cmTimeString() {  //текущее время, в виде строки
    return QDateTime::currentDateTime().toString().toUtf8().constData();
}
//--------------------------------------------------
bool cmIsInt(string s){
    cmAssert(s.length()!=0, "empty line in cmIsInt");
    vector<bool> znach;
    bool ret_znach = true;
    size_t pos = s.find(".",0);
    if (pos==string::npos) {
        for (int i=0; i<s.length(); i++){
            if((s[i]=='-'||s[i]=='+')&&i==0) continue;
            else {
                if (s[i]=='1'||s[i]=='2'||s[i]=='3'||s[i]=='4'||s[i]=='5'||s[i]=='6'||s[i]=='7'||s[i]=='8'||s[i]=='9')
                    znach.push_back(true);
                else {
                    if ((i!=0 || s.length()==1)&& s[i]=='0') znach.push_back(true);
                    else{
                    znach.push_back(false);
                    //cout<<"false1"<<endl;
                    }
                }
                if (i==0){
                    if(s[i]=='0') {
                        if (s.length()==1) znach.push_back(true);
                        else{
                        znach.push_back(false);
                        //cout<<"false2"<<endl;
                        }
                    }
                }
                else if((s[i-1]=='-'||s[i-1]=='+')&& s[i]=='0') {
                    znach.push_back(false);
                    //cout<<"false3"<<endl;
                }
                     else znach.push_back(true);
            }
        }
        for (int j=0; j<znach.size(); j++){
            if(znach[j]==false) ret_znach = false;
        }
    }
    else {
        ret_znach=false;
        //cout<<"false4"<<endl;
    }
    return ret_znach;
}
//-------------------------------------------------
bool cmIsFloat(string s){
    bool ret_znach=true;
    vector<bool> znach;
    cmAssert (s.length()!=0, "empty line in cmIsFloat");
    size_t pos = s.find(".",0);
    if (pos!=string::npos) {
        size_t pos1 = s.find(".",pos+1);
        if (pos1!=string::npos) znach.push_back(false);
        else{
            if (pos==0) znach.push_back(false);
            else{
                for (int i=0; i<pos; i++){
                    int size = s.substr(0,pos).size();
                    if (s[i]=='1'||s[i]=='2'||s[i]=='3'||s[i]=='4'||s[i]=='5'||s[i]=='6'||s[i]=='7'||s[i]=='8'||s[i]=='9'||(s[i]=='0'&& (size==1 ||size==2))
                                                                                                                 ||(s[i]=='+'&&i==0)||(s[i]=='-' && i==0))
                        znach.push_back(true);
                    else znach.push_back(false);
                }
                for (int j=pos+1; j<s.length(); j++){
                    if (s[j]=='1'||s[j]=='2'||s[j]=='3'||s[j]=='4'||s[j]=='5'||s[j]=='6'||s[j]=='7'||s[j]=='8'||s[j]=='9'||s[j]=='0')
                        znach.push_back(true);
                    else znach.push_back(false);
                }
            }

        }

    }
    else znach.push_back(false);
    for (int k=0; k<znach.size(); k++){
        if (znach[k]==false) ret_znach=false;
    }
    return ret_znach;
}
