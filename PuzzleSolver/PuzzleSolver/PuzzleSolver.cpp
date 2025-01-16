#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define PIECE_COUNT 10
#define PIECE_MAX_SIZE 4
#define PIECE_SHAPE_COUNT 8
typedef char PIECE[PIECE_MAX_SIZE][PIECE_MAX_SIZE];

#define MAP_MAX_SIZE_X 8
#define MAP_MAX_SIZE_Y 7
#define MAP_SIGN_CAN_BE_PLACED '.'

char pieces[PIECE_COUNT + 1][PIECE_MAX_SIZE][PIECE_MAX_SIZE] = {
	{
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'1', '1', '1', '1'},
		{'1', '.', '.', '.'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'2', '2', '2', '.'},
		{'2', '2', '.', '.'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'3', '3', '.', '.'},
		{'.', '3', '3', '3'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'4', '4', '4', '4'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'5', '5', '5', '.'},
		{'5', '.', '.', '.'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'6', '6', '.', '.'},
		{'.', '6', '6', '.'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'7', '7', '7', '.'},
		{'7', '.', '.', '.'},
		{'7', '.', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'8', '8', '8', '.'},
		{'8', '.', '8', '.'},
		{'.', '.', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'9', '9', '9', '.'},
		{'.', '9', '.', '.'},
		{'.', '9', '.', '.'},
		{'.', '.', '.', '.'}
	},

	{
		{'X', 'X', '.', '.'},
		{'.', 'X', '.', '.'},
		{'.', 'X', 'X', '.'},
		{'.', '.', '.', '.'}
	},
};

char transformed_pieces[PIECE_COUNT + 1][PIECE_SHAPE_COUNT + 1][PIECE_MAX_SIZE][PIECE_MAX_SIZE];

char map[MAP_MAX_SIZE_X][MAP_MAX_SIZE_Y];

int piece_unused_count = PIECE_COUNT;

int piece_used[PIECE_COUNT + 1];

void print_map()
{
	for (int i = 0; i < MAP_MAX_SIZE_X; i++)
	{
		for (int j = 0; j < MAP_MAX_SIZE_Y; j++)
		{
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
}

void print_piece(int piece_id)
{
	for (int i = 1; i <= PIECE_SHAPE_COUNT; i++)
	{
		for (int j = 0; j < PIECE_MAX_SIZE; j++)
		{

			for (int k = 0; k < PIECE_MAX_SIZE; k++)
			{
				cout << transformed_pieces[piece_id][i][j][k] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void filp_piece(PIECE p)
{
	for (int i = 0; i < PIECE_MAX_SIZE; i++)
		for (int j = 0; j < PIECE_MAX_SIZE / 2; j++)
		{
			swap(p[i][j], p[i][PIECE_MAX_SIZE - j - 1]);
		}
}

void rotate_piece(PIECE p)
{
	PIECE tmp;
	for (int i = 0; i < PIECE_MAX_SIZE; i++)
	{
		for (int j = 0; j < PIECE_MAX_SIZE; j++)
		{
			tmp[i][j] = p[PIECE_MAX_SIZE - j - 1][i];
		}
	}
	for (int i = 0; i < PIECE_MAX_SIZE; i++)
	{
		for (int j = 0; j < PIECE_MAX_SIZE; j++)
		{
			p[i][j] = tmp[i][j];
		}
	}
}

void copy_piece(PIECE p1, PIECE p2)
{
	for (int i = 0; i < PIECE_MAX_SIZE; i++)
	{
		for (int j = 0; j < PIECE_MAX_SIZE; j++)
		{
			p1[i][j] = p2[i][j];
		}
	}
}

// Check if piece #piece_id with shape #shape_id can be placed in (map_x, map_y)
bool can_be_placed(int map_x, int map_y, int piece_id, int shape_id)
{
	for (int piece_x = 0; piece_x < PIECE_MAX_SIZE; piece_x++)
	{
		for (int piece_y = 0; piece_y < PIECE_MAX_SIZE; piece_y++)
		{
			if (transformed_pieces[piece_id][shape_id][piece_x][piece_y] != MAP_SIGN_CAN_BE_PLACED)
			{
				int curr_piece_x = piece_x;
				int curr_piece_y = piece_y;
				while (curr_piece_x < PIECE_MAX_SIZE && curr_piece_y < PIECE_MAX_SIZE)
				{
					int curr_map_x = map_x + curr_piece_x - piece_x;
					int curr_map_y = map_y + curr_piece_y - piece_y;

					if (curr_map_x < 0 || curr_map_y < 0 || curr_map_x >= MAP_MAX_SIZE_X || curr_map_y >= MAP_MAX_SIZE_Y)
					{
						return false;
					}

					if (transformed_pieces[piece_id][shape_id][curr_piece_x][curr_piece_y] != MAP_SIGN_CAN_BE_PLACED)
					{
						if (map[curr_map_x][curr_map_y] != MAP_SIGN_CAN_BE_PLACED)
						{
							return false;
						}
					}
					if (curr_piece_y == PIECE_MAX_SIZE - 1)
					{
						curr_piece_x++;
						curr_piece_y = 0;
					}
					else
					{
						curr_piece_y++;
					}
				}
				return true;
			}
		}
	}
	return false;
}

void place(int map_x, int map_y, int piece_id, int shape_id, bool undo)
{
	piece_used[piece_id] = undo ? 0 : 1;
	piece_unused_count += (undo ? 1 : -1);

	for (int piece_x = 0; piece_x < PIECE_MAX_SIZE; piece_x++)
	{
		for (int piece_y = 0; piece_y < PIECE_MAX_SIZE; piece_y++)
		{
			if (transformed_pieces[piece_id][shape_id][piece_x][piece_y] != MAP_SIGN_CAN_BE_PLACED)
			{
				int curr_piece_x = piece_x;
				int curr_piece_y = piece_y;
				while (curr_piece_x < PIECE_MAX_SIZE && curr_piece_y < PIECE_MAX_SIZE)
				{
					int curr_map_x = map_x + curr_piece_x - piece_x;
					int curr_map_y = map_y + curr_piece_y - piece_y;
					if (transformed_pieces[piece_id][shape_id][curr_piece_x][curr_piece_y] != MAP_SIGN_CAN_BE_PLACED)
					{
						map[curr_map_x][curr_map_y] = undo ? MAP_SIGN_CAN_BE_PLACED : transformed_pieces[piece_id][shape_id][curr_piece_x][curr_piece_y];
					}
					if (curr_piece_y == PIECE_MAX_SIZE - 1)
					{
						curr_piece_x++;
						curr_piece_y = 0;
					}
					else
					{
						curr_piece_y++;
					}
				}
				return;
			}
		}
	}
}

bool dfs(int block_id)
{
	int map_x = block_id / (MAP_MAX_SIZE_X - 1);
	int map_y = block_id % MAP_MAX_SIZE_Y;

	if (piece_unused_count == 0)
	{
		return true;
	}

	if (map[map_x][map_y] != MAP_SIGN_CAN_BE_PLACED)
	{
		if (dfs(block_id + 1))
		{
			return true;
		}
	}

	for (int piece_id = 1; piece_id <= PIECE_COUNT; piece_id++)
	{
		if (piece_used[piece_id])
		{
			continue;
		}
		for (int shape_id = 1; shape_id <= PIECE_SHAPE_COUNT; shape_id++)
		{
			if (can_be_placed(map_x, map_y, piece_id, shape_id))
			{
				place(map_x, map_y, piece_id, shape_id, false);
				if (dfs(block_id + 1))
				{
					return true;
				};
				place(map_x, map_y, piece_id, shape_id, true);
			}
		}
	}

	return false;
}

void transform_pieces()
{
	for (int piece_id = 1; piece_id <= PIECE_COUNT; piece_id++)
	{
		copy_piece(transformed_pieces[piece_id][1], pieces[piece_id]);
		rotate_piece(pieces[piece_id]);
		copy_piece(transformed_pieces[piece_id][2], pieces[piece_id]);
		rotate_piece(pieces[piece_id]);
		copy_piece(transformed_pieces[piece_id][3], pieces[piece_id]);
		rotate_piece(pieces[piece_id]);
		copy_piece(transformed_pieces[piece_id][4], pieces[piece_id]);

		rotate_piece(pieces[piece_id]);
		filp_piece(pieces[piece_id]);

		copy_piece(transformed_pieces[piece_id][5], pieces[piece_id]);
		rotate_piece(pieces[piece_id]);
		copy_piece(transformed_pieces[piece_id][6], pieces[piece_id]);
		rotate_piece(pieces[piece_id]);
		copy_piece(transformed_pieces[piece_id][7], pieces[piece_id]);
		rotate_piece(pieces[piece_id]);
		copy_piece(transformed_pieces[piece_id][8], pieces[piece_id]);
	}
}

void read_input()
{
	ifstream in;
	in.open("input.txt");
	if (!in.is_open())
	{
		cout << "Cannot find input file!!" << endl;
		exit(0);
	};

	string line;
	for (int i = 0; i < MAP_MAX_SIZE_X; ++i)
	{
		getline(in, line);
		for (int j = 0; j < MAP_MAX_SIZE_Y; j++)
		{
			map[i][j] = line[j];
		}
	}
	in.close();
}

void write_output()
{
	ofstream out;
	out.open("output.txt");
	for (int i = 0; i < MAP_MAX_SIZE_X; i++)
	{
		for (int j = 0; j < MAP_MAX_SIZE_Y; j++)
		{
			out << map[i][j];
		}
		out << endl;
	}

	out.close();
}

bool solve()
{
	transform_pieces();
	return dfs(0);
}

int main()
{
	cout << "Calender Puzzle Solver v0.1" << endl;
	cout << endl;
	read_input();
	cout << "INPUT----------" << endl;
	print_map();

	if (solve())
	{
		cout << "OUTPUT----------" << endl;
		print_map();
		write_output();
	}
	else
	{
		cout << "Cannot solve!" << endl;
	}
	return 0;
}
