#pragma once

#include <stdint.h>
#include "LChild_RSibling_Tree_ADT.h"

template <typename ElementType>
LCRSTree<ElementType>::_Node::_Node(){
	element = nullptr;
	child = nullptr;
	parent = nullptr;
	lSibling = nullptr;
	rSibling = nullptr;
}

template <typename ElementType>
LCRSTree<ElementType>::_Node::_Node(const ElementType &elem) {
	element = new ElementType;
	*element = elem;
	child = nullptr;
	parent = nullptr;
	lSibling = nullptr;
	rSibling = nullptr;
}

template <typename ElementType>
LCRSTree<ElementType>::_Node::~_Node() {
	delete element;
}

template <typename ElementType>
typename LCRSTree<ElementType>::_Node &LCRSTree<ElementType>::_Node::operator=(_Node &a) {
	if(element && a.element)
		*element = *(a.element);
	else if(!element && a.element){
		element = new ElementType;
		*element = *(a.element);
	}
	else if(element && !a.element) {
		delete element;
		element = nullptr;
	}
	return *this;
}