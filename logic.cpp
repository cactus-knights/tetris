#include <iostream>
#include <vector>
#include <cstdio>
#include <ctime>
#include "logic.h"

using namespace std;

Field& Game::GetField()
{
	return this->MainField;
}

Field Game::GetClearField()
{
	return this->ClearField;
}

Field Game::GetNextFigure()
{
	return this->NextFigure;
}

int Game::GetScore()
{
	return this->score;
}

void Game::DownFieldAboveLine(int n)
{
	for(int i = n; i >= 1; i--)
	{
		for(int j = 0; j < MainField[i].size(); j++)
		{
			MainField[i][j] = MainField[i-1][j];
			MainField[i-1][j] = 0;
		}
	}
}

void Game::MakeMove()
{
	if (!this->GameStarted)
	{
		this->GameStarted = true;
		Start();
		return;
	}
	if (AbleToMove(Direction::down))
	{
		MoveFigure(Direction::down);
	}
	else
	{
		CheckLines();

		if(linesToDelete.size() != 0)
		{
			deletelines = true;
			DeleteLines();
		}

		this->ClearField = this->MainField;

		CurFigure = this->NextFigure;
		this->NextFigure = GetFigure();

		point coord;
		coord.x = 0;
		coord.y = 4;
		SetFigToField(CurFigure, coord);
	}
}


void Game::Rotate()
{
	if(!AbleToRotate())
		return;

	Field cfield = GetClearField();

    int n = this->CurSize.first;
	int m = this->CurSize.second;
	int x = this->CurCoord.x;
	int y = this->CurCoord.y;

	auto NFigure = vector< vector<int> >(m, vector<int>(n, 0));
 
    for(size_t i(0); i < m; i++)
    {
        for(size_t j(0); j < n; j++)
        {
            if(CurFigure[n-j-1][i] != 0)
            {
            	cfield[x+i][y+j] = CurFigure[n-j-1][i];
            	NFigure[i][j] = cfield[x+i][y+j];
        	}
        }
    }

    CurFigure = NFigure;
    this->MainField = cfield;

    int t = this->CurSize.first;
    this->CurSize.first = this->CurSize.second;
    this->CurSize.second = t;
} 


void Game::MoveLeft()
{
	if (AbleToMove(Direction::left))
	{
		MoveFigure(Direction::left);
	}
}

void Game::MoveRight()
{
	if (AbleToMove(Direction::right))
	{
		MoveFigure(Direction::right);
	}
}

Field Game::GetFigure()
{
	int ind = 1 + rand() % 7;
	switch (ind)
	{
	case 1:
		return this->Figures.vertical;
	case 2:
		return this->Figures.bwd_L_sh;
	case 3:
		return this->Figures.fwd_L_sh;
	case 4:
		return this->Figures.bwd_Z_sh;
	case 5:
		return this->Figures.fwd_Z_sh;
	case 6:
		return this->Figures.T_Shaped;
	case 7:
		return this->Figures.Square;
	default:
		break;
	}
}

void Game::Start()
{
	FillFigures();
	CurFigure = GetFigure();
	NextFigure = GetFigure();
	point coord;
	coord.x = 0;
	coord.y = 4;

	this->CurCoord.x = 0;
	this->CurCoord.y = 4;
	SetFigToField(CurFigure, coord);
}

void Game::FillFigures()
{
	freopen("Figures.txt", "r", stdin);
	for (int i = 0; i < 7; i++)
	{
		int n, m;
		cin >> n >> m;
		Field temp = vector< vector<int> >(n, vector<int>(m, 0));
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < m; k++)
			{
				cin >> temp[j][k];
			}
		}
		switch(i)
		{
			case 0:
				this->Figures.vertical = temp;
				break;
			case 1:
				this->Figures.bwd_L_sh = temp;
				break;
			case 2:
				this->Figures.fwd_L_sh = temp;
				break;
			case 3:
				this->Figures.bwd_Z_sh = temp;
				break;
			case 4:
				this->Figures.fwd_Z_sh = temp;
				break;
			case 5:
				this->Figures.T_Shaped = temp;
				break;
			case 6:
				this->Figures.Square = temp;
				break;
			default:
				break;
		}
	}
	fclose(stdin);
}

void Game::SetFigToField(Field& figure, point LUcoord)
{
	Field field = GetField();
	field[0] = std::vector<int>(FIELD_WIDTH, 0);

	for (int i = 0; i < (int)figure.size(); i++)
	{
		for (int j = 0; j < (int)figure[i].size(); j++)
		{
			field[LUcoord.x + i][LUcoord.y + j] = figure[i][j];
		}
	}
	this->CurCoord = LUcoord;
	this->CurSize.first = figure.size();
	this->CurSize.second = figure[0].size();

	this->MainField = field;
}

bool Game::isEmptyLine(int n)
{
	if(n > FIELD_HEIGHT-1)
		return false;

	for(int i = 0; i < MainField[n].size(); i++)
		if(MainField[n][i] != 0)
			return false;
	return true;
}

void Game::MoveFigure(Direction direction)
{
	Field cfield = Field(this->ClearField);
	Field fig = this->CurFigure;

	int n = this->CurSize.first;
	int m = this->CurSize.second;
	int x = this->CurCoord.x;
	int y = this->CurCoord.y;
	int delta_i = 0;
	int delta_j = 0;

	switch (direction)
	{
		case Direction::left:
		{
			this->CurCoord.y--;
			delta_j = -1;
			break;
		}
		case Direction::right:
		{
			this->CurCoord.y++;
			delta_j = 1;
			break;
		}
		case Direction::down:
		{
			this->CurCoord.x++;
			delta_i = 1;
			break;
		}
		case Direction::up:
			delta_i = -1;
			break;
		default:
			break;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if(fig[i][j] != 0)
				cfield[x + i + delta_i][y + j + delta_j] = fig[i][j];
		}
	}


	this->MainField = cfield;
}

bool Game::AbleToMove(Direction direction)
{	
	Field field = GetField();
	Field cfield = GetClearField();
	int n = this->CurSize.first;
	int m = this->CurSize.second;
	int x = this->CurCoord.x;
	int y = this->CurCoord.y;
	int delta_i = 0;
	int delta_j = 0;

	switch (direction)
	{
		case Direction::left:
			delta_j = -1;
			break;
		case Direction::right:
			delta_j = 1;
			break;
		case Direction::down:
			delta_i = 1;
			break;
		case Direction::up:
			delta_i = -1;
			break;
		default:
			break;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (x + i + delta_i >= this->FIELD_HEIGHT || x + i + delta_i < 0 ||
				y + j + delta_j >= this->FIELD_WIDTH  || y + j + delta_j < 0)
			{
				return false;
			} else
			if (CurFigure[i][j] != 0 && cfield[x + i + delta_i][y + j + delta_j] != 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool Game::AbleToRotate()
{
	Field cfield = GetClearField();

    int n = this->CurSize.first;
	int m = this->CurSize.second;
	int x = this->CurCoord.x;
	int y = this->CurCoord.y;
 
	if(x + m > this->FIELD_HEIGHT || y + n > this->FIELD_WIDTH)
		return false;

	for(size_t i(0); i < n; i++)
		for(size_t j(0); j < n; j++)
			if(cfield[x+i][y+j] != 0)
				return false;

	for(size_t i(0); i < n; i++)
		for(size_t j(0); j < m; j++)
			if(this->CurFigure[i][j] != this->MainField[x+i][y+j])
            	return false;

    for(size_t i(0); i < m; i++)
    {
        for(size_t j(0); j < n; j++)
        {
        	if(x+n-j >= this->FIELD_HEIGHT)
        		return false;
            if(cfield[x+n-j][y+i] != 0) 
            	return false;
        }
    }

    return true;
}


void Game::FieldReverse(Field& field)
{
	int size = field.size();
	for (int i = 0; i < (size / 2); i++)
	{
		swap(field[i], field[size - i]);
	}
}

void Game::CheckLines()
{
	std::vector<int> lines;

	for(int i = 0; i < MainField.size(); i++)
	{
		int k = 0;
		for(int j = 0; j < MainField[i].size(); j++)
		{
			if(MainField[i][j] != 0)
				k += 1;
		}

		if(k == MainField[i].size())
			lines.push_back(i);
	}

	std::reverse(std::begin(lines), std::end(lines));
	linesToDelete = lines;

	switch(linesToDelete.size())
	{
		case 1:
			score += 100;
			break;
		case 2:
			score += 300;
			break;
		case 3:
			score += 400;
			break;
		case 4:
			score += 1500;
			break;
	}

	if(score >= 1000*level)
		level++;
}

void Game::DeleteLines()
{
	int n;
	while(!linesToDelete.empty())
	{
		n = linesToDelete[0];
		for(int i(0); i < MainField[n].size(); i++)
			MainField[n][i] = 0;
		DownFieldAboveLine(n);
		CheckLines();
	}
}

int Game::GetLevel()
{
	return this->level;
}

void Game::IncreaseLevel()
{
	this->level++;
}

bool Game::isLose()
{
	for(auto i : ClearField[3])
	{
		if(i != 0)
			return true;
	}

	return false;
}