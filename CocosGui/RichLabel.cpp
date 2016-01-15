//
//  RichLabel.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/9/2.
//
//

#include "RichLabel.h"

NS_CC_GUI_BEGIN

bool RichLabel::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    m_children.clear();
    
    return true;
}

void RichLabel::insertElement(cocos2d::Node *node)
{
    m_children.push_back(node);
    addChild(node);
    updateChildPosition();
}

void RichLabel::removeElement(cocos2d::Node *node)
{
    auto it = std::find(m_children.begin(), m_children.end(), node);
    if (it != m_children.end())
    {
        node->removeFromParent();
        m_children.erase(it);
        updateChildPosition();
    }
}

void RichLabel::setAnchorPoint(const cocos2d::Vec2 &anchorPoint)
{
    Node::setAnchorPoint(anchorPoint);
    updateChildPosition();
}

void RichLabel::updateChildPosition()
{
    size_t childCounts = m_children.size();
    if (0.5f < _anchorPoint.x)
    {
        for (size_t i = childCounts; i > 0; i--)
        {
            auto child = m_children.at(i - 1);
            if (!child) continue;
            
            _contentSize.width += child->getContentSize().width;
            _contentSize.height = child->getContentSize().height;
            
            child->setAnchorPoint(_anchorPoint);
            
            if (childCounts == i) continue;
            
            auto lastChild  = m_children[i];
            if (!lastChild) continue;
            child->setPositionX(lastChild->getPositionX() - lastChild->getContentSize().width * _anchorPoint.x);
            child->setPositionY(lastChild->getPositionY());
        }
    } else
    {
        for (int i = 0; i < childCounts; i++)
        {
            auto child = m_children.at(i);
            if (!child) continue;
            
            _contentSize.width += child->getContentSize().width;
            _contentSize.height = child->getContentSize().height;
            
            child->setAnchorPoint(_anchorPoint);
            
            if (0 == i) continue;
            
            auto lastChild  = m_children[i - 1];
            if (!lastChild) continue;
            child->setPositionX(lastChild->getPositionX() + lastChild->getContentSize().width * (1 - _anchorPoint.x));
            child->setPositionY(lastChild->getPositionY());
        }
    }
}

NS_CC_GUI_END