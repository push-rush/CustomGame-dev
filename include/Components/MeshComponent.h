#pragma once

#include "./Component.h"

class MeshComponent : public Component
{
private:
    bool mIsSkeletal;
    class Mesh* mMesh;
    size_t mTextureIndex;

public:
    MeshComponent(class Actor* owner, bool isSkeletal = false);
    ~MeshComponent();

    virtual void draw(class Shader* shader);

    // setters
    virtual void setMesh(class Mesh* mesh);
    void setTextureIndex(size_t index);
    // void setMeshName(std::string name);

    // getters
    bool getIsSkeletal() const;
    class Mesh* getMesh() const;
    size_t getTextureID() const;
    // std::string getMeshName() const;

    EComponentType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
