#include <SDL.h>

#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
		#include <vld.h>
	#endif
#endif

#include "../Bomberman/Commands.h"
#include "../Bomberman/EntityMovementComponent.h"
#include "../Bomberman/Main.h"
#include "../Bomberman/Observers.h"
#include "../Bomberman/PathwayCreatorComponent.h"
#include "FPSCounterComponent.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "InputManager.h"
#include "Minigin.h"
#include "ResourceManager.h"
#include "RotatorComponent.h"
#include "SceneManager.h"
#include "TextObjectComponent.h"
#include "TextureComponent.h"
#include "ValuesComponent.h"
#include <iostream>
// #include <steam_api.h>
#include "../Bomberman/BombComponent.h"
#include "../Bomberman/EnemyComponent.h"
#include "../Bomberman/MenuComponent.h"
#include "../Bomberman/PlayerComponent.h"
#include <AudioComponent.h>
#include <FileReader.h>

#include "HighscoreComponent.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using dae::GameObject;
using dae::Input;
using dae::Scene;

std::shared_ptr<dae::Font> font;
std::shared_ptr<dae::Font> font2;
std::shared_ptr<dae::Font> font3;

std::map<MathLib::EEnemyType, MathLib::FEnemyStats> EnemyTypes{
	{ MathLib::EEnemyType::Balloom, MathLib::FEnemyStats{ MathLib::EEnemyType::Balloom, false, 0.65f, 100, "Balloom" } },
	{ MathLib::EEnemyType::Oneal, MathLib::FEnemyStats{ MathLib::EEnemyType::Oneal, true, 0.8f, 200, "Oneal" } },
	{ MathLib::EEnemyType::Doll, MathLib::FEnemyStats{ MathLib::EEnemyType::Doll, false, 0.8f, 300, "Doll" } },
	{ MathLib::EEnemyType::Minvo, MathLib::FEnemyStats{ MathLib::EEnemyType::Minvo, true, 1.f, 400, "Minvo" } },
};

#pragma region Input

void MakePlayerInput(Scene* scene, std::unique_ptr<GameObject>& mainPlayer, Input::InputKey up, Input::InputKey down, Input::InputKey left, Input::InputKey right, Input::InputKey upCancel, Input::InputKey downCancel, Input::InputKey leftCancel, Input::InputKey rightCancel, Input::InputKey spacebar, Input::InputKey detonate, bool isVersus)
{
	Input::GetInstance().BindKey(up, std::make_unique<dae::Move>(scene, mainPlayer.get(), MathLib::EMovement::UP, isVersus ? "Enemies/Balloom" : "Character/moveUp", glm::vec3(0.f, -100.f, 0.0f)));
	Input::GetInstance().BindKey(down, std::make_unique<dae::Move>(scene, mainPlayer.get(), MathLib::EMovement::DOWN, isVersus ? "Enemies/Balloom" : "Character/moveDown", glm::vec3(0.f, 100.f, 0.0f)));
	Input::GetInstance().BindKey(left, std::make_unique<dae::Move>(scene, mainPlayer.get(), MathLib::EMovement::LEFT, isVersus ? "Enemies/Balloom" : "Character/moveLeft", glm::vec3(-100.f, 0.0f, 0.0f)));
	Input::GetInstance().BindKey(right, std::make_unique<dae::Move>(scene, mainPlayer.get(), MathLib::EMovement::RIGHT, isVersus ? "Enemies/Balloom" : "Character/moveRight", glm::vec3(100.f, 0.0f, 0.0f)));

	Input::GetInstance().BindKey(spacebar, std::make_unique<dae::StartBomb>(scene, mainPlayer.get()));
	Input::GetInstance().BindKey(detonate, std::make_unique<dae::Detonate>(scene, mainPlayer.get()));

	Input::GetInstance().BindKey(upCancel, std::make_unique<dae::StopMove>(scene, mainPlayer.get(), MathLib::EMovement::UP));
	Input::GetInstance().BindKey(downCancel, std::make_unique<dae::StopMove>(scene, mainPlayer.get(), MathLib::EMovement::DOWN));
	Input::GetInstance().BindKey(leftCancel, std::make_unique<dae::StopMove>(scene, mainPlayer.get(), MathLib::EMovement::LEFT));
	Input::GetInstance().BindKey(rightCancel, std::make_unique<dae::StopMove>(scene, mainPlayer.get(), MathLib::EMovement::RIGHT));
}

void MapControllerInput(int id, Input::InputKey& up, Input::InputKey& down, Input::InputKey& left, Input::InputKey& right, Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow, Input::InputKey& detonate)
{
	// up = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::JoystickLeft, id);
	up = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, dae::ControllerButton::DpadUp, id);
	down = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, dae::ControllerButton::DpadDown, id);
	left = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, dae::ControllerButton::DpadLeft, id);
	right = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, dae::ControllerButton::DpadRight, id);

	upCancel = std::make_tuple(dae::ButtonStates::BUTTON_UP, dae::ControllerButton::DpadUp, id);
	downCancel = std::make_tuple(dae::ButtonStates::BUTTON_UP, dae::ControllerButton::DpadDown, id);
	leftCancel = std::make_tuple(dae::ButtonStates::BUTTON_UP, dae::ControllerButton::DpadLeft, id);
	rightCancel = std::make_tuple(dae::ButtonStates::BUTTON_UP, dae::ControllerButton::DpadRight, id);

	blow = std::make_tuple(dae::ButtonStates::BUTTON_UP, dae::ControllerButton::ButtonA, id);
	detonate = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, dae::ControllerButton::ButtonB, id);
}
void MapKeyboardInput(int id, Input::InputKey& up, Input::InputKey& down, Input::InputKey& left, Input::InputKey& right, Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow, Input::InputKey& detonate)
{
	up = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, SDLK_w, id);
	down = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, SDLK_s, id);
	left = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, SDLK_a, id);
	right = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, SDLK_d, id);

	upCancel = std::make_tuple(dae::ButtonStates::BUTTON_UP, SDLK_w, id);
	downCancel = std::make_tuple(dae::ButtonStates::BUTTON_UP, SDLK_s, id);
	leftCancel = std::make_tuple(dae::ButtonStates::BUTTON_UP, SDLK_a, id);
	rightCancel = std::make_tuple(dae::ButtonStates::BUTTON_UP, SDLK_d, id);

	blow = std::make_tuple(dae::ButtonStates::BUTTON_UP, SDLK_SPACE, id);
	detonate = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, SDLK_e, id);
}
#pragma endregion

#pragma region Making objects

void MakePlayer(Scene* scene, std::string textureName, int id, dae::PathWay pathway, MathLib::GameMode gm, bool isController)
{

	// Main Player
	auto playerName{ EnumStrings[PlayerGeneral] + std::to_string(id) };
	auto mainPlayer = std::make_unique<GameObject>();
	mainPlayer->SetName(playerName);

	// lives
	auto values{ mainPlayer->AddComponent(std::make_unique<dae::ValuesComponent>(scene)) };
	auto observer{ std::make_shared<dae::ScoreObserver>(scene, id) };
	values->AddObserver(observer);
	values->SetLives(gm == MathLib::GameMode::VERSUS? 1 : 3);
	auto observer2{ std::make_shared<dae::HealthObserver>(scene, id) };
	values->AddObserver(observer2);

	// Texture
	mainPlayer->AddComponent(std::make_unique<dae::TextureComponent>());
	auto tex{ mainPlayer->GetComponent<dae::TextureComponent>() };
	tex->SetName(playerName);
	tex->SetTexture(textureName, 0.1f, 3);

	glm::vec2 startPos = { pathway.Rect.x, pathway.Rect.y };
	mainPlayer->AddComponent(std::make_unique<dae::AudioComponent>());
	startPos.y -= 2.5f;
	mainPlayer->AddComponent(std::make_unique<dae::EntityMovementComponent>(scene, startPos));
	mainPlayer->GetTransform()->Translate(startPos);
	auto playerComp{ mainPlayer->AddComponent(std::make_unique<dae::PlayerComponent>(scene, startPos)) };
	auto observer3{ std::make_shared<dae::DoorObserver>(scene) };
	playerComp->AddObserver(observer3);

	mainPlayer->GetComponent<dae::EntityMovementComponent>()->SetIsController(isController);
	mainPlayer->AddComponent(std::make_unique<dae::InputComponent>(scene, mainPlayer->GetTransform()->GetWorld().Position, false, gm != MathLib::GameMode::VERSUS));

	Input::InputKey up, down, left, right;
	Input::InputKey upCancel, downCancel, leftCancel, rightCancel;
	Input::InputKey spacebar, detonate;

	if (gm == MathLib::GameMode::SOLO)
	{
		MapControllerInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, spacebar, detonate);
		MakePlayerInput(scene, mainPlayer, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, spacebar, detonate, false);

		MapKeyboardInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, spacebar, detonate);
	}
	else
	{
		if (isController)
		{
			MapControllerInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, spacebar, detonate);
		}
		else
		{

			MapKeyboardInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, spacebar, detonate);
		}
	}
	MakePlayerInput(scene, mainPlayer, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, spacebar, detonate, false);

	scene->Add(std::move(mainPlayer));
}

void MakeMainChar(dae::Scene* scene, dae::PathWay startPos, MathLib::GameMode gm, bool isController = false) { MakePlayer(scene, "Character/moveRight.png", 0, startPos, gm, isController); }

void MakeSecondaryChar(Scene* scene, dae::PathWay startPos, MathLib::GameMode gm, bool isController = false) { MakePlayer(scene, "Character/moveRight.png", 1, startPos, gm, isController); }

void MakeVersusChar(Scene* scene, dae::PathWay startPos, MathLib::GameMode gm, bool isController = false) { MakePlayer(scene, "Enemies/Balloom.png", 1, startPos, gm, isController); }

void MakeEnemy(Scene* scene, int id, dae::PathWay startPos, MathLib::FEnemyStats enemyStats, bool isVersus = false)
{
	// Enemy
	auto enemy = std::make_unique<GameObject>();
	enemy->SetName(EnumStrings[Names::EnemyGeneral] + std::to_string(id));

	// Texture
	enemy->AddComponent(std::make_unique<dae::TextureComponent>());
	enemy->AddComponent(std::make_unique<dae::AudioComponent>());

	auto		tex{ enemy->GetComponent<dae::TextureComponent>() };
	std::string textureName{ "Enemies/" + enemyStats.Name + ".png" };
	tex->SetTexture(textureName, 0.1f, 4);

	enemy->AddComponent(std::make_unique<dae::EnemyComponent>(scene, enemyStats, isVersus));
	enemy->GetTransform()->Translate({ startPos.Rect.x, startPos.Rect.y });

	auto comp{ enemy->AddComponent(std::make_unique<dae::EntityMovementComponent>(scene, startPos.Middle, !isVersus)) };
	comp->SetEnemyName(enemyStats.Name);

	comp->SetCurrentTileId(startPos.id);
	const auto& pathwayComp{ scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<dae::PathwayCreatorComponent>() };
	comp->CheckMovement(pathwayComp->GetPathways());

	if (isVersus)
	{
		enemy->AddComponent(std::make_unique<dae::InputComponent>(scene, enemy->GetTransform()->GetWorld().Position, true, false));
		Input::InputKey up, down, left, right;
		Input::InputKey upCancel, downCancel, leftCancel, rightCancel;
		Input::InputKey blow, detonate;
		MapControllerInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow, detonate);
		MakePlayerInput(scene, enemy, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow, detonate, isVersus);
	}

	scene->Add(std::move(enemy));
}

void MakeBalloomEnemy(Scene* scene, dae::PathWay startPos, bool isVersus = false) { MakeEnemy(scene, 1, startPos, EnemyTypes[MathLib::EEnemyType::Balloom], isVersus); }

void MakeOnealEnemy(Scene* scene, dae::PathWay startPos) { MakeEnemy(scene, 1, startPos, EnemyTypes[MathLib::EEnemyType::Oneal], false); }

void MakeDollEnemy(Scene* scene, dae::PathWay startPos) { MakeEnemy(scene, 1, startPos, EnemyTypes[MathLib::EEnemyType::Doll], false); }

void MakeMinvoEnemy(Scene* scene, dae::PathWay startPos) { MakeEnemy(scene, 1, startPos, EnemyTypes[MathLib::EEnemyType::Minvo], false); }

void MakeLevel(Scene* scene, int levelIndex, std::string sceneName)
{
	auto pathObject = std::make_unique<GameObject>();
	pathObject->SetName(EnumStrings[PathCreator]);
	auto pathCreator{ pathObject->AddComponent(std::make_unique<dae::PathwayCreatorComponent>(scene)) };

	dae::FileReader* file{ new dae::FileReader("../Data/game.json") };
	auto			 levels = file->GetDocument()["Levels"].GetObj();
	auto			 level = levels[sceneName.c_str()].GetArray();
	auto			 selectedLevel = level[levelIndex].GetObj()["tiles"].GetArray();

	int		  index{ 0 };
	int		  startX{ 0 }, startY{ static_cast<int>(Cellsize * 3) };
	glm::vec2 pos{ startX, startY };
	for (const auto& row : selectedLevel)
	{
		std::istringstream iss(row.GetString());
		std::string		   token;

		while (std::getline(iss, token, '|'))
		{
			pathCreator->AddPathway(index, pos, token);
			index++;
			pos.x += Cellsize;
		}
		pos.x = static_cast<float>(startX);
		pos.y += Cellsize;
	}
	pathCreator->PickDoorFromBreakableTiles();
	scene->Add(std::move(pathObject));
	delete file;
}
#pragma endregion

#pragma region Making a level scene

void CreateLevelDetails(MathLib::GameMode gm, int levelIndex, std::string sceneName)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(gm == MathLib::GameMode::SOLO ? sceneName + std::to_string(levelIndex) : sceneName);

	MakeLevel(&scene, levelIndex, sceneName);

	auto pathcreator{ scene.GetGameObject(EnumStrings[PathCreator]) };
	auto pathcreatorComp{ pathcreator->GetComponent<dae::PathwayCreatorComponent>() };

	int i{ 0 };
	for (const auto& spawn : pathcreatorComp->GetSpawns())
	{
		glm::vec2 location{ spawn.Rect.x, spawn.Rect.y };
		if (i % 2 == 0)
		{
			MakeMainChar(&scene, spawn, gm, false);
		}
		else
		{
			if (gm == MathLib::GameMode::VERSUS)
			{
				MakeBalloomEnemy(&scene, spawn, true);
			}
			else
			{
				MakeSecondaryChar(&scene, spawn, gm, true);
			}
		}
		i++;
	}

	for (const auto& spawn : pathcreatorComp->GetEnemySpawns())
	{
		auto value{ MathLib::CalculateChance(1000) };
		if (value >= 0 && value <= 250)
		{
			MakeBalloomEnemy(&scene, spawn);
		}
		else if (value > 250 && value <= 500)
		{
			MakeOnealEnemy(&scene, spawn);
		}
		else if (value > 500 && value <= 750)
		{
			MakeDollEnemy(&scene, spawn);
		}
		else if (value > 750 && value <= 1000)
		{
			MakeMinvoEnemy(&scene, spawn);
		}
	}

	// scene.Init();
	scene.SetActive(true);

	CreateGlobalGameObject(&scene, gm, levelIndex + 1);

	pathcreatorComp->Init();
}
#pragma endregion

void CreateLevel(MathLib::GameMode gm, int levelIndex)
{

	switch (gm)
	{
	case MathLib::GameMode::SOLO:
		CreateLevelDetails(gm, levelIndex, EnumStrings[SoloLevelNameGeneral]);
		break;
	case MathLib::GameMode::COOP:
		CreateLevelDetails(gm, levelIndex, EnumStrings[CoopLevelName]);
		break;
	case MathLib::GameMode::VERSUS:
		CreateLevelDetails(gm, levelIndex, EnumStrings[VersusLevelName]);
		break;
	default:
		break;
	}
}

#pragma region Score

void CreateGlobalGameObject(dae::Scene* scene, MathLib::GameMode gm, int levelIndex)
{
	dae::FileReader* file2{ new dae::FileReader("../Data/save.json") };

	auto global = std::make_unique<GameObject>();
	global->SetName(EnumStrings[Names::Global]);
	global->AddComponent(std::make_unique<dae::MenuComponent>(scene, &CreateLevel, &MakeHighScoreScreen, std::vector<glm::vec2>{}, gm));
	global->AddComponent(std::make_unique<dae::AudioComponent>())->QueueMusic(dae::GameMusic);

	Input::InputKey skip;
	skip = std::make_tuple(dae::ButtonStates::BUTTON_DOWN, SDLK_F1, 0);
	Input::GetInstance().BindKey(skip, std::make_unique<dae::Skip>(scene, global.get()));

	if (gm != MathLib::GameMode::SOLO)
	{
		auto scoreHolder2 = new GameObject();
		scoreHolder2->SetName(EnumStrings[Score2]);
		scoreHolder2->AddComponent(std::make_unique<dae::TextObjectComponent>("Player2 Score: " + std::to_string(0), font3));
		scoreHolder2->GetTransform()->Translate(glm::vec3{ WindowSizeX - 200, 10, 0 });
		global->AddChild(std::move(scoreHolder2));

		std::string lives = gm == MathLib::GameMode::VERSUS ? "1" : "3";

		auto lifeHolder2 = new GameObject();
		lifeHolder2->SetName(EnumStrings[Life] + "1");
		lifeHolder2->AddComponent(std::make_unique<dae::TextObjectComponent>("Player2 Lives: " + lives, font3));
		lifeHolder2->GetTransform()->Translate(glm::vec3{ WindowSizeX - 200, 30, 0 });
		global->AddChild(std::move(lifeHolder2));
	}

	auto scoreHolder = new GameObject();
	scoreHolder->SetName(EnumStrings[Names::Score1]);
	scoreHolder->AddComponent(std::make_unique<dae::TextObjectComponent>("Player1 Score: " + std::to_string(0), font3));
	scoreHolder->GetTransform()->Translate(glm::vec3{ 10, 10, 0 });

	std::string previousLives = gm == MathLib::GameMode::VERSUS ? "1" : file2->GetDocument()["Lives"].GetString();
	auto		lifeHolder = new GameObject();
	lifeHolder->SetName(EnumStrings[Life] + "0");
	lifeHolder->AddComponent(std::make_unique<dae::TextObjectComponent>("Player1 Lives: " + previousLives, font3));
	lifeHolder->GetTransform()->Translate(glm::vec3{ 10, 30, 0 });

	auto text2 = new GameObject();
	text2->AddComponent(std::make_unique<dae::TextObjectComponent>("Round" + std::to_string(levelIndex), font));
	text2->GetTransform()->Translate(glm::vec3{ (WindowSizeX / 2) - 60, 30, 0 });

	global->AddChild(std::move(scoreHolder));
	global->AddChild(std::move(lifeHolder));
	global->AddChild(std::move(text2));
	scene->Add(std::move(global));

	auto previousScore = file2->GetDocument()["Score"].GetString();
	scene->GetGameObject(EnumStrings[Names::Player0])->GetComponent<dae::ValuesComponent>()->IncreaseScore(std::stoi(previousScore));
	delete file2;
}
#pragma endregion

#pragma region Menu

void CreateSelectorInput(dae::Scene* scene)
{

	// game mode selection
	auto player1 = std::make_unique<dae::GameObject>();
	auto player2 = std::make_unique<dae::GameObject>();
	auto player3 = std::make_unique<dae::GameObject>();

	player1->AddComponent(std::make_unique<dae::TextObjectComponent>("SOLO", font2));
	player2->AddComponent(std::make_unique<dae::TextObjectComponent>("COOP", font2));
	player3->AddComponent(std::make_unique<dae::TextObjectComponent>("VERSUS", font2));
	player1->GetTransform()->SetPosition(WindowSizeX / 2 - Margin * 3, 75);
	player2->GetTransform()->SetPosition(WindowSizeX / 2 - Margin * 3, 75 + Margin);
	player3->GetTransform()->SetPosition(WindowSizeX / 2 - Margin * 3, 75 + SubMargin);

	// game mode selector
	auto selector = std::make_unique<dae::GameObject>();
	selector->SetName(EnumStrings[Selector]);

	std::vector<glm::vec2> positions{
		player1->GetTransform()->GetWorld().Position,
		player2->GetTransform()->GetWorld().Position,
		player3->GetTransform()->GetWorld().Position,
	};
	positions[0].x -= 20;
	positions[1].x -= 20;
	positions[2].x -= 20;
	selector->AddComponent(std::make_unique<dae::MenuComponent>(scene, &CreateLevel, &MakeHighScoreScreen, positions));
	selector->AddComponent(std::make_unique<dae::TextObjectComponent>(">", font2));
	selector->GetTransform()->SetPosition(positions[0]);

	Input::InputKey up = std::make_tuple(dae::ButtonStates::BUTTON_DOWN, SDLK_w, 0);
	Input::InputKey down = std::make_tuple(dae::ButtonStates::BUTTON_DOWN, SDLK_s, 0);
	Input::InputKey space = std::make_tuple(dae::ButtonStates::BUTTON_DOWN, SDLK_SPACE, 0);

	Input::GetInstance().BindKey(up, std::make_unique<dae::CycleGameMode>(scene, selector.get(), true));
	Input::GetInstance().BindKey(down, std::make_unique<dae::CycleGameMode>(scene, selector.get(), false));
	Input::GetInstance().BindKey(space, std::make_unique<dae::StartGame>(scene, selector.get()));

	up = std::make_tuple(dae::ButtonStates::BUTTON_DOWN, dae::ControllerButton::DpadUp, 0);
	down = std::make_tuple(dae::ButtonStates::BUTTON_DOWN, dae::ControllerButton::DpadDown, 0);
	space = std::make_tuple(dae::ButtonStates::BUTTON_DOWN, dae::ControllerButton::ButtonA, 1);

	Input::GetInstance().BindKey(up, std::make_unique<dae::CycleGameMode>(scene, selector.get(), true));
	Input::GetInstance().BindKey(down, std::make_unique<dae::CycleGameMode>(scene, selector.get(), false));
	Input::GetInstance().BindKey(space, std::make_unique<dae::StartGame>(scene, selector.get()));

	scene->Add(std::move(player1));
	scene->Add(std::move(player2));
	scene->Add(std::move(player3));
	scene->Add(std::move(selector));
}

void MakeMenu()
{
	Input::GetInstance().ClearKeys();
	auto& scene = dae::SceneManager::GetInstance().CreateScene(EnumStrings[MainMenu]);
	scene.SetActive(true);

	dae::FileReader* file2{ new dae::FileReader("../Data/save.json") };
	file2->WriteData({ { "Score", "0" }, { "Lives", "3" } });

	// Title
	auto upText1 = std::make_unique<dae::GameObject>();
	upText1->AddComponent(std::make_unique<dae::TextObjectComponent>("BOMBERMAN", font));
	upText1->GetTransform()->Translate(WindowSizeX / 4, Margin);
	upText1->GetComponent<dae::TextObjectComponent>()->SetColor(SDL_Color{ 220, 20, 60 });
	upText1->AddComponent(std::make_unique<dae::AudioComponent>())->QueueMusic(dae::MenuMusic);
	scene.Add(std::move(upText1));

	CreateSelectorInput(&scene);

	dae::FileReader* file{ new dae::FileReader("../Data/highscore.json") };
	auto			 scores{ file->GetDocument()["data"].GetArray() };

	float yPos{ WindowSizeY / 2 };
	int	  i{ 1 };
	for (const auto& field : scores)
	{
		auto		scoreObj = std::make_unique<dae::GameObject>();
		auto		scoreObj2 = std::make_unique<dae::GameObject>();
		std::string score{ std::to_string(i) + "  " + field.GetObj()["name"].GetString() };
		std::string score2{ std::string(field.GetObj()["score"].GetString()) + " PTS" };
		scoreObj->AddComponent(std::make_unique<dae::TextObjectComponent>(score, font));
		scoreObj2->AddComponent(std::make_unique<dae::TextObjectComponent>(score2, font));
		scoreObj->GetComponent<dae::TextObjectComponent>()->SetPosition(Margin * 2, yPos);
		scoreObj2->GetComponent<dae::TextObjectComponent>()->SetPosition(WindowSizeX / 2 + Margin, yPos);
		scene.Add(std::move(scoreObj));
		scene.Add(std::move(scoreObj2));

		i++;
		yPos += 50;
	}
	delete file;
	delete file2;

	// scene.Add(go);
}
#pragma endregion

void MakeHighScoreScreen()
{
	Input::GetInstance().ClearKeys();
	auto& scene = dae::SceneManager::GetInstance().CreateScene(EnumStrings[HighScoreName]);
	scene.SetActive(true);

	dae::FileReader* file2{ new dae::FileReader("../Data/save.json") };
	std::string		 scoreNr = file2->GetDocument()["Score"].GetString();

	dae::FileReader* file{ new dae::FileReader("../Data/highscore.json") };
	auto			 scores{ file->GetDocument()["data"].GetArray() };
	std::string		 highScore{ scores[0].GetObj()["score"].GetString() };

	int	 scoreNrInt{ -1 };
	bool isNewHighScore{ false };
	auto upText1 = std::make_unique<dae::GameObject>();
	try
	{
		scoreNrInt = std::stoi(scoreNr);
		isNewHighScore = scoreNr > highScore;
		// Title
		upText1->AddComponent(std::make_unique<dae::TextObjectComponent>(isNewHighScore ? "New Highscore!" : "Your Score", font));
		upText1->AddComponent(std::make_unique<dae::AudioComponent>())->QueueMusic(isNewHighScore ? dae::HighscoreMusic : dae::GameOverMusic);
		upText1->GetTransform()->Translate(WindowSizeX / 4, Margin);
		upText1->GetComponent<dae::TextObjectComponent>()->SetColor(SDL_Color{ 220, 20, 60 });
		scene.Add(std::move(upText1));

		auto upText2 = std::make_unique<dae::GameObject>();
		upText2->AddComponent(std::make_unique<dae::TextObjectComponent>(std::to_string(scoreNrInt), font));
		upText2->GetTransform()->Translate(WindowSizeX / 3, Margin * 2);
		upText2->GetComponent<dae::TextObjectComponent>()->SetColor(SDL_Color{ 225, 225, 225 });
		scene.Add(std::move(upText2));
	}
	catch (std::invalid_argument)
	{
		std::string playerWinner{ scoreNr == EnumStrings[Names::Player0] ? "Player 2" : "Player 1" };
		upText1->AddComponent(std::make_unique<dae::TextObjectComponent>(playerWinner + " has won!", font));
		upText1->AddComponent(std::make_unique<dae::AudioComponent>())->QueueMusic(dae::HighscoreMusic);
		upText1->GetTransform()->Translate(WindowSizeX / 4, Margin);
		upText1->GetComponent<dae::TextObjectComponent>()->SetColor(SDL_Color{ 220, 20, 60 });
		scene.Add(std::move(upText1));
	}

	if (isNewHighScore)
	{
		auto upText3 = std::make_unique<dae::GameObject>();
		upText3->SetName("EditableName");
		upText3->AddComponent(std::make_unique<dae::TextObjectComponent>("Enter Name", font));
		upText3->GetTransform()->Translate(WindowSizeX / 3, Margin * 4);
		upText3->GetComponent<dae::TextObjectComponent>()->SetColor(SDL_Color{ 200, 200, 200 });
		scene.Add(std::move(upText3));

		auto scoreObj = std::make_unique<dae::GameObject>();
		scoreObj->AddComponent(std::make_unique<dae::HighscoreComponent>(&scene, scoreNrInt, &MakeHighScoreScreen));
		scene.Add(std::move(scoreObj));
	}
	else
	{
		float yPos{ WindowSizeY / 2 };
		int	  i{ 1 };
		for (const auto& field : scores)
		{
			auto		scoreObj = std::make_unique<dae::GameObject>();
			auto		scoreObj2 = std::make_unique<dae::GameObject>();
			std::string score{ std::to_string(i) + "  " + field.GetObj()["name"].GetString() };
			std::string score2{ std::string(field.GetObj()["score"].GetString()) + " PTS" };
			scoreObj->AddComponent(std::make_unique<dae::TextObjectComponent>(score, font));
			scoreObj2->AddComponent(std::make_unique<dae::TextObjectComponent>(score2, font));
			scoreObj->GetComponent<dae::TextObjectComponent>()->SetPosition(Margin * 2, yPos);
			scoreObj2->GetComponent<dae::TextObjectComponent>()->SetPosition(WindowSizeX / 2 + Margin, yPos);
			scene.Add(std::move(scoreObj));
			scene.Add(std::move(scoreObj2));

			i++;
			yPos += 50;
		}

		Input::InputKey space;
		space = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, dae::ControllerButton::ButtonA, 0);
		Input::GetInstance().BindKey(space, std::make_unique<dae::BackToMenu>(&scene, &MakeMenu));
		space = std::make_tuple(dae::ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0);
		Input::GetInstance().BindKey(space, std::make_unique<dae::BackToMenu>(&scene, &MakeMenu));
	}
	delete file;
	delete file2;
}

void load()
{
	font = dae::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 20);
	font2 = dae::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 14);
	font3 = dae::ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 10);

	// EnemyTypes = std::map<MathLib::EEnemyType, MathLib::FEnemyStats>{
	//	{ MathLib::EEnemyType::Balloom, MathLib::FEnemyStats{ MathLib::EEnemyType::Balloom, false, 0.5f, 100, "Balloom" } },
	//	{ MathLib::EEnemyType::Oneal, MathLib::FEnemyStats{ MathLib::EEnemyType::Oneal, true, 1.f, 200, "Oneal" } },
	//	{ MathLib::EEnemyType::Doll, MathLib::FEnemyStats{ MathLib::EEnemyType::Doll, false, 1.f, 300, "Doll" } },
	//	{ MathLib::EEnemyType::Minvo, MathLib::FEnemyStats{ MathLib::EEnemyType::Minvo, true, 1.5f, 400, "Minvo" } },
	// };

	MakeMenu();
	// SceneManager::GetInstance().GetScene(EnumStrings[MainMenu])->SetActive(true);

	// MakeHighScoreScreen(1000, true);
}

int main(int, char*[])
{
	/*if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else {
		std::cout << "Successfully initialized steam." << std::endl;
	}*/

	dae::Minigin engine("../Data/");
	engine.Run(load);

	// SteamAPI_Shutdown();
	return 0;
}