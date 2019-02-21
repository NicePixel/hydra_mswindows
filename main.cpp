#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <random>
#include <chrono>
#include <mutex>
#include <Windows.h>
#include "sound.h"
#include "red.h"
#include "blue.h"
#include "yellow.h"

static std::mt19937 rng;
static std::uniform_int_distribution<int> distribution(0, 2);

void live(sf::Sprite* heads, int colour)
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(320, 320), "...", sf::Style::Close);

	sf::SoundBuffer bufferoar;
	sf::Sound sound;
	bufferoar.loadFromMemory(hydra_sound, sizeof(hydra_sound));
	sound.setBuffer(bufferoar);
	sound.play();

	std::uniform_int_distribution<int> distribution_w(0, vm.width - 320);
	std::uniform_int_distribution<int> distribution_h(0, vm.height - 320);
	sf::Vector2i position(distribution_w(rng), distribution_h(rng));
	window.setPosition(position);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::F8)
				{
					exit(1);
				}
			}
		}

		window.clear();
		window.draw(heads[colour]);
		window.display();
	}

	std::thread th0(live, heads, distribution(rng));
	std::thread th1(live, heads, distribution(rng));
	th0.join();
	th1.join();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	sf::Texture headtextures[3];
	sf::Sprite heads[3];
	rng.seed(std::random_device()());
	headtextures[0].loadFromMemory(hydra_blue,   sizeof(hydra_blue));
	headtextures[1].loadFromMemory(hydra_red,    sizeof(hydra_red));
	headtextures[2].loadFromMemory(hydra_yellow, sizeof(hydra_yellow));
	heads[0].setTexture(headtextures[0]);
	heads[1].setTexture(headtextures[1]);
	heads[2].setTexture(headtextures[2]);

	std::thread th0(live, heads, distribution(rng));
	th0.join();
	
	return 0;
}
