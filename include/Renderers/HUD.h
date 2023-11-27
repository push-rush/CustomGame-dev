#include "./UIScreen.h"

class HUD : public UIScreen
{
private:
    class Texture* mCrosschair;
    class Texture* mCrosshairEnemy;
    class Texture* mRadar;
    class Texture* mBlipTex;
    class Texture* mRadarArrow;
    class Texture* mHealthBar;

    bool mTargetEnemy;

    std::vector<class TargetComponent*> mTargetComponents;
    std::vector<Vector2> mBlips; // 雷达光点
    float mRadarRange;          // 雷达扫描范围
    float mRadarRadius;         // 屏幕上雷达半径

public:
    HUD(class Game* game);
    ~HUD();

    // add or remove
    void addTargetComp(class TargetComponent*);
    void removeTargetComp(class TargetComponent*);

    // 更新瞄准十字线
    void updateCrosshair(float dt);

    void draw(class Shader* spriteShader, class Shader* fontShader) override;

    // 更新雷达
    void updateRadar(float dt);

    // 更新组件
    void update(float dt) override;
};