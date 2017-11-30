#ifndef __RESOURCECONFIG_H__
#define __RESOURCECONFIG_H__

#include "cocos2d.h"
#include <string>

extern cocos2d::Size designResolutionSize;

extern std::string BackgroundImagePath;
extern std::string CenterImagePath;
extern std::string SideImage1Path;
extern std::string SideImage2Path;
extern std::string SideImage3Path;
extern std::string ConnectionErrorImagePath;
extern std::string StencliOutRightPath;
extern std::string StencliOutLeftPath;
extern std::string StencliInRightPath;
extern std::string StencliInLeftPath;
extern std::string WaitBackgroundPath;
extern std::string HelpPath;

extern std::string editBoxTexture;
extern std::string pressTwiceNote;
extern std::string scrollBarTexture;
extern std::string scrollBarImg;

extern int standardWidth;
extern int textPaddingHeight;
extern int typeTextSize;
extern int standardContentHeight;
extern int maxNodeContentLength;
extern int maxDetailContentLength;
extern int searchBoxMaxLength;

extern cocos2d::Size searchBoxSize;
extern cocos2d::Size frameSize;
#endif //  __RESOURCECONFIG_H__