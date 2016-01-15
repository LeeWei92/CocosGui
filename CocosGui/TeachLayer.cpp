//
//  TeachLayer.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/5/20.
//
//

#include "TeachLayer.h"

#define DEFAULT_FINGER_MOVE_RANGE_X 20
#define DEFAULT_FINGER_MOVE_RANGE_Y 20

NS_CC_GUI_BEGIN

TeachLayer::TeachLayer()
: _fingerMoveRangeX(0)
, _touchCallback(nullptr)
, _fingerMoveRangeY(DEFAULT_FINGER_MOVE_RANGE_Y)
{
    
}

TeachLayer::~TeachLayer()
{
    
}

TeachLayer* TeachLayer::create(const Vec2 &stencilWorldPos, FingerDirection fingerDirection)
{
    auto teachLayer = new (std::nothrow) TeachLayer();
    if (teachLayer && teachLayer->init(stencilWorldPos, fingerDirection))
    {
        teachLayer->autorelease();
        return teachLayer;
    }
    CC_SAFE_DELETE(teachLayer);
    return nullptr;
}

bool TeachLayer::init()
{
    return init(Vec2::ZERO, FingerDirection::Down);
}

bool TeachLayer::init(const cocos2d::Vec2 &stencilWorldPos, FingerDirection fingerDirection)
{
    if (!Layer::init())
    {
        return false;
    }
    
    _stencilPosition = stencilWorldPos;
    _fingerDirection = fingerDirection;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    // bind touch event
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    // bind touch event
    touchListener->onTouchBegan = CC_CALLBACK_2(TeachLayer::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(TeachLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    // 裁剪图层
    auto pClip = ClippingNode::create();
    // 设置反相
    pClip->setInverted(true);
    
    // 蒙版
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    pClip->addChild(colorLayer);
    
    // 模板初始化
    _stencilNode = DrawNode::create();
    // 绘制圆形区域
    float fRadius = 55.0f; // 圆的半径
    const int nCount = 100; // 圆形其实可以看做正多边形,我们这里用正100边型来模拟园
    const float coef = 2.0f * (float)M_PI / nCount; // 计算每两个相邻顶点与中心的夹角
    Vec2 circle[nCount]; // 顶点数组
    for(int i = 0;i < nCount; i++)
    {
        float rads = i*coef;//弧度
        circle[i].x = fRadius * cosf(rads); // 对应顶点的x
        circle[i].y = fRadius * sinf(rads); // 对应顶点的y
    }
    // 绘制这个多边形!
    _stencilNode->drawPolygon(circle, nCount, Color4F::GREEN, 0, Color4F::GREEN);
    
    _stencilNode->setPosition(winSize.width * 0.5, winSize.height * 0.5);
    // 设模板
    pClip->setStencil(_stencilNode);
    addChild(pClip);
    
    _stencilButton = SpriteButton::create("guide_circle.png", CC_CALLBACK_1(TeachLayer::stencilTouchCallBack, this));
    _stencilButton->setOpacity(0);
    _stencilButton->setPosition(winSize.width * 0.5, winSize.height * 0.5);
    addChild(_stencilButton);
    
    _fingerSprite = Sprite::createWithSpriteFrameName("finger_1.png");
    addChild(_fingerSprite);
    
    _fingerEffSprite = Sprite::createWithSpriteFrameName("finger_2.png");
    addChild(_fingerEffSprite);
    
    setStencilPosition(_stencilPosition);
    
    runMoveAction();
    
    return true;
}

void TeachLayer::onEnter()
{
    auto pNode = this ->getParent();
    if (pNode && dynamic_cast<LayerColor*>(pNode))
    {
        if (0 < pNode->getOpacity())
        {
            _pNodeOriginalOpacity = pNode->getOpacity();
            pNode->setOpacity(0);
        }
    }
    Layer::onEnter();
}

void TeachLayer::onExit()
{
    auto pNode = this ->getParent();
    if (pNode && dynamic_cast<LayerColor*>(pNode))
    {
        if (0 == pNode->getOpacity())
        {
            pNode->setOpacity(_pNodeOriginalOpacity);
        }
    }
    Layer::onExit();
}

bool TeachLayer::touchHits(Touch *touch)
{
    auto hitPos = this->convertToNodeSpace(touch->getLocation());
    if (hitPos.x >= 0 && hitPos.y >= 0 &&
        hitPos.x <= _stencilNode->getContentSize().width * _stencilNode->getScaleX() &&
        hitPos.y <= _stencilNode->getContentSize().height * _stencilNode->getScaleY())
    {
        return true;
    }
    return false;
}

bool TeachLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused)
{
    return true;
}

void TeachLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (touchHits(touch))
    {
        // drawNode replace spriteButton ...
    }
}

void TeachLayer::setFingerDirection(FingerDirection fingerDirection)
{
    Point stencilBtnPos = _stencilButton->getPosition();
    Size fingerSpriteSize = _fingerSprite->getContentSize();
    switch (fingerDirection)
    {
        case FingerDirection::Down:
        {
            _fingerMoveRangeX = 0;
            _fingerMoveRangeY = -DEFAULT_FINGER_MOVE_RANGE_Y;
            _fingerSprite->setFlippedY(false);
            _fingerEffSprite->setFlippedY(false);
            _fingerSprite->setPosition(stencilBtnPos.x,
                                       stencilBtnPos.y + fingerSpriteSize.height);
            break;
        }
        case FingerDirection::Up:
        {
            _fingerMoveRangeX = 0;
            _fingerMoveRangeY = DEFAULT_FINGER_MOVE_RANGE_Y;
            _fingerSprite->setFlippedY(true);
            _fingerEffSprite->setFlippedY(true);
            _fingerSprite->setPosition(stencilBtnPos.x,
                                       stencilBtnPos.y - fingerSpriteSize.height);
            break;
        }
        case FingerDirection::LeftDown:
        {
            _fingerMoveRangeX = DEFAULT_FINGER_MOVE_RANGE_X;
            _fingerMoveRangeY = -DEFAULT_FINGER_MOVE_RANGE_Y;
            _fingerSprite->setFlippedY(false);
            _fingerEffSprite->setFlippedY(false);
            _fingerSprite->setRotation(-45.0f);
            _fingerSprite->setPosition(stencilBtnPos.x - fingerSpriteSize.width * 0.75,
                                       stencilBtnPos.y + fingerSpriteSize.height * 0.75);
            break;
        }
        case FingerDirection::LeftUp:
        {
            _fingerMoveRangeX = DEFAULT_FINGER_MOVE_RANGE_X;
            _fingerMoveRangeY = DEFAULT_FINGER_MOVE_RANGE_Y;
            _fingerSprite->setFlippedY(true);
            _fingerEffSprite->setFlippedY(true);
            _fingerSprite->setRotation(45.0f);
            _fingerSprite->setPosition(stencilBtnPos.x - fingerSpriteSize.width * 0.75,
                                       stencilBtnPos.y - fingerSpriteSize.height * 0.75);
            break;
        }
        case FingerDirection::Left:
        {
            _fingerMoveRangeX = DEFAULT_FINGER_MOVE_RANGE_X;
            _fingerMoveRangeY = 0;
            _fingerSprite->setFlippedY(false);
            _fingerEffSprite->setFlippedY(false);
            _fingerSprite->setRotation(-90.0f);
            _fingerSprite->setPosition(stencilBtnPos.x - fingerSpriteSize.width,
                                       stencilBtnPos.y);
            break;
        }
        case FingerDirection::Right:
        {
            _fingerMoveRangeX = -DEFAULT_FINGER_MOVE_RANGE_X;
            _fingerMoveRangeY = 0;
            _fingerSprite->setFlippedY(false);
            _fingerEffSprite->setFlippedY(false);
            _fingerSprite->setRotation(90.0f);
            _fingerSprite->setPosition(stencilBtnPos.x + fingerSpriteSize.width,
                                       stencilBtnPos.y);
            break;
        }
        case FingerDirection::RightDown:
        {
            _fingerMoveRangeX = -DEFAULT_FINGER_MOVE_RANGE_X;
            _fingerMoveRangeY = -DEFAULT_FINGER_MOVE_RANGE_Y;
            _fingerSprite->setFlippedY(false);
            _fingerEffSprite->setFlippedY(false);
            _fingerSprite->setRotation(45.0f);
            _fingerSprite->setPosition(stencilBtnPos.x + fingerSpriteSize.width * 0.75,
                                       stencilBtnPos.y + fingerSpriteSize.height * 0.75);
            break;
        }
        case FingerDirection::RightUp:
        {
            _fingerMoveRangeX = -DEFAULT_FINGER_MOVE_RANGE_X;
            _fingerMoveRangeY = DEFAULT_FINGER_MOVE_RANGE_Y;
            _fingerSprite->setFlippedY(true);
            _fingerEffSprite->setFlippedY(true);
            _fingerSprite->setRotation(-45.0f);
            _fingerSprite->setPosition(stencilBtnPos.x + fingerSpriteSize.width * 0.75,
                                       stencilBtnPos.y - fingerSpriteSize.height * 0.75);
            break;
        }
        default:
            break;
    }
    _fingerEffSprite->setRotation(_fingerSprite->getRotation());
    _fingerEffSprite->setPosition(_fingerSprite->getPosition());
    
    runMoveAction();
}

void TeachLayer::runMoveAction()
{
    _fingerEffSprite->stopAllActions();
    _fingerSprite->stopAllActions();
    auto moveAct = MoveBy::create(0.5, Vec2(_fingerMoveRangeX, _fingerMoveRangeY));
    _fingerSprite->runAction(RepeatForever::create(Sequence::create(moveAct,
                                                    moveAct->reverse(), nullptr)));
    
    _fingerEffSprite->setOpacity(0);
    _fingerEffSprite->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.5, 255),FadeTo::create(0.5, 0), nullptr)));
    auto moveEffAct = MoveBy::create(0.5, Vec2(_fingerMoveRangeX, _fingerMoveRangeY));
    _fingerEffSprite->runAction(RepeatForever::create(Sequence::create(moveEffAct,
                                                        moveEffAct->reverse(), nullptr)));
}

void TeachLayer::setStencilPosition(const cocos2d::Vec2 &worldPos)
{
    _stencilPosition = worldPos;
    _stencilNode->setPosition(worldPos);
    _stencilButton->setPosition(worldPos);
    setFingerDirection(_fingerDirection);
}

void TeachLayer::setStencilAnchorPoint(const cocos2d::Vec2 &point)
{
//    _stencilAnchorPoint = point;
//    _stencilNode->setAnchorPoint(point);
//    _stencilButton->setAnchorPoint(point);
//    setFingerDirection(_fingerDirection);
//    setStencilPosition(_stencilPosition);
}

void TeachLayer::setStencilPosition(float worldPosX, float worldPosY)
{
    setStencilPosition(Vec2(worldPosX, worldPosX));
}

void TeachLayer::setStencilScale(float scale)
{
    _stencilScale = scale;
    _stencilNode->setScale(scale);
    _stencilButton->setScale(scale);
    _fingerSprite->setScale(scale);
}

void TeachLayer::setCustomTouchCallBack(const ccTouchCallback &callBack)
{
    _touchCallback = callBack;
}

void TeachLayer::stencilTouchCallBack(cocos2d::Ref* ref)
{
    if (_touchCallback)
    {
        _touchCallback(this);
        removeFromParent();
    } else
    {
        auto pSender = dynamic_cast<Node*>(ref);
        auto pLayer = pSender->getParent()->getParent();
        auto stencilRect = getNodeRect(pSender);
        checkChildType(pLayer, stencilRect);
    }
}

bool TeachLayer::checkChildType(cocos2d::Node *pNode, cocos2d::Rect stencilRect)
{
    if (!pNode) return false;
    if (0 >= pNode->getChildrenCount()) return false;
    
    for (auto child : pNode->getChildren())
    {
        if (dynamic_cast<SpriteButton*>(child))
        {
            auto spriteButton = dynamic_cast<SpriteButton*>(child);
            Rect btnRect = getNodeRect(spriteButton);
            if (stencilRect.intersectsRect(btnRect))
            {
                removeFromParent();
//                spriteButton->execCallBackEvent();
                return true;
            }
        }
    }
    
    for (auto child : pNode->getChildren())
    {
        if (checkChildType(child, stencilRect))
            return true;
    }
    return false;
}

Rect TeachLayer::getNodeRect(cocos2d::Node *pNode)
{
    if (!pNode) return Rect::ZERO;
    
    Rect rect = Rect(0, 0, 0, 0);
    auto pLayer = pNode->getParent();
    rect.origin.x = pLayer->convertToWorldSpace(pNode->getPosition()).x - (pNode->getContentSize().width * 0.5);
    rect.origin.y = pLayer->convertToWorldSpace(pNode->getPosition()).y - (pNode->getContentSize().height * 0.5);
    rect.size.width = pNode->getContentSize().width * pNode->getScaleX();
    rect.size.height = pNode->getContentSize().height * pNode->getScaleY();
    return rect;
}

NS_CC_GUI_END