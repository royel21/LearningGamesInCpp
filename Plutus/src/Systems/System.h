namespace Plutus
{
    class Scene;
    class Camera2D;

    class ISystem
    {
    protected:
        Scene *mScene;

    public:
        ISystem(Scene *scene) : mScene(scene){};
        virtual void update(float dt) = 0;
        virtual void draw(Camera2D *camera) = 0;
    };

} // namespace Plutus
