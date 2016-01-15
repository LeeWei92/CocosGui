//
//  ButtonProtocol.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/18.
//
//

#ifndef ButtonProtocol_h
#define ButtonProtocol_h

#include "cocos2d.h"
#include "GuiMacros.h"

NS_CC_GUI_BEGIN

typedef std::function<void(Ref*)> ccButtonCallback;

class ButtonProtocol
{
public:
    enum class ButtonState
    {
        Noramal = 0,
        Highlighted,
        Disabled,
        Selected
    };
    
    struct ParentProperty
    {
        ParentProperty()
        {
            pBeginPos = Vec2::ZERO;
            pNode = NULL;
        }
        
        Vec2 pBeginPos;
        Node* pNode;
    };
    
    CC_SYNTHESIZE_READONLY(bool, _selected, Selected);
    CC_SYNTHESIZE_READONLY(ButtonState, _buttonState, State);
    CC_SYNTHESIZE_READONLY(bool, _enabled, Enabled);
    CC_SYNTHESIZE_READONLY(bool, _highlighted, Highlighted);
    CC_SYNTHESIZE_READONLY(bool, _needSwallow, SwallowTouches);
    
    virtual void setEnabled(bool enabled);
    virtual void setHighlighted(bool highlighted);
    virtual void setSelected(bool selected);
    
    virtual void setSwallowTouches(bool needSwallow);
    virtual void setTouchPriority(int touchPriority);
    
    void setBackgroudImageForState(const std::string& filename, ButtonState state);
    std::string getBackgroudImageForState(ButtonState state) const;
    
    virtual void addClickEventListener(const ccButtonCallback& callback);
    // virtual void addLongClickEventListener(const ccButtonCallback& callback);
protected:
    ccButtonCallback _callBack;
    EventListenerTouchOneByOne* _eventListener;
    std::map<ButtonState, std::string> _backgroundrStateMap;

    ButtonProtocol(void);
    ~ButtonProtocol(void);

    virtual void needsLayout(void) {};
    
    bool checkParentScroll(std::vector<ParentProperty> parentPropertyArr);
    
    Node* getBtnParent(Node* pNode);
    std::vector<ParentProperty> getBtnParentPropertyList(Node* pSendser);
    
    virtual bool onTouchBegan(Touch  *touch, Event  *event) = 0;
    virtual void onTouchEnded(Touch  *touch, Event  *event) = 0;
    virtual void onTouchMoved(Touch  *touch, Event  *event) = 0;
    virtual void onTouchCancelled(Touch *touch, Event *event) {};
};

NS_CC_GUI_END

#endif /* ButtonProtocol_h */
