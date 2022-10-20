#include "Layer.h"

void OnButtonHover() {
	auto& button = UI::on_button;
	button->textScale += 0.35f;
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
		{ 0, 1, 0, 1 },
		[&]() { state.sceneAddition = 1; },
		Color::Transparent,
		OnButtonHover
	);

	settingButton = UI::CreateButton(
		"SET.TING",
		1.25f,
		Color::Black,
		{ 0, -270 },
		{ 270, 70 },
		{ 0, 1, 0, 1 },
		[&]() { in_setting = true; },
		Color::Transparent,
		OnButtonHover
	);

	quitButton = UI::CreateButton(
		"QUIT",
		1.25f,
		Color::Black,
		{ 0, -390 },
		{ 150, 70 },
		{ 0, 1, 0, 1 },
		[&]() { state.terminate = true; },
		Color::Transparent,
		OnButtonHover
	);

	fulLScreenButton = UI::CreateButton(
		"SCREEN : WINDOWED",
		2,
		Color::Black,
		{ 0, -200 },
		{ 250, 100 },
		{ 0, 1, 0, 1 },
		[&]() {},
		Color::Transparent,
		OnButtonHover
	);

	backButton = UI::CreateButton(
		"BACK",
		0.75f,
		Color::White,
		{ -850, 480 },
		{ 160, 75 },
		Color::Black,
		[&]() { in_setting = false; },
		Color::Black,
		OnButtonHover
	);

	UI::EndUI();
}

void MenuLayer::OnUpdate(Time time) {
	scene->OnUpdate(time);

	UI::StartUI(glm::ivec2{ 1920, 1080 });

	if (in_setting) {
		UI::Anchor(CENTER);
		UI::DrawString("== SETTING ==", { 0, 400 }, 1.5f, Color::Black);

		UI::Anchor(CENTER);
		if (backButton) {
			UI::DrawButton(backButton, time);
		}
	}
	else {
		UI::Anchor(CENTER);
		UI::DrawString("== THE GAME ==", { 0, 400 }, 2, Color::Black);
		UI::Anchor(LEFT);
		UI::DrawString("VERSION 0.1A", { -950, -525 }, 0.45f, Color::Black, "Arial");
		UI::Anchor(RIGHT);
		UI::DrawString("MADE BY RACHATA K.", { 950, -525 }, 0.45f, Color::Black, "Arial");

		UI::Anchor(CENTER);
		if (playButton) {
			UI::DrawButton(playButton, time);
			UI::DrawButton(settingButton, time);
			UI::DrawButton(quitButton, time);
		}
	}

	UI::EndUI();
}