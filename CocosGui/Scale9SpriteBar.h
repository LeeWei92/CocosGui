//
//  Scale9SpriteBar.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/1/14.
//
//

#ifndef Scale9SpriteBar_h
#define Scale9SpriteBar_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class Scale9SpriteBar : public Sprite
{
public:
    static Scale9SpriteBar *create(const std::string& bgFileName, const std::string& barFileName);
    static Scale9SpriteBar *create(ui::Scale9Sprite* bgScale9Sprite, Size bgScale9SpriteSize, const std::string& barFileName);
    CC_SYNTHESIZE_READONLY(float, _percentage, Percentage);
    
    void setPercentage(float percentage);
    
    void fadeIn(float time);
    void fadeOut(float time);
private:
    ui::Scale9Sprite* _barScale9Sprite;
    Size _originalBarSpriteSize;
    
CC_CONSTRUCTOR_ACCESS:
    Scale9SpriteBar(void);
    virtual ~Scale9SpriteBar(void);
    bool initWithFile(const std::string& fileName);
};

#endif /* Scale9SpriteBar_h */
