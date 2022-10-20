#include "Layer.h"

void OnButtonHover() {
	auto& button = UI::on_button;
	button->textScale = 1.5f;
	button->text = "- " + button->text + " -";
}

MenuLayer::MenuLayer() {
	scene = Scene::Create();
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>().primary = true;
}

MenuLayer::~MenuLayer() {
	UI::ClearBuffers();
	Texture::ClearHandles();
	Audio::ClearBuffers();
	scene->Destroy();
}

void MenuLayer::OnAttach() {
	Layer::SetClearColor({ 0.95f, 0.97f, 1, 1 });

	UI::StartUI(glm::ivec2{ 1920, 1080 });

	UI::Anchor(CENTER);
	playButton = UI::CreateButton(
		"START",
		1.25f,
		Color::Black,
		{ 0, -150 },
		{ 230, 70 },
		{0, 1, 0, 0},
		[&]() { state.sceneAddition = 1; },
		Color::Transparent,
		OnButtonHover
	);

	settingButton = UI::CreateButton(
		"SETTING",
		1.25f,
		Color::Black,
		{ 0, -270 },
		{ 270, 70 },
		{ 0, 1, 0, 0 },
		[&]() { std::cout << "Pressed\n"; },
		Color::Transparent,
		OnButtonHover
	);

	quitButton = UI::CreateButton(
		"QUIT",
		1.25f,
		Color::Black,
		{ 0, -390 },
		{ 150, 70 },
		{ 0, 1, 0, 0 },
		[&]() { state.terminate = true; },
		Color::Transparent,
		OnButtonHover
	);

	UI::EndUI();
}

void MenuLayer::OnUpdate(Time time) {
	scene->OnUpdate(time);

	UI::StartUI(glm::ivec2{ 1920, 1080 });

	UI::Anchor(CENTER);
	UI::DrawString("== THE GAME ==", {0, 400}, 2, Color::Black);
	UI::DrawString("VERSION 0.1A", { -880, -520 }, 0.45f, Color::Black, "Arial");

	UI::Anchor(CENTER);
	if (playButton) {
		UI::DrawButton(playButton, time);
		UI::DrawButton(settingButton, time);
		UI::DrawButton(quitButton, time);
	}

	UI::EndUI();
}