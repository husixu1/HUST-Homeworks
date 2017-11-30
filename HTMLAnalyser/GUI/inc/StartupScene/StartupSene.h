#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "HTML_analyser.h"

class HelloWorld : public cocos2d::Layer
{
private:
	bool MouseDownCallback(cocos2d::EventMouse *event);
	void ShowSubMenuCallback();
	
	void SideButton1Callback();
	void SideButton2Callback();
	void SideButton3Callback();

	void MouseMoveCallback(cocos2d::Event *event);
	void keyDownCallback(cocos2d::EventKeyboard::KeyCode code,cocos2d::Event *event);

	bool isCenterButtonClicked;

	~HelloWorld();

	typedef struct _Parameter {
		HTMLAnalyser *analyser;
		HelloWorld *performer;
	}Parameter;
	static DWORD WINAPI AnalyseHTMLThread(LPVOID lpParameter);
	void analyseFailRetrnCallback();
	void analyseSuccRetrnCallback();
	DOMTree analyseResult;

	void addWaitingSign();

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
//TODO: screen resolution fit