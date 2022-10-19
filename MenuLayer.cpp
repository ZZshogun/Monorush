#include "Layer.h"

void OnButtonHover() {
	auto button = UI::on_button;
	button->textScale = 1.5f;
	button->text = "- " + button->text + " -";
}

void MenuLayer::OnAttach() {
	Layer::SetClearColor({ 0.95f, 0.97f, 1, 1 });

	UI::StartUI(glm::ivec2{ 1920, 1080 });

	UI::Anchor(CENTER);
	UI::CreateButton(
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

	UI::CreateButton(
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

	UI::CreateButton(
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
	Layer::OnUpdate(time);

	UI::StartUI(glm::ivec2{ 1920, 1080 });

	UI::Anchor(CENTER);
	UI::DrawString("== THE GAME ==", {0, 400}, 2, Color::Black);
	UI::DrawString("VERSION 0.1A", { -880, -520 }, 0.45f, Color::Black, "Arial");

	UI::EndUI();
}