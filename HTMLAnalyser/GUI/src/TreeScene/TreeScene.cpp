#include "TreeScene.h"
#include "StartupSene.h"
#include "cocos2d.h"
#include "resourceConfig.h"
#include "DOM_tree_ADT.h"
#include "detailPanel.h"
//#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UITextField.h"

USING_NS_CC;

cocos2d::Scene *TreeScene::createScene(DOMTree *tree) {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TreeScene::create(tree);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool TreeScene::init() {
	// super init first
	if (!Layer::init())
	{
		return false;
	}

	MenuItemFont::setFontName(std::string("ºÚÌå"));
	// background
	auto background = Sprite::create(BackgroundImagePath);
	background->setName("backgroundImage");
	background->setContentSize(designResolutionSize * 11.f / 10.f);
	background->setAnchorPoint(Vec2(1.f/22.f, 1.f/22.f));
	background->setPosition(Vec2::ZERO);
	this->addChild(background,1);

	auto visiableSize = Director::getInstance()->getVisibleSize();
	// create an edit box
	auto searchBox1 = ui::TextField::create();
	searchBox1->setTouchAreaEnabled(true);
	searchBox1->setTouchSize(searchBoxSize);
	searchBox1->setContentSize(searchBoxSize);
	searchBox1->setCursorPosition(1);
	searchBox1->setName("searchBox1");
	searchBox1->setPlaceHolder("Search Tag:");
	searchBox1->setPlaceHolderColor(Color4B::YELLOW);
	searchBox1->setMaxLengthEnabled(true);
	searchBox1->setMaxLength(searchBoxMaxLength);
	searchBox1->setTextColor(Color4B::WHITE);
	searchBox1->setAnchorPoint(Vec2::ZERO);
	searchBox1->setPosition(Vec2(
		visiableSize.width - searchBoxSize.width - textPaddingHeight, 
		visiableSize.height - searchBoxSize.height - textPaddingHeight));
	
	auto searchBox2 = ui::TextField::create();
	searchBox2->setTouchAreaEnabled(true);
	searchBox2->setTouchSize(searchBoxSize);
	searchBox2->setContentSize(searchBoxSize);
	searchBox2->setCursorEnabled(true);
	searchBox2->setName("searchBox2");
	searchBox2->setPlaceHolder("Search Properties:");
	searchBox2->setPlaceHolderColor(Color4B::YELLOW);
	searchBox2->setMaxLengthEnabled(true);
	searchBox2->setMaxLength(searchBoxMaxLength);
	searchBox2->setTextColor(Color4B::WHITE);
	searchBox2->setAnchorPoint(Vec2::ZERO);
	searchBox2->setPosition(Vec2(
		visiableSize.width - searchBoxSize.width - textPaddingHeight,
		visiableSize.height - searchBoxSize.height*2 - textPaddingHeight*2));
	
	auto searchBox3 = ui::TextField::create();
	searchBox3->setTouchAreaEnabled(true);
	searchBox3->setTouchSize(searchBoxSize);
	searchBox3->setContentSize(searchBoxSize);
	searchBox3->setCursorEnabled(true);
	searchBox3->setName("searchBox3");
	searchBox3->setPlaceHolder("Search Content:");
	searchBox3->setPlaceHolderColor(Color4B::YELLOW);
	searchBox3->setMaxLengthEnabled(true);
	searchBox3->setMaxLength(searchBoxMaxLength);
	searchBox3->setTextColor(Color4B::WHITE);
	searchBox3->setAnchorPoint(Vec2::ZERO);
	searchBox3->setPosition(Vec2(
		visiableSize.width - searchBoxSize.width - textPaddingHeight,
		visiableSize.height - searchBoxSize.height*3 - textPaddingHeight*3));

	searchBox1->addEventListener(CC_CALLBACK_2(TreeScene::searchCallback, this));
	searchBox2->addEventListener(CC_CALLBACK_2(TreeScene::searchCallback, this));
	searchBox3->addEventListener(CC_CALLBACK_2(TreeScene::searchCallback, this));

	auto searchBoxBackground1 = Sprite::create(editBoxTexture);
	searchBoxBackground1->setName("searchBoxBackground1");
	searchBoxBackground1->setContentSize(searchBoxSize);
	searchBoxBackground1->setOpacity(0.6 * 255);
	searchBoxBackground1->setAnchorPoint(Vec2::ZERO);
	searchBoxBackground1->setPosition(Vec2(
		visiableSize.width - searchBoxSize.width - textPaddingHeight,
		visiableSize.height - searchBoxSize.height - textPaddingHeight));

	auto searchBoxBackground2 = Sprite::create(editBoxTexture);
	searchBoxBackground2->setName("searchBoxBackground2");
	searchBoxBackground2->setContentSize(searchBoxSize);
	searchBoxBackground2->setOpacity(0.6 * 255);
	searchBoxBackground2->setAnchorPoint(Vec2::ZERO);
	searchBoxBackground2->setPosition(Vec2(
		visiableSize.width - searchBoxSize.width - textPaddingHeight,
		visiableSize.height - searchBoxSize.height * 2 - textPaddingHeight * 2));

	auto searchBoxBackground3 = Sprite::create(editBoxTexture);
	searchBoxBackground3->setName("searchBoxBackground3");
	searchBoxBackground3->setContentSize(searchBoxSize);
	searchBoxBackground3->setOpacity(0.6 * 255);
	searchBoxBackground3->setAnchorPoint(Vec2::ZERO);
	searchBoxBackground3->setPosition(Vec2(
		visiableSize.width - searchBoxSize.width - textPaddingHeight,
		visiableSize.height - searchBoxSize.height * 3 - textPaddingHeight * 3));

	this->addChild(searchBoxBackground1, 5);
	this->addChild(searchBoxBackground2, 5);
	this->addChild(searchBoxBackground3, 5);

	this->addChild(searchBox1, 6);
	this->addChild(searchBox2, 6);
	this->addChild(searchBox3, 6);

	_currentDrawingNum = 0;
	_isMouseDown = false;
	usedNodeNum = 0;
	totoalNodeNum = 1;
	_NodeSequenceTable = new hashTable::Node [1];
	_maxDepth = 0;

	searchStr1 = "Search Tag:";
	searchStr2 = "Search Properties:";
	searchStr3 = "Search Content:";

	auto MouseEventListener = EventListenerMouse::create();
	MouseEventListener->onMouseScroll = CC_CALLBACK_1(TreeScene::onMouseScrollCallback, this);
	MouseEventListener->onMouseMove = CC_CALLBACK_1(TreeScene::onMouseMoveCallback, this);
	MouseEventListener->onMouseDown = CC_CALLBACK_1(TreeScene::onMouseDownCallback, this);
	MouseEventListener->onMouseUp = CC_CALLBACK_1(TreeScene::onMouseUpCallBack, this);
	
	auto keyboardEventListener = EventListenerKeyboard::create();
	keyboardEventListener->onKeyPressed = CC_CALLBACK_2(TreeScene::onKeyDownCallback, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseEventListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);
	
	if(currentDOMTree)
		initializeTree(currentDOMTree);

	return true;
}

void TreeScene::initializeTree(DOMTree *domTree) {
	// clear if tree exists
	if (this->getChildByName("MainTree")) {
		this->removeAllChildren();
		_currentDrawingNum = 0;

		usedNodeNum = 0;
		totoalNodeNum = 1;
		delete[] _NodeSequenceTable;
		_NodeSequenceTable = new hashTable::Node[1];
	}

	// add the tree
	auto tree = Menu::create();
	tree->setName("MainTree");
	this->addChild(tree,2);

	// the drawing process	
	int depth = 0;
	DOMTree::iterator tempItr = domTree->begin();
	for (; *tempItr;) {
		if (usedNodeNum > totoalNodeNum / 2) {
			totoalNodeNum *= 2;
			hashTable::Node *newTable = new hashTable::Node[totoalNodeNum];
			for (int i = 0; i < totoalNodeNum / 2; i++)
				newTable[i] = _NodeSequenceTable[i];
			delete[] _NodeSequenceTable;
			_NodeSequenceTable = newTable;
		}
		hashTable::Node tempNode;
		tempNode.renderNode = initializeOneRenderNode(depth, **tempItr);
		tempNode.dataNode = *tempItr;
		tempNode.hasRSibling = bool( *(tempItr.rSibling()) );
		tempNode.level = depth;
		tempNode.isCollapsed = false;
		tempNode._positionBeforeMove = tempNode.renderNode->getPosition();
		if (depth > _maxDepth)
			_maxDepth = depth;

		// insert to sequence table
		_NodeSequenceTable[usedNodeNum] = tempNode;
		usedNodeNum++;

		if (*(tempItr.firstChild())) {
			depth++;
			tempItr = tempItr.firstChild();
		}
		else if (*(tempItr.rSibling())) {
			tempItr = tempItr.rSibling();
		}
		else {
			for (; *tempItr && !*(tempItr.rSibling()); tempItr = tempItr.parent(), depth--);
			if (*tempItr)
				tempItr = tempItr.rSibling();
		}
	}
	// ** creat the hash table after insertion complete, or the address of the node will change
	for (int i = 0; i < usedNodeNum; i++)
		_nodeHashTable.insertNode(&_NodeSequenceTable[i]);
	
	//set the render table;
	for (int i = 0; i < usedNodeNum; i++)
		_renderTableBeforeCollapse.push_back(&_NodeSequenceTable[i]);
	
	//set current activated node
	activatedNode = _NodeSequenceTable;

	auto visiableSize = Director::getInstance()->getVisibleSize();
	tree->setPosition(Vec2(0, visiableSize.height));
	renderCurrentTree();
}

inline cocos2d::MenuItem *TreeScene::initializeOneRenderNode(int depth, HTMLNode &Node) {
	auto node = MenuItem::create();
	
	// ** add one to the ref count, otherwise it'll be unreferenced automatically
	node->retain();
	
	auto nodeShape = DrawNode::create();


	//get the standard font height
	auto standardContent = MenuItemFont::create("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	standardContentHeight = standardContent->getContentSize().height;

	//draw the type
	std::string nodeContent = std::string("<") += Node.type;
	nodeContent += std::string(">");
	auto nodeContentType = MenuItemFont::create(nodeContent);
	nodeContentType->setFontSizeObj(typeTextSize);
	//nodeContentType->setAnchorPoint(Vec2(0.0f, 0.5f));
	nodeContentType->setPosition(Vec2(standardContentHeight * 3.0f / 2.0f + 3 * textPaddingHeight,
		standardContentHeight * 3.0f / 2.0f + 3 * textPaddingHeight));
	
	node->addChild(nodeContentType, 3);

	// draw the properties
	for (int i = 0; i < 3; i++) {
		if (i >= Node.properties.size())
			break;

		auto property = (Node.properties)[i];

		std::string buffer;
		// draw property text
		if (property.first.length() > maxNodeContentLength)
			buffer = property.first.substr(0, maxNodeContentLength) + " ... ";
		else
			buffer = property.first + " ";
		auto nodeContentKey = MenuItemFont::create(buffer);
		
		if (property.second.length() > maxNodeContentLength)
			buffer = property.second.substr(0, maxNodeContentLength) + " ... ";
		else
			buffer = property.second + " ";
		auto nodeContentValue = MenuItemFont::create(buffer);

		nodeContentKey->setAnchorPoint(Vec2::ZERO);
		nodeContentValue->setAnchorPoint(Vec2::ZERO);

		nodeContentKey->setPosition(Vec2(standardContentHeight * 3 + textPaddingHeight * 7,
			textPaddingHeight*(5 - i * 2) + standardContentHeight*(2 - i)) );
		nodeContentValue->setPosition(Vec2(standardWidth/3.0f+ standardContentHeight * 1.5f + textPaddingHeight * 4, 
			textPaddingHeight*(5 - i * 2) + standardContentHeight*(2 - i)) );

		node->addChild(nodeContentKey, 3);
		node->addChild(nodeContentValue, 3);
	}

	// draw node shape
	cocos2d::Vec2 point[4];
	point[0] = Vec2(0, 0);
	point[1] = Vec2(standardWidth , 0);
	point[2] = Vec2(standardWidth , standardContentHeight*3 + textPaddingHeight*6);
	point[3] = Vec2(0, standardContentHeight*3 + textPaddingHeight*6);
	nodeShape->drawPolygon(point, 4, Color4F(0.306, 0.4, 0.914, 0.7), 1, Color4F(1, 1, 1, 0.12));
	// draw the type box
	point[0] = Vec2(0, 0);
	point[1] = Vec2(standardContentHeight * 3 + textPaddingHeight * 6, 0);
	point[2] = Vec2(standardContentHeight * 3 + textPaddingHeight * 6, standardContentHeight * 3 + textPaddingHeight * 6);
	point[3] = Vec2(0, standardContentHeight * 3 + textPaddingHeight * 6);
	nodeShape->drawPolygon(point, 4, Color4F(0.294, 0.459, 0.91, 1), 1, Color4F(0, 0, 0, 0));

	node->addChild(nodeShape, 1);
	//set node position
	node->setPosition(Vec2(depth*10	*textPaddingHeight, -((_currentDrawingNum+1) * (standardContentHeight*3 + 6*textPaddingHeight) )));
	_currentDrawingNum++;

	return node;
}

void TreeScene::renderCurrentTree() {
	// delete the existing tree
	auto tree = this->getChildByName("MainTree");
	if (!tree)
		return;
	tree->removeAllChildren();

	_currentRenderNodeTable = _renderTableBeforeCollapse;
	hashTable::Node *temp;
	int i = 0;
	for (i; i < _currentRenderNodeTable.size(); i++) {
		temp = _currentRenderNodeTable[i];
			//change the color 
		if (temp == activatedNode && temp && !temp->renderNode->getChildByName("activatedColor")) {	
			auto activatedColor = DrawNode::create();
			cocos2d::Vec2 point[4];
			point[0] = Vec2(0, 0);
			point[1] = Vec2(standardWidth, 0);
			point[2] = Vec2(standardWidth, standardContentHeight * 3 + textPaddingHeight * 6);
			point[3] = Vec2(0, standardContentHeight * 3 + textPaddingHeight * 6);
			activatedColor->drawPolygon(point, 4, Color4F(1, 0.639, 0.239, 0.8), 1, Color4F(0, 0, 0, 0));
			temp->renderNode->addChild(activatedColor,2,"activatedColor");
		}
		tree->addChild(temp->renderNode);
	}
	// perform the animation
	for (int j = 0, nodeCount=0; j <_currentRenderNodeTable.size(); j++, nodeCount++) {
		if (_currentRenderNodeTable[j]->isCollapsed) {
			int currentLevel = _currentRenderNodeTable[j]->level;
			for (j++; j< _currentRenderNodeTable.size() && _currentRenderNodeTable[j]->level > currentLevel;) {
				tree->removeChild(_currentRenderNodeTable[j]->renderNode);
				_currentRenderNodeTable.erase(_currentRenderNodeTable.begin() + j);
			}
			j--;
		}
		temp = _currentRenderNodeTable[j];
		//calculate the current position
		auto _nodeAnime = MoveTo::create(2.0f,Vec2(
			temp->level*10*textPaddingHeight, 
			-(nodeCount+1)*(6 * textPaddingHeight + 3 * standardContentHeight)));
		auto nodeAnime = EaseElasticOut::create(_nodeAnime, 4);
		//run the anime
		temp->renderNode->runAction(nodeAnime);
	}

	// move the tree to correct position
	if (tree) {
		auto screenHeight = Director::getInstance()->getVisibleSize().height;
		auto screenWidth = Director::getInstance()->getVisibleSize().width;

		Vec2 finalPosition = tree->getPosition();
		if (tree->getPosition().y > screenHeight + (_currentRenderNodeTable.size() - 1)*(6 * textPaddingHeight + 3 * standardContentHeight))
			finalPosition.y = screenHeight + (_currentRenderNodeTable.size() - 1)*(6 * textPaddingHeight + 3 * standardContentHeight);
		else if (tree->getPosition().y < screenHeight)
			finalPosition.y = screenHeight;
		if (tree->getPosition().x > 0)
			finalPosition.x = 0;
		else if (tree->getPosition().x < -_maxDepth * 10 * textPaddingHeight)
			finalPosition.x = -_maxDepth * 10 * textPaddingHeight;

		if (finalPosition == tree->getPosition())
			return;

		auto _move = MoveTo::create(0.5f, finalPosition);
		auto move = EaseOut::create(_move, 4);
		tree->runAction(move);
	}
}

void TreeScene::onMouseScrollCallback(EventMouse *event) {
	auto tree = this->getChildByName("MainTree");

	if (tree) {
		auto screenHeight = Director::getInstance()->getVisibleSize().height;
		
		// stops the previous action
		tree->stopActionByTag(1);

		auto treeLocationX = tree->getPosition().x;
		auto treeLocationY = tree->getPosition().y;
		treeLocationY += (standardContentHeight * 3 + textPaddingHeight * 6) * event->getScrollY() * 3;
		if (treeLocationY <= screenHeight)
			treeLocationY = screenHeight;
		else if (treeLocationY >= screenHeight + (_currentRenderNodeTable.size() - 1) * (6 * textPaddingHeight + 3 * standardContentHeight))
			treeLocationY = screenHeight + (_currentRenderNodeTable.size() - 1) * (6 * textPaddingHeight + 3 * standardContentHeight);
		
		auto _treeSceneAction = MoveTo::create(1.0f, Vec2(treeLocationX, treeLocationY));
		auto treeSceneAction = EaseOut::create(_treeSceneAction, 7);
		treeSceneAction->setTag(1);

		tree->runAction(treeSceneAction);
	}
}

void TreeScene::onMouseMoveCallback(EventMouse *event) {
	auto screenHeight = Director::getInstance()->getVisibleSize().height;
	auto bg = this->getChildByName("backgroundImage");

	bg->setPosition(
		-(event->getCursorX() - designResolutionSize.width / 2) / 10.f,
		-(event->getCursorY() - designResolutionSize.width / 2) / 10.f);

	// cover the other content when mouse on detail panel
	auto detailPanel = this->getChildByName("detailPanel");
	if (detailPanel &&
		_mouseStartLocation.x > detailPanel->getPosition().x &&
		(screenHeight - _mouseStartLocation.y) > (detailPanel->getPosition().y - screenHeight * 2.f / 5.f) &&
		(screenHeight - _mouseStartLocation.y) < (detailPanel->getPosition().y + screenHeight * 2.f / 5.f))
		return;

	// remove the scroll bar bg
	auto scrollBg = this->getChildByName("scrollBarBg");
	if (!_isMouseDown && event->getCursorX() > 30 && scrollBg) {
		scrollBg->setName("scrollBg_remove");
		auto _fadeOut = FadeOut::create(0.3f);
		auto fadeOut = Sequence::createWithTwoActions(_fadeOut, RemoveSelf::create());
		scrollBg->runAction(fadeOut);
	}

	auto tree = this->getChildByName("MainTree");
	// scroll directly
	if (tree && _isMouseDown && _mouseStartLocation.x > 30) {
		Vec2 finalPosition;

		Vec2 deltaPosition = event->getLocation() - _mouseStartLocation;
		deltaPosition.y = -deltaPosition.y;
		deltaPosition *= 4.0f;

		finalPosition = _treeStartLocation + deltaPosition;

		tree->setPosition(finalPosition);
	}// scroll by scrollbar 
	else if(tree && _isMouseDown && _mouseStartLocation.x < 30) {
		Vec2 finalPosition;

		auto finalHeight = event->getLocation().y;
		finalHeight = (_currentRenderNodeTable.size() * (textPaddingHeight * 6 + standardContentHeight * 3)) *
			finalHeight / designResolutionSize.height + designResolutionSize.height;

		finalPosition.x = _treeStartLocation.x;
		finalPosition.y = finalHeight;
		if(scrollBg) {
			auto scrollBar = scrollBg->getChildByName("scrollBar");
			scrollBar->setPosition(Vec2(0, designResolutionSize.height * (1 - (tree->getPositionY() - designResolutionSize.height) /
				(_currentRenderNodeTable.size() * (textPaddingHeight * 6 + standardContentHeight * 3)))));
		}

		tree->setPosition(finalPosition);
	}
	else if (tree && !_isMouseDown && event->getCursorX() < 30 && !scrollBg) {
		scrollBg = Sprite::create(scrollBarTexture);
		scrollBg->setName("scrollBarBg");
		scrollBg->setContentSize(Size(30, designResolutionSize.height));
		scrollBg->setAnchorPoint(Vec2::ZERO);
		scrollBg->setPosition(Vec2::ZERO);
		scrollBg->setOpacity(0);

		auto scrollBar = Sprite::create(scrollBarImg);
		scrollBar->setName("scrollBar");
		scrollBar->setContentSize(Size(30, 60));
		scrollBar->setAnchorPoint(Vec2(0, 0.5));
		scrollBar->setPosition(Vec2(0, designResolutionSize.height * (1 - (tree->getPositionY() - designResolutionSize.height) / 
			(_currentRenderNodeTable.size() * (textPaddingHeight*6 + standardContentHeight*3)) )));
		scrollBar->setOpacity(255*0.6f);
		scrollBg->addChild(scrollBar, 1);
		
		auto fadeIn = FadeTo::create(0.3f,255*0.6f);
		scrollBg->runAction(fadeIn);
		this->addChild(scrollBg, 9);
	}
}

void TreeScene::onMouseDownCallback(EventMouse *event){
	_isMouseDown = true;
	_mouseStartLocation = event->getLocation();
	auto screenHeight = Director::getInstance()->getVisibleSize().height;
	
	auto tree = getChildByName("MainTree");
	auto detailPanel = this->getChildByName("detailPanel");

	auto search1 = this->getChildByName("searchBoxBackground1");
	auto search2 = this->getChildByName("searchBoxBackground2");
	auto search3 = this->getChildByName("searchBoxBackground3");

	// cover the other content when activating search box
	if( _mouseStartLocation.x > search1->getPosition().x &&
		_mouseStartLocation.x < search1->getPosition().x + search1->getContentSize().width) {
		if( (screenHeight - _mouseStartLocation.y) > search1->getPosition().y &&
			(screenHeight - _mouseStartLocation.y) < search1->getPosition().y + search1->getContentSize().height) {
			
				search1->setOpacity(255);
				search2->setOpacity(255 * 0.6f);
				search3->setOpacity(255 * 0.6f);
				return;
		}
		else if ((screenHeight - _mouseStartLocation.y) > search2->getPosition().y &&
			(screenHeight - _mouseStartLocation.y) < search2->getPosition().y + search2->getContentSize().height) {

				search1->setOpacity(255 * 0.6f);
				search2->setOpacity(255);
				search3->setOpacity(255 * 0.6f);
				return;
		}
		else if ((screenHeight - _mouseStartLocation.y) > search3->getPosition().y &&
			(screenHeight - _mouseStartLocation.y) < search3->getPosition().y + search3->getContentSize().height) {
				
				search1->setOpacity(255 * 0.6f);
				search2->setOpacity(255 * 0.6f);
				search3->setOpacity(255);
				return;
		}
	}

	// cover the other content when mouse on detail panel
	if (detailPanel &&
		_mouseStartLocation.x > detailPanel->getPosition().x &&
		(screenHeight - _mouseStartLocation.y) > (detailPanel->getPosition().y - screenHeight * 2.f/5.f) &&
		(screenHeight - _mouseStartLocation.y) < (detailPanel->getPosition().y + screenHeight * 2.f/5.f) )
		return;

	if (tree && _mouseStartLocation.x > 30) {
		_treeStartLocation = tree->getPosition();
		if (_mouseStartLocation.y < _treeStartLocation.y &&
			_treeStartLocation.y - (screenHeight - _mouseStartLocation.y) <
			_currentRenderNodeTable.size() *(6 * textPaddingHeight + 3 * standardContentHeight))
		{
			auto currentNode = _currentRenderNodeTable[int((_treeStartLocation.y - (screenHeight - _mouseStartLocation.y)) /
				(6 * textPaddingHeight + 3 * standardContentHeight))];
			
			//click on the collapse button 
			if (_mouseStartLocation.x - tree->getPositionX() > currentNode->renderNode->getPositionX() &&
				_mouseStartLocation.x - tree->getPositionX() < currentNode->renderNode->getPositionX()
				+ (6 * textPaddingHeight + 3 * standardContentHeight))
			{
				// change the collapse status
				currentNode->isCollapsed = (currentNode->isCollapsed) == true ? false : true;

				if (currentNode->isCollapsed) {
					// add a collapse mark
					auto collapseNode = currentNode->renderNode->getChildByName("CollapseMark");
					if (!collapseNode) {
						auto mark = DrawNode::create();
						mark->setName("CollapseMark");
						Vec2 Point[4] = {
							Vec2(0,0),
							Vec2(6 * textPaddingHeight + 3 * standardContentHeight , 0),
							Vec2(6 * textPaddingHeight + 3 * standardContentHeight ,  standardContentHeight),
							Vec2(0, standardContentHeight)
						};
						mark->drawPolygon(Point, 4, Color4F(0.02, 0.192, 0.667, 1), 1, Color4F(0, 0, 0, 0));
						currentNode->renderNode->addChild(mark, 4);
					}
				}
				else {
					// remove the collapse mark							
					currentNode->renderNode->removeChildByName("CollapseMark");
				}
				renderCurrentTree();
			}
		}
	}
}

void TreeScene::onMouseUpCallBack(EventMouse *event) {
	_isMouseDown = false;
	auto tree = this->getChildByName("MainTree");
	if (tree) {
		auto screenHeight = Director::getInstance()->getVisibleSize().height;
		auto screenWidth = Director::getInstance()->getVisibleSize().width;

		Vec2 finalPosition = tree->getPosition();
		if (tree->getPosition().y > screenHeight + (_currentRenderNodeTable.size() - 1)*(6 * textPaddingHeight + 3 * standardContentHeight))
			finalPosition.y = screenHeight + (_currentRenderNodeTable.size() - 1)*(6 * textPaddingHeight + 3 * standardContentHeight);
		else if (tree->getPosition().y < screenHeight)
			finalPosition.y = screenHeight;
		if (tree->getPosition().x > 0)
			finalPosition.x = 0;
		else if (tree->getPosition().x < -_maxDepth * 10 * textPaddingHeight)
			finalPosition.x = -_maxDepth * 10 * textPaddingHeight;

		if (finalPosition == tree->getPosition())
			goto dont_move_tree;

		auto _move = MoveTo::create(0.5f, finalPosition);
		auto move = EaseOut::create(_move, 4);
		tree->runAction(move);

	dont_move_tree:
		auto mouseEndLocation = event->getLocation();
		auto treeEndLocation = tree->getPosition();

		

		if (mouseEndLocation.y < treeEndLocation.y &&
			treeEndLocation.y - (screenHeight - mouseEndLocation.y) <
			_currentRenderNodeTable.size() *(6 * textPaddingHeight + 3 * standardContentHeight))
		{

			if (int((treeEndLocation.y - (screenHeight - mouseEndLocation.y)) /
				(6 * textPaddingHeight + 3 * standardContentHeight)) >= _currentRenderNodeTable.size() ||
				int((treeEndLocation.y - (screenHeight - mouseEndLocation.y)) /
				(6 * textPaddingHeight + 3 * standardContentHeight)) < 0)
				return;

			hashTable::Node *currentNode = _currentRenderNodeTable[int((treeEndLocation.y - (screenHeight - mouseEndLocation.y)) /
				(6 * textPaddingHeight + 3 * standardContentHeight))];
			
			//clicked on the detail button
			if (
				mouseEndLocation.x - tree->getPositionX() >
				currentNode->renderNode->getPositionX()
				+ (6 * textPaddingHeight + 3 * standardContentHeight) &&

				mouseEndLocation.x - tree->getPositionX() <
				currentNode->renderNode->getPositionX()
				+ standardWidth)
			{
				// set the current activated node
				activatedNode->renderNode->removeChildByName("activatedColor");
				activatedNode = _nodeHashTable.getNodeByRenderNode(currentNode->renderNode);

				if (!currentNode->renderNode->getChildByName("activatedColor")) {
					auto activatedColor = DrawNode::create();
					cocos2d::Vec2 point[4];
					point[0] = Vec2(0, 0);
					point[1] = Vec2(standardWidth, 0);
					point[2] = Vec2(standardWidth, standardContentHeight * 3 + textPaddingHeight * 6);
					point[3] = Vec2(0, standardContentHeight * 3 + textPaddingHeight * 6);
					activatedColor->drawPolygon(point, 4, Color4F(1, 0.639, 0.239, 0.8), 1, Color4F(0, 0, 0, 0));
					currentNode->renderNode->addChild(activatedColor, 2, "activatedColor");
				}

				// remove the old panel
				if (this->getChildByName("detailPanel")) {
					auto temp = this->getChildByName("detailPanel");
					temp->setName("detailPanel_remove");
					auto __panelAction = MoveTo::create(0.6f, Vec2(screenWidth, screenHeight / 2));
					auto _panelAction = EaseIn::create(__panelAction, 4);
					auto panelAction = Sequence::createWithTwoActions(_panelAction, RemoveSelf::create());
					temp->runAction(panelAction);
				}

				auto temp = detailPanel::create(currentNode->dataNode);

				temp->setPosition(Vec2(screenWidth, screenHeight / 2));
				temp->setName("detailPanel");

				auto _panelMove = MoveTo::create(1.0f, Vec2(screenWidth*3.0f / 5.0f, screenHeight / 2));
				auto panelMove = EaseBounceOut::create(_panelMove);

				this->addChild(temp, 5);
				temp->runAction(panelMove);
			}
		}
	}
}

void TreeScene::onKeyDownCallback(EventKeyboard::KeyCode code, Event *event) {

	auto detailPanel = this->getChildByName("detailPanel");
	if (detailPanel && code == EventKeyboard::KeyCode::KEY_ESCAPE) {
		auto screenWidth = Director::getInstance()->getVisibleSize().width;
		auto screenHeight = Director::getInstance()->getVisibleSize().height;

		detailPanel->setName("detailPanel_remove");
		auto __panelAction = MoveTo::create(0.6f, Vec2(screenWidth, screenHeight / 2));
		auto _panelAction = EaseIn::create(__panelAction, 4);
		auto panelAction = Sequence::createWithTwoActions(_panelAction, RemoveSelf::create());
		detailPanel->runAction(panelAction);
	}
	else if(code== EventKeyboard::KeyCode::KEY_ESCAPE){
		if (this->getChildByName("pressTwiceNote")) {
			// exit
			auto newScene = HelloWorld::createScene();
			auto transitions = TransitionFade::create(0.5, newScene);
			Director::getInstance()->replaceScene(transitions);
		}
		auto note = Sprite::create(pressTwiceNote);
		note->setName("pressTwiceNote");
		auto size = note->getContentSize();
		note->setContentSize(size * designResolutionSize.height / 1080.f);
		note->setPosition(designResolutionSize / 2);
		note->setOpacity(0);
		auto _fadeIn = FadeIn::create(0.5f);
		auto _fadeOut = FadeOut::create(0.5f);
		auto fade = Sequence::create(_fadeIn, _fadeOut, RemoveSelf::create(), nullptr);
		note->runAction(fade);
		this->addChild(note, 10);
	}
}

void TreeScene::searchCallback(Ref *node, ui::TextField::EventType event) {
	if (event == ui::TextField::EventType::INSERT_TEXT ||
		event == ui::TextField::EventType::DELETE_BACKWARD) {

		auto searchBox1 = dynamic_cast<ui::TextField *>(this->getChildByName("searchBox1"));
		auto searchBox2 = dynamic_cast<ui::TextField *>(this->getChildByName("searchBox2"));
		auto searchBox3 = dynamic_cast<ui::TextField *>(this->getChildByName("searchBox3"));

		//if no string has changed
		if (searchBox1->getString() == searchStr1 &&
			searchBox2->getString() == searchStr2 &&
			searchBox3->getString() == searchStr3)
			return;

		auto tree = this->getChildByName("MainTree");
		_renderTableBeforeCollapse.clear();

		searchStr1 = searchBox1->getString();
		searchStr2 = searchBox2->getString();
		searchStr3 = searchBox3->getString();

		if (searchStr1 != "Search Tag:" && searchStr1 != "") {
			for (int i = 0; i < usedNodeNum; i++) {
				if (_NodeSequenceTable[i].dataNode->searchType(searchStr1).size() != 0)
					_renderTableBeforeCollapse.push_back(_NodeSequenceTable + i);
			}
		}
		else {
			for (int i = 0; i < usedNodeNum; i++)
				_renderTableBeforeCollapse.push_back(_NodeSequenceTable + i);
		}

		if (searchBox2->getString() != "Search Properties:" && searchStr2 != "") {
			for (int i = 0; i < _renderTableBeforeCollapse.size(); i++)
				if (_renderTableBeforeCollapse[i]->dataNode->searchProperty(searchStr2) == false)
					_renderTableBeforeCollapse.erase(_renderTableBeforeCollapse.begin() + i), i--;
		}

		if (searchBox3->getString() != "Search Content:" && searchStr3 != "") {
			for (int i = 0; i < _renderTableBeforeCollapse.size(); i++)
				if (_renderTableBeforeCollapse[i]->dataNode->searchContent(searchStr3).size() == 0)
					_renderTableBeforeCollapse.erase(_renderTableBeforeCollapse.begin() + i), i--;
		}

		renderCurrentTree();
	}
}

TreeScene::~TreeScene() {
	// release the node
	for (int i = 0; i < usedNodeNum; i++)
		_NodeSequenceTable[i].renderNode->release();
	delete[] _NodeSequenceTable;
}