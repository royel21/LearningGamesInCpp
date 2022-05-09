
#include <Core/Window.h>

#include <Graphics/Shader.h>
#include <Graphics/Graphic.h>

#include <Time/Timer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Assets/Texture.h>

using namespace Plutus;

struct Point {
    float x;
    float y;
    float index;
    Point(float _x, float _y, float i) : x(_x), y(_y), index(i) {}
};

int main(int argc, char** argv)
{
    //Create a window
    Window window;
    window.init("Plutus App", 768, 768);

    //Shader Program;
    Shader shader;
    if (!shader.init("Projects/ver.glsl", "Projects/frag.glsl", "Projects/geoquad.glsl")) {
        window.close();
    }

    Point points[] = {
      {128.0f, 128.0f, 0.0f},
      {198.0f, 128.0f, 1.0f},
      {264.0f, 128.6f, 2.f},
      {328.0f, 384.0f, 3.0f}
    };

    auto vertArray = Graphic::createVertexArray();
    auto bufferArray = Graphic::createVertexBuffer();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, index));

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glm::vec2 sze = { 512,512 };
    auto project = glm::ortho(0.0f, sze.x, 0.0f, sze.y);


    Texture tex("assets/textures/Player1.png", 0, 0, GL_LINEAR);

    float scale = 1.0f;
    glm::vec2 pos = { 0,0 };

    glPointSize(40.0f);

    while (window.isFinish())
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), { pos, 0.0f });
        model = glm::translate(model, glm::vec3(0.5f * sze.x, 0.5f * sze.y, 0.0f));
        model = glm::rotate(model, Timer::seconds(), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * sze.x, -0.5f * sze.y, 0.0f));

        shader.enable();
        shader.setUniform4f("uColor", { 1.0f,0.0f,0.0f, 1.0f });
        shader.setUniformMat4("uCamera", project * model);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindTexture(GL_TEXTURE_2D, tex.mTexId);

        shader.setUniform1i("hasTexture", 1);
        shader.setUniform1f("tw", 64);
        shader.setUniform1f("th", 64);
        if (tex.mTileWidth) {
            shader.setUniform1f("uvWidth", (float)tex.mTileWidth / (float)tex.mWidth);
            shader.setUniform1f("uvHeight", (float)tex.mTileHeight / (float)tex.mHeight);
            shader.setUniform1i("uColumns", tex.mWidth / tex.mTileWidth);
        }
        shader.setUniform4f("uColor", { 1.0f,1.0f,1.0f, 1.0f });
        shader.setUniformMat4("uCamera", project * glm::mat4(1.0f));
        glDrawArrays(GL_POINTS, 0, 4);

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