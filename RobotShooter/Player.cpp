#include <iostream>
#include "Player.h"
#include "Arena.h"
#include "globals.h"
using namespace std;

Player::Player(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** The player must be in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_age = 0;
	m_dead = false;
}

int Player::row() const
{
	return m_row;
}

int Player::col() const
{
	return m_col;
}

int Player::age() const
{
	return m_age; 
}

string Player::takeComputerChosenTurn() {
	//move when there's a robot next to you, but not where 
	//you may encounter a robot
	if (nearbyRobot()) {
		if (canMoveUp()) { move(UP); return "Moved"; }
		if (canMoveDown()) { move(DOWN); return "Moved"; }
		if (canMoveRight()) { move(RIGHT); return "Moved"; }
		if (canMoveLeft()) { move(LEFT); return "Moved"; }
	}
	
	//if there's no robot next to you, shoot the nearest one
	//within max shooting distance
	if (!nearbyRobot()) {
		for (int shotlen = 1; shotlen != MAXSHOTLEN; shotlen++) {
			if (m_arena->nRobotsAt(m_row - shotlen, m_col) >= 1) {
				if (shoot(UP)) 
					return "Shot and hit!";
				else 
					return "Shot and missed!";
			}
			if (m_arena->nRobotsAt(m_row, m_col + shotlen) >= 1) {
				if (shoot(RIGHT)) 
					return "Shot and hit!";
				else 
					return "Shot and missed!";
			}
			if (m_arena->nRobotsAt(m_row + shotlen, m_col) >= 1) {
				if (shoot(DOWN))
					return "Shot and hit!";
				else
					return "Shot and missed!";
			}
			if (m_arena->nRobotsAt(m_row, m_col - shotlen) >= 1) {
				if (shoot(LEFT))
					return "Shot and hit!";
				else
					return "Shot and missed!";
			}
		}
	}

	//If you can't move to a safer place, or there's no 
	//robot next to you nor in shooting distance, simply stand
		stand(); 
		return "Stood";
}

//returns true if there's a robot nest to the player
bool Player::nearbyRobot() {
	if (m_arena->nRobotsAt(m_row - 1, m_col) >= 1) return true;
	if (m_arena->nRobotsAt(m_row + 1, m_col) >= 1) return true;
	if (m_arena->nRobotsAt(m_row, m_col + 1) >= 1) return true;
	if (m_arena->nRobotsAt(m_row, m_col - 1) >= 1) return true;
	else
		return false;
}

//returns false if moving up puts the player in a position in
//which they could possibly collide with a robot
bool Player::canMoveUp() {
	int r = m_row - 1;

	if (r < 1) return false;
	if (m_arena->nRobotsAt(r, m_col) >= 1) return false;
	if (m_arena->nRobotsAt(r - 1, m_col) >= 1) return false;
	if (m_arena->nRobotsAt(r, m_col - 1) >= 1) return false;
	if (m_arena->nRobotsAt(r, m_col + 1) >= 1) return false;
	else
		return true;
}

bool Player::canMoveDown() {
	int r = m_row + 1;

	if (r > m_arena->rows()) return false;
	if (m_arena->nRobotsAt(r, m_col) >= 1) return false;
	if (m_arena->nRobotsAt(r + 1, m_col) >= 1) return false;
	if (m_arena->nRobotsAt(r, m_col - 1) >= 1) return false;
	if (m_arena->nRobotsAt(r, m_col + 1) >= 1) return false;
	else
		return true;
}

bool Player::canMoveRight() {
	int c = m_col + 1;

	if (c > m_arena->cols()) return false;
	if (m_arena->nRobotsAt(m_row, c) >= 1) return false;
	if (m_arena->nRobotsAt(m_row, c + 1) >= 1) return false;
	if (m_arena->nRobotsAt(m_row - 1, c) >= 1) return false;
	if (m_arena->nRobotsAt(m_row + 1, c) >= 1) return false;
	else
		return true;
}

bool Player::canMoveLeft() {
	int c = m_col - 1;

	if (c < 1) return false;
	if (m_arena->nRobotsAt(m_row, c) >= 1) return false;
	if (m_arena->nRobotsAt(m_row, c - 1) >= 1) return false;
	if (m_arena->nRobotsAt(m_row - 1, c) >= 1) return false;
	if (m_arena->nRobotsAt(m_row + 1, c) >= 1) return false;
	else
		return true;
}

void Player::stand() {
	m_arena->history().record(m_row, m_col);
	m_age++;
}

//Moves the player in direction dir, if possible.
void Player::move(int dir)
{
	m_age++;
	switch (dir)
	{
	case UP:
		if (m_row != 1) 
			m_row -= 1;
		break;
	case DOWN:
		if (m_row != m_arena->rows())
			m_row += 1;
		break;
	case LEFT:
		if (m_col != 1)
			m_col -= 1;
		break;
	case RIGHT:
		if (m_col != m_arena->cols())
			m_col += 1;
		break;
	}
	m_arena->history().record(m_row, m_col);
}

bool Player::shoot(int dir) {
	m_age++;

	if (rand() % 3 == 0)  // miss with 1/3 probability
		return false;

	//Damage the nearest robot in direction dir, returning
	// true if a robot is hit and damaged, false if not hit.
	switch (dir) {
	case UP:
		for (int shotlen = 1; shotlen != MAXSHOTLEN; shotlen++) {
			int shotRow = m_row - shotlen;

			if (shotRow < 1)
					return false;

			if (m_arena->nRobotsAt(shotRow, m_col) > 0) {
				m_arena->damageRobotAt(shotRow, m_col);
				return true;
			}

		}
		break;

	case DOWN:
		for (int shotlen = 1; shotlen != MAXSHOTLEN; shotlen++) {
			int shotRow = m_row + shotlen;

			if (shotRow > m_arena->rows())
				return false;

			if (m_arena->nRobotsAt(shotRow, m_col) > 0) {
				m_arena->damageRobotAt(shotRow, m_col);
				return true;
			}
		}
		break;

	case LEFT:
		for (int shotlen = 1; shotlen != MAXSHOTLEN; shotlen++) {
			int shotCol = m_col - shotlen;

			if (shotCol < 1)
				return false;

				if (m_arena->nRobotsAt(m_row, shotCol) > 0) {
					m_arena->damageRobotAt(m_row, shotCol);
					return true;
				}
		}
		break;

	case RIGHT:
		for (int shotlen = 1; shotlen != MAXSHOTLEN; shotlen++) {
			int shotCol = m_col + shotlen;

			if (shotCol > m_arena->cols())
				return false;

			if (m_arena->nRobotsAt(m_row, shotCol) > 0) {
				m_arena->damageRobotAt(m_row, shotCol);
				return true;
			}
		}
		break;
	}
	return false;
}

bool Player::isDead() const {
	if (m_dead)
		return true;
	else
		return false;
}

void Player::setDead() {
	m_dead = true;
}