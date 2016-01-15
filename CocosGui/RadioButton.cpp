//
//  RadioButton.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/24.
//
//

#include "RadioButton.h"

NS_CC_GUI_BEGIN

RadioButton::RadioButton()
: _bMoved(false)
{
    _parentPropertyArr.clear();
}

RadioButton::~RadioButton()
{
}

RadioButton* RadioButton::create(const std::string& normalImage, const ccButtonCallback& callback)
{
    return create(normalImage, normalImage, normalImage, callback);
}

RadioButton* RadioButton::create(const std::string& normalImage, const std::string& selectedImage, const ccButtonCallback& callback)
{
    return create(normalImage, selectedImage, normalImage, callback);
}

RadioButton* RadioButton::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccButtonCallback& callback)
{
    auto radioButton = new (std::nothrow) RadioButton();
    if (radioButton && radioButton->initWithFile(normalImage))
    {
        radioButton->setBackgroudImageForState(normalImage, ButtonState::Noramal);
        radioButton->setBackgroudImageForState(selectedImage, ButtonState::Selected);
        radioButton->setBackgroudImageForState(disabledImage, ButtonState::Disabled);
        radioButton->_callBack = callback;
        
        radioButton->autorelease();
        
        return radioButton;
    }
    CC_SAFE_DELETE(radioButton);
    return nullptr;
}

bool RadioButton::initWithFile(const std::string& filename)
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

void RadioButton::setSpriteWithFile(const std::string& filename)
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

void RadioButton::needsLayout(void)
{
    std::string stateFileName = getBackgroudImageForState(_buttonState);
    if (0 < stateFileName.size())
    {
        setSpriteWithFile(stateFileName);
    }
}

bool RadioButton::touchHits(Touch *touch)
{
    auto hitPos = getParent()->convertToNodeSpace(touch->getLocation());
    if (this->boundingBox().containsPoint(hitPos))
    {
        return true;
    }
    return false;
}

bool RadioButton::onTouchBegan(Touch  *touch, Event  *event)
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

void RadioButton::onTouchEnded(Touch  *touch, Event  *event)
{
    if (checkParentScroll(_parentPropertyArr))
    {
        // if parent is scrolling, intercept button event
        return;
    }
    if(_enabled && !_bMoved && !getSelected() && isVisible())
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

void RadioButton::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto hits = touchHits(touch);
    if (hits && _enabled && isVisible())
    {
        _bMoved = false;
    } else
    {
        _bMoved = true;
    }
}

void RadioButton::activeTouchEvent()
{
    if(_enabled && !getSelected() && isVisible())
    {
        setSelected(!getSelected());
        if (_callBack)
        {
            _callBack(this);
        }
        //        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Audio_Effect_Btn_Click);
    }
}

void RadioButton::setVisible(bool visible)
{
    setEnabled(visible);
    Sprite::setVisible(visible);
}

/* RadioGroup
 **************************************************************
 */
RadioGroup::RadioGroup()
: _eventListener(nullptr)
, _selectedBtn(nullptr)
, _callBack(nullptr)
, _enabled(true)
{
}

RadioGroup::~RadioGroup()
{
}

RadioGroup* RadioGroup::create()
{
    auto radioGroup = new (std::nothrow) RadioGroup();
    if (radioGroup && radioGroup->init())
    {
        radioGroup->autorelease();
        return radioGroup;
    }
    CC_SAFE_DELETE(radioGroup);
    return nullptr;
}

RadioGroup* RadioGroup::create(std::vector<RadioButton*> groupList)
{
    auto radioGroup = new (std::nothrow) RadioGroup();
    if (radioGroup && radioGroup->initWithArray(groupList))
    {
        radioGroup->autorelease();
        return radioGroup;
    }
    CC_SAFE_DELETE(radioGroup);
    return nullptr;
}

bool RadioGroup::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _eventListener = EventListenerTouchOneByOne::create();
    _eventListener->onTouchBegan = CC_CALLBACK_2(RadioGroup::onTouchBegan, this);
    _eventListener->onTouchEnded = CC_CALLBACK_2(RadioGroup::onTouchEnded, this);
    _eventListener->onTouchMoved = CC_CALLBACK_2(RadioGroup::onTouchMoved, this);
    _eventListener->setSwallowTouches(true);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_eventListener, this);
    
    return true;
}

bool RadioGroup::initWithArray(std::vector<RadioButton*> groupList)
{
    if (!Layer::init())
    {
        return false;
    }
    
    int z(0);
    for (auto& toggleBtn : groupList)
    {
        addChild(toggleBtn, z);
        z++;
    }

    _eventListener = EventListenerTouchOneByOne::create();
    _eventListener->onTouchBegan = CC_CALLBACK_2(RadioGroup::onTouchBegan, this);
    _eventListener->onTouchEnded = CC_CALLBACK_2(RadioGroup::onTouchEnded, this);
    _eventListener->onTouchMoved = CC_CALLBACK_2(RadioGroup::onTouchMoved, this);
    _eventListener->setSwallowTouches(true);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_eventListener, this);
    
    return true;
}

/*
 * override addChild:
 */
void RadioGroup::addChild(Node * child)
{
    CCASSERT( dynamic_cast<RadioButton*>(child) != nullptr, "RadioGroup only supports RadioButton objects as children");
    Layer::addChild(child);
}

void RadioGroup::addChild(Node * child, int zOrder)
{
    CCASSERT( dynamic_cast<RadioButton*>(child) != nullptr, "RadioGroup only supports RadioButton objects as children");
    Layer::addChild(child, zOrder);
}

void RadioGroup::addChild(Node * child, int zOrder, int tag)
{
    CCASSERT( dynamic_cast<RadioButton*>(child) != nullptr, "RadioGroup only supports RadioButton objects as children");
    Layer::addChild(child, zOrder, tag);
}

void RadioGroup::addChild(Node * child, int zOrder, const std::string &name)
{
    CCASSERT( dynamic_cast<RadioButton*>(child) != nullptr, "RadioGroup only supports RadioButton objects as children");
    Layer::addChild(child, zOrder, name);
}

void RadioGroup::removeChild(Node* child, bool cleanup)
{
    RadioButton *radioBtn = dynamic_cast<RadioButton*>(child);
    CCASSERT(radioBtn != nullptr, "RadioGroup only supports RadioButton objects as children");
    
    if (radioBtn == _selectedBtn)
    {
        _selectedBtn = nullptr;
    }
    
    Node::removeChild(child, cleanup);
}

bool RadioGroup::onTouchBegan(Touch  *touch, Event  *event)
{
    if (!isVisible()) return false;
    
    if (this->getItemForTouch(touch))
    {
        return true;
    }
    return false;
}

void RadioGroup::onTouchEnded(Touch  *touch, Event  *event)
{
    if (!isVisible() || !_enabled) return;

    auto selectedItem = this->getItemForTouch(touch);
    if (selectedItem)
    {
        resetAllItemState(selectedItem);
    }
}

void RadioGroup::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
}

void RadioGroup::resetAllItemState(RadioButton* selectedItem)
{
    for (auto iter = _children.crbegin(); iter != _children.crend(); ++iter)
    {
        RadioButton* child = dynamic_cast<RadioButton*>(*iter);
        if (!child || !child->isVisible() || !child->getEnabled())
        {
            continue;
        }
        if (selectedItem != child)
        {
            child->setSelected(false);
        } else
        {
            _selectedBtn = child;
            child->activeTouchEvent();
            if (_callBack)
            {
                _callBack(this, child);
            }
        }
    }
}

void RadioGroup::addRadioEventListener(const ccRadioGroupCallback &callback)
{
    _callBack = callback;
}

RadioButton* RadioGroup::getItemForTouch(Touch *touch)
{
    auto hitPos = this->convertToNodeSpace(touch->getLocation());
    if (!_children.empty())
    {
        for (auto iter = _children.crbegin(); iter != _children.crend(); ++iter)
        {
            RadioButton* child = dynamic_cast<RadioButton*>(*iter);
            if (!child || !child->isVisible() || !child->getEnabled())
            {
                continue;
            }
            if (child->boundingBox().containsPoint(hitPos))
            {
                return child;
            }
        }
    }
    return nullptr;
}

NS_CC_GUI_END