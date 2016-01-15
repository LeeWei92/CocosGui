//
//  Scale9Button.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/23.
//
//

#include "Scale9Button.h"

NS_CC_GUI_BEGIN

Scale9Button::Scale9Button()
: _zoomOnTouchDown(false)
, _titleRenderer(nullptr)
{
    _titleColorStateMap.clear();
    _parentPropertyArr.clear();
}

Scale9Button::~Scale9Button()
{
}

Scale9Button* Scale9Button::create(const std::string& normalImage, const ccButtonCallback& callback)
{
    return create(normalImage, normalImage, normalImage, callback);
}

Scale9Button* Scale9Button::create(const std::string& normalImage, const std::string& highlightedImage,
                                   const ccButtonCallback& callback)
{
    return create(normalImage, highlightedImage, normalImage, callback);
}

Scale9Button* Scale9Button::create(const std::string& normalImage, const std::string& highlightedImage, const std::string& disabledImage, const ccButtonCallback& callback)
{
    auto scale9Btn = new (std::nothrow) Scale9Button();
    if (scale9Btn && scale9Btn->initWithFile(normalImage))
    {
        scale9Btn->setBackgroudImageForState(normalImage, ButtonState::Noramal);
        scale9Btn->setBackgroudImageForState(highlightedImage, ButtonState::Highlighted);
        scale9Btn->setBackgroudImageForState(disabledImage, ButtonState::Disabled);
        scale9Btn->initRenderer();
        scale9Btn->_callBack = callback;
        
        scale9Btn->autorelease();
        
        return scale9Btn;
    }
    CC_SAFE_DELETE(scale9Btn);
    return nullptr;
}


bool Scale9Button::initWithFile(const std::string& filename)
{
    if (filename.size() <= 0)
    {
        return false;
    }
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
    if (frame)
    {
        return ui::Scale9Sprite::initWithSpriteFrameName(filename.c_str());
    } else
    {
        return ui::Scale9Sprite::initWithFile(filename.c_str());
    }
    return true;
}

void Scale9Button::setSpriteWithFile(const std::string& filename)
{
    CCAssert(filename.size() > 0, "file is not exists");
    
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
    if (frame)
    {
        Size tmpSize = _preferredSize;
        setSpriteFrame(frame);
        initRenderer();
        setTitleProperity(_titleStr, _titlFontName, _titlFontSize);
        setPreferredSize(tmpSize);
    }
    else
    {
//        CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(filename.c_str());
//        setTexture(pTexture);
//        setSpriteWithFile(filename);
    }
}

void Scale9Button::setVisible(bool visible)
{
    setEnabled(visible);
    ui::Scale9Sprite::setVisible(visible);
}

void Scale9Button::setHighlighted(bool highlighted)
{
    ButtonProtocol::setHighlighted(highlighted);
    if (highlighted != _highlighted)
    {
        if (highlighted)
        {
            if (_zoomOnTouchDown)
                setScale(_originalScale - 0.05f);
        } else
        {
            setScale(_originalScale);
        }
    }
}

void Scale9Button::needsLayout(void)
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

bool Scale9Button::touchHits(Touch *touch)
{
    auto hitPos = getParent()->convertToNodeSpace(touch->getLocation());
    if (this->boundingBox().containsPoint(hitPos))
    {
        return true;
    }
    return false;
}

bool Scale9Button::onTouchBegan(Touch  *touch, Event  *event)
{
    _parentPropertyArr.clear();
    _parentPropertyArr = getBtnParentPropertyList(this);
    
    bool hits = touchHits(touch);
    if (hits && !_highlighted && _enabled && isVisible())
    {
        if (_zoomOnTouchDown)
            _originalScale = this->getScale();
        this->setHighlighted(true);
    }
    return hits;
}

void Scale9Button::onTouchEnded(Touch  *touch, Event  *event)
{
    if (checkParentScroll(_parentPropertyArr))
    {
        // if parent is scrolling, intercept button event
        return;
    }
    
    if(_enabled && isVisible())
    {
        bool hits = touchHits(touch);
        if (hits)
        {
            this->setHighlighted(false);
            auto hits = touchHits(touch);
            if (hits && _callBack)
            {
                _callBack(this);
            }
            //        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Audio_Effect_Btn_Click);
        }
    }
}

void Scale9Button::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(_highlighted)
    {
        bool hits = touchHits(touch);
        if (!hits)
        {
            this->setHighlighted(false);
        }
    }
}

void Scale9Button::initRenderer()
{
    _titleRenderer = Label::create();
    _titleRenderer->setAnchorPoint(Vec2(0.5, 0.5));
    _titleRenderer->setPosition(Vec2((_contentSize.width - _titleRenderer->getContentSize().width) * 0.5, (_contentSize.height - _titleRenderer->getContentSize().height) * 0.5));
    addChild(_titleRenderer);
}

void Scale9Button::setTitleText(const std::string &text)
{
    _titleStr = text;
    _titleRenderer->setString(text.c_str());
}

const std::string& Scale9Button::getTitleText() const
{
    return _titleRenderer->getString();
}

void Scale9Button::setTitleColor(const Color3B &color)
{
    _titleColor = color;
    _titleColorStateMap[ButtonState::Noramal] = color;
    _titleRenderer->setColor(color);
}

void Scale9Button::setTitleColorForState(const Color3B &color, ButtonState state)
{
    _titleColorStateMap[state] = color;
    if (state == ButtonState::Noramal)
    {
        _titleColor = color;
        _titleRenderer->setColor(color);
    }
}

const Color3B& Scale9Button::getTitleColorForState(ButtonState state) const
{
    if (_titleColorStateMap.find(state) != _titleColorStateMap.end())
    {
        return _titleColorStateMap.at(state);
    }
    return Color3B::WHITE;
}

bool Scale9Button::isExistTitleColorForState(ButtonState state)
{
    return _titleColorStateMap.find(state) != _titleColorStateMap.end();
}

void Scale9Button::setTitleFontName(const std::string& fontName)
{
    _titlFontName = fontName;
    _titleRenderer->setSystemFontName(fontName);
}

const std::string& Scale9Button::getTitleFontName() const
{
    return _titleRenderer->getSystemFontName();
}

void Scale9Button::setTitleFontSize(float size)
{
    _titlFontSize = size;
    _titleRenderer->setSystemFontSize(size);
}

float Scale9Button::getTitleFontSize() const
{
    return _titleRenderer->getSystemFontSize();
}

void Scale9Button::setTitleProperity(const std::string &text, const std::string &fontName, float size)
{
    setTitleFontSize(size);
    setTitleFontName(fontName);
    setTitleText(text);
}

void Scale9Button::updateRendererPos()
{
    _titleRenderer->setAnchorPoint(Vec2(0.5, 0.5));
    _titleRenderer->setPosition(Vec2((_contentSize.width - _titleRenderer->getContentSize().width) * 0.5, (_contentSize.height - _titleRenderer->getContentSize().height) * 0.5));
}

void Scale9Button::setPreferredSize(const cocos2d::Size &preferredSize)
{
    ui::Scale9Sprite::setPreferredSize(preferredSize);
    updateRendererPos();
}

NS_CC_GUI_END