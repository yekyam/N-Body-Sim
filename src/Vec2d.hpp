#pragma once

struct Vec2d
{
	float x;
	float y;

	friend Vec2d operator+(const Vec2d &lhs, const Vec2d &rhs)
	{ //
		return Vec2d{lhs.x + rhs.x, lhs.y + rhs.y};
	}

	friend Vec2d operator-(const Vec2d &lhs, const Vec2d &rhs)
	{ //
		return Vec2d{lhs.x - rhs.x, lhs.y - rhs.y};
	}

	friend Vec2d operator*(const Vec2d &lhs, const Vec2d &rhs)
	{ //
		return Vec2d{lhs.x * rhs.x, lhs.y * rhs.y};
	}

	friend Vec2d operator/(const Vec2d &lhs, const Vec2d &rhs)
	{ //
		return Vec2d{lhs.x / rhs.x, lhs.y / rhs.y};
	}
};