#include <iostream>
#include <string>
#include "Arena.h"
#include "Robot.h"
#include "Player.h"
#include "globals.h"
using namespace std;

Arena::Arena(int nRows, int nCols) : m_history(nRows, nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nRobots = 0;
}

History& Arena:: history() {
	return m_history;
}

Arena::~Arena() {
	for (int rPosition = 0; rPosition != m_nRobots; rPosition++) {
		delete m_robots[rPosition];
	}
	delete m_player;
	//Delete the player and all remaining dynamically allocated robots.
}

int Arena::rows() const {
	return m_rows;
}

int Arena::cols() const {
	return m_cols;
}

Player* Arena::player() const {
	return m_player;
}

int Arena::robotCount() const {
	return m_nRobots;
}


//Return the number of robots at row r, column c.
int Arena::nRobotsAt(int r, int c) const {

	int numRobots = 0;

	for (int rArrayPos = 0; rArrayPos != m_nRobots; rArrayPos++) {
		if (m_robots[rArrayPos]->row() == r && m_robots[rArrayPos]->col() == c)
			numRobots++;
	}
	return numRobots;
}

void Arena::display(string msg) const {
	// Position (row,col) in the arena coordinate system is represented in
	// the array element grid[row-1][col-1]
	char grid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill the grid with dots
	for (r = 0; r < rows(); r++)
		for (c = 0; c < cols(); c++)
			grid[r][c] = '.';

	// Indicates each robot's position
	// If one robot is at some grid point, it is set to 'R'.
	// If it's 2 though 8, it's set to '2' through '8'.
	// For 9 or more, it's set to '9'.
	for (int rArrayPos = 0; rArrayPos < m_nRobots; rArrayPos++) {

		char &gridChar = grid[m_robots[rArrayPos]->row() - 1][m_robots[rArrayPos]->col() - 1];

		if (gridChar == '.') gridChar = 'R';
		else if (gridChar == 'R') gridChar = '2';
		else if (gridChar == '9') gridChar = '9';
		else gridChar++;
	}

	// Indicate player's position
	if (m_player != nullptr) {
		// Set the char to '@', unless there's also a robot there,
		// in which case set it to '*'.
		char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
		if (gridChar == '.')
			gridChar = '@';
		else
			gridChar = '*';
	}

	// Draw the grid
	clearScreen();
	for (r = 0; r < rows(); r++) {
		for (c = 0; c < cols(); c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

	// Write message, robot, and player info
	cout << endl;
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << robotCount() << " robots remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player." << endl;
	else {
		if (m_player->age() > 0)
			cout << "The player has lasted " << m_player->age() << " steps." << endl;
		if (m_player->isDead())
			cout << "The player is dead." << endl;
	}
}

bool Arena::addRobot(int r, int c) {
	// If MAXROBOTS have already been added, return false. If
	//r or c is a invalid row/comlumn, return false.
	if (m_nRobots >= MAXROBOTS)
		return false;
	if (r > m_rows || c > m_cols)
		return false;

	// dynamically allocate a new robot at coordinates (r,c).  Save the
	// pointer to the newly allocated robot and return true.
	m_robots[m_nRobots] = new Robot(this, r, c); 
	
	//incrament the number of robots
	m_nRobots++;

	return true;
}

bool Arena::addPlayer(int r, int c) {
	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Dynamically allocate a new Player and add it to the arena
	m_player = new Player(this, r, c);
	return true;
}

void Arena::damageRobotAt(int r, int c) {
	// Damage one robot at row r, column c if at least one is there.
	// If the robot does not survive the damage, destroy it.
	for (int rPosition = 0; rPosition < m_nRobots; rPosition++) {
		if (m_robots[rPosition]->row() == r && m_robots[rPosition]->col() == c) {
			if (m_robots[rPosition]->takeDamageAndLive() == false) {
				deleteRobot(rPosition);
			}
			return;
		}
	}
}

bool Arena::moveRobots() {
	for (int rPosition = 0; rPosition != m_nRobots; rPosition++) {
		m_robots[rPosition]->move();
		// Have the k-th robot in the arena make one move.
		// If that move results in that robot being in the same
		// position as the player, the player dies.
		// return true if the player is still alive, false otherwise
	}
	for (int rPosition = 0; rPosition != m_nRobots; rPosition++) {
		if (m_robots[rPosition]->row() == m_player->row() && 
			m_robots[rPosition]->col() == m_player->col()) {
				m_player->setDead();
		}
	}
		return !m_player->isDead();
}

void Arena::deleteRobot(int position) {
	//If the robot we want to delete is last in the array, 
	//simply delete the robot
	if (position == m_nRobots-1) {
		delete m_robots[position];
		m_robots[position] = nullptr;
		m_nRobots--;
		return;
	}

	//If it is not the last in the array, delete the robot and
	//move the rest of the robots down
	delete m_robots[position];
	for (position; position != m_nRobots - 1; position++) {
		m_robots[position] = m_robots[position + 1];
	}
	m_robots[m_nRobots - 1] = nullptr;
	m_nRobots--;
}