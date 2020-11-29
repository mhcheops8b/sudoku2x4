// sudoku2x4.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <fstream>

static const int fieldmap[8][8] = {
	{1,1,1,1,2,2,2,2},
	{1,1,1,1,2,2,2,2},
	{3,3,4,4,4,4,5,5},
	{3,3,4,4,4,4,5,5},
	{3,3,6,6,6,6,5,5},
	{3,3,6,6,6,6,5,5},
	{7,7,7,7,8,8,8,8},
	{7,7,7,7,8,8,8,8}
};

void error_place(std::ostream &os, char *buffer, char *current_position) {
	os << buffer << std::endl;
	for (int i = 0; i < (current_position - buffer); i++)
		os << " ";
	os << "^" << std::endl;
}

bool read_sudoku(char *filename, int(&sudoku_field)[8][8]) {

	std::ifstream infile;

	infile.open(filename, std::ifstream::in);

	if (infile.good()) {
		char buffer[512];
		int field_col = 0, field_row = 0, file_row = 0;

		do {
			infile.getline(buffer, 512);
			if ((infile.good() || infile.eof())) {
				char *pom = buffer;
				//skip_whitespaces
				while (*pom && (*pom == ' ' || *pom == '\t'))
					pom++;
				if (*pom == '#')
				{
					file_row++;
					continue;
				}
				while (*pom && field_col < 8) {
					//cout << *pom << endl;
					if (*pom >= '1' && *pom <= '9') {
						sudoku_field[field_row][field_col++] = (*pom - '0');
					}
					else {
						if (*pom == '.')
							sudoku_field[field_row][field_col++] = 0;
						else {
							if (*pom == '#')
								break;
							else
							{
								if (*pom == ' ' || *pom == '\t')
								{
									// ignore white space characters
								}
								else
								{
									error_place(std::cerr, buffer, pom);
									std::cerr << "Unknown character '" << *pom << "'." << std::endl;
									infile.close();
									return false;
								}
							}

						}
					}
					pom++;
				}
				if (field_col != 8)
				{
					std::cerr << "Incorrect number of character on row no. " << (file_row + 1) << '\n';
					std::cerr << buffer;
					infile.close();
					return false;
				}
				field_col = 0;
				field_row++;
				file_row++;
			}
		} while (infile.good() && field_row < 8);
		if (field_row != 8)
		{
			std::cerr << "Incorrect number of character of rows, expecting 8.\n";
			infile.close();
			return false;

		}
	}
	else {
		std::cerr << "Error opening file \"" << filename << "\"." << std::endl;
		infile.close();
		return false;
	}

	infile.close();
	return true;
}



bool can_be_filled(int r, int c, int elem, int(&sudoku)[8][8]) {
	// check column
	for (int i = 0; i < 8; i++) {
		if (sudoku[i][c] == elem) {
			//std::cerr << "Row: x:" << x << ", y: " << y << ", elem: " << elem << ", i: " << i << '\n';
			return false;
		}
	}
	
	//check row
	for (int i = 0; i < 8; i++) {
		if (sudoku[r][i] == elem) {
			//std::cerr << "Column: x:" << x << ", y: " << y << ", elem: " << elem << ", i: " << i << '\n';;
			return false;
		}
	}
	
	// check area
	int field_idx = fieldmap[r][c];
	
	for (int rr = 0; rr < 8; rr++)
		for (int cc = 0; cc < 8; cc++)
			if (fieldmap[rr][cc] == field_idx && sudoku[rr][cc] == elem) {		
				//std::cerr << "Field " << field_idx << ", x:" << x << ", y: " << y << ", elem: " << elem << ", i: " << i << '\n';
				return false;
			}

	return true;
}

static const char* usage_msg = "\n \
File syntax: empty field = '.', filled_field = <num>\n \
\n \
Table 8x8 of [.[0-9]] characters\n \
";

// global total number of solutions
int g_total_num_solutions = 0;

// sets also g_total_num_solutions
void solve(int x, int y, int(&sudoku)[8][8]) {
	//std::cout << "x = " << x << ", y = " << y << '\n';
	
	if (x >= 8) {
		g_total_num_solutions++;

		std::cout << "\nSolution:\n";
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)
				std::cout << sudoku[i][j];
			std::cout << '\n';
		}
	}
	else {
		if (sudoku[x][y] == 0) {

			for (int i = 1; i <= 8; i++) {
				if (can_be_filled(x, y, i, sudoku)) {
					sudoku[x][y] = i;
					if (y < 7)
						solve(x, y + 1, sudoku);
					else
						solve(x + 1, 0, sudoku);
					sudoku[x][y] = 0;
				}
			}

		}
		else {
			if (y < 7)
				solve(x, y + 1, sudoku);
			else
				solve(x + 1, 0, sudoku);
		}

	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <filename>\n";
		std::cout << usage_msg;
	}
	else
	{
		int sudoku[8][8];

		if (read_sudoku(argv[1], sudoku))
		{
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++)
					if (sudoku[i][j] == 0)
						std::cout << '.';
					else
						std::cout << sudoku[i][j];

				std::cout << '\n';
			}
			
			solve(0, 0, sudoku);
			if (g_total_num_solutions == 0)
				std::cout << "\nNo solution\n";
			else
				std::cout << "\nTotal number of solutions: " << g_total_num_solutions << '\n';
		}
	}
	return 0;
#if 0
	// No solution !
	int sudoku[8][8] = {
		{-1,-1, 7,-1,-1,-1,-1, 1},
		{-1, 4,-1, 6,-1,-1, 7, 5},
		{-1,-1,-1,-1, 8, 3, 6, 7},
		{-1,-1,-1, 7,-1,-1, 2, 3},
		{ 2, 7,-1, 3, 1,-1, 5,-1},
		{-1, 3,-1,-1,-1, 7, 1,-1},
		{-1,-1, 3,-1, 7, 5,-1, 2},
		{ 7,-1,-1,-1,-1, 1,-1, 5},
	};

	int sudoku2[8][8] = {
		{ -1,-1, 7,-1,-1,-1,-1, 1 },
		{ -1, 4,-1, 6,-1,-1, 7,-1 },
		{ -1,-1,-1,-1, 8,-1, 6,-1 },
		{ -1,-1,-1,-1,-1,-1,-1, 3 },
		{ 2,-1,-1,-1, 1,-1,-1,-1 },
		{ -1, 3,-1,-1,-1, 7,-1,-1 },
		{ -1,-1, 3,-1,-1, 5,-1, 2 },
		{ 7,-1,-1,-1,-1, 1,-1,-1 },
	};

	solve(0, 0, sudoku2);

	return 0;
#endif
}

