//
//  PageView.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 14/12/18.
//
//

#ifndef PageView_h
#define PageView_h

#include "GuiMacros.h"

NS_CC_GUI_BEGIN

class PageView : public Layer
{
public:
    enum class Direction
    {
        Horizontal,
        Vertical
    };
    
    enum class TouchDirection
    {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    
    enum class EventType
    {
        TURNING
    };
    
    typedef std::function<void(Ref*, EventType)> ccPageViewCallback;
    
    PageView();
    virtual ~PageView();
    
    virtual bool init() override;
    
    CREATE_FUNC(PageView);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
    virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
    virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
    virtual void onTouchCancelled(Touch *touch, Event *unused_event) override;
    
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    
    CC_SYNTHESIZE(Direction, _direction, Direction);
    
    void addPage(Layer *page);
    void insertPage(Layer* page, int idx);
    void removePage(Layer* page);
    void removePageAtIndex(ssize_t index);
    void removeAllPages();
    
    void scrollToPage(ssize_t idx);
    
    std::vector<Layer*>& getPages();
    
    bool scrollPages(float touchOffset);
    
    void addEventListener(const ccPageViewCallback& callback);
    
    void setCustomScrollThreshold(float threshold);
    float getCustomScrollThreshold()const;
    
    void setUsingCustomScrollThreshold(bool flag);
    bool isUsingCustomScrollThreshold()const;
    ssize_t getCurPageIndex()const;
    Layer* getPage(ssize_t index);
    
    virtual void update(float dt) override;
protected:
    ssize_t _curPageIdx;
    std::vector<Layer*> _pages;
    ClippingRectangleNode* _clippingNode;
    bool _isAutoScrolling;
    bool _isInterceptTouch;
    float _autoScrollDistance;
    float _autoScrollSpeed;
    
    TouchDirection _autoScrollDirection;
    TouchDirection _touchMoveDirection;
    
    Layer* _frontChild;
    Layer* _behindChild;
    
    EventListenerTouchOneByOne* _touchListener;
    Ref* _pageViewEventListener;
    ccPageViewCallback _eventCallback;
    
    float _customScrollThreshold;
    bool _usingCustomScrollThreshold;
    
    float _childFocusCancelOffset;
    
    bool _doLayerDirty;
    
    void needsLayout(void);
    
    ssize_t getPageCount()const;
    void updateAllPagesPosition();
    void updateBoundaryPages();
    
    void handleMoveLogic(Touch *touch);
    void handleReleaseLogic(Touch *touch);
    void pageTurningEvent();
    void movePages(float offset);
    void autoScroll(float dt);
};

NS_CC_GUI_END

#endif /* PageView_h */
