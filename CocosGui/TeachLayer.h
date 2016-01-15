//
//  TeachLayer.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/5/20.
//
//

#ifndef TeachLayer_h
#define TeachLayer_h

#include "GuiMacros.h"
#include "SpriteButton.h"

NS_CC_GUI_BEGIN

typedef std::function<void(Ref*)> ccTouchCallback;

class TeachLayer : public Layer
{
public:
    enum class FingerDirection
    {
        Up,         // ↑
        Down,       // ↓
        Left,       // ←
        Right,      // →
        LeftDown,   // ↙
        RightDown,  // ↘
        LeftUp,     // ↖
        RightUp,    // ↗
    };
    
    CREATE_FUNC(TeachLayer);
    
    static TeachLayer* create(const Vec2& stencilWorldPos, FingerDirection fingerDirection = FingerDirection::Down);
    
    virtual bool init() override;
    virtual bool init(const Vec2& stencilWorldPos, FingerDirection fingerDirection);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    // 蒙版坐标
    CC_SYNTHESIZE_READONLY(Point, _stencilPosition, StencilPosition)
    CC_SYNTHESIZE_READONLY(Point, _stencilAnchorPoint, StencilAnchorPoint)
    // 蒙版大小
    CC_SYNTHESIZE_READONLY(float, _stencilScale, StencilScale)
    // 箭头指示坐标 美术提供图片箭头方向要求向下
    CC_SYNTHESIZE_READONLY(FingerDirection, _fingerDirection, FingerDirection)

    void setStencilPosition(const Vec2 &worldPos);
    void setStencilPosition(float worldPosX, float worldPosY);
    void setStencilAnchorPoint(const Vec2& point);
    void setStencilScale(float scale);
    
    void setFingerDirection(FingerDirection fingerDirection);
    
    void setCustomTouchCallBack(const ccTouchCallback& callBack);
    void loadTeachView();
protected:
    // 模板节点
    DrawNode *_stencilNode;
    SpriteButton *_stencilButton;
    GLubyte _pNodeOriginalOpacity;
    Sprite* _fingerSprite, *_fingerEffSprite;
    ccTouchCallback _touchCallback;
    
    TeachLayer(void);
    virtual ~TeachLayer(void);
    
    void stencilTouchCallBack(Ref* ref);
    
    bool checkChildType(Node* pNode, Rect stencilRect);
    Rect getNodeRect(Node* pNode);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused) override;
    virtual void onTouchEnded(Touch  *touch, Event  *event) override;
    virtual void runMoveAction();
private:
    // 移动距离
    float _fingerMoveRangeX, _fingerMoveRangeY;
    bool touchHits(Touch *touch);
};

NS_CC_GUI_END

#endif /* TeachLayer_h */
