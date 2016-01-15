//
//  SpriteButton.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/12/18.
//
//

#ifndef SpriteButton_h
#define SpriteButton_h

#include "GuiMacros.h"
#include "ButtonProtocol.h"

NS_CC_GUI_BEGIN

class SpriteButton : public Sprite, public ButtonProtocol
{
public:
    static SpriteButton* create(const std::string& title, const std::string& fontName, float fontSize);
    static SpriteButton *create(const std::string& normalImage, const ccButtonCallback& callback = nullptr);
    static SpriteButton *create(const std::string& normalImage, const std::string& highlightedImage, const ccButtonCallback& callback = nullptr);
    static SpriteButton *create(const std::string& normalImage, const std::string& highlightedImage, const std::string& disabledImage, const ccButtonCallback& callback = nullptr);
    
    CC_SYNTHESIZE(bool, _zoomOnTouchDown, ZoomOnTouchDown);
    
    virtual void setHighlighted(bool highlighted) override;
    
    void setTitleText(const std::string& text);
    const std::string& getTitleText() const;
    
    void setTitleColor(const Color3B& color);
    const Color3B& getTitleColor() const;
    
    void setTitleColorForState(const Color3B& color, ButtonState state);
    const Color3B& getTitleColorForState(ButtonState state) const;
    
    void setTitleFontSize(float size);
    float getTitleFontSize() const;
    
    void setTitleFontName(const std::string& fontName);
    const std::string& getTitleFontName() const;
    
    void setTitleProperity(const std::string& text, const std::string& fontName, float size);
    
    virtual void setVisible(bool visible) override;
private:
    std::map<ButtonState, Color3B> _titleColorStateMap;
protected:
    Label* _titleRenderer;

    std::vector<ParentProperty> _parentPropertyArr;
    float _originalScale;

    SpriteButton(void);
    virtual ~SpriteButton(void);
    
    bool initWithFile(const std::string& filename) override;
    
    void setSpriteWithFile(const std::string& filename);
    
    void needsLayout(void) override;
    
    bool touchHits(Touch *touch);
    
    void initRenderer();
    bool isExistTitleColorForState(ButtonState state);
    
    virtual bool onTouchBegan(Touch  *touch, Event  *event) override;
    virtual void onTouchEnded(Touch  *touch, Event  *event) override;
    virtual void onTouchMoved(Touch  *touch, Event  *event) override;
};

NS_CC_GUI_END

#endif /* SpriteButton_h */
