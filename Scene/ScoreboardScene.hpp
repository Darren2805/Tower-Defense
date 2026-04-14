#ifndef SCOREBOARDSCENE_HPP
#define SCOREBOARDSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include <vector>
#include <string>

using std::pair;
using std::string;
using std::vector;

struct scoreboardData
{
    string name;
    int score;
    string date;
};

class ScoreboardScene final : public Engine::IScene
{
private:
    int w, h, halfW, halfH;
    int page;
    int numOfPages;
    vector<scoreboardData> nameList;

    float ticks;
    int showOnScreenMax;
    bool redraw;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

    string reformatDate(string date);

public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void StartOnClick();
    void OnNextPage();
    void OnPrevPage();
};

#endif