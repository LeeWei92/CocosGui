//
//  Scale9Button.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/23.
//
//

#ifndef Scale9Button_h
#define Scale9Button_h

#include "GuiMacros.h"
#include "ButtonProtocol.h"
#include "ui/CocosGUI.h"

NS_CC_GUI_BEGIN

class Scale9Button : public ui::Scale9Sprite, public ButtonProtocol
{
public:
    static Scale9Button *create(const std::string& normalImage, const ccButtonCallback& callback = nullptr);
    static Scale9Button *create(const std::string& normalImage, const std::string& highlightedImage, const ccButtonCallback& callback = nullptr);
    static Scale9Button *create(const std::string& normalImage, const std::string& highlightedImage, const std::string& disabledImage, const ccButtonCallback& callback = nullptr);
    
    CC_SYNTHESIZE(bool, _zoomOnTouchDown, ZoomOnTouchDown);
    
    virtual void setHighlighted(bool highlighted) override;
    virtual void setVisible(bool visible) override;
    
    void setTitleText(const std::string& text);
    const std::string& getTitleText() const;
    
    void setTitleColor(const Color3B& color);
    void setTitleColorForState(const Color3B& color, ButtonState state);
    const Color3B& getTitleColorForState(ButtonState state) const;
    
    void setTitleFontSize(float size);
    float getTitleFontSize() const;
    void setTitleFontName(const std::string& fontName);
    const std::string& getTitleFontName() const;
    
    virtual void setPreferredSize(const Size& preferredSize);
        
    void setTitleProperity(const std::string& text, const std::string& fontName, float size);
private:
    std::map<ButtonState, Color3B> _titleColorStateMap;
protected:
    Label* _titleRenderer;
    float _originalScale;
    float _titlFontSize;
    std::string _titlFontName, _titleStr;
    Color3B _titleColor;
    
    std::vector<ParentProperty> _parentPropertyArr;
    
    Scale9Button(void);
    virtual ~Scale9Button(void);
    
    bool initWithFile(const std::string& filename) override;
    
    void setSpriteWithFile(const std::string& filename);
    
    void initRenderer();
    bool isExistTitleColorForState(ButtonState state);
    
    void needsLayout(void) override;
    
    bool touchHits(Touch *touch);
    
    void updateRendererPos();
    
    virtual bool onTouchBegan(Touch  *touch, Event  *event) override;
    virtual void onTouchEnded(Touch  *touch, Event  *event) override;
    virtual void onTouchMoved(Touch  *touch, Event  *event) override;
};

NS_CC_GUI_END

#endif /* Scale9Button_h */
