#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include "Engine/IScene.hpp"
#include "UI/Component/TextInput.hpp"

class WinScene final : public Engine::IScene
{
private:
	float ticks;
	ALLEGRO_SAMPLE_ID bgmId;
	Engine::TextInput *nameInput;

	void SaveToScoreboard();

public:
	explicit WinScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void BackOnClick(int stage);
};

#endif // WINSCENE_HPP
