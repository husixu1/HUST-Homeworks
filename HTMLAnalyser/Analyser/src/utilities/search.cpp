#include "search.h"
#include <vector>
#include <string>

//// KMP works perfect
std::vector<int> KMPSearch(const std::string &str, const std::string &pattern) {
	using namespace std;
	if (pattern.size() > str.size())
		return vector<int>();
	if (pattern.size() == 0)
		return vector<int>({ 0 });

	int *next = new int[pattern.length()];
	next[0] = 0;
	// initialize next table;
	for(int i=1; i<pattern.length(); i++) {
		if (pattern[next[i-1]] == pattern[i-1])
			next[i] = next[i-1]+1;
		else
			next[i] = next[next[i-1]];
	}
	
	// do the full-text search
	vector<int> result = {};
	int patternPos=0;
	for(int i=0; i<str.length(); i++){
		for(;patternPos<pattern.length() && i < str.length() && pattern[patternPos]== str[i];
			 i++, patternPos++);
		// if find a match
		if(patternPos == pattern.length()) {
			result.push_back(i - pattern.length());
			patternPos = 0;
		}
		// if doesn't find
		else
			patternPos -= next[patternPos];
	}
	delete[] next;
	return result;
}

int KMPSearchFirst(const std::string &str, const std::string &pattern){
	using namespace std;
	
	int *next = new int[pattern.length()];
	next[0] = 0;
	// initialize next table;
	for(int i=1; i<pattern.length(); i++) {
		if (pattern[next[i-1]] == pattern[i-1])
			next[i] = next[i-1]+1;
		else
			next[i] = next[next[i-1]];
	}
	
	// do the full-text search
	vector<int> result;
	int patternPos=0;
	for(int i=0; i<str.length(); i++){
		for(;patternPos<pattern.length() && i < str.length() && pattern[patternPos]== str[i];
			 i++, patternPos++);
		// if find a match
		if(patternPos == pattern.length())
			return i-pattern.length();
		else
			patternPos -= next[patternPos];
	}
	return -1;
}

std::vector<int> BMSearch(const std::string str, const std::string pattern){
	return {-1};
}