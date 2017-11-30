#pragma once

#include "LChild_RSibling_Tree_ADT.h"
#include <iostream>
#include <vector>
#include <utility>

typedef struct _HTMLNode{
	std::string type;
	std::vector<std::pair<std::string, std::string> > properties;
	std::string content;
	
	struct _HTMLNode &operator=(const struct _HTMLNode &);
	void addProperty(std::string key, std::string value);
	void setProperty(std::string key, std::string value);
	void delProperty(std::string key);
	
	std::vector<int> searchType(const std::string &pattern);
	std::vector<std::vector<int> > searchPropertyKey(const std::string &pattern);
	std::vector<std::vector<int> > searchPropertyValue(const std::string &pattern);
	bool searchProperty(const std::string &pattern);
	std::vector<int> searchContent(const std::string &pattern);

}HTMLNode;

class DOMTree : public LCRSTree<HTMLNode>{
private:
	std::string url;
	std::string encoding;
	
	static void saveAnHTMLNode(std::ofstream &, HTMLNode &);
	static void loadAnHTMLNode(std::ifstream &, HTMLNode &);
	
	static bool compareByType(const HTMLNode &origin, const std::string &pattern);
	static bool compareByPropertyKey(const HTMLNode &origin, const std::string &pattern);
	static bool compareByPropertyValue(const HTMLNode &origin, const std::string &pattern);
	static bool compareByContent(const HTMLNode &origin, const std::string &pattern);

public:
	void saveTo(std::string fileName);
	void loadFrom(std::string fileName);

	std::vector<HTMLNode *> filterByType(const std::string &pattern);
	std::vector<HTMLNode *> &filterByType(std::vector<HTMLNode *> &list, const std::string &pattern);
	std::vector<HTMLNode *> filterByPropertKey(const std::string &pattern);
	std::vector<HTMLNode *> &filterByPropertyKey(std::vector<HTMLNode *> &list, const std::string &pattern);
	std::vector<HTMLNode *> filterByPropertvalue(const std::string &pattern);
	std::vector<HTMLNode *> &filterByPropertyValue(std::vector<HTMLNode *> &list, const std::string &pattern);
	std::vector<HTMLNode *> filterGlobally(const std::string &pattern);
	std::vector<HTMLNode *> &filterGlobally(std::vector<HTMLNode *> &list, const std::string &pattern);
};
