#pragma once
#include "cocos2d.h"
#include "DOM_tree_ADT.h"

// key = MenuItem * or HTMLNode *, value = Node
// the pointer table

class hashTable {
private:
	friend class TreeScene;
	// cross-include problem, so it has to be put here
	//(change the hashTable to a template class will fix it, but too complex)
	typedef struct _Node{
		cocos2d::MenuItem *renderNode;
		HTMLNode *dataNode;

		int level;
		bool isCollapsed;
		bool hasRSibling;
		cocos2d::Vec2 _positionBeforeMove;
		_Node() { isCollapsed = false; }
	}Node;

	typedef struct _HashNode {
		uint32_t hashSum;
		typename Node *node;
		_HashNode *next;

		_HashNode() { node = nullptr;  next = nullptr; }
	}HashNode;

	hashTable();
	~hashTable();

	// length = hashMask+1
	HashNode **_hashTable_renderTable;	
	HashNode **_hashTable_dataTable;

	unsigned int _usedLength;
	// decides the hashTable length and hash mask, initial x is 10
	unsigned int _hashMaskLength;
	// = ((1<<hashMaskLength)-1)
	unsigned int _hashMask;
	

	uint32_t murmurHash_32(const char* key, size_t len, uint32_t seed=0);
	// do reHash while utility <1/2 or >2
	void _reHash(void);
	
	std::vector<HashNode *> _releasePool;

public:

	void insertNode(Node *node);
	Node *getNodeByRenderNode(cocos2d::MenuItem *renderNode);
	Node *getNodeByHTMLNode(HTMLNode *htmlNode);
};