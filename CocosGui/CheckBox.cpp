//
//  CheckBox.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/23.
//
//

#include "CheckBox.h"

NS_CC_GUI_BEGIN

CheckBox::CheckBox()
: _bMoved(false)
{
    _parentPropertyArr.clear();
}

CheckBox::~CheckBox()
{
}

CheckBox* CheckBox::create(const std::string& normalImage, const ccButtonCallback& callback)
{
    return create(normalImage, normalImage, normalImage, callback);
}

CheckBox* CheckBox::create(const std::string& normalImage, const std::string& selectedImage,
                                   const ccButtonCallback& callback)
{
    return create(normalImage, selectedImage, normalImage, callback);
}

CheckBox* CheckBox::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccButtonCallback& callback)
{
    auto checkBox = new (std::nothrow) CheckBox();
    if (checkBox && checkBox->initWithFile(normalImage))
    {
        checkBox->setBackgroudImageForState(normalImage, ButtonState::Noramal);
        checkBox->setBackgroudImageForState(selectedImage, ButtonState::Selected);
        checkBox->setBackgroudImageForState(disabledImage, ButtonState::Disabled);
        checkBox->_callBack = callback;
        
        checkBox->autorelease();
        
        return checkBox;
    }
    CC_SAFE_DELETE(checkBox);
    return nullptr;
}

bool CheckBox::initWithFile(const std::string& filename)
{
    if (filename.size() <= 0)
    {
        return false;
    }
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
    if (frame)
    {
        return Sprite::initWithSpriteFrameName(filename);
    } else
    {
        return Sprite::initWithFile(filename);
    }
    return true;
}

void CheckBox::setSpriteWithFile(const std::string& filename)
{
    CCAssert(filename.size() > 0, "file is not exists");
    
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
    if (frame)
    {
        setSpriteFrame(filename);
    } else
    {
        setTexture(filename);
    }
}

void CheckBox::needsLayout(void)
{
    std::string stateFileName = getBackgroudImageForState(_buttonState);
    if (0 < stateFileName.size())
    {
        setSpriteWithFile(stateFileName);
    }
}

bool CheckBox::touchHits(Touch *touch)
{
    auto hitPos = getParent()->convertToNodeSpace(touch->getLocation());
    if (this->boundingBox().containsPoint(hitPos))
    {
        return true;
    }
    return false;
}

bool CheckBox::onTouchBegan(Touch  *touch, Event  *event)
{
    _parentPropertyArr.clear();
    _parentPropertyArr = getBtnParentPropertyList(this);
    _bMoved = false;
    
    auto hits = touchHits(touch);
    if (hits && _enabled && isVisible())
    {
        return true;
    }
    return false;
}

void CheckBox::onTouchEnded(Touch  *touch, Event  *event)
{
    if (checkParentScroll(_parentPropertyArr))
    {
        // if parent is scrolling, intercept button event
        return;
    }
    if(_enabled && !_bMoved && isVisible())
    {
        setSelected(!getSelected());
        auto hits = touchHits(touch);
        if (hits && _callBack)
        {
            _callBack(this);
        }
        //        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Audio_Effect_Btn_Click);
    }
}

void CheckBox::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto hits = touchHits(touch);
    if (hits && _enabled && isVisible())
    {
        setSelected(!getSelected());
    } else
    {
        setSelected(!getSelected());
    }
}

void CheckBox::setVisible(bool visible)
{
    setEnabled(visible);
    Sprite::setVisible(visible);
}

NS_CC_GUI_END