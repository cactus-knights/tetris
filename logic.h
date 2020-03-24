#pragma once
#include <vector>
#include <algorithm>
using namespace std;

typedef vector< vector<int> > Field;

void print_field(Field f);

class point
{
	public:

		int x;
		int y;
};

class Figure
{
	public:

		Field vertical;
		Field bwd_L_sh;
		Field fwd_L_sh;
		Field bwd_Z_sh;
		Field fwd_Z_sh;
		Field T_Shaped;
		Field Square;
};

enum class Direction { left, right, up, down };

class Game
{
	public:

		Field& GetField();
		Field GetClearField();
		Field GetNextFigure();
		int GetScore();
		int GetLevel();

		void IncreaseLevel();
		void MakeMove();
		void Rotate();
		void MoveLeft();
		void MoveRight();
		bool isLose();

		bool deletelines = false;
	private:

		static const int FIELD_HEIGHT = 24;
		static const int FIELD_WIDTH = 10;
		bool GameStarted = false;
		int score = 0;
		int level = 1;
		Field MainField = vector< vector<int> >(FIELD_HEIGHT, vector<int>(FIELD_WIDTH, 0));
		Field ClearField = vector< vector<int> >(FIELD_HEIGHT, vector<int>(FIELD_WIDTH, 0));
		Figure Figures;
		point CurCoord;
		pair<int, int> CurSize;

		Field CurFigure;
		Field NextFigure;

		Field GetFigure();

		std::vector<int> linesToDelete;

	public:
		void Start();
	private:
		void FillFigures();
		void SetFigToField(Field& figure, point LUcoord);
		void MoveFigure(Direction direction);
	public:
		bool AbleToMove(Direction direction);
	private:
		bool AbleToRotate();
		void FieldReverse(Field& field);

		void CheckLines();
		void DeleteLines();
		void DownFieldAboveLine(int n);

		bool isEmptyLine(int n);
};