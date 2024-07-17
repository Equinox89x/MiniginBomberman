#pragma once
#include <SDL_rect.h>
#include <glm/glm.hpp>
//#include <glm/gtc/random.hpp>
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include <cmath>
#include <random>
#include "Timer.h"
#include <chrono>

namespace MathLib {

	inline float Square(float val) {
		return val * val;
	}

	#pragma region Bezier
	inline int BinomialCoeff(int n, int i) {
		if (i == 0 || i == n)
			return 1;

		int numerator = 1;
		int denominator = 1;

		for (int j = 1; j <= i; j++) {
			numerator *= (n - j + 1);
			denominator *= j;
		}

		return numerator / denominator;
	}

	inline glm::vec2 CalculateBezierPoint(float t, const std::vector<glm::vec2>& controlPoints) {
		const int n{ static_cast<int>(controlPoints.size()) - 1 };
		float x{ 0.0 };
		float y{ 0.0 };

		//Apply the quadtratic bezier formula as many times as there are controlPoint values
		for (int i = 0; i <= n; i++) {
			float blend{ static_cast<float>(BinomialCoeff(n, i) * pow(1 - t, n - i) * pow(t, i)) };
			x += controlPoints[i].x * blend;
			y += controlPoints[i].y * blend;
		}

		return { x, y };
	}
	#pragma endregion

	#pragma region Overlap
	inline bool IsOverlapping(const SDL_Rect& source, const SDL_Rect& target) {
		//bool doesXOverlap = (source.x < target.x + target.w) && (source.x + source.w > target.x);
		//bool doesYOverlap = (source.y < target.y + target.h) && (source.y + source.h > target.y);
		//return doesXOverlap && doesYOverlap;

		// Check for no overlap along the x-axis
		if ((source.x + source.w <= target.x) || (target.x + target.w <= source.x))
			return false;

		// Check for no overlap along the y-axis
		if ((source.y + source.h <= target.y) || (target.y + target.h <= source.y))
			return false;

		// If there is overlap along both axes, the rectangles overlap
		return true;
	}

	inline bool IsCompletelyOverlapping(const SDL_Rect& rect1, const SDL_Rect& rect2) {
		// Calculate the coordinates of the bottom-right corners for both rectangles
		const int rect1Right = rect1.x + rect1.w;
		const int rect1Bottom = rect1.y + rect1.h;

		const int rect2Right = rect2.x + rect2.w;
		const int rect2Bottom = rect2.y + rect2.h;

		// Check if rect1 is completely to the left of rect2 or completely to the right of rect2
		if (rect1.x >= rect2Right || rect2.x >= rect1Right) {
			return false;
		}

		// Check if rect1 is completely above rect2 or completely below rect2
		if (rect1.y >= rect2Bottom || rect2.y >= rect1Bottom) {
			return false;
		}

		// If none of the above conditions are met, then rect1 and rect2 are overlapping
		return true;
	}

	inline bool IsPointInsideRect(const SDL_Point& point, const SDL_Rect& rect) {
		return (point.x >= rect.x && point.x <= rect.x + rect.w &&
			point.y >= rect.y && point.y <= rect.y + rect.h);
	}
	#pragma endregion

	inline bool IsSameRect(const SDL_Rect& rect1, const SDL_Rect& rect)
	{
		return (rect1.x == rect.x &&
			rect1.y == rect.y &&
			rect1.w == rect.w &&
			rect1.h == rect.h);
	}

	enum Movement {
		UP, DOWN, LEFT, RIGHT
	};	
	
	enum EMovingState {
		MovingLeft,
		MovingRight,
		MovingUp,
		MovingDown,
		Still
	};

	enum class Side {
		None,
		Top,
		Bottom,
		Left,
		Right
	};

	enum class GameMode {
		SOLO, COOP, VERSUS
	};

	enum ELifeState {
		ALIVE, DEAD, RESPAWN, INVINCIBLE, INFLATING
	};

	inline Side GetNonOverlappingSide(const SDL_Rect& rect1, const SDL_Rect& rect2) {
		//// Calculate the coordinates of the edges for both rectangles
		//int rect1Right = rect1.x + rect1.w;
		//int rect1Bottom = rect1.y + rect1.h;

		//int rect2Right = rect2.x + rect2.w;
		//int rect2Bottom = rect2.y + rect2.h;

		//// Check if rect1 is completely to the left of rect2
		//if (rect1Right <= rect2.x) {
		//	return Side::Left;
		//}

		//// Check if rect1 is completely to the right of rect2
		//if (rect1.x >= rect2Right) {
		//	return Side::Right;
		//}

		//// Check if rect1 is completely above rect2
		//if (rect1Bottom <= rect2.y) {
		//	return Side::Top;
		//}

		//// Check if rect1 is completely below rect2
		//if (rect1.y >= rect2Bottom) {
		//	return Side::Bottom;
		//}

		//// If none of the above conditions are met, the squares are overlapping or intersecting
		//return Side::None;

		const auto xLeft{ rect1.x };
		const auto xRight{ rect1.x + rect1.w };

		/*	const auto yTop{ rect1.y };
			const auto yBottom{ rect1.y - rect1.h };*/

		SDL_Rect one{ xLeft, rect1.y + rect1.h, rect1.w / 5, rect1.h / 5 };
		SDL_Rect two{ xRight, rect1.y + rect1.h, rect1.w / 5, rect1.h / 5 };
		two.x -= two.w;

		//if (rect1.y + 10 > rect2.y && rect1.y - 10 < rect2.y) {
		if (!IsOverlapping(one, rect2)) {
			return Side::Left;
		}
		else if (!IsOverlapping(two, rect2)) {
			return Side::Right;
		}
		/*else if (!IsOverlapping(SDL_Point(rect1.x, yTop), rect2)) {
			return Side::Top;
		}
		else if (!IsOverlapping(SDL_Point(rect1.x, yBottom), rect2)) {
			return Side::Bottom;
		}*/
		else {
			return Side::None;
		}
		//}
	}

	inline glm::vec2 MoveRectTowards(SDL_Rect& rect1, const SDL_Rect& rect2, float speed)
	{
		const glm::vec2 rect1Center(rect1.x + rect1.w / 2, rect1.y + rect1.h / 2);
		const glm::vec2 rect2Center(rect2.x + rect2.w / 2, rect2.y + rect2.h / 2);

		const glm::vec2 direction = glm::normalize(rect2Center - rect1Center);

		return direction * speed;
	}

	inline int CalculateChance(/*int min = 0.f,*/ int max = 100.f) {
		if (max <= 0) {
			return 0;
		}
		std::random_device rd; // Non-deterministic random number generator
		std::mt19937 gen(rd() + 1 * static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
		auto distribution = std::uniform_int_distribution<>(0,max);
		return static_cast<int>(distribution(gen));
	}
}