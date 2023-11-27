#include "./MeshComponent.h"

const size_t MAX_SKELETON_BONES = 96;
struct MatrixPalette
{
    Matrix4 mEntry[MAX_SKELETON_BONES];
};

class SkeletalMeshComponent : public MeshComponent
{
public:
    SkeletalMeshComponent(class Actor* owner);
    ~SkeletalMeshComponent();

    // 绘制网格组件
    void draw(class Shader* shader) override;

    // 计算矩阵调色板
    void computeMatrixPalette();

    // 播放动画
    float playAnimation(class Animation* anim, float playRate = 1.0f);

    // 更新
    void update(float dt) override;

    // setters
    void setSkeleton(class Skeleton* sk);

    EComponentType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;

private:
    // 矩阵调色板
    MatrixPalette mPalette;
    // 动画播放速率
    float mAnimPlayRate;
    // 动画的当前时间
    float mAnimTime;

    // 骨骼
    class Skeleton* mSkeleton;
    // 当前播放的动画
    class Animation* mAnimation;
};

