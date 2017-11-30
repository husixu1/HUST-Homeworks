#pragma once
#include "cocos2d.h"
#include "ui/UITextField.h"
#include "DOM_tree_ADT.h"
#include "HTML_analyser.h"
#include "hash.h"
#include <vector>

class TreeScene : public cocos2d::Layer {
private:
	std::string url;
	DOMTree *currentDOMTree;

	int _currentDrawingNum;
	cocos2d::MenuItem *initializeOneRenderNode(int depth, HTMLNode &HTMLNode);
	
	void onMouseScrollCallback(cocos2d::EventMouse *event);
	void onMouseMoveCallback(cocos2d::EventMouse *event);
	void onMouseDownCallback(cocos2d::EventMouse *event);
	void onMouseUpCallBack(cocos2d::EventMouse *event);

	void onKeyDownCallback(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event);
	// the current activated node
	hashTable::Node *activatedNode;
	
	// the value table (the only place that directly store the value rather than the pointer)
	// do not use the vector(will change the address of the node)
	// can't be changed after set (except delete)
	hashTable::Node *_NodeSequenceTable;
	int usedNodeNum;
	int totoalNodeNum;
	// for quick search nodes £¨Binded with the sequenceTable£©
	hashTable _nodeHashTable;

	// the render node table before collapse
	std::vector<hashTable::Node *> _renderTableBeforeCollapse;
	// the node for current rendering
	std::vector<hashTable::Node *> _currentRenderNodeTable;

	std::vector<TreeScene::Node *> _convertHTMLNodeListToNodeList(std::vector<HTMLNode *>);

	int _maxDepth;
	// metric: pixel
	int treeLength;

	std::string searchStr1;
	std::string searchStr2;
	std::string searchStr3;
	void searchCallback(cocos2d::Ref *node, cocos2d::ui::TextField::EventType event);

	bool _isMouseDown;
	cocos2d::Vec2 _mouseStartLocation;
	cocos2d::Vec2 _treeStartLocation;

	~TreeScene();
public:

	static cocos2d::Scene *createScene(DOMTree *tree);
	virtual bool init();

	// will initialize sequenceTable and renderTable at the same time
	void initializeTree(DOMTree *);
	
	void setRenderTable(DOMTree *);
	void setRenderTable(std::vector<HTMLNode *>);
	void renderCurrentTree(void);

	void setTree(DOMTree *tree) { currentDOMTree = tree; }
	// implement the "static create()" method manually
	static TreeScene *create(DOMTree *tree){ 
		TreeScene *pRet = new(std::nothrow) TreeScene(); 
		if (pRet) 
		{ 
			pRet->setTree(tree);
			if (pRet->init()) {
				pRet->autorelease();
				return pRet;
			}
		} 
		else 
		{ 
			delete pRet; 
			pRet = nullptr; 
			return nullptr; 
		} 
	}
};