//
//  PageView.cpp
//  Puzzle
//  shengxiaainj@gmail.com
//  Created by Lee on 14/12/18.
//
//

#include "PageView.h"

NS_CC_GUI_BEGIN

PageView::PageView()
: _curPageIdx(0)
, _isInterceptTouch(false)
, _direction(Direction::Vertical)
, _isAutoScrolling(false)
, _autoScrollDistance(0.0f)
, _autoScrollSpeed(0.0f)
, _autoScrollDirection(TouchDirection::LEFT)
, _touchMoveDirection(TouchDirection::LEFT)
, _frontChild(nullptr)
, _behindChild(nullptr)
, _customScrollThreshold(0.0)
, _usingCustomScrollThreshold(false)
, _childFocusCancelOffset(5.0f)
, _eventCallback(nullptr)
{
}

PageView::~PageView()
{
    _eventCallback = nullptr;
}

bool PageView::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _clippingNode = ClippingRectangleNode::create();
    addChild(_clippingNode);
    
    return true;
}

void PageView::onEnter()
{
    // bind touch event
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = CC_CALLBACK_2(PageView::onTouchBegan, this);
    _touchListener->onTouchMoved = CC_CALLBACK_2(PageView::onTouchMoved, this);
    _touchListener->onTouchCancelled = CC_CALLBACK_2(PageView::onTouchCancelled, this);
    _touchListener->onTouchEnded = CC_CALLBACK_2(PageView::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    
    Layer::onEnter();
    scheduleUpdate();
}

void PageView::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
    Layer::onExit();
    unscheduleUpdate();
}

bool PageView::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto hitPos = this->convertToNodeSpace(touch->getLocation());
    if (hitPos.x >= 0 && hitPos.y >= 0 && hitPos.x <= _contentSize.width && hitPos.y <= _contentSize.height)
    {
        return true;
    }
    return false;
}

void PageView::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    Layer::onTouchMoved(touch, unused_event);
    if (!_isInterceptTouch)
    {
        handleMoveLogic(touch);
    }
}

void PageView::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    Layer::onTouchEnded(touch, unused_event);
    if (!_isInterceptTouch)
    {
        handleReleaseLogic(touch);
    }
    _isInterceptTouch = false;
}

void PageView::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    Layer::onTouchCancelled(touch, unused_event);
    if (!_isInterceptTouch)
    {
        handleReleaseLogic(touch);
    }
    _isInterceptTouch = false;
}

void PageView::handleMoveLogic(cocos2d::Touch *touch)
{
    Vec2 touchPoint = touch->getLocation();
    
    float offset = 0.0;
    
    if (Direction::Horizontal == _direction)
    {
        offset = touchPoint.x - touch->getPreviousLocation().x;
    } else
    {
        offset = touchPoint.y - touch->getPreviousLocation().y;
    }
    
    if (offset < 0)
    {
        _touchMoveDirection = (Direction::Horizontal == _direction ? TouchDirection::LEFT : TouchDirection::DOWN);
    } else if (offset > 0)
    {
        _touchMoveDirection = (Direction::Horizontal == _direction ? TouchDirection::RIGHT : TouchDirection::UP);
    }
    scrollPages(offset);
}

void PageView::handleReleaseLogic(cocos2d::Touch *touch)
{
    if (this->getPageCount() <= 0)
    {
        return;
    }
    
    auto curPage = dynamic_cast<Layer*>(this->getPages().at(_curPageIdx));
    if (curPage)
    {
        Vec2 curPagePos = curPage->getPosition();
        ssize_t pageCount = this->getPageCount();
        float curPageLocation = (Direction::Horizontal == _direction ? curPagePos.x : curPagePos.y);
        float pageDiff = Direction::Horizontal == _direction ? getContentSize().width : getContentSize().height;
        if (!_usingCustomScrollThreshold)
        {
            _customScrollThreshold = pageDiff / 2.0;
        }
        float boundary = _customScrollThreshold;
        if (curPageLocation <= -boundary)
        {
            if (_curPageIdx >= pageCount - 1)
            {
                scrollPages(-curPageLocation);
            } else
            {
                scrollToPage(_curPageIdx + 1);
            }
        } else if (curPageLocation >= boundary)
        {
            if (_curPageIdx <= 0)
            {
                scrollPages(-curPageLocation);
            } else
            {
                scrollToPage(_curPageIdx - 1);
            }
        } else
        {
            scrollToPage(_curPageIdx);
        }
    }
}

void PageView::addEventListener(const ccPageViewCallback &callback)
{
    _eventCallback = callback;
}

void PageView::addPage(Layer *page)
{
    if (!page || std::find(_pages.begin(), _pages.end(), page) != _pages.end())
    {
        return;
    }
    _clippingNode->setClippingRegion(Rect(0, 0, getContentSize().width, getContentSize().height));
//    _clippingNode->setAnchorPoint(Vec2(0.5, 0.5));

    _clippingNode->addChild(page);
    page->setAnchorPoint(Vec2(0, 0));
    _pages.push_back(page);
    
    _doLayerDirty = true;
}

void PageView::insertPage(Layer* page, int idx)
{
    if (idx < 0 || !page || std::find(_pages.begin(), _pages.end(), page) != _pages.end())
    {
        return;
    }
    
    ssize_t pageCount = this->getPageCount();
    if (idx >= pageCount)
    {
        addPage(page);
    } else
    {
        _pages.insert((std::begin(_pages) + idx), page);
        _clippingNode->addChild(page);
    }
    
    _doLayerDirty = true;
}

void PageView::removePage(cocos2d::Layer *page)
{
    if (!page)
    {
        return;
    }
    _clippingNode->removeChild(page);

    auto iter = std::find(_pages.begin(), _pages.end(), page);
    if (iter != _pages.end())
    {
        _pages.erase(iter);
    }
    
    _doLayerDirty = true;
}

void PageView::removePageAtIndex(ssize_t index)
{
    if (index < 0 || index >= _pages.size())
    {
        return;
    }
    auto page = _pages.at(index);
    removePage(page);
}

void PageView::removeAllPages()
{
    for(const auto& node : _pages)
    {
        _clippingNode->removeChild(node);
    }
    _pages.clear();
}

void PageView::update(float dt)
{
    if (_isAutoScrolling)
    {
        this->autoScroll(dt);
    }
}

void PageView::setCustomScrollThreshold(float threshold)
{
    CCASSERT(threshold > 0, "Invalid threshold!");
    _customScrollThreshold = threshold;
    this->setUsingCustomScrollThreshold(true);
}

float PageView::getCustomScrollThreshold()const
{
    return _customScrollThreshold;
}

void PageView::setUsingCustomScrollThreshold(bool flag)
{
    _usingCustomScrollThreshold = flag;
}

bool PageView::isUsingCustomScrollThreshold()const
{
    return _usingCustomScrollThreshold;
}

ssize_t PageView::getCurPageIndex() const
{
    return _curPageIdx;
}

Layer* PageView::getPage(ssize_t index)
{
    if (index < 0 || index >= this->getPages().size())
    {
        return nullptr;
    }
    return _pages.at(index);
}

void PageView::autoScroll(float dt)
{
    switch (_autoScrollDirection)
    {
        case TouchDirection::DOWN:
        case TouchDirection::LEFT:
        {
            float step = _autoScrollSpeed*dt;
            if (_autoScrollDistance + step >= 0.0f)
            {
                step = -_autoScrollDistance;
                _autoScrollDistance = 0.0f;
                _isAutoScrolling = false;
            } else
            {
                _autoScrollDistance += step;
            }
            scrollPages(-step);
            if (!_isAutoScrolling)
            {
                pageTurningEvent();
            }
            break;
        }
        case TouchDirection::UP:
        case TouchDirection::RIGHT:
        {
            float step = _autoScrollSpeed*dt;
            if (_autoScrollDistance - step <= 0.0f)
            {
                step = _autoScrollDistance;
                _autoScrollDistance = 0.0f;
                _isAutoScrolling = false;
            } else
            {
                _autoScrollDistance -= step;
            }
            scrollPages(step);
            if (!_isAutoScrolling)
            {
                pageTurningEvent();
            }
            break;
        }
        default:
            break;
    }
    
}

void PageView::scrollToPage(ssize_t idx)
{
    if (idx < 0 || idx >= this->getPageCount())
    {
        return;
    }
    _curPageIdx = idx;
    auto curPage = _pages.at(idx);
    
    if (Direction::Horizontal == _direction)
    {
        _autoScrollDistance = -(curPage->getPosition().x);
        _autoScrollDirection = _autoScrollDistance > 0 ? TouchDirection::RIGHT : TouchDirection::LEFT;
    } else
    {
        _autoScrollDistance = -(curPage->getPosition().y);
        _autoScrollDirection = _autoScrollDistance > 0 ? TouchDirection::UP : TouchDirection::DOWN;
    }
    _autoScrollSpeed = fabs(_autoScrollDistance) / 0.2f;
    
    _isAutoScrolling = true;

}

bool PageView::scrollPages(float touchOffset)
{
    if (this->getPageCount() <= 0)
    {
        return false;
    }
    
    if (!_frontChild || !_behindChild)
    {
        return false;
    }
    
    float realOffset = touchOffset;
    
    Point frontChildPos = _frontChild->getPosition();
    Point behindChildPos = _behindChild->getPosition();
    
    Point frontChildAnchorPoint = _frontChild->getAnchorPoint();
    Point behindChildAnchorPoint = _behindChild->getAnchorPoint();
    
    Size frontChildContentSize = _frontChild->getContentSize();
    Size behindChildContentSize = _behindChild->getContentSize();
    
    float behindChildRightBoundary = behindChildPos.x - behindChildAnchorPoint.x * behindChildContentSize.width + behindChildContentSize.width;
    float frontChildLeftBoundary = frontChildPos.x - frontChildAnchorPoint.x * frontChildContentSize.width;

    float behindChildUpBoundary = behindChildPos.y - behindChildAnchorPoint.y * behindChildContentSize.height + behindChildContentSize.height;
    float frontChildDownBoundary = frontChildPos.y - frontChildAnchorPoint.y * frontChildContentSize.height;

    switch (_touchMoveDirection)
    {
        case TouchDirection::LEFT: // left
            if (behindChildRightBoundary + touchOffset <= getContentSize().width)
            {
                realOffset = getContentSize().width - behindChildRightBoundary;
                movePages(realOffset);
                return false;
            }
            break;
        case TouchDirection::RIGHT: // right
            if (frontChildLeftBoundary + touchOffset >= 0)
            {
                realOffset = 0 - frontChildLeftBoundary;
                movePages(realOffset);
                return false;
            }
            break;
        case TouchDirection::DOWN: // down
            if (behindChildUpBoundary + touchOffset <= getContentSize().height)
            {
                realOffset = getContentSize().height - behindChildUpBoundary;
                movePages(realOffset);
                return false;
            }
            break;
        case TouchDirection::UP: // up
            if (frontChildDownBoundary + touchOffset >= 0)
            {
                realOffset = 0 - frontChildDownBoundary;
                movePages(realOffset);
                return false;
            }
            break;
        default:
            break;
    }
    
    movePages(realOffset);
    return true;
}

void PageView::movePages(float offset)
{
    for (auto& page : this->getPages())
    {
        if (Direction::Horizontal == _direction)
        {
            page->setPosition(page->getPosition().x + offset, page->getPosition().y);
        } else
        {
            page->setPosition(page->getPosition().x, page->getPosition().y + offset);
        }
    }
}

void PageView::pageTurningEvent()
{
    this->retain();
    
    if (_eventCallback)
    {
        _eventCallback(this, EventType::TURNING);
    }

    this->release();
}

void PageView::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    if (!_visible)
    {
        return;
    }
    needsLayout();
    Node::visit(renderer, parentTransform, parentFlags);
}

std::vector<Layer*>& PageView::getPages()
{
    return _pages;
}

ssize_t PageView::getPageCount()const
{
    return _pages.size();
}

void PageView::needsLayout()
{
    if (!_doLayerDirty)
    {
        return;
    }
    updateAllPagesPosition();
    updateBoundaryPages();
    
    _doLayerDirty = false;
}

void PageView::updateBoundaryPages()
{
    if (_pages.size() <= 0)
    {
        _frontChild = nullptr;
        _behindChild = nullptr;
        return;
    }
    _frontChild = _pages.at(0);
    _behindChild = _pages.at(this->getPageCount() - 1);
}

void PageView::updateAllPagesPosition()
{
    ssize_t pageCount = this->getPageCount();
    
    if (pageCount <= 0)
    {
        _curPageIdx = 0;
        return;
    }
    
    if (_curPageIdx >= pageCount)
    {
        _curPageIdx = pageCount - 1;
    }
    
    float pageWidth = getContentSize().width;
    float pageHeight = getContentSize().height;
    for (int i = 0; i < pageCount; i++)
    {
        auto page = _pages.at(i);
        if (Direction::Horizontal == _direction)
        {
            page->setPosition((i - _curPageIdx) * pageWidth, 0);
        } else
        {
            page->setPosition(0, (i - _curPageIdx) * pageHeight);
        }
    }
}

NS_CC_GUI_END