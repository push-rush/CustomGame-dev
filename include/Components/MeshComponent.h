#pragma once

#include "./Component.h"

class MeshComponent : public Component
{
private:
    class Mesh* mMesh;
    size_t mTextureIndex;
    bool mIsSkeletal;

public:
    MeshComponent(class Actor* owner, bool isSkeletal = false);
    ~MeshComponent();

    virtual void draw(class Shader* shader);

    // setters
    virtual void setMesh(class Mesh* mesh);
    void setTextureIndex(size_t index);

    // getters
    bool getIsSkeletal() const;
    class Mesh* getMesh() const;
    size_t getTextureID() const;

    EComponentType getType() const override;
    void loadProperties(const rapidjson::Value& inObj) override;
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props) override;
};
