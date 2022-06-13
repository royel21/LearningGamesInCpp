#include "Scene.h"
#include "Components.h"

namespace Plutus
{
    Entity::~Entity() {
        mId = entt::null;
        mScene = nullptr;
    }

    Vec2f Entity::getPosition() {
        auto trans = getComponent<TransformComponent>();
        return trans ? trans->getPosition() : Vec2f{};
    }

    Vec2f Entity::getCenter() {
        auto trans = getComponent<TransformComponent>();
        return trans ? trans->getCenter() : Vec2f{};
    }

    Vec2f Entity::getDirection(Entity& Ent) {
        if (isValid() && Ent) {
            return (getCenter() - Ent.getCenter()).unit();
        }
        return {};
    }

    float Entity::getDistance(Entity& Ent) {
        if (isValid() && Ent) {
            return (getCenter() - Ent.getCenter()).length();
        }
        return 0;
    }

    const std::string Entity::getName()
    {
        return isValid() ? mScene->mRegistry.get<TagComponent>(mId).Name : "";
    }

    bool Entity::isValid() const {
        return mScene != nullptr && mScene->mRegistry.valid(mId);
    }

    void Entity::setName(const std::string& name)
    {
        if (isValid())
            mScene->mRegistry.get<TagComponent>(mId).Name = name;
    }
    /*******************************     Scene    ************************/

    Entity Scene::createEntity(const std::string& name, bool visible)
    {
        auto ent = Entity{ mRegistry.create(), this };
        auto tag = ent.addComponent<TagComponent>(name);
        tag->Visible = visible;
        return { ent.mId, this };
    }
    // Get Entity by entt::id = int
    Entity Scene::getEntity(int Id)
    {
        auto ent = entt::entity(Id);
        return { mRegistry.valid(ent) ? ent : entt::null , this };
    }

    // Get Entity by name using std::string as key to find the entity
    Entity Scene::getEntityByName(const std::string& name)
    {
        entt::entity ent = entt::null;
        auto view = mRegistry.view<const TagComponent>();
        for (const auto& [e, tag] : view.each()) {
            if (tag.Name.compare(name) == 0) {
                ent = e;
                break;
            }
        }

        return { ent, this };
    }

    TransformComponent* Scene::getTransform(entt::entity id) {
        if (mRegistry.valid(id) && mRegistry.any_of<TransformComponent>(id)) {
            return  &(mRegistry.get<TransformComponent>(id));
        }

        return nullptr;
    }

    void Scene::copyScene(Scene* scene) {

        mRegistry.clear();
        scene->getRegistry()->each([&](auto& e) {
            Entity ent(e, scene);
            Entity newEnt = createEntity(ent.getName());
            CopyEntity(ent, newEnt);
            });
    }


    Entity Scene::CreateCopy(Entity& ent) {
        Entity newEnt = createEntity(ent.getName() + " copy");
        CopyEntity(ent, newEnt);
        return newEnt;
    }

    void CopyEntity(Entity& source, Entity& dest) {

        CopyComponent<TransformComponent>(source, dest);
        CopyComponent<SpriteComponent>(source, dest);
        CopyComponent<AnimationComponent>(source, dest);
        CopyComponent<TileMapComponent>(source, dest);
        CopyComponent<ScriptComponent>(source, dest);
        CopyComponent<RigidBodyComponent>(source, dest);
        CopyComponent<PhysicBodyComponent>(source, dest);
    }

} // namespace Plutus
