//
//  GridView.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/29.
//
//

#ifndef GridView_h
#define GridView_h

#include "GuiMacros.h"
#include "cocos-ext.h"

NS_CC_GUI_BEGIN

class GridViewCell;
class GridView;

class GridViewDelegate
{
public:
    virtual void gridCellTouched(GridView* view, GridViewCell* cell) = 0;
};

class GridViewCell : public LayerColor
{
public:
    static GridViewCell * create(const Color4B& color);
    
    CC_SYNTHESIZE(int, _row, Row);
    CC_SYNTHESIZE(int, _idx, Idx);
protected:
    GridViewCell();
    ~GridViewCell();
};

class GridView : public extension::ScrollView
{
public:
    enum class Direction
    {
        Horizontal,
        Vertical
    };
    
    static GridView* create(const Size& size, Node* container = nullptr);
    CC_SYNTHESIZE(int, _cols, Cols);
    CC_SYNTHESIZE(int, _rows, Rows);
    CC_SYNTHESIZE(int, _cellCounts, CellCounts);

    void setCellSize(const Size& cellsize);
    const Size& getCellSize();
    
    void setDirection(Direction dir);
    
    void setDelegate(GridViewDelegate* delegate);
    GridViewDelegate* getDelegate();
    GridViewCell* cellAtIndex(int idx);

    void reloadData();
private:
    Size _cellSize;
    
    bool initViewSize(const Size& size, Node* container = nullptr);
protected:
    std::list<GridViewCell*> _usedCellList, _freeCellList;
    std::set<int> _idxIndices;
    bool _doLayerDirty;
    Direction _direction;
    GridViewDelegate* _gridViewDelegate;
    GridViewCell *_touchedCell;
    EventListenerTouchOneByOne* _eventListener;

    GridView();
    ~GridView();
    
    void onDrawCellView();
    void onDrawCellViewH();
    void onDrawCellViewV();
    void updateCellPosition();
    Vec2 cellPosition(int idx, int row);
    GridViewCell* cellForTouch(Touch *touch);
    
    void insertCell(GridViewCell* cell, int idx);
    void updateCellAtIndex(int idx, int row);
    
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    void needsLayout();
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
//    virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
//    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;
};

NS_CC_GUI_END

#endif /* GridView_h */
