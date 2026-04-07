#pragma once

namespace GridPathfinding
{
	inline constexpr int32 INVALID_SORTING_COST = -1;
	inline constexpr int32 DEFAULT_COST_TO_ENTER = 1;
	inline constexpr int32 DEFAULT_COST = 999999;
}

namespace Grid
{
	inline constexpr int32 INVALID_POINT_VALUE = -999;
	inline constexpr int32 DEFAULT_GRID_LOWER_Z = 999999;
}

namespace Unit
{
	inline constexpr float INVALID_UNIT_LOCATION_VALUE = -99999.0f;
}

namespace Tile
{
	inline constexpr float DEFAULT_TILE_SIZE_X = 100.0f;
	inline constexpr float DEFAULT_TILE_SIZE_Y = 100.0f;
	inline constexpr float DEFAULT_TILE_SIZE_Z = 100.0f;
}
