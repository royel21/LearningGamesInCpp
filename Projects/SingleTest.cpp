
#include <Core/Window.h>

#include <Graphics/Shader.h>
#include <Graphics/Graphic.h>

#include <Time/Timer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Assets/Assets.h>

#include <Core/Project.h>
#include <ECS/Scene.h>
#include <ECS/Components.h>
#include <Serialize/SceneLoader.h>

#include <vector>

#include <Input/Input.h>


using namespace Plutus;

struct Point {
    float x;
    float y;
    float index;
    int texIndex;
    Point(float _x, float _y, float i, int tIndex) : x(_x), y(_y), index(i), texIndex(tIndex) {}
};

int main(int argc, char** argv)
{
    //Create a window
    Window window;
    window.init("Plutus App", 1280, 768);

    //Shader Program;
    Shader shader;
    if (!shader.init("Projects/ver.glsl", "Projects/frag.glsl", "Projects/geoquad.glsl")) {
        window.close();
    }

    // Point points[] = {
    //   {128.0f, 128.0f, 0.0f},
    //   {198.0f, 128.0f, 1.0f},
    //   {264.0f, 128.6f, 2.f},
    //   {328.0f, 384.0f, 3.0f}
    // };

    auto vertArray = Graphic::createVertexArray();
    auto bufferArray = Graphic::createVertexBuffer();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, index));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_TRUE, sizeof(Point), (void*)offsetof(Point, texIndex));


    glm::vec2 sze = { 1280,768 };
    auto projection = glm::ortho(0.0f, sze.x, 0.0f, sze.y);


    // Texture tex("assets/textures/Player1.png", 0, 0, GL_LINEAR);

    float scale = 1.0f;
    glm::vec2 pos = { 0,0 };

    glPointSize(40.0f);

    Project project;

    project.load("ZombiesGame/ZombiesGame.json");
    project.loadScene(project.currentScene);

    auto tilemap = project.scene->getEntityByName("floor");

    if (!tilemap) {
        shader.destroy();
        if (vertArray)
            glDeleteVertexArrays(1, &vertArray);

        if (bufferArray)
            glDeleteBuffers(1, &bufferArray);

        window.close();
        return 0;
    }

    auto mapCom = tilemap.getComponent<TileMapComponent>();

    auto tex = AssetManager::get()->getAsset<Texture>("desert_spacing.png");
    // auto tex2 = AssetManager::get()->getAsset<Texture>("cave.png");

    std::vector<Point> points;
    points.reserve(mapCom->mTiles.size());

    for (auto& tile : mapCom->mTiles) {
        points.emplace_back((float)tile.x, (float)tile.y, (float)tile.texcoord, tile.texture);
    }

    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Point), points.data(), GL_STATIC_DRAW);

    float speed = 5;
    auto mInput = Input::get();

    while (window.isFinish())
    {
        if (mInput->onKeyDown("Right"))
        {
            pos.x -= speed;
        }

        if (mInput->onKeyDown("Left"))
        {
            pos.x += speed;
        }

        if (mInput->onKeyDown("Up"))
        {
            pos.y -= speed;
        }

        if (mInput->onKeyDown("Down"))
        {
            pos.y += speed;
        }

        glm::mat4 model = glm::translate(glm::mat4(1.0f), { pos, 0.0f });
        // model = glm::translate(model, glm::vec3(0.5f * sze.x, 0.5f * sze.y, 0.0f));
        // model = glm::rotate(model, Timer::seconds(), glm::vec3(0.0f, 0.0f, 1.0f));
        // model = glm::translate(model, glm::vec3(-0.5f * sze.x, -0.5f * sze.y, 0.0f));

        shader.enable();
        shader.setUniform4f("uColor", { 1.0f,0.0f,0.0f, 1.0f });
        shader.setUniformMat4("uCamera", projection * model);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glActiveTexture(GL_TEXTURE0 + tex->mTexureUnit);
        glBindTexture(GL_TEXTURE_2D, tex->mTexId);


        shader.setUniform1i("hasTexture", 1);
        shader.setUniform1f("tw", 64);
        shader.setUniform1f("th", 64);
        if (tex->mTileWidth) {
            shader.setUniform1f("uvWidth", (float)tex->mTileWidth / (float)tex->mWidth);
            shader.setUniform1f("uvHeight", (float)tex->mTileHeight / (float)tex->mHeight);
            shader.setUniform1i("uColumns", tex->mWidth / tex->mTileWidth);
        }
        shader.setUniform4f("uColor", { 1.0f,1.0f,1.0f, 1.0f });
        shader.setUniformMat4("uCamera", projection * model);
        glDrawArrays(GL_POINTS, 0, points.size());

        shader.disable();

        window.update();
        // printf("time: %.3f\n", Timer::seconds());
    }

    shader.destroy();

    if (vertArray)
        glDeleteVertexArrays(1, &vertArray);

    if (bufferArray)
        glDeleteBuffers(1, &bufferArray);

    return 0;
}