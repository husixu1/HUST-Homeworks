#include "cocos2d.h"
#include "detailPanel.h"
#include "DOM_tree_ADT.h"
#include "resourceConfig.h"
#include "ui/UIScrollView.h"

detailPanel* detailPanel::create(HTMLNode *contnet){
	detailPanel *pRet = new(std::nothrow) detailPanel(); 
		if (pRet) 
		{
			pRet->setContent(contnet);
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

bool detailPanel::init() {
	if(!Menu::init()) {
		return false;
	}
	auto BackGround = DrawNode::create();
	auto visiableSize = Director::getInstance()->getVisibleSize();
	Vec2 Point[4] = {
		Vec2(0, visiableSize.height *2.0f / 5.0f),
		Vec2(visiableSize.width *2.0f / 5.0f,visiableSize.height *2.0f / 5.0f),
		Vec2(visiableSize.width *2.0f / 5.0f, -visiableSize.height *2.0f / 5.0f),
		Vec2(0, -visiableSize.height *2.0f / 5.0f),
	};
	BackGround->drawPolygon(Point, 4, Color4F(0.949, 0.227, 0.608,0.85), 1, Color4F(1,1,1,0.2));
	auto bgItem = MenuItem::create();
	bgItem->addChild(BackGround, 1);
	this->addChild(bgItem);
	
	if (contentNode) {
		std::string buffer, subbuffer;
		
		//write the type
		buffer += "______________________________________________________\n\n";
		buffer += contentNode->type;
		buffer += "\n______________________________________________________\n\n";
		
		//write the properties
		for(int i=0,counter = 0;i<contentNode->properties.size();i++) {
			subbuffer += (contentNode->properties)[i].first;
			subbuffer += "  =  ";
			subbuffer += (contentNode->properties)[i].second;
			for(int i=0; i<subbuffer.size()/maxDetailContentLength; i++)
				subbuffer.insert(i*maxDetailContentLength,"\n");
			subbuffer.push_back('\n');
		}
		buffer += subbuffer;
		buffer += "\n______________________________________________________\n\n";
		
		//write the content
		for(int i=0, count = 0; i<contentNode->content.size(); i++, count++) {
			buffer.push_back((contentNode->content)[i]);
			if ((contentNode->content)[i] == '\n' || (contentNode->content)[i] == '\r')
				count = 0;
			if (count > maxDetailContentLength) {
				buffer.push_back('\n');
				count = 0;
			}
		}
		
		buffer += "\n______________________________________________________";
		// prohibit an empty buffer
		if (buffer.size() == 0)
			buffer = " ";

		auto content = MenuItemFont::create(buffer);
		content->setAnchorPoint(Vec2(0, 1));
		content->setPosition(Vec2(textPaddingHeight, content->getContentSize().height));

		auto scrollView = ui::ScrollView::create();
		scrollView->setScrollBarAutoHideEnabled(true);
		scrollView->setScrollBarAutoHideTime(2);
		scrollView->setBounceEnabled(true);
		scrollView->setContentSize(Size(visiableSize.width*2.f / 5.f, visiableSize.height*4.f / 5.f));
		scrollView->setScrollBarEnabled(true);
		scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
		if (content->getContentSize().height <= visiableSize.height*4.f / 5.f)
			scrollView->setInnerContainerSize(Size(visiableSize.width*2.f / 5.f, visiableSize.height*4.f / 5.f));
		else
			scrollView->setInnerContainerSize(Size(visiableSize.width*2.f / 5.f, content->getContentSize().height));
		scrollView->setScrollBarOpacity(255 * 0.7);
		scrollView->setAnchorPoint(Vec2(0, 0.5));
		scrollView->setInnerContainerPosition(Vec2(0, visiableSize.height *2.0f / 5.0f));

		scrollView->setPosition(Vec2::ZERO);

		scrollView->addChild(content, 1);
		auto item = MenuItem::create();
		item->addChild(scrollView,1);
		this->addChild(item, 2);
	}
	return true;
}

