#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <iostream>
#include <chrono>
#include <imgui_plot.h>
#include <functional>

#pragma region cache
//static std::vector<float> timingarr3{ };
//static std::vector<float> timingarr2{ };
//static std::vector<float> timingarr4{ };
//
//
//struct Transform
//{
//	float matrix[16] = {
//		1,0,0,0,
//		0,1,0,0,
//		0,0,1,0,
//		0,0,0,1 };
//};
//
//class GameObject3D
//{
//public:
//	Transform transform;
//	int ID;
//};
//
//class GameObject3DAlt
//{
//public:
//	Transform transform;
//	int ID = 1;
//};
//
//void Benchmark(std::function<void(int step)> func, std::vector<float>& timingArray) {
//	// Code to execute when the button is clicked
//	std::cout << "Button Clicked!\n";
//	for (int step = 1; step <= 1024; step *= 2)
//	{
//		std::vector<long long> timingarr;
//		for (size_t i = 0; i < 11; i++)
//		{
//			const auto start = std::chrono::high_resolution_clock::now();
//			func(step);
//			//for (int i2 = 0; i2 < size; i2 += step) arr3[i2].ID *= 2;
//			const auto end = std::chrono::high_resolution_clock::now();
//			const long long total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//			timingarr.push_back(static_cast<long long>(total));
//		}
//		timingarr.erase(std::remove(timingarr.begin(), timingarr.end(), timingarr[0]));
//		timingarr.erase(std::remove(timingarr.begin(), timingarr.end(), timingarr[timingarr.size() - 1]));
//		int average{ 0 };
//		for (auto item : timingarr) average += int(item);
//		timingArray.push_back(float(float(average / timingarr.size())));
//	}
//}
//
//void TrashTheCash()
//{
//	int buf_size = 11;
//	std::vector<float> arr4 = { 1, 2, 4, 8, 15, 32, 64, 128, 512, 1024 };
//
//	const float* y_data[] = { timingarr2.data(), timingarr3.data(), timingarr4.data() };
//
//	std::vector<ImU32> colors = { ImColor(255, 0, 0), ImColor(0, 255, 0), ImColor(0, 255, 255) };
//	uint32_t selection_start = 0, selection_length = 0;
//
//	ImGui::PlotConfig conf2;
//	conf2.values.xs = arr4.data();  // x-axis values
//	conf2.values.count = buf_size;  // number of data points
//	conf2.values.ys = timingarr2.data();  // y-axis values
//	conf2.values.colors = colors.data();  // line colors
//	conf2.tooltip.show = true;  // show tooltips
//	conf2.grid_x.show = true;  // show x-axis grid
//	conf2.grid_y.show = true;  // show y-axis grid
//	conf2.selection.show = true;  // show selection
//	conf2.selection.start = &selection_start;  // selection start index
//	conf2.selection.length = &selection_length;  // selection length
//	conf2.frame_size = ImVec2(400, 200);  // plot frame size
//	
//	ImGui::Begin("Timing plots", nullptr, ImGuiWindowFlags_None);
//
//	//button 1
//	if (ImGui::Button("Generate plot 1")) {
//		int size{ int(std::pow(2, 26)) / 10 };
//		int* arr{ new int[size] };
//
//		Benchmark([&](int step) {
//			for (int i2 = 0; i2 < size; i2 += step) arr[i2] *= 2;
//			}, timingarr2);
//		delete[] arr;
//	}
//
//	if (timingarr2.size() > 0) {
//
//		conf2.scale.min = timingarr2[timingarr2.size() - 1];  // minimum value on y-axis
//		conf2.scale.max = timingarr2[0];   // maximum value on y-axis
//		conf2.grid_x.size = timingarr2[0];   // x-axis grid size
//		conf2.grid_y.size = timingarr2[0];  // y-axis grid size
//
//		ImGui::Plot("plot2", conf2);  // draw plot
//	}
//
//	//buttonn 2
//	if (ImGui::Button("Generate plot 2")) {
//		int size{ int(std::pow(2, 26)) / 10 };
//
//		GameObject3D* arr2{ new GameObject3D[size] };
//		for (int i = 0; i < size; i++)
//		{
//			arr2[i].ID = 50;
//		}
//
//		Benchmark([&](int step) {
//			for (int i2 = 0; i2 < size; i2 += step) arr2[i2].ID *= 2;
//			}, timingarr3);
//		delete[] arr2;
//	}
//
//	if (timingarr3.size() > 0) {
//		conf2.values.ys = timingarr3.data();
//		conf2.scale.min = timingarr3[timingarr3.size() - 1];  // minimum value on y-axis
//		conf2.scale.max = timingarr3[0];   // maximum value on y-axis
//		conf2.grid_x.size = timingarr3[0];   // x-axis grid size
//
//		ImGui::Plot("plot3", conf2);
//	}
//
//	//button 3
//	if (ImGui::Button("Generate plot 3")) {
//		int size{ int(std::pow(2, 26)) / 10 };
//
//		GameObject3DAlt* arr3{ new GameObject3DAlt[size] };
//		for (int i = 0; i < size; i++)
//		{
//			arr3[i].ID = 50;
//		}
//		Benchmark([&](int step) {
//			for (int i2 = 0; i2 < size; i2 += step) arr3[i2].ID *= 2;
//			}, timingarr4);
//		delete[] arr3;
//	}
//
//	if (timingarr4.size() > 0) {
//		conf2.values.ys = timingarr4.data();
//		conf2.scale.min = timingarr4[timingarr4.size() - 1];  // minimum value on y-axis
//		conf2.scale.max = timingarr4[0];   // maximum value on y-axis
//		conf2.grid_x.size = timingarr4[0];   // x-axis grid size
//
//		ImGui::Plot("plot4", conf2);
//	}
//
//	//combied plot
//	ImGui::Text("Combined Plot");
//	if (timingarr2.size() > 0 && timingarr3.size() > 0 && timingarr4.size() > 0) {
//		ImGui::PlotConfig conf;
//		conf.values.xs = arr4.data();  // x-axis values
//		conf.values.count = buf_size;  // number of data points
//		conf.values.ys_list = y_data;  // y-axis values
//		conf.values.ys_count = 3;  // number of lines to plot
//
//		conf.values.colors = colors.data();  // line colors
//
//		conf.scale.min = timingarr2[timingarr2.size() - 1];  // minimum value on y-axis
//		conf.scale.max = timingarr2[0];   // maximum value on y-axis
//
//		conf.tooltip.show = true;  // show tooltips
//		conf.grid_x.show = true;  // show x-axis grid
//		conf.grid_x.size = timingarr2[0];   // x-axis grid size
//
//		conf.grid_y.show = true;  // show y-axis grid
//		conf.grid_y.size = 1024;  // y-axis grid size
//
//		conf.selection.show = true;  // show selection
//		conf.selection.start = &selection_start;  // selection start index
//		conf.selection.length = &selection_length;  // selection length
//
//		conf.frame_size = ImVec2(400, 200);  // plot frame size
//		ImGui::Plot("plot1", conf);  // draw plot
//	}
//	ImGui::End();
//
//}
#pragma endregion

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();

	//TrashTheCash();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();
	
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame(m_window);
	//ImGui::NewFrame();
	////ImGui::ShowDemoWindow();
	////TrashTheCash();
	////draw_multi_plot(timingarr2, timingarr3, timingarr4);

	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
