//
//  Scale9SpriteBar.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/1/14.
//
//

#include "Scale9SpriteBar.h"

USING_NS_CC;

Scale9SpriteBar::Scale9SpriteBar()
: _barScale9Sprite(nullptr)
, _percentage(0.0f)
, _originalBarSpriteSize(Size::ZERO)
{
}

Scale9SpriteBar::~Scale9SpriteBar()
{
}

Scale9SpriteBar* Scale9SpriteBar::create(const std::string& backgroundFileName, const std::string& barFileName)
{
    auto scale9SpriteBar = new (std::nothrow) Scale9SpriteBar();
    
    if (scale9SpriteBar && scale9SpriteBar->initWithFile(backgroundFileName))
    {
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(barFileName);
        if (frame)
        {
            scale9SpriteBar->_barScale9Sprite = ui::Scale9Sprite::createWithSpriteFrameName(barFileName);
        } else
        {
            scale9SpriteBar->_barScale9Sprite = ui::Scale9Sprite::create(barFileName);
        }
        scale9SpriteBar->_barScale9Sprite->setAnchorPoint(Vec2(Vec2(0.5, 0.5)));
        scale9SpriteBar->_originalBarSpriteSize = scale9SpriteBar->_barScale9Sprite->getContentSize();
        scale9SpriteBar->setPercentage(0);
        scale9SpriteBar->addChild(scale9SpriteBar->_barScale9Sprite);
        
        scale9SpriteBar->autorelease();
        
        return scale9SpriteBar;
    }
    CC_SAFE_DELETE(scale9SpriteBar);
    return nullptr;
}

Scale9SpriteBar* Scale9SpriteBar::create(ui::Scale9Sprite *bgScale9Sprite, cocos2d::Size bgScale9SpriteSize, const std::string &barFileName)
{
    auto scale9SpriteBar = new (std::nothrow) Scale9SpriteBar();
    
    if (scale9SpriteBar && bgScale9Sprite && scale9SpriteBar->init())
    {
        bgScale9Sprite->setContentSize(bgScale9SpriteSize);
        scale9SpriteBar->setContentSize(bgScale9SpriteSize);
        scale9SpriteBar->addChild(bgScale9Sprite);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(barFileName);
        if (frame)
        {
            scale9SpriteBar->_barScale9Sprite = ui::Scale9Sprite::createWithSpriteFrameName(barFileName);
        } else
        {
            scale9SpriteBar->_barScale9Sprite = ui::Scale9Sprite::create(barFileName);
        }
        scale9SpriteBar->_barScale9Sprite->setAnchorPoint(Vec2(0.5, 0.5));
        scale9SpriteBar->_originalBarSpriteSize = scale9SpriteBar->_barScale9Sprite->getContentSize();
        scale9SpriteBar->setPercentage(0);
        bgScale9Sprite->addChild(scale9SpriteBar->_barScale9Sprite);
        
        scale9SpriteBar->autorelease();
        
        return scale9SpriteBar;
    }
    CC_SAFE_DELETE(scale9SpriteBar);
    return nullptr;
}

bool Scale9SpriteBar::initWithFile(const std::string& fileName)
{
    if (fileName.size() <= 0)
    {
        return false;
    }
    
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
    if (frame)
    {
        return initWithSpriteFrameName(fileName);
    } else
    {
        return initWithFile(fileName);
    }
    return true;
}

void Scale9SpriteBar::setPercentage(float percentage)
{
    if (0 > percentage) percentage = 0;
    if (1 < percentage) percentage = 1;
    
    Size scale9SpriteBarSize = getContentSize();
    _barScale9Sprite->setPreferredSize(Size((scale9SpriteBarSize.width) * percentage,
                                            _barScale9Sprite->getContentSize().height));
    _barScale9Sprite->setPosition(Vec2(scale9SpriteBarSize.width * 0.5 * percentage,
                                       scale9SpriteBarSize.height * 0.5));
    if (scale9SpriteBarSize.width * percentage < _originalBarSpriteSize.width)
    {
        float scaleX = scale9SpriteBarSize.width * getScaleX() * percentage / _originalBarSpriteSize.width;
        _barScale9Sprite->setPosition(Vec2(scale9SpriteBarSize.width * getScaleX() * 0.5 * percentage * (1 - scaleX), scale9SpriteBarSize.height * 0.5));
        _barScale9Sprite->setScaleX(scaleX);
    } else
    {
        _barScale9Sprite->setScaleX(1);
    }
    _percentage = percentage;
}

void Scale9SpriteBar::fadeIn(float time)
{
    for (auto child : getChildren())
    {
        if (0 == child->getOpacity()) continue;
        child->setOpacity(0);
        child->runAction(FadeTo::create(time, 255));
    }
}

void Scale9SpriteBar::fadeOut(float time)
{
    for (auto child : getChildren())
    {
        if (255 == child->getOpacity()) continue;
        child->setOpacity(255);
        child->runAction(FadeTo::create(time, 0));
    }
}