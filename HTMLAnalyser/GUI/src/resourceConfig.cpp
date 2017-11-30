#include <string>
#include "cocos2d.h"

USING_NS_CC;

Size designResolutionSize = cocos2d::Size(1920, 1080);

std::string BackgroundImagePath = "Resources\\StartupScene\\Background.jpg";
std::string CenterImagePath = "Resources\\StartupScene\\Center.png";
std::string SideImage1Path = "Resources\\StartupScene\\Side1.png";
std::string SideImage2Path = "Resources\\StartupScene\\Side2.png";
std::string SideImage3Path = "Resources\\StartupScene\\Side3.png";
std::string ConnectionErrorImagePath = "Resources\\StartupScene\\ConnectionError.png";
std::string StencliOutRightPath = "Resources\\StartupScene\\OutRClip.png";
std::string StencliOutLeftPath = "Resources\\StartupScene\\OutLClip.png";
std::string StencliInRightPath = "Resources\\StartupScene\\InRClip.png";
std::string StencliInLeftPath = "Resources\\StartupScene\\InLClip.png";
std::string WaitBackgroundPath = "Resources\\StartupScene\\WaitBG.png";
std::string HelpPath = "Resources\\StartupScene\\Help.png";

std::string editBoxTexture = "Resources\\TreeScene\\EditBoxTexture.png";
std::string pressTwiceNote = "Resources\\TreeScene\\pressTwice.png";
std::string scrollBarTexture = "Resources\\TreeScene\\ScrollBarTexture.jpg";
std::string scrollBarImg = "Resources\\TreeScene\\ScrollBar.jpg";

int standardWidth = 1000;
int textPaddingHeight = 4;
int typeTextSize = 30;
int standardContentHeight;
int maxNodeContentLength = 30;
int maxDetailContentLength = 45;
int searchBoxMaxLength = 25;

Size searchBoxSize = Size(300,30);
Size frameSize;