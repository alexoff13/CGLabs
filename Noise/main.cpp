#include <cmath>
#include <random>
#include <SFML/Graphics.hpp>

int main()
{
    int w = 800;
    int h = 800;
    int mouseX = w / 2;
    int mouseY = h / 2;
    float mouseSensitivity = 5.0f;
    float speed = 0.1f;
    bool wasdUD[6] = { false, false, false, false, false, false };
    sf::Vector3f pos = sf::Vector3f(3.0f, 15.0f, -1.0f);
    sf::Clock clock;
    int framesStill = 1;

    sf::RenderWindow window(sf::VideoMode(w, h), "Noise", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(144);

    sf::RenderTexture firstTexture;
    firstTexture.create(w, h);
    sf::Sprite firstTextureSprite = sf::Sprite(firstTexture.getTexture());
    sf::Sprite firstTextureSpriteFlipped = sf::Sprite(firstTexture.getTexture());
    firstTextureSpriteFlipped.setScale(1, -1);
    firstTextureSpriteFlipped.setPosition(0, h);

    sf::RenderTexture outputTexture;
    outputTexture.create(w, h);
    sf::Sprite outputTextureSprite = sf::Sprite(outputTexture.getTexture());
    sf::Sprite outputTextureSpriteFlipped = sf::Sprite(firstTexture.getTexture());
    outputTextureSpriteFlipped.setScale(1, -1);
    outputTextureSpriteFlipped.setPosition(0, h);

    sf::Shader shader;

    shader.loadFromFile("../noise.frag", sf::Shader::Fragment);
    shader.setUniform("iResolution", sf::Vector2f(w, h));

    std::random_device rd;
    std::uniform_real_distribution<> dist(0.0f, 1.0f);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        float mx = ((float) mouseX / w - 0.5f) * mouseSensitivity;
        float my = ((float) mouseY / h - 0.5f) * mouseSensitivity;
        sf::Vector3f dir = sf::Vector3f(0.0f, 0.0f, 0.0f);
        sf::Vector3f dirTemp;
        if (wasdUD[0]) dir = sf::Vector3f(1.0f, 0.0f, 0.0f);
        else if (wasdUD[2]) dir = sf::Vector3f(-1.0f, 0.0f, 0.0f);
        if (wasdUD[1]) dir += sf::Vector3f(0.0f, -1.0f, 0.0f);
        else if (wasdUD[3]) dir += sf::Vector3f(0.0f, 1.0f, 0.0f);
        dirTemp.z = dir.z * std::cos(-my) - dir.x * std::sin(-my);
        dirTemp.x = dir.z * std::sin(-my) + dir.x * std::cos(-my);
        dirTemp.y = dir.y;
        dir.x = dirTemp.x * std::cos(mx) - dirTemp.y * std::sin(mx);
        dir.y = dirTemp.x * std::sin(mx) + dirTemp.y * std::cos(mx);
        dir.z = dirTemp.z;
        pos += dir * speed;

        if (wasdUD[4]) pos.z -= speed;
        else if (wasdUD[5]) pos.z += speed;
        for (bool i : wasdUD) {
            if (i) {
                framesStill = 1;
                break;
            }
        }

        if (framesStill % 2 == 1)
        {
            shader.setUniform("u_sample", firstTexture.getTexture());
            outputTexture.draw(firstTextureSpriteFlipped, &shader);
            window.draw(outputTextureSprite);
        }
        else
        {
            shader.setUniform("u_sample", outputTexture.getTexture());
            firstTexture.draw(outputTextureSpriteFlipped, &shader);
            window.draw(firstTextureSprite);
        }
        window.display();
        framesStill++;
        shader.setUniform("iTime",clock.getElapsedTime().asSeconds());
    }
    return 0;
}