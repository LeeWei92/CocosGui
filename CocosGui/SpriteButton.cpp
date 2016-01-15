//
//  SpriteButton.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/18.
//
//

#include "SpriteButton.h"

NS_CC_GUI_BEGIN

SpriteButton::SpriteButton()
: _originalScale(1.0f)
, _titleRenderer(nullptr)
, _zoomOnTouchDown(true)
{
    _titleColorStateMap.clear();
    _parentPropertyArr.clear();
}

SpriteButton::~SpriteButton()
{
}

SpriteButton* SpriteButton::create(const std::string &title, const std::string &fontName, float fontSize)
{
    auto spriteBtn = new (std::nothrow) SpriteButton();
    if (spriteBtn && spriteBtn->init())
    {
        spriteBtn->initRenderer();
        spriteBtn->setTitleProperity(title, fontName, fontSize);
        spriteBtn->setContentSize(spriteBtn->_titleRenderer->getContentSize());
        spriteBtn->autorelease();
        return spriteBtn;
    }
    CC_SAFE_DELETE(spriteBtn);
    return nullptr;
}

SpriteButton* SpriteButton::create(const std::string& normalImage, const ccButtonCallback& callback)
{
    return create(normalImage, normalImage, normalImage, callback);
}

SpriteButton* SpriteButton::create(const std::string& normalImage, const std::string& highlightedImage,
                                   const ccButtonCallback& callback)
{
    return create(normalImage, highlightedImage, normalImage, callback);
}

SpriteButton* SpriteButton::create(const std::string& normalImage, const std::string& highlightedImage, const std::string& disabledImage, const ccButtonCallback& callback)
{
    auto spriteButton = new (std::nothrow) SpriteButton();
    if (spriteButton && spriteButton->initWithFile(normalImage))
    {
        spriteButton->setBackgroudImageForState(normalImage, ButtonState::Noramal);
        spriteButton->setBackgroudImageForState(highlightedImage, ButtonState::Highlighted);
        spriteButton->setBackgroudImageForState(disabledImage, ButtonState::Disabled);
        spriteButton->initRenderer();
        spriteButton->_callBack = callback;
        
        spriteButton->autorelease();
        
        return spriteButton;
    }
    CC_SAFE_DELETE(spriteButton);
    return nullptr;
}

bool SpriteButton::initWithFile(const std::string& filename)
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

void SpriteButton::setSpriteWithFile(const std::string& filename)
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

void SpriteButton::needsLayout(void)
{
    std::string stateFileName = getBackgroudImageForState(_buttonState);
    if (0 < stateFileName.size())
    {
        setSpriteWithFile(stateFileName);
    }
    if (_titleRenderer && isExistTitleColorForState(_buttonState))
    {
        _titleRenderer->setColor(getTitleColorForState(_buttonState));
    }
}

bool SpriteButton::touchHits(Touch *touch)
{
    auto hitPos = getParent()->convertToNodeSpace(touch->getLocation());
    if (this->boundingBox().containsPoint(hitPos))
    {
        return true;
    }
    return false;
}

bool SpriteButton::onTouchBegan(Touch  *touch, Event  *event)
{
    _parentPropertyArr.clear();
    _parentPropertyArr = getBtnParentPropertyList(this);
    
    auto hits = touchHits(touch);
    if (hits && !_highlighted && _enabled && isVisible())
    {
        if (_zoomOnTouchDown)
            _originalScale = this->getScale();
        setHighlighted(true);
    }
    return hits;
}

void SpriteButton::setHighlighted(bool highlighted)
{
    ButtonProtocol::setHighlighted(highlighted);
    if (highlighted)
    {
        if (_zoomOnTouchDown)
            setScale(_originalScale - 0.05f);
    } else
    {
        setScale(_originalScale);
    }
}

void SpriteButton::onTouchEnded(Touch  *touch, Event  *event)
{
    if (checkParentScroll(_parentPropertyArr))
    {
        // if parent is scrolling, intercept button event
        return;
    }
    if(_enabled && isVisible())
    {
        setHighlighted(false);
        auto hits = touchHits(touch);
        if (hits && _callBack)
        {
            _callBack(this);
        }
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Audio_Effect_Btn_Click);
    }
}

void SpriteButton::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(_highlighted)
    {
        bool hits = touchHits(touch);
        if (!hits)
        {
            setHighlighted(false);
        }
    }
}

void SpriteButton::initRenderer()
{
    _titleRenderer = Label::create();
    _titleRenderer->setAnchorPoint(Vec2(0, 0));
    _titleRenderer->setPosition(Vec2((_contentSize.width - _titleRenderer->getContentSize().width) * 0.5, (_contentSize.height - _titleRenderer->getContentSize().height) * 0.5));
    addChild(_titleRenderer);
}

void SpriteButton::setVisible(bool visible)
{
    setEnabled(visible);
    Sprite::setVisible(visible);
}

void SpriteButton::setTitleText(const std::string &text)
{
    _titleRenderer->setString(text.c_str());
}

const std::string& SpriteButton::getTitleText() const
{
    return _titleRenderer->getString();
}

void SpriteButton::setTitleColor(const Color3B &color)
{
    _titleColorStateMap[ButtonState::Noramal] = color;
    _titleRenderer->setColor(color);
}

void SpriteButton::setTitleColorForState(const Color3B &color, ButtonState state)
{
    _titleColorStateMap[state] = color;
    if (state == ButtonState::Noramal)
    {
        _titleRenderer->setColor(color);
    }
}

const Color3B& SpriteButton::getTitleColorForState(ButtonState state) const
{
    if (_titleColorStateMap.find(state) != _titleColorStateMap.end())
    {
        return _titleColorStateMap.at(state);
    }
    return Color3B::WHITE;
}

bool SpriteButton::isExistTitleColorForState(ButtonState state)
{
    return _titleColorStateMap.find(state) != _titleColorStateMap.end();
}

const Color3B& SpriteButton::getTitleColor() const
{
    return _titleRenderer->getColor();
}

void SpriteButton::setTitleFontName(const std::string& fontName)
{
    _titleRenderer->setSystemFontName(fontName);
}

const std::string& SpriteButton::getTitleFontName() const
{
    return _titleRenderer->getSystemFontName();
}

void SpriteButton::setTitleFontSize(float size)
{
    _titleRenderer->setSystemFontSize(size);
}

float SpriteButton::getTitleFontSize() const
{
    return _titleRenderer->getSystemFontSize();
}

void SpriteButton::setTitleProperity(const std::string &text, const std::string &fontName, float size)
{
    _titleRenderer->setSystemFontName(fontName);
    _titleRenderer->setString(text);
    _titleRenderer->setSystemFontSize(size);
}

NS_CC_GUI_END