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
int maxFreq; // max of cntToken(doc,token)
double pearsonSimMultiplier; // 10 / maxFreq

double idf[MAX_NO_TOKEN];
double tf[MAX_NO_DOC][MAX_NO_TOKEN];
double tfidf[MAX_NO_DOC][MAX_NO_TOKEN];

double distL[MAX_NO_DOC][MAX_NO_DOC];
double distCos[MAX_NO_DOC][MAX_NO_DOC];
double pearsonSim[MAX_NO_DOC][MAX_NO_DOC];

int docFreq[MAX_NO_TOKEN]; // number of docs that has a specific token at least once
int noAllTokens[MAX_NO_DOC]; // number of all tokens in a specific doc
int noDiffTokens[MAX_NO_DOC]; // mumber of different tokens in a specific doc
int charConvertedValue[MAX_CHAR]; // uppercase -> lowercase etc.
int cntToken[MAX_NO_DOC][MAX_NO_TOKEN]; // # of appearance of a token in a doc

double avgFreq[MAX_NO_DOC]; // avg of cntToken(doc,token) for each doc

char inputFileNames[MAX_NO_DOC][MAX_FILE_NAME_SIZE];

set<string>nonusedWords; // stop words
map<string,int>hashToken; // id of tokens
map<string,string>stemWords; // hashing of words to original ones

string tokens[MAX_NO_TOKEN]; // list of different words extracted from scripts

// Map characters to other characters like uppercase -> lowercase
// Also consider other characters as spaces
// Dash '-' can be considered as '-' or space, not sure!
void calcConvertChar() {
	for( int i = 0 ; i < MAX_CHAR ; i++ )
		charConvertedValue[i] = SPACE_VALUE;
	for( int i = (int) 'a' ; i <= (int) 'z' ; i++ )
		charConvertedValue[i] = charConvertedValue[i-CASE_DIFF_VALUE] = i;
	//charConvertedValue[DASH_VALUE] = DASH_VALUE;
}

// Map stemming words to originals (roots)
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

// Copy words in the text to stop-words set
void getNonUsedWords() {
	cin.clear();
	freopen( "nonused.txt" , "r" , stdin );
	string word;
	while( getline( cin , word ) )
		nonusedWords.insert( word );
}

// Get names of script files that will be used during testing
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

// Check if a word should be used in TF-IDF
// Returns true if it should be used, false otherwise
bool checkWord( string &word ) {
	int len = word.length();
	// All words with length <= 2 are stop words
	if( len <= 2 )
		return false;
	// Check if given word is a stop word or not
	if( nonusedWords.find( word ) != nonusedWords.end() )
		return false;
	return true;
	/*
	// A word may be represented as full of '-' characters
	// May spend too much time unnecessarily
	for( int i = 0 ; i < len ; i++ )
		if( word[i] != '-' )
			return true;
	return false;
	*/
}

// Calculation of TF-IDF
void calcTFIDF() {
	// STL Map iterators
	map<string,int>::iterator mit_si;
	map<string,string>::iterator mit_ss;
	// Look for all script files
	for( int fileId = 0 ; fileId < n ; fileId++ ) {
		cin.clear();
		freopen( inputFileNames[fileId] , "r" , stdin );
		string line;
		// Look for all lines of the file, file may consist of more than 1 line
		while( getline( cin , line ) ) {
			int length = line.length();
			// Find words in the line, use 2 pointers (left & right) logic
			for( int i = 0 , j = 0 ; i < length ; i = ++j ) {
				int ci = charConvertedValue[ (int) line[i] ];
				// Skip spaces
				if( ci == SPACE_VALUE ) {
					while( j+1 < length && charConvertedValue[ (int) line[j+1] ] == SPACE_VALUE )
						j++;
				}
				// Currently, looking for a word
				else {
					// Get the next word
					string word = "";
					char c = (char) charConvertedValue[ (int) line[i] ];
					word.pb( c );
					while( j+1 < length && charConvertedValue[ (int) line[j+1] ] != SPACE_VALUE ) {
						char c = (char) charConvertedValue[ (int) line[++j] ];
						word.pb( c );
					}
					// Check if it's a stemming word that means its original is different
					// If it's so, convert it to the original one
					mit_ss = stemWords.find( word );
					if( mit_ss != stemWords.end() )
						word = (*mit_ss).se;
					// Check if it's a step word or not
					if( checkWord(word) ) {
						// Check if the word is in the list of words
						// that are extracted from script files up to now
						int tokenId;
						mit_si = hashToken.find( word );
						// If the word's not in the list, add give next id to that word
						if( mit_si == hashToken.end() ) {
							hashToken[word] = tokenId = m++;
							tokens[tokenId] = word;
						}
						// Otherwise, use the id of word that is given before
						else
							tokenId = (*mit_si).se;
						// Increase # of tokens (words including the same ones) by 1
						// It's needed to calculate tf in later steps
						noAllTokens[fileId]++;
						// Update max frequency
						maxFreq = max( maxFreq , ++cntToken[fileId][tokenId] );
						// If it's appearance of the word in the doc
						// Then increase appearance of that token's id in different docs by 1
						// Also increase number of different tokens in that doc
						if( cntToken[fileId][tokenId] == 1 )
							docFreq[tokenId]++ , noDiffTokens[fileId]++;
					}
				}
			}
		}
		avgFreq[fileId] = (double) noAllTokens[fileId] / noDiffTokens[fileId];
	}
	// Display all words in the list
	printf( "# All words : %d\n" , m );
	for( int i = 0 ; i < n ; i++ )
		cout << inputFileNames[i] << " : " << noAllTokens[i] << endl;
	// Calculate idf of the words in the list
	for( int i = 0 ; i < m ; i++ )
		idf[i] = log10( (double) n / docFreq[i] );
	// Calculate tf, then tf-idf
	for( int i = 0 ; i < n ; i++ )
		for( int j = 0 ; j < m ; j++ ) {
			tf[i][j] = (double) cntToken[i][j] / noAllTokens[i];
			tfidf[i][j] = tf[i][j] * idf[j];
		}
	pearsonSimMultiplier = 10.0 / maxFreq;
	printf( "MaxFreq: %lf / PearsonMult: %lf\n" , maxFreq , pearsonSimMultiplier );
}

// O(logN) implementation of x^y
double calcPower( double value , int pwr ) {
	if( pwr == 0 )
		return 1;
	double res = calcPower( value , pwr / 2 );
	res *= res;
	if( pwr % 2 )
		res *= value;
	return res;
}

// Calculate Pearson Similarity
double calcPearsonSim( int docX , int docY ) {
	double numerator = 0 , sumSquareX = 0 , sumSquareY = 0;
	double rxAverage = avgFreq[docX] * pearsonSimMultiplier;
	double ryAverage = avgFreq[docY] * pearsonSimMultiplier;
	for( int i = 0 ; i < m ; i++ )
		if( cntToken[docX][i] > 0 && cntToken[docY][i] > 0 ) {
			double rx = cntToken[docX][i] * pearsonSimMultiplier;
			double ry = cntToken[docY][i] * pearsonSimMultiplier;
			numerator += (rx - rxAverage) * (ry - ryAverage);
			sumSquareX += (rx - rxAverage) * (rx - rxAverage);
			sumSquareY += (ry - ryAverage) * (ry - ryAverage);
		}
	double denominator = sqrt( sumSquareX ) * sqrt( sumSquareY );
	return numerator / denominator;
}

// Calculate L2 distance
double calcDistLX( int docX , int docY , int dim ) {
	double ans = 0;
	// If dim = inf, then store it as -1
	if( dim == -1 )
		for( int i = 0 ; i < m ; i++ )
			ans = max( ans , fabs( tfidf[docX][i] - tfidf[docY][i] ) );
	// Calculate squares of differences
	else {
		for( int i = 0 ; i < m ; i++ )
			ans += calcPower( fabs( tfidf[docX][i] - tfidf[docY][i] ) , dim );
		ans = pow( ans , 1.0 / dim );
	}
	return ans;
}

// Calculate cosine distance in degree
double calcDistCos( int docX , int docY ) {
	double multSum = 0 , lenX = 0 , lenY = 0;
	for( int i = 0 ; i < m ; i++ ) {
		// Sum up terms of the numerator
		multSum += tfidf[docX][i] * tfidf[docY][i];
		lenX += tfidf[docX][i] * tfidf[docX][i];
		lenY += tfidf[docY][i] * tfidf[docY][i];
	}
	// Calculate length of vectors - terms of the denominator
	lenX = sqrt( lenX );
	lenY = sqrt( lenY );
	// Calculate numerator / denominator
	double cosVal = multSum / ( lenX * lenY );
	// Get cosine distance in radian
	double tetaVal = acos( cosVal );
	// Get cosine distance in degree
	double cosDist = tetaVal * 180.0 / M_PI;
	return cosDist;
}

// Display different kinds of distances
void calcAllDistances() {
	// Calculate Pearson Similarity
	for( int i = 0 ; i < n ; i++ )
		for( int j = i ; j < n ; j++ )
			pearsonSim[i][j] = pearsonSim[j][i] = calcPearsonSim( i , j );
	// Calculate L2 distances
	for( int i = 0 ; i < n ; i++ )
		for( int j = i ; j < n ; j++ )
			distL[i][j] = distL[j][i] = calcDistLX( i , j , 2 );
	// Calculate Cos distances
	for( int i = 0 ; i < n ; i++ )
		for( int j = i ; j < n ; j++ )
			distCos[i][j] = distCos[j][i] = calcDistCos( i , j );
	// Display different kinds of distances
	for( int i = 0 ; i < n ; i++ )
		for( int j = i ; j < n ; j++ )
			cout << i << " , " << j << " : " << "Cos = " << distCos[i][j] << " / " << "L2 = " << distL[i][j] << " / " << "PearsonSim = " << pearsonSim[i][j] << endl;
}

// A short test function code that can be run to check if the algo works correctly
void test() {
	n = 3;
	m = 3;
	for( int i = 0 ; i < 3 ; i++ )
		tfidf[i][i] = 1;
	calcAllDistances();
	while(true);
}

int main() {
	// test();
	// 1st step: map characters
	calcConvertChar();
	// 2nd step: get stop words
	getNonUsedWords();
	// 3rd step: map words to original ones - stemming job
	getStemmingWords();
	// 4th step: get file names that will be used during test
	getInputFileNames();
	// 5th step: major step that calculates tf-idf
	calcTFIDF();
	// 6th step: calculate distances in different measures
	calcAllDistances();
	return 0;
}
