#pragma once

#include "./UIScreen.h"
#include "../General/TreeStruct.h"

// template class TreeStruct<EmptySprite>;

class HUD : public UIScreen
{
private:
    class UIElement* mCrosschair;
    class UIElement* mCrosshairEnemy;
    class UIElement* mRadar;
    class UIElement* mBlipTex;
    class UIElement* mRadarArrow;
    class UIElement* mHealthBar;

    bool mTargetEnemy;

    std::vector<class TargetComponent*> mTargetComponents;
    std::vector<Vector2> mBlips; // 雷达光点
    float mRadarRange;          // 雷达扫描范围
    float mRadarRadius;         // 屏幕上雷达半径

    // ui树
    TreeStruct mMenuTree;
    std::vector<TreeNode*> mNodeStack;

public:
    HUD(class Game* game);
    ~HUD();

    // add or remove
    void addTargetComp(class TargetComponent*);
    void removeTargetComp(class TargetComponent*);

    // 更新瞄准十字线
    void updateCrosshair(float dt);

    void draw(class Shader* spriteShader, class Shader* fontShader, EmptySprite* elem = nullptr) override;

    // 更新雷达
    void updateRadar(float dt);

    // 更新组件
    void update(float dt) override;

    // 事件绑定
    void bindEvent(const UIBindEvent& event) override;

    void processInput(const uint8_t* keys) override;

    void handleKeyPress(int key) override;
    
    // getters
    TreeStruct* getUIMenuTree();
    void nodeAddToStack(TreeNode* node);
    // setters
};