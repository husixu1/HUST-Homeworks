#include "StartupSene.h"
#include "SimpleAudioEngine.h"
#include "resourceConfig.h"
#include "TreeScene.h"
#include <windows.h>

USING_NS_CC;

static bool _HTMLDownloadLock;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto background = Sprite::create(BackgroundImagePath);
	background->setContentSize(cocos2d::Size(1920, 1080));
	background->setAnchorPoint(Vec2::ZERO);
	background->setPosition(Vec2::ZERO);
	this->addChild(background, 1);
	
	// floating action 
	auto mouseEventListener = EventListenerMouse::create();
	mouseEventListener->onMouseMove = CC_CALLBACK_1(HelloWorld::MouseMoveCallback, this);
	mouseEventListener->onMouseDown = CC_CALLBACK_1(HelloWorld::MouseDownCallback, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEventListener, this);

	auto keyboardEventListener = EventListenerKeyboard::create();
	keyboardEventListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::keyDownCallback, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);

	// the center button
    auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto centerImage = MenuItemImage::create(
			CenterImagePath,
			CenterImagePath);

	centerImage->setName("CenterImage");
	centerImage->setPosition(Vec2(visibleSize.width / 2 , 
									visibleSize.height / 2) );
	// make center button dynamic
	auto scale1 = ScaleTo::create(0.1f, 1.1f);
	auto _scale2 = ScaleTo::create(0.6f, 1.0f);
	auto scale2 = EaseSineIn::create(_scale2);
	auto _viberate = Sequence::create({ scale1,scale2 });
	auto viberate = RepeatForever::create(_viberate);
	centerImage->runAction(viberate);

	auto menu = Menu::create();
	menu->addChild(centerImage, 2);
	menu->setName("Menu");
	menu->setPosition(Vec2::ZERO);

	this->addChild(menu, 1);
    
	isCenterButtonClicked = false;
	_HTMLDownloadLock = false;

#ifdef STATUS_DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

    return true;
}

HelloWorld::~HelloWorld() {
#ifdef STATUS_DEBUG
	FreeConsole();
	#endif
}


bool HelloWorld::MouseDownCallback(EventMouse *event) {
	auto positionX = event->getCursorX();
	auto positionY = event->getCursorY();
	
	auto centerImage = this->getChildByName("Menu")->getChildByName("CenterImage");
	auto sideImage1 = this->getChildByName("Menu")->getChildByName("SideImage1");
	auto sideImage2 = this->getChildByName("Menu")->getChildByName("SideImage2");
	auto sideImage3 = this->getChildByName("Menu")->getChildByName("SideImage3");


	Vec2 Position(positionX, positionY);

	if ((Position - centerImage->getPosition() ).length() < 350)
		ShowSubMenuCallback();
	else if (sideImage1 && sideImage2 && sideImage3) {
		if (abs((Position - sideImage1->getPosition()).x) < sideImage1->getContentSize().width / 2 &&
				abs((Position - sideImage1->getPosition()).y) < sideImage1->getContentSize().height / 2)
			SideButton1Callback(); 
		if (abs((Position - sideImage2->getPosition()).x) < sideImage2->getContentSize().width / 2 &&
				abs((Position - sideImage2->getPosition()).y) < sideImage2->getContentSize().height / 2)
			SideButton2Callback(); 
		if (abs((Position - sideImage3->getPosition()).x) < sideImage3->getContentSize().width / 2 &&
				abs((Position - sideImage3->getPosition()).y) < sideImage3->getContentSize().height / 2)
			SideButton3Callback();
	}
	return true;
}

void HelloWorld::ShowSubMenuCallback() {
	if (!isCenterButtonClicked) {
		isCenterButtonClicked = true;
		auto menu = this->getChildByName("Menu");
		auto CenterImage = menu->getChildByName("CenterImage");
		//auto originalSize = CenterImage->getContentSize();
		//CenterImage->setContentSize(originalSize * frameSize.height / designResolutionSize.height);

		auto SideImage1 = MenuItemImage::create(
			SideImage1Path,
			SideImage1Path);

		auto SideImage2 = MenuItemImage::create(
			SideImage2Path,
			SideImage2Path);

		auto SideImage3 = MenuItemImage::create(
			SideImage3Path,
			SideImage3Path);

		//originalSize = SideImage1->getContentSize();
		//SideImage1->setContentSize(originalSize * designResolutionSize.height / 1080.f);
		//originalSize = SideImage2->getContentSize();
		//SideImage2->setContentSize(originalSize * designResolutionSize.height / 1080.f);
		//originalSize = SideImage3->getContentSize();
		//SideImage3->setContentSize(originalSize * designResolutionSize.height / 1080.f);

		SideImage1->setPosition(Vec2(
			CenterImage->getPositionX(),
			CenterImage->getPositionY() + SideImage2->getContentSize().height*1.2f));

		SideImage2->setPosition(Vec2(
			CenterImage->getPositionX(),
			CenterImage->getPositionY()));

		SideImage3->setPosition(Vec2(
			CenterImage->getPositionX(),
			CenterImage->getPositionY() - SideImage2->getContentSize().height*1.2f));

		menu->addChild(SideImage1, 1, "SideImage1");
		menu->addChild(SideImage2, 1, "SideImage2");
		menu->addChild(SideImage3, 1, "SideImage3");
		
		auto _CenterImageMove = MoveBy::create(0.3f, Vec2(-(CenterImage->getContentSize().width / 3.0), 0));
		auto CenterImageMove = EaseOut::create(_CenterImageMove, 3);


		auto __SideImageMove1 = MoveBy::create(0.3f, Vec2(SideImage2->getContentSize().width / 5, 0));
		auto __SideImageMove2 = MoveBy::create(0.3f, Vec2(SideImage2->getContentSize().width / 5, 0));
		auto __SideImageMove3 = MoveBy::create(0.3f, Vec2(SideImage2->getContentSize().width / 5, 0));

		auto _SideImageMove1 = EaseOut::create(__SideImageMove1, 3.0f);
		auto _SideImageMove2 = EaseOut::create(__SideImageMove2, 3.0f);
		auto _SideImageMove3 = EaseOut::create(__SideImageMove3, 3.0f);
		
		CenterImage->runAction(CenterImageMove);
		SideImage1->runAction(_SideImageMove1);
		SideImage2->runAction(_SideImageMove2);
		SideImage3->runAction(_SideImageMove3);
	}
	else {
		isCenterButtonClicked = false;
		auto menu = this->getChildByName("Menu");
		auto CenterImage = menu->getChildByName("CenterImage");
		
		auto SideImage1 = menu->getChildByName("SideImage1");
		auto SideImage2 = menu->getChildByName("SideImage2");
		auto SideImage3 = menu->getChildByName("SideImage3");

		auto _CenterImageMove = MoveBy::create(0.3f, Vec2((CenterImage->getContentSize().width / 3.0), 0));
		auto CenterImageMove = EaseOut::create(_CenterImageMove, 3);
		CenterImage->runAction(CenterImageMove);

		auto _SideImageMove1 = MoveBy::create(0.3f, Vec2(-SideImage2->getContentSize().width / 5, 0));
		auto _SideImageMove2 = MoveBy::create(0.3f, Vec2(-SideImage2->getContentSize().width / 5, 0));
		auto _SideImageMove3 = MoveBy::create(0.3f, Vec2(-SideImage2->getContentSize().width / 5, 0));

		auto SideImageMove1 = EaseOut::create(_SideImageMove1, 3.0f);
		auto SideImageMove2 = EaseOut::create(_SideImageMove2, 3.0f);
		auto SideImageMove3 = EaseOut::create(_SideImageMove3, 3.0f);

		menu->removeChild(SideImage1);
		menu->removeChild(SideImage2);
		menu->removeChild(SideImage3);

	}

	//Director::getInstance()->end();
	//	exit(0);
}

void HelloWorld::SideButton1Callback() {
	auto help = Sprite::create(HelpPath);
	help->setName("help");
	auto size = help->getContentSize();
	help->setContentSize(designResolutionSize);
	help->setAnchorPoint(Vec2::ZERO);
	help->setPosition(Vec2::ZERO);
	help->setOpacity(0);
	auto fadeIn = FadeIn::create(0.8f);
	help->runAction(fadeIn);
	this->addChild(help, 100);
}

void HelloWorld::SideButton2Callback() {
	auto currentScene = getScene();
	
	// prevent urlBox appear twice
	auto examer = this->getChildByName("urlBox");
	if (!examer) {
		auto urlBox = ui::TextField::create();
		urlBox->setName("urlBox");
		urlBox->setTouchAreaEnabled(true);
		urlBox->setTouchSize(Size(designResolutionSize.width / 5, designResolutionSize.height / 20));
		urlBox->setFontName(std::string("ºÚÌå"));
		urlBox->setFontSize(70.f * designResolutionSize.height / 1080);
		urlBox->setPlaceHolder("InputURL:");
		urlBox->setPlaceHolderColor(Color4B::GRAY);
		urlBox->setFocused(true);
		urlBox->setFocusEnabled(true);

		urlBox->setContentSize(Size(designResolutionSize.width / 5, designResolutionSize.height / 20));
		urlBox->setPosition(Vec2(designResolutionSize.width / 2, designResolutionSize.height - designResolutionSize.height / 20));

		this->addChild(urlBox, 10);
		urlBox->requestFocus();
	}
}


void HelloWorld::SideButton3Callback() {
	exit(0);
}

void HelloWorld::MouseMoveCallback(Event *event) {
	auto visiableSize = Director::getInstance()->getVisibleSize();
	auto menu = this->getChildByName("Menu");
	menu->setPosition(Vec2(
		-(((EventMouse *)event)->getCursorX()-visiableSize.width/2.0)/10.0,
		-(((EventMouse *)event)->getCursorY()-visiableSize.height/2.0)/10.0
	));
}

void HelloWorld::keyDownCallback(EventKeyboard::KeyCode code, Event *event) {
	auto urlBox = static_cast<ui::TextField *>(this->getChildByName("urlBox"));
	auto help = this->getChildByName("help");
	if (code == EventKeyboard::KeyCode::KEY_ENTER) {
		if (urlBox) {

			std::string url = urlBox->getString();
			// append http
			if (url.substr(0, 7) != "http://" &&
				url.substr(0, 8) != "https://")
				url.insert(url.begin(), { 'h','t','t','p',':','/','/' });
			
			// get and analyse
			HTMLAnalyser *temp = new HTMLAnalyser;
			temp->setUrl(url);
		
			if (!_HTMLDownloadLock) {
				Parameter* parameter = new Parameter;
				*parameter = { temp, this };

				DWORD ThreadID;
				HANDLE newThread = CreateThread(0, 0, HelloWorld::AnalyseHTMLThread, static_cast<void *>(parameter), 0, &ThreadID);
			}
		}
	}
	else if (help && code == EventKeyboard::KeyCode::KEY_ESCAPE) {
		// remove the url box if exist
		auto _fadeOut = FadeOut::create(0.5f);
		auto fadeOut = Sequence::createWithTwoActions(_fadeOut, RemoveSelf::create());
		help->runAction(fadeOut);
		return;
	}
	else if(urlBox && code == EventKeyboard::KeyCode::KEY_ESCAPE) {
		// remove the url box if exist
		auto _fadeOut = FadeOut::create(0.5f);
		auto fadeOut = Sequence::createWithTwoActions(_fadeOut, RemoveSelf::create());
		urlBox->runAction(fadeOut);
		return;

	}
	else if (code == EventKeyboard::KeyCode::KEY_ESCAPE) {
		if (this->getChildByName("pressTwiceNote")) {
			exit(0);
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

void HelloWorld::addWaitingSign() {
	auto waiting = ClippingNode::create();
	waiting->setName("waitingSign");
	waiting->setPosition(designResolutionSize / 2);

	auto stencil = Node::create();
	auto stencilOutLeft = Sprite::create(StencliOutLeftPath);
	auto stencilOutRight = Sprite::create(StencliOutRightPath);
	auto stencilInLeft = Sprite::create(StencliInLeftPath);
	auto stencilInRight = Sprite::create(StencliInRightPath);
	stencil->addChild(stencilInLeft);
	stencil->addChild(stencilInRight);
	stencil->addChild(stencilOutLeft);
	stencil->addChild(stencilOutRight);

	auto _rotateClockwise = RotateBy::create(1.2f, 220);
	auto _rotateAntiClockwise = RotateBy::create(1.2f, -220);
	auto _rotateClockwise2 = RotateBy::create(1.f, 220);
	auto _rotateAntiClockwise2 = RotateBy::create(1.f, -220);

	auto rotateClockwise = EaseSineOut::create(_rotateClockwise);
	auto rotateAntiClockWise = EaseSineOut::create(_rotateAntiClockwise);
	auto rotateClockwise2 = EaseSineOut::create(_rotateClockwise2);
	auto rotateAntiClockWise2 = EaseSineOut::create(_rotateAntiClockwise2);
	
	auto _sinRotateClockwise = RepeatForever::create(rotateClockwise);
	auto _sinRotateAntiClockwise = RepeatForever::create(rotateAntiClockWise);
	auto _sinRotateClockwise2 = RepeatForever::create(rotateClockwise2);
	auto _sinRotateAntiClockwise2 = RepeatForever::create(rotateAntiClockWise2);

	stencilOutLeft->runAction(_sinRotateAntiClockwise);
	stencilOutRight->runAction(_sinRotateAntiClockwise2);
	stencilInRight->runAction(_sinRotateClockwise);
	stencilInLeft->runAction(_sinRotateClockwise2);

	waiting->setStencil(stencil);
	waiting->setInverted(true);
	waiting->setAlphaThreshold(0);
	waiting->addChild(Sprite::create(WaitBackgroundPath));

	this->addChild(waiting,11);
}


//------------------------analyse Interface---------------------
DWORD WINAPI HelloWorld::AnalyseHTMLThread(LPVOID lpParameter) {
	std::cout << "Start Analyse At Thread 2" << std::endl;
	_HTMLDownloadLock = true;
	
	Parameter *para = static_cast<Parameter *>(lpParameter);

	Director::getInstance()
		->getScheduler()
		->performFunctionInCocosThread(CC_CALLBACK_0(HelloWorld::addWaitingSign, para->performer));
	// open another thread here 
	try {
		para->performer->analyseResult = para->analyser->downloadAndAnalyse("testhtml.html");
	}
	catch (int a) {
		Director::getInstance()
		->getScheduler()
		->performFunctionInCocosThread(CC_CALLBACK_0(HelloWorld::analyseFailRetrnCallback, para->performer));
		_HTMLDownloadLock = false;
		return -1;
	}

	Director::getInstance()
		->getScheduler()
		->performFunctionInCocosThread(CC_CALLBACK_0(HelloWorld::analyseSuccRetrnCallback, para->performer));


	delete para->analyser;
	delete para;
	_HTMLDownloadLock = false;
	return 0;
}

void HelloWorld::analyseFailRetrnCallback(){
	// delWaitingSign (Immediate)
	auto waitingSign = this->getChildByName("waitingSign");
	if (waitingSign)
		this->removeChild(waitingSign);
	
	// pop a message showing connection error
	auto errorSprite = Sprite::create(ConnectionErrorImagePath);
	errorSprite->setPosition(designResolutionSize / 2.f);
	auto _fadeIn = FadeIn::create(0.7f);
	auto _fadeOut = FadeOut::create(0.7f);
	auto _delay = DelayTime::create(0.5f);
	auto finalAct = Sequence::create(_fadeIn, _delay, _fadeOut, RemoveSelf::create(), nullptr);
	errorSprite->runAction(finalAct);


	this->addChild(errorSprite, 11);
}


void HelloWorld::analyseSuccRetrnCallback() {
	// if not catch any excption
	auto newScene = TreeScene::createScene(&analyseResult);
	auto transition = TransitionFade::create(0.5, newScene);
	Director::getInstance()->replaceScene(transition);
}
