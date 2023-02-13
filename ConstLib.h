#pragma once

enum class Keys
{
	UP = 'W',
	DOWN = 'X',
	LEFT = 'A',
	RIGHT = 'D',
	STOP = 'S',
	ESC = 27
};

/*
static constexpr char Map_init[21][22] =
{ "###################",
 "#........#........#",
 "#.##.###.#.###.##.#",
 "#.................#",
 "#.##.#.#####.#.##.#",
 "#....#...#...#....#",
 "####.###.#.###.####",
 "   #.#... ...#.#   ",
 "####.#.# # #.#.####",
 "..... .# # #.......",
 "####.#.#####.#.####",
 "   #.#.......#.#   ",
 "####.#.#####.#.####",
 "#........#........#",
 "#.##.###.#.###.##.#",
 "#..#...........#..#",
 "##.#.#.#####.#.#.##",
 "#....#...#...#....#",
 "#.######.#.######.#",
 "#.................#",
 "###################" };
 */

static constexpr char WALL = '#';
static constexpr char BREADCRUMBS = '.';
static constexpr char GHOST = '$';
static constexpr char UNVISITED = '%';
static constexpr char SPACE = ' ';
static constexpr char PACMAN = '@';
