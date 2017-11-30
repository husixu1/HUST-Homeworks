#pragma once
#include "cocos2d.h"
#include "DOM_tree_ADT.h"
#include <string>

USING_NS_CC;

class detailPanel : public cocos2d::Menu {
private:
	HTMLNode *contentNode;
public:
	virtual bool init();
	
	void setContent(HTMLNode *node) { contentNode = node; }

	static detailPanel* create(HTMLNode *content_);
};