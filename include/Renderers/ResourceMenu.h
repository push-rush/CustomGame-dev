#pragma once

#include "./UIScreen.h"

#include "../General/TreeStruct.h"

#include "../../include/General/ResourceManager.h"

class ResourceMenu : public UIScreen
{
private:
    // ui树
    TreeStruct mResourceMenuTree;
    std::vector<TreeNode*> mNodeStack;
    std::vector<TreeNode*> mResourceStack;
    bool mFlag;
    
    // 资源绑定事件树
    TreeStruct mResourceEventTree;
    std::vector<TreeNode*> mResourceEventStack;

    Vector2 mLeftButtonPos;
    Vector2 mBottomButtonPos;
    Vector2 mRightButtonPos;

public:
    ResourceMenu(class Game* game);
    ~ResourceMenu();

    // void draw(class Shader* spriteShader, class Shader* fontShader, 
    //         class EmptySprite* elem = nullptr, 
    //         const Vector2& = Vector2{0.0f, 0.0f}) override;

    // void drawElement(class Shader* spriteShader, class Shader* fontShader, 
    //         class EmptySprite* elem = nullptr, 
    //         const Vector2& = Vector2{0.0f, 0.0f}, const Vector2& pos = Vector2{0.0f, 0.0f});

    void draw(
        class Shader* basicShader, 
        class Shader* spriteShader, 
        class Shader* fontShader,
        class EmptySprite* elem
    ) override;

    // 更新组件
    void update(float dt) override;

    // 事件绑定
    void bindEvent(const UIBindEvent& event, class Button* b) override;

    // 外部输入处理
    void processInput(const uint8_t* keys) override;

    // 处理按键输入
    void handleKeyPress(int key) override;

    void handleMouseWheel(const int& mouse_wheel) override;

    // getters
    TreeStruct* getResourceMenuTree();
    void nodeAddToStack(TreeNode* node);

    TreeStruct* getResourceEventTree();
    void updateResourceEventStack();

    std::map<std::string, std::string> allocTex(const ResourceManager::ResourceType& type);

    void setUIBorderPos(const Vector2& left, const Vector2& bottom, const Vector2& right);
};
