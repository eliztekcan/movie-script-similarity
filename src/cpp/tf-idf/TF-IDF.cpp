#include <bits/stdc++.h>
#define  fi        first
#define  se	   	   second
#define  pb		   push_back
using	 namespace std;
typedef  long long LL;
typedef  pair<int,int> pi;

const int DASH_VALUE = (int) '-';
const int SPACE_VALUE = (int) ' ';
const int CASE_DIFF_VALUE = (int) 'a' - (int) 'A';

const int MAX_CHAR = 256;
const int MAX_NO_DOC = 100;
const int MAX_NO_TOKEN = 100000;
const int MAX_FILE_NAME_SIZE = 100;

int n,m;

double idf[MAX_NO_TOKEN];
double tf[MAX_NO_DOC][MAX_NO_TOKEN];
double tfidf[MAX_NO_DOC][MAX_NO_TOKEN];

double distL[MAX_NO_DOC][MAX_NO_DOC];
double distCos[MAX_NO_DOC][MAX_NO_DOC];

int docFreq[MAX_NO_TOKEN];
int noAllTokens[MAX_NO_DOC];
int charConvertedValue[MAX_CHAR];
int cntToken[MAX_NO_DOC][MAX_NO_TOKEN];

char inputFileNames[MAX_NO_DOC][MAX_FILE_NAME_SIZE];

set<string>nonusedWords;
map<string,int>hashToken;
map<string,string>stemWords;
string tokens[MAX_NO_TOKEN];

void calcConvertChar() {
	for( int i = 0 ; i < MAX_CHAR ; i++ )
		charConvertedValue[i] = SPACE_VALUE;
	for( int i = (int) 'a' ; i <= (int) 'z' ; i++ )
		charConvertedValue[i] = charConvertedValue[i-CASE_DIFF_VALUE] = i;
	//charConvertedValue[DASH_VALUE] = DASH_VALUE;
}

void getStemmingWords() {
	cin.clear();
	freopen( "diffs.txt" , "r" , stdin );
	string word,word1,word2;
	int counter = 0;
	while( getline( cin , word ) ) {
		word1 = word2 = "";
		bool isSpace = false;
		int length = word.length();
		for( int i = 0 ; i < length ; i++ ) {
			if( word[i] == ' ' )
				isSpace = true;
			else if( isSpace )
				word2 += word[i];
			else
				word1 += word[i];
		}
		stemWords[word1] = word2;
	}
}

void getNonUsedWords() {
	cin.clear();
	freopen( "nonused.txt" , "r" , stdin );
	string word;
	while( getline( cin , word ) )
		nonusedWords.insert( word );
}

void getInputFileNames() {
	cin.clear();
	freopen( "inputFileNames.txt" , "r" , stdin );
	string inputName;
	while( getline( cin , inputName ) ) {
		int length = inputName.length();
		for( int i = 0 ; i < length ; i++ )
			inputFileNames[n][i] = inputName[i];
		inputFileNames[n++][length] = '\0';
	}
}

bool checkWord( string &word ) {
	int len = word.length();
	if( len <= 2 )
		return false;
	if( nonusedWords.find( word ) != nonusedWords.end() )
		return false;
	return true;
	/*
	for( int i = 0 ; i < len ; i++ )
		if( word[i] != '-' )
			return true;
	return false;
	*/
}

void calcTFIDF() {
	map<string,int>::iterator mit_si;
	map<string,string>::iterator mit_ss;
	for( int fileId = 0 ; fileId < n ; fileId++ ) {
		cin.clear();
		freopen( inputFileNames[fileId] , "r" , stdin );
		string line;
		while( getline( cin , line ) ) {
			int length = line.length();
			for( int i = 0 , j = 0 ; i < length ; i = ++j ) {
				int ci = charConvertedValue[ (int) line[i] ];
				if( ci == SPACE_VALUE ) {
					while( j+1 < length && charConvertedValue[ (int) line[j+1] ] == SPACE_VALUE )
						j++;
				}
				else {
					string word = "";
					char c = (char) charConvertedValue[ (int) line[i] ];
					word.pb( c );
					while( j+1 < length && charConvertedValue[ (int) line[j+1] ] != SPACE_VALUE ) {
						char c = (char) charConvertedValue[ (int) line[++j] ];
						word.pb( c );
					}
					mit_ss = stemWords.find( word );
					if( mit_ss != stemWords.end() )
						word = (*mit_ss).se;
					if( checkWord(word) ) {
						int tokenId;
						mit_si = hashToken.find( word );
						if( mit_si == hashToken.end() ) {
							hashToken[word] = tokenId = m++;
							tokens[tokenId] = word;
						}
						else
							tokenId = (*mit_si).se;
						noAllTokens[fileId]++;
						if( ++cntToken[fileId][tokenId] == 1 )
							docFreq[tokenId]++;
					}
				}
			}
		}
	}
	printf( "# All words : %d\n" , m );
	for( int i = 0 ; i < n ; i++ )
		cout << inputFileNames[i] << " : " << noAllTokens[i] << endl;
	for( int i = 0 ; i < m ; i++ )
		idf[i] = log10( (double) n / docFreq[i] );
	for( int i = 0 ; i < n ; i++ )
		for( int j = 0 ; j < m ; j++ ) {
			tf[i][j] = (double) cntToken[i][j] / noAllTokens[i];
			tfidf[i][j] = tf[i][j] * idf[j];
		}
}

double calcPower( double value , int pwr ) {
	if( pwr == 0 )
		return 1;
	double res = calcPower( value , pwr / 2 );
	res *= res;
	if( pwr % 2 )
		res *= value;
	return res;
}

double calcDistLX( int docX , int docY , int dim ) {
	double ans = 0;
	// dim = inf
	if( dim == -1 )
		for( int i = 0 ; i < m ; i++ )
			ans = max( ans , fabs( tfidf[docX][i] - tfidf[docY][i] ) );
	else {
		for( int i = 0 ; i < m ; i++ )
			ans += calcPower( fabs( tfidf[docX][i] - tfidf[docY][i] ) , dim );
		ans = pow( ans , 1.0 / dim );
	}
	return ans;
}

double calcDistCos( int docX , int docY ) {
	double multSum = 0 , lenX = 0 , lenY = 0;
	for( int i = 0 ; i < m ; i++ ) {
		multSum += tfidf[docX][i] * tfidf[docY][i];
		lenX += tfidf[docX][i] * tfidf[docX][i];
		lenY += tfidf[docY][i] * tfidf[docY][i];
	}
	lenX = sqrt( lenX );
	lenY = sqrt( lenY );
	double cosVal = multSum / ( lenX * lenY );
	double tetaVal = acos( cosVal );
	double cosDist = tetaVal * 180.0 / M_PI;
	return cosDist;
}

void calcAllDistances() {
	for( int i = 0 ; i < n ; i++ )
		for( int j = i + 1 ; j < n ; j++ )
			distL[i][j] = distL[j][i] = calcDistLX( i , j , 2 );
	for( int i = 0 ; i < n ; i++ )
		for( int j = i + 1 ; j < n ; j++ )
			distCos[i][j] = distCos[j][i] = calcDistCos( i , j );
	for( int i = 0 ; i < n ; i++ )
		for( int j = i ; j < n ; j++ )
			cout << i << " , " << j << " : " << "Cos = " << distCos[i][j] << " / " << "L2 = " << distL[i][j] << endl;
}

void test() {
	n = 3;
	m = 3;
	for( int i = 0 ; i < 3 ; i++ )
		tfidf[i][i] = 1;
	calcAllDistances();
	while(true);
}

int main() {
	//test();
	calcConvertChar();
	getNonUsedWords();
	getStemmingWords();
	getInputFileNames();
	calcTFIDF();
	calcAllDistances();
	return 0;
}
