#include "DOM_tree_ADT.h"
#include "search.h"
#include <stack>
#include <utility>

HTMLNode& HTMLNode::operator=(const HTMLNode &a) {
	type = a.type;
	content = a.content;
	properties = a.properties;
	return *this;
}

/**
 * search pattern in type
 * @param pattern
 * @return the matching position list
 */
std::vector<int> HTMLNode::searchType(const std::string &pattern) {
	return KMPSearch(type,pattern);
}

/**
 * search pattern in each propertyKey
 * @param pattern
 * @return the matching position list for each propertyKey
 */
std::vector<std::vector<int> > HTMLNode::searchPropertyKey(const std::string &pattern) {
	using namespace std;
	vector<vector<int> > result;
	for(auto itr : properties)
		result.push_back(KMPSearch(itr.first, pattern));
	return result;
}

/**
 * search pattern in each propertyValue
 * @param pattern
 * @return the matching position list for each propertyValue
 */
std::vector<std::vector<int> > HTMLNode::searchPropertyValue(const std::string &pattern) {
	using namespace std;
	vector<vector<int> > result;
	for(auto itr : properties)
		result.push_back(KMPSearch(itr.second, pattern));
	return result;
}

bool _HTMLNode::searchProperty(const std::string &pattern) {
	for (auto itr : properties)
		if (KMPSearch(itr.first, pattern).size() != 0)
			return  true;
	for (auto itr : properties)
		if (KMPSearch(itr.second, pattern).size() != 0)
			return  true;
	
	return false;
}

/**
 * search pattern in content
 * @param pattern
 * @return the matching position list
 */
std::vector<int> HTMLNode::searchContent(const std::string &pattern) {
	return KMPSearch(content, pattern);
}

void HTMLNode::addProperty(std::string key, std::string value) {
	properties.push_back({key, value});
}

void DOMTree::saveAnHTMLNode(std::ofstream &outFile, HTMLNode &node) {
	using namespace std;
	outFile << node.type << endl;
	outFile << node.content.length() << endl;
	outFile << node.content << endl;
	outFile << node.properties.size() << endl;
	for(auto itr : node.properties){
		outFile << itr.first << endl;
		outFile << itr.second << endl;
	}
}

void DOMTree::loadAnHTMLNode(std::ifstream &inFile, HTMLNode &node) {
	using namespace std;
	inFile >> node.type;
	
	unsigned long long length;
	inFile >> length;
	
	// read the content
	char *contentBuffer = new char [length + 1];
	inFile.read(contentBuffer,length);
	contentBuffer[length] = '\0';
	node.content = string(contentBuffer);
	
	// read the properties
	inFile >> length;
	pair<string, string> propertyBuffer;
	for(unsigned i=0; i < unsigned(length); i++){
		inFile >> propertyBuffer.first;
		inFile >> propertyBuffer.second;
		node.properties.push_back(propertyBuffer);
	}
	delete[] contentBuffer;
}

bool DOMTree::compareByType(const HTMLNode &origin, const std::string &pattern) {
	//use vague search
	int i=0, j=0;
	for(; i<origin.type.length() && j<pattern.length(); i++){
		if(origin.type[i] == pattern[j])
			j++;
	}

	return j == pattern.length();
}
bool DOMTree::compareByPropertyKey(const HTMLNode &origin, const std::string &pattern) {
	using namespace std;
	vector<int> result;
	
	for(int i=0; i<origin.properties.size(); i++)
		// if has search result
		if(KMPSearchFirst(origin.properties[i].first, pattern)!=-1)
			return true;
	return false;
}

bool DOMTree::compareByPropertyValue(const HTMLNode &origin, const std::string &pattern) {
	using namespace std;
	vector<int> result;
	
	for(int i=0; i<origin.properties.size(); i++)
		// if has search result
		if(KMPSearchFirst(origin.properties[i].second, pattern)!=-1)
			return true;
	return false;
}

bool DOMTree::compareByContent(const HTMLNode &origin, const std::string &pattern) {
	return KMPSearchFirst(origin.content, pattern)!=-1;
}
