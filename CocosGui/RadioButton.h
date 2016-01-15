//
//  RadioButton.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/24.
//
//

#ifndef RadioButton_h
#define RadioButton_h

#include "GuiMacros.h"
#include "ButtonProtocol.h"

NS_CC_GUI_BEGIN

class RadioButton : public Sprite, public ButtonProtocol
{
public:
    static RadioButton *create(const std::string& normalImage, const ccButtonCallback& callback = nullptr);
    static RadioButton *create(const std::string& normalImage, const std::string& selectedImage, const ccButtonCallback& callback = nullptr);
    static RadioButton *create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccButtonCallback& callback = nullptr);
    
    virtual void setVisible(bool visible) override;
    void activeTouchEvent();
protected:
    std::vector<ParentProperty> _parentPropertyArr;
    bool _bMoved;
    
    RadioButton(void);
    virtual ~RadioButton(void);
    
    bool initWithFile(const std::string& filename) override;
    
    void setSpriteWithFile(const std::string& filename);
    
    void needsLayout(void) override;
    
    bool touchHits(Touch *touch);
    
    virtual bool onTouchBegan(Touch  *touch, Event  *event) override;
    virtual void onTouchEnded(Touch  *touch, Event  *event) override;
    virtual void onTouchMoved(Touch  *touch, Event  *event) override;
};

typedef std::function<void(Ref*, RadioButton*)> ccRadioGroupCallback;

class RadioGroup : public Layer
{
    friend class RadioButton;
public:
    static RadioGroup* create();
    static RadioGroup* create(std::vector<RadioButton*> groupList);
    
    CC_SYNTHESIZE(bool, _enabled, Enabled);

    // overrides
    virtual void removeChild(Node* child, bool cleanup) override;
    
    virtual void addChild(Node * child) override;
    virtual void addChild(Node * child, int zOrder) override;
    virtual void addChild(Node * child, int zOrder, int tag) override;
    virtual void addChild(Node * child, int zOrder, const std::string &name) override;
    
    virtual void addRadioEventListener(const ccRadioGroupCallback& callback);
protected:
    EventListenerTouchOneByOne* _eventListener;
    RadioButton* _selectedBtn;
    ccRadioGroupCallback _callBack;
    
    RadioGroup(void);
    ~RadioGroup(void);
    
    bool initWithArray(std::vector<RadioButton*> groupList);
    virtual bool init() override;
    RadioButton* getItemForTouch(Touch *touch);
    void resetAllItemState(RadioButton* selectedItem);
    
    virtual bool onTouchBegan(Touch  *touch, Event  *event) override;
    virtual void onTouchEnded(Touch  *touch, Event  *event) override;
    virtual void onTouchMoved(Touch  *touch, Event  *event) override;
};

NS_CC_GUI_END

#endif /* RadioButton_h */
