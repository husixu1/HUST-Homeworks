#include "DOM_tree_ADT.h"
#include <vector>

void DOMTree::saveTo(std::string fileName) {
	LCRSTree<HTMLNode>::saveTo(fileName, saveAnHTMLNode);
}

void DOMTree::loadFrom(std::string fileName) {
	LCRSTree<HTMLNode>::loadFrom(fileName,loadAnHTMLNode);
}

std::vector<HTMLNode *> DOMTree::filterByType(const std::string &pattern) {
	using namespace std;
	vector<HTMLNode *> result;
	for(auto itr : *this)
		if(compareByType(*itr, pattern))
			result.push_back(itr);
	return result;
}

std::vector<HTMLNode *> &DOMTree::filterByType(std::vector<HTMLNode *> &list, const std::string &pattern) {
	using namespace std;
	vector<HTMLNode *> temp;
	for(auto itr : list)
		if(compareByType(*itr, pattern))
			temp.push_back(itr);
	list = temp;
	return list;
}

std::vector<HTMLNode *> DOMTree::filterByPropertKey(const std::string &pattern) {
	using namespace std;
	vector<HTMLNode *> result;
	for(auto itr : *this)
		if(compareByPropertyKey(*itr, pattern))
			result.push_back(itr);
	return result;
}

std::vector<HTMLNode *> &DOMTree::filterByPropertyKey(std::vector<HTMLNode *> &list, const std::string &pattern) {
	using namespace std;
	vector<HTMLNode *> result;
	for(auto itr : list)
		if(compareByPropertyKey(*itr, pattern))
			result.push_back(itr);
	list = result;
	return list;
}

std::vector<HTMLNode *> DOMTree::filterByPropertvalue(const std::string &pattern) {
	using namespace std;
	vector<HTMLNode *> result;
	for(auto itr : *this)
		if(compareByPropertyValue(*itr, pattern))
			result.push_back(itr);
	return result;
}

std::vector<HTMLNode *> &DOMTree::filterByPropertyValue(std::vector<HTMLNode *> &list, const std::string &pattern) {
	using namespace std;
	vector<HTMLNode *> result;
	for(auto itr : list)
		if(compareByPropertyValue(*itr, pattern))
			result.push_back(itr);
	list = result;
	return list;
}

std::vector<HTMLNode *> DOMTree::filterGlobally(const std::string &pattern) {
	using namespace std;
	vector<HTMLNode *> result;
	for(auto itr : *this)
		if(compareByType(*itr, pattern) ||
				compareByPropertyKey(*itr, pattern) ||
				compareByPropertyValue(*itr, pattern) ||
				compareByContent(*itr, pattern))
			result.push_back(itr);
	return result;
}

std::vector<HTMLNode *>& DOMTree::filterGlobally(std::vector<HTMLNode *> &list, const std::string &pattern) {
	using namespace std;
	vector<HTMLNode *> result;
	for(auto itr : list)
		if(compareByType(*itr, pattern) ||
		   compareByPropertyKey(*itr, pattern) ||
		   compareByPropertyValue(*itr, pattern) ||
		   compareByContent(*itr, pattern))
			result.push_back(itr);
	list = result;
	return list;
}