#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include <iostream>


using namespace dae;

class dae::InputManager::InputManagerImpl
{
	bool m_IsConnectedPreviousframe{ false };

	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};

	WORD buttonsPressedThisFrame{};
	WORD buttonsReleasedThisFrame{};

	int controllerIndex;

public:
	InputManagerImpl(int controllerIndex)
		:controllerIndex(controllerIndex)
	{
		ZeroMemory(&previousState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	}

	void Update()
	{
		CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));

		DWORD dwResult;
		dwResult = XInputGetState(controllerIndex, &currentState);

		bool connected{ dwResult == ERROR_SUCCESS };

		if (m_IsConnectedPreviousframe != connected)
		{
			if (connected) std::cout << "Controller " << controllerIndex << " connected" << std::endl;
			else std::cout << "Controller " << controllerIndex << " disconnected" << std::endl;
		}

		m_IsConnectedPreviousframe = connected;

		auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
		buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
	}

	bool IsDownThisFrame(unsigned button) const
	{
		return buttonsPressedThisFrame & button;
	}

	bool IsUpThisFrame(unsigned button) const
	{
		return buttonsReleasedThisFrame & button;
	}

	bool IsPressed(unsigned button) const
	{
		return currentState.Gamepad.wButtons & button;
	}
	float NormalizeAxis(SHORT value, int deadZone) const
	{
		if (abs(value) < deadZone)
			return 0.0f;

		// Normalize to range -1.0f to 1.0f
		return (value < 0 ? -1.0f : 1.0f) * ((float)(abs(value) - deadZone) / (32767.0f - deadZone));
	}
	glm::vec2 GetLeftThumbstick() const
	{
		return glm::vec2(
			NormalizeAxis(currentState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE),
			NormalizeAxis(currentState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		);
	}

	glm::vec2 GetRightThumbstick() const
	{
		return glm::vec2(
			NormalizeAxis(currentState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE),
			NormalizeAxis(currentState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		);
	}


};


dae::InputManager::InputManager(int controllerIndex)
{
	pImpl = new InputManagerImpl(controllerIndex);
}

dae::InputManager::~InputManager()
{
	Cleanup();
}

void dae::InputManager::Cleanup()
{
	if (pImpl) {
		delete pImpl;
	}
	pImpl = nullptr;
}

glm::vec2 dae::InputManager::GetLeftThumbstick() const
{
	return pImpl->GetLeftThumbstick();
}
glm::vec2 dae::InputManager::GetRightThumbstick() const
{ 
	return pImpl->GetRightThumbstick(); 
}




void dae::Input::ClearKeys()
{
	m_consoleCommands.clear();
	m_KeyboardCommands.clear();
	m_Clear = true;
}

bool dae::InputManager::IsPressed(ControllerButton button) const { return pImpl->IsPressed(static_cast<unsigned int>(button)); }
bool dae::InputManager::IsDownThisFrame(ControllerButton button) const { return pImpl->IsDownThisFrame(static_cast<unsigned int>(button)); }
bool dae::InputManager::IsUpThisFrame(ControllerButton button) const { return pImpl->IsUpThisFrame(static_cast<unsigned int>(button)); }

bool dae::InputManager::HandleInput()
{
	pImpl->Update();

#pragma region Controller
	for (const auto& [controllerKey, action] : Input::GetInstance().GetConsoleCommands())
	{

		auto [state, button, id] = std::get<std::tuple<ButtonStates, ControllerButton, int>>(controllerKey);

		//if (button == ControllerButton::JoystickLeft || button == ControllerButton::JoystickRight) {
		//	auto pos{ GetLeftThumbstick() };
		//	action->Execute(pos);

		//}
		if (button == ControllerButton::JoystickLeft)	{
			auto pos{ GetLeftThumbstick() };
			if (pos.x > 0 || pos.x < 0) {
				action->Execute(pos);
			}
			if(pos.y > 0 || pos.y < 0 ){
				action->Execute(pos);
			}
			//if (pos.x != 0 && pos.y != 0) {
			//}
		}	
		else if (button == ControllerButton::JoystickRight) {
			auto pos{ GetRightThumbstick() };
			//if (pos.x != 0 && pos.y != 0) {
			//	action->Execute();
			//}
			if (pos.x > 0 || pos.x < 0) {
				action->Execute(pos);
			}
			if (pos.y > 0 || pos.y < 0) {
				action->Execute(pos);
			}
		}
		else {
			switch (state)
			{
			case ButtonStates::BUTTON_DOWN:
				if (IsDownThisFrame(button))
					action->Execute();
				break;

			case ButtonStates::BUTTON_PRESSED:
				if (IsPressed(button))
					action->Execute();
				break;

			case ButtonStates::BUTTON_UP:
				if (IsUpThisFrame(button))
					action->Execute();
				break;
			}
		}


		if (Input::GetInstance().GetClear() == true)
		{
			Input::GetInstance().SetClear(false);
			break;
		}
	}
#pragma endregion Controller

#pragma region Keyboard
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		for (const auto& [key, action] : Input::GetInstance().GetKeyboardCommands())
		{
			auto [state, button, id] = std::get<std::tuple<ButtonStates, SDL_KeyCode, int>>(key);
			switch (state)
			{
			case ButtonStates::BUTTON_DOWN:
				if (e.key.type == SDL_KEYDOWN)
				{
					if (e.key.repeat == 0)
					{
						if (button == e.key.keysym.sym) action->Execute();
					}
				}
				break;
			case ButtonStates::BUTTON_PRESSED:
				if (e.key.type == SDL_KEYDOWN)
				{
					if (button == e.key.keysym.sym) action->Execute();
				}
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					SDL_Quit();
					return false;
				}
				break;
			case ButtonStates::BUTTON_UP:
				if (e.key.type == SDL_KEYUP)
				{
					if (button == e.key.keysym.sym) action->Execute();
				}
				break;
			}

			if (Input::GetInstance().GetClear() == true)
			{
				Input::GetInstance().SetClear(false);
				break;
			}
		}
	}
#pragma endregion Keyboard
	return true;
}