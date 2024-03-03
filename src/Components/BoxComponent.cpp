#include "../../include/Components/BoxComponent.h"

#include "../../include/Components/MeshComponent.h"

#include "../../include/Actors/Actor.h"

#include "../../include/Renderers/Collision.h"
#include "../../include/Renderers/PhysWorld.h"
#include "../../include/Renderers/Mesh.h"
#include "../../include/Renderers/Renderer.h"

#include "../../include/General/LevelLoader.h"

#include "../../include/Game.h"

BoxComponent::BoxComponent(class Actor* owner)
: Component(owner), mObjectBox(Vector3::Zero, Vector3::Zero),
mWorldBox(Vector3::Zero, Vector3::Zero)
{
    this->mShouldRotate = true;
    owner->getGame()->getPhysWorld()->addBoxComponent(this);
}

BoxComponent::~BoxComponent()
{
    this->getActor()->getGame()->getPhysWorld()->removeBoxComponent(this);
}

void BoxComponent::onUpdateWorldTransform()
{
    this->mWorldBox = this->mObjectBox;

    if (this->getActor()->getType() == 11)
    {
        auto dist = this->mWorldBox.mMax - this->mWorldBox.mMin;
        SDL_Log("[BoxComponent] Before update Box size: w: %.2f h: %.2f l: %.2f", dist.x, dist.y, dist.z);
    }

    auto scale = this->getActor()->getScale();
    auto quat = this->getActor()->getRotation();
    auto trans = this->getActor()->getPosition();
    
    // 缩放
    this->mWorldBox.mMin *= scale;
    this->mWorldBox.mMax *= scale;

    if (this->getActor()->getType() == 11)
    {
        auto dist = this->mWorldBox.mMax - this->mWorldBox.mMin;
        SDL_Log("[BoxComponent] Update Box size: w: %.2f h: %.2f l: %.2f", dist.x, dist.y, dist.z);
    }

    // 旋转
    if (this->mShouldRotate)
    {
        this->mWorldBox.rotate(quat);
    }

    // 平移
    this->mWorldBox.mMin += trans;
    this->mWorldBox.mMax += trans;

}

AABB& BoxComponent::getObjectBox()
{
    return this->mObjectBox;
}

AABB& BoxComponent::getWorldBox()
{
    return this->mWorldBox;
}

void BoxComponent::setWorldBox(const AABB& box)
{
    this->mWorldBox = box;
}

void BoxComponent::setObjectBox(const AABB& box)
{
    this->mObjectBox = box;
}

void BoxComponent::setShouldRotate(bool shouldRotate)
{
    this->mShouldRotate = shouldRotate;
}

Component::EComponentType BoxComponent::getType() const
{
    return EBoxComponent;
}

void BoxComponent::loadProperties(const rapidjson::Value& inObj)
{
    Component::loadProperties(inObj);

    auto act = this->getActor();
    if (act->reUpdateColliBox())
    {
        SDL_Log("[BoxComponent] Update colli box...");

        auto comps = act->getGame()->getRenderer()->getMeshComponents();
        // for (auto cp : comps)
        // {
        //     if (cp->getActor() == act)
        //     {
        //         this->setObjectBox(*(cp->getMesh()->getBox()));
        //         break;
        //     }
        // }

        MeshComponent* mc = nullptr;
        for (int i = comps.size() - 1; i >= 0; i--)
        {
            if (comps[i]->getActor() == act)
            {
                auto box =*(comps[i]->getMesh()->getBox());
                this->setObjectBox(box);
                this->onUpdateWorldTransform();
                
                mc = comps[i];

                auto dist = box.mMax - box.mMin;

                SDL_Log("[BoxComponent] Box size: w: %.2f h: %.2f l: %.2f", dist.x, dist.y, dist.z);
                
                break;
            }
        }

        if (mc)
        {
            MeshComponent* temp = nullptr;
            for (auto it = comps.begin(); it != comps.end(); it++)
            {
                if ((*it)->getActor() == act && (*it) != mc)
                {
                    temp = (*it);
                    it = comps.erase(it);
                    break;
                }
            }

            if (temp)
            {
                delete  temp;
                temp = nullptr;
            }
        }

        act->updateColliBox(false);
    }
}

void BoxComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& props)
{
    Component::saveProperties(alloc, props);
}