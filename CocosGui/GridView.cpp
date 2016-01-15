//
//  GridView.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/29.
//
//

#include "GridView.h"

NS_CC_GUI_BEGIN

#define GRID_CELL_MARGIN 2

GridViewCell::GridViewCell()
{
    ignoreAnchorPointForPosition(false);
}

GridViewCell::~GridViewCell()
{
    
}

GridViewCell *GridViewCell::create(const Color4B& color)
{
    GridViewCell *layer = new (std::nothrow) GridViewCell();
    if( layer && layer->initWithColor(color))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

GridView::GridView()
: _cellSize(Vec2::ZERO)
, _cellCounts(0)
, _cols(1)
, _rows(0)
, _touchedCell(nullptr)
, _gridViewDelegate(nullptr)
, _doLayerDirty(true)
, _eventListener(nullptr)
{
    _idxIndices.clear();
    _usedCellList.clear();
    _freeCellList.clear();
}

GridView::~GridView()
{
}

GridView* GridView::create(const Size& size, Node* container)
{
    auto gridView = new (std::nothrow) GridView();
    if (gridView && gridView->initWithViewSize(size, container))
    {
        gridView->autorelease();
        return gridView;
    }
    CC_SAFE_DELETE(gridView);
    return nullptr;
}

bool GridView::initViewSize(const cocos2d::Size &size, Node* container)
{
    if(!ScrollView::initWithViewSize(size, container))
    {
        return false;
    }
    
    _eventListener = EventListenerTouchOneByOne::create();
    _eventListener->onTouchBegan = CC_CALLBACK_2(GridView::onTouchBegan, this);
    _eventListener->onTouchEnded = CC_CALLBACK_2(GridView::onTouchEnded, this);
    _eventListener->onTouchMoved = CC_CALLBACK_2(GridView::onTouchMoved, this);
    _eventListener->setSwallowTouches(true);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_eventListener, this);
    
    return true;
}

void GridView::updateCellAtIndex(int idx, int row)
{
    if (0 >= _cellCounts) return;
 
    GridViewCell* pCell = GridViewCell::create(Color4B(0, 0, 255, 255));
    pCell->setIdx(idx);
    pCell->setRow(row);
    pCell->setAnchorPoint(Vec2(0, 1));
    pCell->setContentSize(_cellSize);
    pCell->setPosition(cellPosition(idx, row));
    _container->addChild(pCell);
    insertCell(pCell, idx);
//    pCell->retain();
    
    _idxIndices.insert(idx);
}

Vec2 GridView::cellPosition(int idx, int row)
{
    Vec2 point = Vec2::ZERO;
    if (Direction::Vertical == _direction)
    {
        int col = idx - row * _cols;
        point.x = col * _cellSize.width + col * GRID_CELL_MARGIN;
        point.y = _container->getContentSize().height - row * _cellSize.height - row * GRID_CELL_MARGIN;
    } else
    {
        int col = ceil(idx / _cols);
        point.x = col * _cellSize.width + col * GRID_CELL_MARGIN;
        point.y = _viewSize.height - row * _cellSize.height - row * GRID_CELL_MARGIN;
    }
    return point;
}

void GridView::updateCellPosition()
{
}

void GridView::insertCell(GridViewCell *cell, int idx)
{
    if (_usedCellList.empty())
    {
        _usedCellList.push_back(cell);
    } else
    {
        for (auto it = _usedCellList.begin(); it != _usedCellList.end(); it++)
        {
            if (idx < (*it)->getIdx())
            {
                _usedCellList.insert(it, cell);
                break;
            }
        }
    }
}

void GridView::reloadData()
{
    onDrawCellView();
}

void GridView::setDelegate(GridViewDelegate* delegate)
{
    _gridViewDelegate = delegate;
}

GridViewDelegate* GridView::getDelegate()
{
    return _gridViewDelegate;
}

void GridView::setCellSize(const cocos2d::Size &cellsize)
{
    _cellSize = cellsize;
}

const Size& GridView::getCellSize()
{
    return _cellSize;
}

void GridView::onDrawCellView()
{
    if (Direction::Vertical == _direction)
    {
        onDrawCellViewV();
    } else
    {
        onDrawCellViewH();
    }
}

void GridView::onDrawCellViewV()
{
    if (0 >= _cellCounts) return;
    _rows = (_cellCounts % _cols == 0) ? _cellCounts / _cols : ceil(_cellCounts / _cols) + 1;
    float fHeight =  _contentSize.height;
    float fWidth = _cols * _cellSize.width;
    
    _cellSize.width = (_contentSize.width - (_cols - 1) * GRID_CELL_MARGIN) / _cols;
    fHeight = std::max(_rows * _cellSize.height + (_rows - 1) * GRID_CELL_MARGIN, _contentSize.height);
    _container->setContentSize(Size(fWidth, fHeight));
    
    int nBeginRow(0)/*, nEndRow(0) */;
    //    Vec2 Offset = getContentOffset();
    //    nBeginRow = cellBeginRowFromOffset(Offset);
    //    nEndRow = cellEndRowFromOffset(Offset);
    for(int row = nBeginRow; /*row <= nEndRow && */ row < _rows; ++row)
    {
        int nCellBeginIndex = _cols * row;
        int nCellEndIndx = nCellBeginIndex + _cols;
        int idx = nCellBeginIndex;
        
        for(; idx < nCellEndIndx && idx < _cellCounts; ++idx)
        {
            if( _idxIndices.find(idx) != _idxIndices.end() )
            {
                continue;
            }
            updateCellAtIndex(idx, row);
        }
    }
    
    if (_container->getContentSize().height > _viewSize.height)
    {
        setContentOffset(Vec2(0,  _viewSize.height - _container->getContentSize().height));
    }
}

void GridView::onDrawCellViewH()
{
    if (0 >= _cellCounts) return;
    _rows = (_cellCounts % _cols == 0) ? _cellCounts / _cols : ceil(_cellCounts / _cols) + 1;
    float fHeight =  _contentSize.height;
    float fWidth = _cols * _cellSize.width;
    
    _cellSize.width = (_contentSize.width - (_cols - 1) * GRID_CELL_MARGIN) / _cols;
    fWidth = std::max(_rows * _cellSize.width + (_rows - 1) * GRID_CELL_MARGIN, _contentSize.width);
    _container->setContentSize(Size(fWidth, fHeight));
    
    
    int nBeginRow(0)/*, nEndRow(0)*/;
    //    Vec2 Offset = getContentOffset();
    //    nBeginRow = cellBeginRowFromOffset(Offset);
    //    nEndRow = cellEndRowFromOffset(Offset);
    int idx(0);
    for(int row = nBeginRow; row < _rows; ++row)
    {
        if (idx < _cellCounts)
        {
            if( _idxIndices.find(idx) == _idxIndices.end() )
            {
                updateCellAtIndex(idx, row);
            }
        }
        
        if (_rows - 1 == row && idx < _cellCounts)
        {
            row = -1;
        }
        idx++;
    }
    
    if (_container->getContentSize().height > _viewSize.width)
    {
        setContentOffset(Vec2(_viewSize.width - _container->getContentSize().width, 0));
    }
}

void GridView::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    if (!_visible)
    {
        return;
    }
    needsLayout();
    ScrollView::visit(renderer, parentTransform, parentFlags);
}

void GridView::needsLayout()
{
    if (!_doLayerDirty)
    {
        return;
    }
    onDrawCellView();
    
    _doLayerDirty = false;
}

void GridView::setDirection(Direction dir)
{
    switch (dir)
    {
        case Direction::Vertical:
            ScrollView::setDirection(ScrollView::Direction::VERTICAL);
            break;
        case Direction::Horizontal:
            ScrollView::setDirection(ScrollView::Direction::HORIZONTAL);
            break;
        default:
            break;
    }
    _direction = dir;
}

void GridView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible())
    {
        return;
    }
    
    if (_touchedCell)
    {
        Rect bb = this->getBoundingBox();
        bb.origin = _parent->convertToWorldSpace(bb.origin);
        
        if (bb.containsPoint(pTouch->getLocation()) && _gridViewDelegate != nullptr)
        {
//            _gridViewDelegate->tableCellUnhighlight(this, _touchedCell);
            _gridViewDelegate->gridCellTouched(this, _touchedCell);
        }
        
        _touchedCell = nullptr;
    }
    
    ScrollView::onTouchEnded(pTouch, pEvent);
}

bool GridView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    for (Node *c = this; c != nullptr; c = c->getParent())
    {
        if (!c->isVisible())
        {
            return false;
        }
    }
    
    bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);
    
    if(_touches.size() == 1)
    {
//        long index;
//        Vec2 point;
        
        _touchedCell = cellForTouch(pTouch);
        
//        point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        
//        index = this->_indexFromOffset(point);
//        if (index == CC_INVALID_INDEX)
//        {
//            _touchedCell = nullptr;
//        }
//        else
//        {
//            _touchedCell  = this->cellAtIndex(index);
//        }
        
        if (_touchedCell && _gridViewDelegate != nullptr)
        {
//            _gridViewDelegate->tableCellHighlight(this, _touchedCell);
        }
    } else if (_touchedCell)
    {
        if(_gridViewDelegate != nullptr)
        {
//            _gridViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = nullptr;
    }
    
    return touchResult;
}

GridViewCell *GridView::cellAtIndex(int idx)
{
    if (_idxIndices.find(idx) != _idxIndices.end())
    {
        for (const auto& cell : _usedCellList)
        {
            if (cell->getIdx() == idx)
            {
                return cell;
            }
        }
    }
    return nullptr;
}

//void GridView::onTouchMoved(Touch *pTouch, Event *pEvent)
//{
//    ScrollView::onTouchMoved(pTouch, pEvent);
//    
//    if (_touchedCell && isTouchMoved())
//    {
//        if(_gridViewDelegate != nullptr)
//        {
////            _gridViewDelegate->tableCellUnhighlight(this, _touchedCell);
//        }
//        
//        _touchedCell = nullptr;
//    }
//}

//void GridView::onTouchCancelled(Touch *pTouch, Event *pEvent)
//{
//    ScrollView::onTouchCancelled(pTouch, pEvent);
//    
//    if (_touchedCell)
//    {
//        if(_gridViewDelegate != nullptr)
//        {
////            _gridViewDelegate->tableCellUnhighlight(this, _touchedCell);
//        }
//        
//        _touchedCell = nullptr;
//    }
//}

GridViewCell* GridView::cellForTouch(Touch *touch)
{
    Vec2 offset = getContentOffset();
    const Size size = getViewSize();
    const float scale = getZoomScale();
    
    Rect viewRect = Rect::ZERO;
    
    if (Direction::Horizontal == _direction)
    {
        if (0 <= offset.x) // right
        {
            offset.x = offset.x + _cellSize.width;
            
        } else // left
        {
            offset.x = offset.x - _cellSize.width;
        }
    } else if (Direction::Vertical == _direction)
    {
        if (0 <= offset.y) // down
        {
            offset.y = offset.y + _cellSize.height;
            
        } else // up
        {
            offset.y = offset.y - _cellSize.height;
        }
    }
    viewRect = Rect(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale);
    
    for (int i = 0; i < _container->getChildrenCount(); i++)
    {
        GridViewCell* pCell = dynamic_cast<GridViewCell*>(_container->getChildren().at(i));
        if (!pCell) continue;
        
        if (viewRect.intersectsRect(pCell->boundingBox()))
        {
            if (pCell->boundingBox().containsPoint(_container->convertTouchToNodeSpace(touch)))
            {
                return pCell;
            }
        }
    }
    return nullptr;
}
NS_CC_GUI_END