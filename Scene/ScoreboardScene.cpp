#include <functional>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdio.h>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "StartScene.h"
#include "ScoreboardScene.hpp"

using std::ifstream;
using std::pair;
using std::string;
using std::vector;

bool comparator(scoreboardData s1, scoreboardData s2)
{
    if (s1.score > s2.score)
        return true;
    else if (s1.score < s2.score)
        return false;
    else
    {
        return s1.name.compare(s2.name) < 0;
    }
}

vector<scoreboardData> readScoreboardFile()
{
    string fileName = string("Resource/scoreboard.txt");
    ifstream fin(fileName);

    vector<scoreboardData> list;
    string name;
    int score;
    string date;
    while (fin >> name >> score >> date)
        list.push_back(scoreboardData{name, score, date});

    std::sort(list.begin(), list.end(), comparator);
    fin.close();

    return list;
}

void ScoreboardScene::Initialize()
{
    page = 0;
    redraw = true;
    showOnScreenMax = 10;

    nameList = readScoreboardFile();
    numOfPages = nameList.size() / showOnScreenMax + 1;

    ticks = 0;
    w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    halfW = w / 2;
    halfH = h / 2;

    bgmInstance = AudioHelper::PlaySample("scoreboard_bgm.mp3", true, AudioHelper::BGMVolume);
}

void ScoreboardScene::Terminate()
{
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void ScoreboardScene::StartOnClick()
{
    StartScene *scene = dynamic_cast<StartScene *>(Engine::GameEngine::GetInstance().GetScene("play"));
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

string ScoreboardScene::reformatDate(string date)
{
    string formatted = date.substr(4, 3) + " ";
    formatted += date.substr(7, 2) + ", ";
    formatted += date.substr(0, 4);
    return formatted;
}

void ScoreboardScene::Update(float deltaTime)
{
    if (redraw)
    {
        printf("draw page %d\n", page);
        Clear();
        //* Page Title
        AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 32, halfW, 100, 255, 255, 30, 255, 0.5, 0.5));

        //* Scoreboard List
        for (int i = page * showOnScreenMax; i < nameList.size() && i < (page + 1) * showOnScreenMax; i++)
        {
            scoreboardData curr = nameList[i];
            AddNewObject(new Engine::Label(curr.name, "pirulen.ttf", 32, halfW - 100, 150 + 42 * ((i - page * showOnScreenMax) + 1), 255, 120, 50, 255, 1, 0.5));
            AddNewObject(new Engine::Label(std::to_string(curr.score), "pirulen.ttf", 32, halfW, 150 + 42 * ((i - page * showOnScreenMax) + 1), 255, 255, 255, 255, 0.5, 0.5));
            AddNewObject(new Engine::Label(reformatDate(curr.date), "pirulen.ttf", 32, halfW + 100, 150 + 42 * ((i - page * showOnScreenMax) + 1), 50, 255, 200, 255, 0.0, 0.5));
        }

        //* Back Button
        Engine::ImageButton *btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h - 200, 400, 100, 0.0, 0.0);
        btn->SetOnClickCallback(std::bind(&ScoreboardScene::StartOnClick, this));
        AddNewControlObject(btn);
        AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, h - 150, 255, 255, 30, 255, 0.5, 0.5));

        //* Prev Page Button
        if (page > 0)
        {
            btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 200, h - 200, 100, 100, 0.0, 0.0);
            btn->SetOnClickCallback(std::bind(&ScoreboardScene::OnPrevPage, this));
            AddNewControlObject(btn);
            AddNewObject(new Engine::Label("<", "pirulen.ttf", 48, 230, h - 150, 255, 255, 30, 255, 0.0, 0.5));
        }

        //* Next Page Button
        if (page < numOfPages - 1)
        {
            btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w - 230, h - 200, 100, 100, 1.0, 0.0);
            btn->SetOnClickCallback(std::bind(&ScoreboardScene::OnNextPage, this));
            AddNewControlObject(btn);
            AddNewObject(new Engine::Label(">", "pirulen.ttf", 48, w - 230, h - 150, 255, 255, 30, 255, 1.0, 0.5));
        }

        Draw();

        redraw = false;
    }
}

void ScoreboardScene::OnPrevPage()
{
    page--;
    redraw = true;
}

void ScoreboardScene::OnNextPage()
{
    printf("Go to next page\n");
    page++;
    redraw = true;
}
