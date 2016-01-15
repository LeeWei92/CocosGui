//
//  RichLabel.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by Lee on 15/9/2.
//
//

#ifndef RichLabel_h
#define RichLabel_h

#include "GuiMacros.h"

NS_CC_GUI_BEGIN

class RichLabel : public Node
{
public:
    CREATE_FUNC(RichLabel);
    
    virtual bool init() override;
    
    void insertElement(Node* node);
    void removeElement(Node* node);
    
    void setAnchorPoint(const Vec2& anchorPoint) override;
private:
    void updateChildPosition();

    std::vector<Node*> m_children;
    
};

NS_CC_GUI_END

#endif /* RichLabel_h */
