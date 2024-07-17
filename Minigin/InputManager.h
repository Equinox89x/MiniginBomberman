#pragma once
#include "Singleton.h"
#include <tuple>
#include <map>
#include <memory>
#include "Command.h"
#include <Windows.h>
#include <Xinput.h>
#include <variant>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#define MAX_PLAYERS 2

namespace dae
{

	enum class ControllerButton
	{
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
		DpadUp = 0x0001,
		DpadDown = 0x0002,
		DpadLeft = 0x0004,
		DpadRight = 0x0008,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		Start = 0x0010,
		Back = 0x0020,
		JoystickLeft = 99999999,
		JoystickRight = 88888888,
	};

	enum class ButtonStates
	{
		BUTTON_DOWN,
		BUTTON_PRESSED,
		BUTTON_UP
	};

	class Input final : public Singleton<Input>
	{
	public:
		using InputKey = std::variant<std::tuple<ButtonStates, SDL_KeyCode, int>, std::tuple<ButtonStates, ControllerButton, int>>;

	private:
		using ControllerCommandsMap = std::map<InputKey, std::unique_ptr<Command>>;
		using KeyboardCommandsMap = std::map<InputKey, std::unique_ptr<Command>>;

		ControllerCommandsMap m_consoleCommands{};
		KeyboardCommandsMap m_KeyboardCommands{};

		bool m_Clear{};

	public:

		void ClearKeys();
		bool GetClear() const { return m_Clear; };
		void SetClear(bool clear) { m_Clear = clear; };
		void Cleanup();
		void BindKey(InputKey key, std::unique_ptr<Command> c) {
			if (std::holds_alternative<std::tuple<ButtonStates, SDL_KeyCode, int>>(key)) {
				m_KeyboardCommands[key] = std::move(c);
			}
			else {
				m_consoleCommands[key] = std::move(c);
			}
		}		
		
		const ControllerCommandsMap& GetConsoleCommands()
		{
			return m_consoleCommands;
		};

		const KeyboardCommandsMap& GetKeyboardCommands()
		{
			return m_KeyboardCommands;
		};

	private:

	};

	class InputManager
	{
		class InputManagerImpl;
		InputManagerImpl* pImpl;

	public:
		InputManager(int controllerIndex);
		~InputManager();
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		//void ProcessInput();
		bool IsPressed(ControllerButton button) const;
		bool IsDownThisFrame(ControllerButton button) const;
		bool IsUpThisFrame(ControllerButton button) const;
		bool HandleInput();
		void Cleanup();
		glm::vec2 GetLeftThumbstick() const;
		glm::vec2 GetRightThumbstick() const;

	};

	class InputCollection {

	public:
		InputCollection(int nrOfControllers) : m_NrOfControllers{ nrOfControllers - 1 } {
			for (int i = 0; i < m_NrOfControllers; i++)
			{
				m_Inputs.push_back(std::make_unique<InputManager>(i));
			}
		};
		~InputCollection() {
			//Cleanup();
		};
		InputCollection(const InputCollection& other) = delete;
		InputCollection(InputCollection&& other) = delete;
		InputCollection& operator=(const InputCollection& other) = delete;
		InputCollection& operator=(InputCollection&& other) = delete;

		bool ProcessInput() {
			for (int i = 0; i < m_NrOfControllers; i++)
			{
				m_IsQuitCommandCalled = m_Inputs[i]->HandleInput();
				if (!m_IsQuitCommandCalled) {
					return m_IsQuitCommandCalled;
				}
			}

			return m_IsQuitCommandCalled;
		}
		void Cleanup() {
			for (int i = 0; i < m_NrOfControllers; i++)
			{
				m_Inputs[i]->Cleanup();
			}
			for (int i = 0; i < m_NrOfControllers; i++)
			{
				m_Inputs[i].reset();
				m_Inputs[i] = nullptr;
			}
		}

	private:
		int m_NrOfControllers{ 0 };
		std::vector<std::unique_ptr<InputManager>> m_Inputs;
		bool m_IsQuitCommandCalled{ false };

	};
}