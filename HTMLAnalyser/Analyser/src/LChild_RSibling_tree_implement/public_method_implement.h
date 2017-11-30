#pragma once

#include "LChild_RSibling_Tree_ADT.h"
#include "utilities/md5.h"
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

template <typename ElementType>
void LCRSTree<ElementType>::throwException(LCRSTreeConst::error_t errorCode) {
	if(doesTrowException)
		throw errorCode;
}
		
template <typename ElementType>
LCRSTree<ElementType>::LCRSTree() {
	root = nullptr;
	doesTrowException = true;
}

template <typename ElementType>
LCRSTree<ElementType>::LCRSTree(bool _doesThrowException){
	root = nullptr;
	doesTrowException = _doesThrowException;
}

template <typename ElementType>
LCRSTree<ElementType>::~LCRSTree() {
	using namespace std;
	using namespace LCRSTreeConst;
	
	if(!root)
		return;

	Node *temp, *temp_shadow;
	stack<Node *> record_stack;
	// while not reaching the end
	for(temp=root; !temp || !record_stack.empty(); ){
		if(!temp) {
			temp = record_stack.top();
			record_stack.pop();
		}
		else if(temp->child) {
			record_stack.push(temp);
			temp = temp->child;
			temp->parent->child = nullptr;
		}else if(temp->rSibling){
			temp_shadow = temp->rSibling;
			delete(temp);
			temp = temp_shadow;
		}else{
			delete(temp);
			temp = record_stack.top();
			record_stack.pop();
		}
	}
	root = nullptr;
}

template <typename ElementType>
bool LCRSTree<ElementType>::isEmpty() {
	return !root;
}

template <typename ElementType>
ElementType *LCRSTree<ElementType>::getRoot() {
	return root->element;
}

template <typename ElementType>
void LCRSTree<ElementType>::assignRoot(const ElementType &elem) {
	if(!root)
		root = new Node(elem);
	else
		*(root->element) = elem;
}


template <typename ElementType>
void LCRSTree<ElementType>::traverse(void (*visit)(ElementType &)) {
	for (ElementType *temp: *this)
		if(temp)
			visit(*temp);
}

template <typename ElementType>
void LCRSTree<ElementType>::traverse(void(*visit)(int level, ElementType &, void *),
		void *user_data) {
	using namespace std;
	using namespace LCRSTreeConst;
	Node *tempNode = root;

	unsigned int depth = 0;
	for (; tempNode;) {
		visit(depth, *(tempNode->element), user_data);
		if (tempNode->child) {
			tempNode = tempNode->child;
			depth++;
		}
		else if (tempNode->rSibling)
			tempNode = tempNode->rSibling;
		else {
			for (; tempNode && !tempNode->rSibling; tempNode = tempNode->parent, depth--);
			// if has rSibling
			if (tempNode)
				tempNode = tempNode->rSibling;
			else
				break;
		}
	}
}

template <typename ElementType>
typename LCRSTree<ElementType>::iterator LCRSTree<ElementType>::begin() const {
	return iterator(root, doesTrowException);
}


template <typename ElementType>
typename LCRSTree<ElementType>::iterator LCRSTree<ElementType>::end() const {
	return iterator(nullptr);
}

template <typename ElementType>
void LCRSTree<ElementType>::insertChildAtFront(iterator & itr, const ElementType & elem) {
	itr.insertChildAtFront(elem);
}

template <typename ElementType>
void LCRSTree<ElementType>::insertChildAtTail(iterator & itr, const ElementType & elem) {
	itr.insertChildAtTail(elem);
}

template <typename ElementType>
void LCRSTree<ElementType>::insertLSibling(iterator & itr, const ElementType & elem) {
	itr.insertLSibling(elem);
}

template <typename ElementType>
void LCRSTree<ElementType>::insertRSibling(iterator & itr, const ElementType & elem) {
	itr.insertRSibling(elem);
}


template <typename ElementType>
void LCRSTree<ElementType>::insertSiblingAtFront(iterator & itr, const ElementType & elem) {
	itr.insertSiblingAtFront(elem);
}

template <typename ElementType>
void LCRSTree<ElementType>::insertSiblingAtTail(iterator & itr, const ElementType & elem) {
	itr.insertSiblingAtTail(elem);
}

template <typename ElementType>
ElementType *LCRSTree<ElementType>::findFirst(
		const ElementType &elem, bool (* compare)(const ElementType &, const ElementType &)) {
	
	for(ElementType *temp : *this){
		if(compare(elem, *temp))
			return *temp;
	}
	return nullptr;
}

template <typename ElementType>
std::vector<ElementType *> LCRSTree<ElementType>::find(
		const ElementType &elem,
		bool (*compare)(const ElementType &, const ElementType &)) {
	using namespace std;
	vector<ElementType *> result;
	for(ElementType *temp : *this){
		if(compare(elem, *temp))
			result.push_back(temp);
	}
	return result;
}

template <typename ElementType>
std::vector<ElementType *> &LCRSTree<ElementType>::find(
		std::vector<ElementType *> &list,
		const ElementType &elem,
		bool (*compare)(const ElementType &, const ElementType &)) {
	using namespace std;
	for(int i=0; i<list.size(); i++){
		if(!compare(*(list[i]), elem))
			list.erase(list.begin()+i), i--;
	}
	return list;
}

template <typename ElementType>
void LCRSTree<ElementType>::destroy() {
	using namespace std;
	using namespace LCRSTreeConst;
	if(!root)
		return;
	
	Node *temp, *temp_shadow;
	stack<Node *> record_stack;
	// while not reaching the end
	for(temp=root; temp || !record_stack.empty(); ){
		if(!temp) {
			temp = record_stack.top();
			record_stack.pop();
		}
		else if(temp->child) {
			record_stack.push(temp);
			temp = temp->child;
		}else if(temp->rSibling){
			temp_shadow = temp->rSibling;
			delete temp;
			temp = temp_shadow;
		}else{
			temp = record_stack.top();
			record_stack.pop();
			temp = temp->rSibling;
		}
	}
	
	root = nullptr;
}

template <typename ElementType>
void LCRSTree<ElementType>::saveTo(std::string fileName, void (*write_an_elem)(std::ofstream &, ElementType &)) {
	using namespace std;
	using namespace LCRSTreeConst;
	
	ofstream outFile(fileName);
	if(!outFile.is_open()){
		throwException(error_opening_file);
		return;
	}
	
	// MD5 positioning line
	outFile << "00000000000000000000000000000000" << endl;
	// write file guard and save time
	outFile << "LCHILD_RSIBLING_SAVE_FILE" << time(0) << endl;
	//write data
	Node *temp = root;
	if(!temp) {
		outFile << "EMPTY" << endl;
		goto end;
	}
	outFile << "NOT_EMPTY" << endl;
	for(;temp;){
		if (temp->child){
			// if the node is empty
			if(!temp->element)
				outFile << 1 << endl;
			else {
				outFile << 0 << endl;
				write_an_elem(outFile, *(temp->element));
			}
			outFile << "C" << endl;
			temp = temp->child;
		}
		else if (temp->rSibling){
			// if the node is empty
			if(!temp->element)
				outFile << 1 << endl;
			else{
				outFile << 0 << endl;
				write_an_elem(outFile, *(temp->element));
			}
			outFile << "S" << endl;
			temp = temp->rSibling;
		}
		else {
			// if the node is empty
			if(!temp->element)
				outFile << 1 << endl;
			else {
				outFile << 0 << endl;
				write_an_elem(outFile, *(temp->element));
			}
			// record the backtracking level
			int level = 0;
			for(;temp && !temp->rSibling; temp = temp->parent, level++);
			
			if(temp) {
				temp = temp->rSibling;
				outFile << "E" << endl;
				outFile << level << endl;
			}
			else
				outFile << "e" << endl;
		}
	}
	
	
end:
	outFile.close();
	
	// insert MD5SUM at the front
	string MD5 = md5file(fileName.c_str());
	outFile.open(fileName,ios::in | ios::out);
	outFile.seekp(0,ios::beg);
	outFile.write(MD5.c_str(), MD5.length());
	outFile.close();
}

template <typename ElementType>
void LCRSTree<ElementType>::loadFrom(std::string fileName, void (*load_an_elem)(std::ifstream &, ElementType &)) {
	using namespace std;
	using namespace LCRSTreeConst;
	
	// check if the file is valid
	error_t err;
	if((err = checkIsFileValid(fileName)) != no_error){
		if(doesTrowException)
			throwException(err);
		return;
	}
	
	char tempstr[100];
	// read the data
	destroy();
	ifstream inFile(fileName);
		//skip the frist two lines
	inFile.getline(tempstr,100,'\n');
	inFile.getline(tempstr,100,'\n');
	
	// if the tree is empty
	string isEmpty;
	inFile >> isEmpty;
	int nodeIsEmpty;
	if(isEmpty == "EMPTY"){
		inFile.close();
		return;
	} else{
		root = new Node;
		inFile >> nodeIsEmpty;
		if(!nodeIsEmpty)
			root->element = new ElementType;
	}
	
	//clear the current tree
	char hierarchy;
	Node *temp = root;
	for(;!inFile.eof();){
		// load the element in node (if not empty)
		if(!nodeIsEmpty)
			load_an_elem(inFile, *(temp->element));
		
		inFile >> hierarchy;
		switch (hierarchy){
			case 'C':{
				temp->child = new Node;
				
				// if the node is empty
				inFile >> nodeIsEmpty;
				if(!nodeIsEmpty)
					temp->child->element = new ElementType;
				
				temp->child->parent = temp;
				temp = temp->child;
			}
				break;
			case 'S':{
				temp->rSibling = new Node;
				
				// if the node is empty
				inFile >> nodeIsEmpty;
				if(!nodeIsEmpty)
					temp->rSibling->element = new ElementType;
				
				temp->rSibling->lSibling = temp;
				temp->rSibling->parent = temp->parent;
				temp = temp->rSibling;
			}
				break;
			case 'E':{
				int backTraceLevel;
				inFile >> backTraceLevel;
				for(int i=0; i<backTraceLevel; i++, temp = temp->parent);
				temp->rSibling = new Node;
				
				// if the node is empty
				inFile >> nodeIsEmpty;
				if(!nodeIsEmpty)
					temp->rSibling->element = new ElementType;
				
				temp->rSibling->lSibling = temp;
				temp->rSibling->parent = temp->parent;
				temp = temp->rSibling;
			}
				break;
			case 'e':{
				goto end;
			}
		}
	}
	
end:
	inFile.close();
}

template <typename ElementType>
LCRSTreeConst::error_t LCRSTree<ElementType>::checkIsFileValid(std::string fileName) {
	using namespace std;
	using namespace LCRSTreeConst;
	
	fstream inFile(fileName, ios::in | ios::out);
	if(!inFile.is_open())
		return error_opening_file;
	
	string MD5;
	inFile >> MD5;
	inFile.seekp(0, ios::beg);
	inFile << "00000000000000000000000000000000";
	inFile.close();
	
	// if checksum failed
	if(MD5!=md5file(fileName.c_str())){
		// write the MD5 back
		inFile.open(fileName,ios::in | ios::out);
		inFile.seekp(0,ios::beg);
		inFile.write(MD5.c_str(), MD5.length());
		inFile.close();
		return error_file_corrupt;
	}
	
	//write the MD5 back
	inFile.open(fileName,ios::in | ios::out);
	inFile.seekp(0,ios::beg);
	inFile.write(MD5.c_str(), MD5.length());
	
	//inspect the file gurad
	char tempstr[100];
	inFile.getline(tempstr, 100, '\n');
	inFile.getline(tempstr, 100, '\n');
	MD5 = tempstr;
	if( MD5.substr(0,string("LCHILD_RSIBLING_SAVE_FILE").length()) != "LCHILD_RSIBLING_SAVE_FILE" ){
		inFile.close();
		return error_wrong_file_type;
	}
	inFile.close();
	
	return no_error;
}