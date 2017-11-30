#include "hash.h"
#include "TreeScene.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "spine/extension.h"

hashTable::hashTable() {
	_usedLength = 0;
	_hashMaskLength = 1;
	_hashMask = ((1 << _hashMaskLength) - 1);

	_hashTable_renderTable = new HashNode  *[_hashMask + 1];
	_hashTable_dataTable = new HashNode *[_hashMask + 1];

	memset(_hashTable_renderTable, 0, sizeof(HashNode *)*(_hashMask + 1));
	memset(_hashTable_dataTable, 0, sizeof(HashNode *)*(_hashMask + 1));
}

hashTable::~hashTable() {
	// delete the nodes
	for (HashNode *temp : _releasePool)
		delete temp;
	delete[] _hashTable_dataTable;
	delete[] _hashTable_renderTable;
}

void hashTable::insertNode(Node *node_) {
	std::stringstream buffer;
	_reHash();
		

	_HashNode *temp[2];

	// 0 for renderTable and 1 for dataTable
	for (int i = 0; i < 2; i++) {
		temp[i] = new HashNode;
		_releasePool.push_back(temp[i]);
		buffer.clear();
		if (i == 0) {
			buffer << uint32_t(node_->renderNode) << std::ends;
		}
		else if (i == 1) {
			buffer << uint32_t(node_->dataNode) << std::ends;
		}
		temp[i]->hashSum = murmurHash_32(buffer.str().c_str(), buffer.str().length());
		temp[i]->node = node_;
	}
	_usedLength ++;

	// insert the node
	_HashNode *finder = _hashTable_renderTable[temp[0]->hashSum & _hashMask];
	if (!finder)
		_hashTable_renderTable[temp[0]->hashSum & _hashMask] = temp[0];
	else {
		for (; finder->next; finder = finder->next);
		finder->next = temp[0];
	}
	
	finder = _hashTable_dataTable[temp[1]->hashSum & _hashMask];
	if (!finder)
		_hashTable_dataTable[temp[1]->hashSum & _hashMask] = temp[1];
	else {
		for (; finder->next; finder = finder->next);
		finder->next = temp[1];
	}
}

hashTable::Node *hashTable::getNodeByHTMLNode(HTMLNode *htmlNode) {
	std::stringstream buffer;
	buffer << uint32_t(htmlNode) << std::ends;
	uint32_t tempHash = murmurHash_32(buffer.str().c_str(), buffer.str().length());
	
	HashNode *temp = _hashTable_dataTable[tempHash & _hashMask];
	for (; temp && temp->hashSum != tempHash; temp = temp->next);
	
	if(!temp)
		return nullptr;
	return temp->node;
}

hashTable::Node *hashTable::getNodeByRenderNode(cocos2d::MenuItem *renderNode) {
	std::stringstream buffer;
	buffer << uint32_t(renderNode) << std::ends;
	uint32_t tempHash = murmurHash_32(buffer.str().c_str(), buffer.str().length());

	HashNode *temp = _hashTable_renderTable[tempHash & _hashMask];
	for (; temp && temp->hashSum != tempHash; temp = temp->next);

	if (!temp)
		return nullptr;
	return temp->node;
}

uint32_t hashTable::murmurHash_32(const char* key, size_t len, uint32_t seed) {
	uint32_t h = seed;
	if (len > 3) {
		const uint32_t* key_x4 = (const uint32_t*)key;
		size_t i = len >> 2;
		do {
			uint32_t k = *key_x4++;
			k *= 0xcc9e2d51;
			k = (k << 15) | (k >> 17);
			k *= 0x1b873593;
			h ^= k;
			h = (h << 13) | (h >> 19);
			h += (h << 2) + 0xe6546b64;
		} while (--i);
		key = (const char*)key_x4;
	}
	if (len & 3) {
		size_t i = len & 3;
		uint32_t k = 0;
		key = &key[i - 1];
		do {
			k <<= 8;
			k |= *key--;
		} while (--i);
		k *= 0xcc9e2d51;
		k = (k << 15) | (k >> 17);
		k *= 0x1b873593;
		h ^= k;
	}
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

void hashTable::_reHash() {
	// resize the hash table to a double
	if (_usedLength > (_hashMask + 1) * 2) {
		_hashMaskLength++;
		_hashMask = ((1 << _hashMaskLength) - 1);

		HashNode **newTable1 = new HashNode *[_hashMask + 1];
		HashNode **newTable2 = new HashNode *[_hashMask + 1];
		memset(newTable1, 0, sizeof(HashNode *)*(_hashMask + 1));
		memset(newTable2, 0, sizeof(HashNode *)*(_hashMask + 1));

		HashNode *temp, *temp_shadow, *inserter, **indexer;

		for (indexer = _hashTable_renderTable; indexer < _hashTable_renderTable + (_hashMask + 1) / 2; indexer++) {
			for (temp = *indexer; temp; temp = temp_shadow) {
				temp_shadow = temp->next;
				temp->next = nullptr;

				inserter = newTable1[temp->hashSum & _hashMask];
				if (!inserter)
					newTable1[temp->hashSum & _hashMask] = temp;
				else {
					for (; inserter->next; inserter = inserter->next);
					inserter->next = temp;
				}
			}
		}
		delete[] _hashTable_renderTable;
		_hashTable_renderTable = newTable1;

		for (indexer = _hashTable_dataTable; indexer < _hashTable_dataTable + (_hashMask + 1) / 2; indexer++) {
			for (temp = *indexer; temp; temp = temp_shadow) {
				temp_shadow = temp->next;
				temp->next = nullptr;

				inserter = newTable2[temp->hashSum & _hashMask];
				if (!inserter)
					newTable2[temp->hashSum & _hashMask] = temp;
				else {
					for (; inserter->next; inserter = inserter->next);
					inserter->next = temp;
				}
			}
		}
		delete[] _hashTable_dataTable;
		_hashTable_dataTable = newTable2;
	}
}