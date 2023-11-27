#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

class Mesh
{
private:
    std::vector<class Texture*> mTextures;
    class VertexArray* mVertexArray;
    std::string mShaderName;
    float mRadius; // 对象空间边界球体的半径
    float mSpecPower;
    class AABB* mBox;
    std::string mFileName;
    
public:
    Mesh(/* args */);
    ~Mesh();

    bool load(const std::string& fileName, class Renderer*);
    void unLoad();

    // getters
    class VertexArray* getVertexArray();
    class Texture* getTexture(size_t index);
    const std::string& getShaderName() const;
    float getRadius() const;
    float getSpecPower();
    const AABB* getBox();
    const std::string& getFileName();
};