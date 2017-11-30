#pragma once

#include "LChild_RSibling_Tree_ADT.h"


template <typename ElementType>
void LCRSTree<ElementType>::_iterator::throwException(LCRSTreeConst::error_t errorCode) {
	using namespace LCRSTreeConst;
	if (doesTrowException)
		throw errorCode;
	else
		std::cout << "exception " << errorCode << " caught but not throwed" << std::endl;
}


template <typename ElementType>
LCRSTree<ElementType>::_iterator::_iterator(Node *node, bool _doesThrowException) {
	current = node;
	doesTrowException = _doesThrowException;
}

template <typename ElementType>
LCRSTree<ElementType>::_iterator::_iterator(bool _doesThrowException) {
	current = nullptr;
	doesTrowException = _doesThrowException;
}



template <typename ElementType>
LCRSTree<ElementType>::_iterator::~_iterator() {
	current = nullptr;
}

template <typename ElementType>
ElementType* LCRSTree<ElementType>::_iterator::getValue() {
	using namespace LCRSTreeConst;
	if(!current){
		throwException(error_node_empty);
		return nullptr;
	}
	return current->element;
}

template <typename ElementType>
void LCRSTree<ElementType>::_iterator::assignData(const ElementType &elem) {
	using namespace LCRSTreeConst;
	if(!current){
		throwException(error_node_empty);
		return;
	}
	
	if(!current->element)
		current->element = new ElementType;
	*(current->element) = elem;

}

template <typename ElementType>
typename LCRSTree<ElementType>::_iterator &LCRSTree<ElementType>::_iterator::operator++() {
	using namespace LCRSTreeConst;
	if (!current) {
		throwException(error_node_empty);
		return *this;
	}
	if (current->child) {
		current = current->child;
		return *this;
	}
	else if (current->rSibling) {
		current = current->rSibling;
		return *this;
	}
	else {
		for (; current && !current->rSibling; current = current->parent);
		
		// if has rSibling (cannot judge by current->rSibling directly because current can be nullptr)
		if (current)
			current = current->rSibling;
		return *this;
	}
	
}

template <typename ElementType>
typename LCRSTree<ElementType>::_iterator &LCRSTree<ElementType>::_iterator::operator++(int) {
	using namespace LCRSTreeConst;
	if (!current){
		throwException(error_node_empty);
		return *this;
	}
	
	if (current->child) {
		current = current->child;
		return *this;
	}
	else if (current->rSibling) {
		current = current->rSibling;
		return *this;
	}
	else {
		for (; current && !current->rSibling; current = current->parent);
		// if has rSibling
		if (current)
			current = current->rSibling;
		return *this;
	}
}

template <typename ElementType>
bool LCRSTree<ElementType>::_iterator::operator==(const _iterator &a) {
	if (!current && !a.current)
		return true;
	else if (!current || !a.current)
		return false;
	else
		return current->element == a.current->element;
}

template <typename ElementType>
bool LCRSTree<ElementType>::_iterator::operator!=(const _iterator &a) {
	if (!current && !a.current)
		return false;
	else if (!current || !a.current)
		return true;
	else
		return current->element != a.current->element;
}

template <typename ElementType>
typename LCRSTree<ElementType>::_iterator &LCRSTree<ElementType>::_iterator::operator=(const _iterator &a) {
	current = a.current;
	return *this;
}

template <typename ElementType>
ElementType *LCRSTree<ElementType>::_iterator::operator*() {
	using namespace LCRSTreeConst;
	if (!current) {
		//throwException(error_node_empty);
		return nullptr;
	}
	return current->element;
}


template <typename ElementType>
bool LCRSTree<ElementType>::_iterator::isEmpty(void) {
	return bool(!current);
}

template <typename ElementType>
typename LCRSTree<ElementType>::_iterator LCRSTree<ElementType>::_iterator::lSibling() {
	using namespace LCRSTreeConst;
	if(!current){
		throwException(error_node_empty);
		return *this;
	}
	
	return _iterator(current->lSibling, doesTrowException);
}

template <typename ElementType>
typename LCRSTree<ElementType>::_iterator LCRSTree<ElementType>::_iterator::rSibling() {
	using namespace LCRSTreeConst;
	if(!current){
		throwException(error_node_empty);
		return *this;
	}
	
	return _iterator(current->rSibling, doesTrowException);
}

template <typename ElementType>
typename LCRSTree<ElementType>::_iterator LCRSTree<ElementType>::_iterator::firstChild(){
	using namespace LCRSTreeConst;
	if(!current){
		throwException(error_node_empty);
		return *this;
	}
	
	return _iterator(current->child, doesTrowException);
}

template <typename ElementType>
typename LCRSTree<ElementType>::_iterator LCRSTree<ElementType>::_iterator::parent() {
	using namespace LCRSTreeConst;
	if(!current){
		throwException(error_node_empty);
		return *this;
	}
	
	return _iterator(current->parent, doesTrowException);
}




template <typename ElementType>
void LCRSTree<ElementType>::_iterator::insertChildAtFront(const ElementType &elem) {
	using namespace LCRSTreeConst;
	if (!current){
		throwException(error_node_empty);
		return;
	}
	//deal with hierarchy
	Node *temp = current->child;
	current->child = new _Node(elem);
	
	current->child->parent = current;
	current->child->rSibling = temp;
	if (temp) {
		temp->lSibling = current->child;
	}
	
}

template <typename ElementType>
void LCRSTree<ElementType>::_iterator::insertChildAtTail(const ElementType &elem) {
	using namespace LCRSTreeConst;
	if (!current){
		throwException(error_node_empty);
		return;
	}
	// if doesn't have child
	if (!current->child) {
		current->child = new _Node(elem);
		current->child->parent = current;
	} // if already has child
	else {
		Node *temp = current->child;
		for (; temp->rSibling; temp = temp->rSibling);
		temp->rSibling = new _Node(elem);
		temp->rSibling->lSibling = temp;
		// don't forget to add parent (remember to check four hierarchy directions actually)
		temp->rSibling->parent = current;
		
	}
}

template <typename ElementType>
void LCRSTree<ElementType>::_iterator::insertLSibling(const ElementType &elem) {
	using namespace LCRSTreeConst;
	if (!current){
		throwException(error_node_empty);
		return;
	}
	// deal with the hierarchy
	Node *temp = current->lSibling;
	current->lSibling = new Node(elem);
	current->lSibling->rSibling = current;
	current->lSibling->parent = current->parent;
	
	if (temp) {
		current->lSibling->lSibling = temp;
		temp->rSibling = current->lSibling;
	} // don't forget the parent, it's a bi-direction
	else {
		current->parent->child = current->lSibling;
	}

}

template <typename ElementType>
void LCRSTree<ElementType>::_iterator::insertRSibling(const ElementType &elem) {
	using namespace LCRSTreeConst;
	if (!current){
		throwException(error_node_empty);
		return;
	}
	// deal with the hierarchy
	Node *temp = current->rSibling;
	current->rSibling = new Node(elem);
	current->rSibling->lSibling = current;
	current->rSibling->parent = current->parent;
	
	if (temp) {
		current->rSibling->rSibling = temp;
		temp->lSibling = current->rSibling;
	}

}


template <typename ElementType>
void LCRSTree<ElementType>::_iterator::insertSiblingAtFront(const ElementType &elem) {
	using namespace LCRSTreeConst;
	if(!current) {
		throwException(error_node_empty);
		return;
	}
	
	Node *temp = current;
	for(;temp->lSibling; temp = temp->lSibling);
	temp->lSibling = new Node(elem);
	temp->lSibling->rSibling = temp;
	temp->lSibling->parent = temp->parent;
	// don't forget to change the parent's first child
	temp->parent->child = temp->lSibling;

}

template <typename ElementType>
void LCRSTree<ElementType>::_iterator::insertSiblingAtTail(const ElementType &elem) {
	using namespace LCRSTreeConst;
	if(!current){
		throwException(error_node_empty);
		return;
	}
	
	Node *temp = current;
	for(;temp->rSibling;temp = temp->rSibling);
	temp->rSibling = new _Node(elem);
	temp->rSibling->lSibling = temp;
	temp->rSibling->parent = temp->parent;
	
}

