#include <functional>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "WinScene.hpp"
#include "UI/Component/TextInput.hpp"
#include "Shared/shared.hpp"

using std::ofstream;

void WinScene::Initialize()
{
	ticks = 0;
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;

	AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));

	Engine::ImageButton *btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);

	AddNewObject(new Engine::Label("Enter your name", "pirulen.ttf", 28, 100, 100, 255, 255, 0, 255, 0, 0));

	nameInput = new Engine::TextInput("pirulen.ttf", 28, 100, 150, 400, 50, 2, 3, 4, 255, 0, 0);
	AddNewControlObject(nameInput);

	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

	bgmId = AudioHelper::PlayAudio("win.wav");
}

std::string getDate()
{
	auto now = std::chrono::system_clock::now();
	time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	tm now_tm;
	localtime_s(&now_tm, &now_time_t);

	// Format the time into a string
	std::ostringstream time_stream;
	time_stream << std::put_time(&now_tm, "%Y%b%d");
	return time_stream.str();
}

void WinScene::SaveToScoreboard()
{
	if (nameInput->Text != "")
	{
		// getDate();

		std::cout << "Append to file\n";
		ofstream buildFout, diskFout;

		diskFout.open("../Resource/scoreboard.txt", std::ios_base::app);
		buildFout.open("Resource/scoreboard.txt", std::ios_base::app);

		diskFout << nameInput->Text << " " << gameScore << " " << getDate() << std::endl;
		buildFout << nameInput->Text << " " << gameScore << " " << getDate() << std::endl;
		diskFout.close();
		buildFout.close();
	}
}

void WinScene::Terminate()
{
	SaveToScoreboard();
	IScene::Terminate();
	AudioHelper::StopBGM(bgmId);
}

void WinScene::Update(float deltaTime)
{
	ticks += deltaTime;
	if (ticks > 4 && ticks < 100 &&
		dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2)
	{
		ticks = 100;
		bgmId = AudioHelper::PlayBGM("happy.ogg");
	}
}
void WinScene::BackOnClick(int stage)
{
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
