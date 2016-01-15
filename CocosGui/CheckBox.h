//
//  CheckBox.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/23.
//
//

#ifndef CheckBox_h
#define CheckBox_h

#include "GuiMacros.h"
#include "ButtonProtocol.h"

NS_CC_GUI_BEGIN

class CheckBox : public Sprite, public ButtonProtocol
{
public:
    static CheckBox *create(const std::string& normalImage, const ccButtonCallback& callback = nullptr);
    static CheckBox *create(const std::string& normalImage, const std::string& selectedImage, const ccButtonCallback& callback = nullptr);
    static CheckBox *create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccButtonCallback& callback = nullptr);
    
    virtual void setVisible(bool visible) override;
protected:
    std::vector<ParentProperty> _parentPropertyArr;
    bool _bMoved;
    
    CheckBox(void);
    virtual ~CheckBox(void);
    
    bool initWithFile(const std::string& filename) override;
    
    void setSpriteWithFile(const std::string& filename);
    
    void needsLayout(void) override;
    
    bool touchHits(Touch *touch);
    
    virtual bool onTouchBegan(Touch  *touch, Event  *event) override;
    virtual void onTouchEnded(Touch  *touch, Event  *event) override;
    virtual void onTouchMoved(Touch  *touch, Event  *event) override;
};
NS_CC_GUI_END

#endif /* CheckBox_h */
