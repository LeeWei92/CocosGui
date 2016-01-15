//
//  ButtonProtocol.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/18.
//
//

#include "ButtonProtocol.h"

NS_CC_GUI_BEGIN

ButtonProtocol::ButtonProtocol(void)
: _buttonState(ButtonState::Noramal)
, _enabled(true)
, _selected(false)
, _highlighted(false)
, _needSwallow(true)
, _callBack(nullptr)
{
    _backgroundrStateMap.clear();
    
    _eventListener = EventListenerTouchOneByOne::create();
    _eventListener->onTouchBegan = CC_CALLBACK_2(ButtonProtocol::onTouchBegan, this);
    _eventListener->onTouchEnded = CC_CALLBACK_2(ButtonProtocol::onTouchEnded, this);
    _eventListener->onTouchCancelled = CC_CALLBACK_2(ButtonProtocol::onTouchCancelled, this);
    _eventListener->onTouchMoved = CC_CALLBACK_2(ButtonProtocol::onTouchMoved, this);
    _eventListener->setSwallowTouches(_needSwallow);
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithFixedPriority(_eventListener, 1);
}

ButtonProtocol::~ButtonProtocol(void)
{
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->removeEventListener(_eventListener);
}

void ButtonProtocol::setEnabled(bool enabled)
{
    if (enabled != _enabled)
    {
        _enabled = enabled;
        _buttonState = (_enabled) ? _buttonState : ButtonState::Disabled;
        needsLayout();
    }
}

void ButtonProtocol::setSelected(bool selected)
{
    if (selected != _selected)
    {
        _selected = selected;
        _buttonState = (_selected) ? ButtonState::Selected : ButtonState::Noramal;
        needsLayout();
    }
}

void ButtonProtocol::setHighlighted(bool bHighlighted)
{
    _highlighted = bHighlighted;
    _buttonState = (_highlighted) ? ButtonState::Highlighted : ButtonState::Noramal;
    needsLayout();
}

void ButtonProtocol::addClickEventListener(const ccButtonCallback &callback)
{
    _callBack = callback;
}

void ButtonProtocol::setSwallowTouches(bool needSwallow)
{
    if (needSwallow != _needSwallow)
    {
        _needSwallow = needSwallow;
        _eventListener->setSwallowTouches(needSwallow);
    }
}

void ButtonProtocol::setTouchPriority(int touchPriority)
{
    _needSwallow = true;
    _eventListener->setSwallowTouches(true);
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->setPriority(_eventListener, touchPriority);
}

void ButtonProtocol::setBackgroudImageForState(const std::string &filename, ButtonState state)
{
    _backgroundrStateMap[state] = filename;
}

std::string ButtonProtocol::getBackgroudImageForState(ButtonState state) const
{
    if (_backgroundrStateMap.find(state) != _backgroundrStateMap.end())
    {
        return _backgroundrStateMap.at(state);
    }
    return "";
}

std::vector<ButtonProtocol::ParentProperty> ButtonProtocol::getBtnParentPropertyList(Node* pSendser)
{
    std::vector<ParentProperty> propertyList;
    Node* startNode = pSendser;
    do
    {
        Node* pNode = getBtnParent(startNode);
        if (!pNode) break;
        
        ParentProperty property;
        property.pNode = pNode;
        property.pBeginPos = pNode->getPosition();
        propertyList.push_back(property);
        
        startNode = pNode;
        
    } while(true);
    
    return propertyList;
}

Node* ButtonProtocol::getBtnParent(Node *pNode)
{
    if (dynamic_cast<Node*>(pNode->getParent()))
    {
        if (!dynamic_cast<Scene*>(pNode->getParent()))
        {
            return pNode->getParent();
        }
    }
    return nullptr;
}

bool ButtonProtocol::checkParentScroll(std::vector<ParentProperty> parentPropertyArr)
{
    for (int i = 0; i < parentPropertyArr.size(); i++)
    {
        ParentProperty property = parentPropertyArr.at(i);
        Node* pSender = property.pNode;
        Vec2 pBeginPos = property.pBeginPos;
        if (pSender)
        {
            Vec2 parentEndPos = pSender->getPosition();
            if (std::abs(parentEndPos.x - pBeginPos.x) > 5 ||
                std::abs(parentEndPos.y - pBeginPos.y) > 5 )
            {
                // if parent is scrolling, intercept button event
                return true;
            }
        }
    }
    
    return false;
}

NS_CC_GUI_END