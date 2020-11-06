#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Game.h"
using namespace std;

#include <cassert>
#include "History.h"
#include "Robot.h"
#include "Player.h"
#include "Arena.h"
#include "Game.h"

#define CHECKTYPE(f, t) { (void)(t)(f); }

void thisFunctionWillNeverBeCalled()
{
	// If the student deleted or changed the interfaces to the public
	// functions, this won't compile.  (This uses magic beyond the scope
	// of CS 2.)

	Robot(static_cast<Arena*>(0), 1, 1);
	CHECKTYPE(&Robot::row, int  (Robot::*)() const);
	CHECKTYPE(&Robot::col, int  (Robot::*)() const);
	CHECKTYPE(&Robot::move, void (Robot::*)());
	CHECKTYPE(&Robot::takeDamageAndLive, bool (Robot::*)());

	Player(static_cast<Arena*>(0), 1, 1);
	CHECKTYPE(&Player::row, int    (Player::*)() const);
	CHECKTYPE(&Player::col, int    (Player::*)() const);
	CHECKTYPE(&Player::age, int    (Player::*)() const);
	CHECKTYPE(&Player::isDead, bool   (Player::*)() const);
	CHECKTYPE(&Player::takeComputerChosenTurn, string(Player::*)());
	CHECKTYPE(&Player::stand, void   (Player::*)());
	CHECKTYPE(&Player::move, void   (Player::*)(int));
	CHECKTYPE(&Player::shoot, bool   (Player::*)(int));
	CHECKTYPE(&Player::setDead, void   (Player::*)());

	Arena(1, 1);
	CHECKTYPE(&Arena::rows, int     (Arena::*)() const);
	CHECKTYPE(&Arena::cols, int     (Arena::*)() const);
	CHECKTYPE(&Arena::player, Player* (Arena::*)() const);
	CHECKTYPE(&Arena::robotCount, int     (Arena::*)() const);
	CHECKTYPE(&Arena::nRobotsAt, int     (Arena::*)(int, int) const);
	CHECKTYPE(&Arena::display, void    (Arena::*)(string) const);
	CHECKTYPE(&Arena::addRobot, bool    (Arena::*)(int, int));
	CHECKTYPE(&Arena::addPlayer, bool    (Arena::*)(int, int));
	CHECKTYPE(&Arena::damageRobotAt, void    (Arena::*)(int, int));
	CHECKTYPE(&Arena::moveRobots, bool    (Arena::*)());

	Game(1, 1, 1);
	CHECKTYPE(&Game::play, void (Game::*)());
}

void doBasicTests()
{
	{
		Arena a(10, 20);
		assert(a.addPlayer(2, 6));
		Player* pp = a.player();
		assert(pp->row() == 2 && pp->col() == 6 && !pp->isDead());
		pp->move(UP);
		assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
		pp->move(UP);
		assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
		pp->setDead();
		assert(pp->row() == 1 && pp->col() == 6 && pp->isDead());
	}
	{
		Arena a(2, 2);
		assert(a.addPlayer(1, 1));
		assert(a.addRobot(2, 2));
		Player* pp = a.player();
		assert(a.moveRobots());
		assert(!pp->isDead());
		for (int k = 0; k < 1000 && !pp->isDead() && a.moveRobots(); k++)
			;
		assert(pp->isDead());
	}
	{
		Arena a(1, 4);
		assert(a.addPlayer(1, 1));
		assert(a.addRobot(1, 4));
		assert(a.addRobot(1, 4));
		assert(a.addRobot(1, 3));
		assert(a.robotCount() == 3 && a.nRobotsAt(1, 4) == 2);
		Player* pp = a.player();
		for (int k = 0; k < 1000 && a.robotCount() != 0; k++)
			pp->shoot(RIGHT);
		assert(a.robotCount() == 0);
		assert(a.addRobot(1, 4));
		for (int k = 0; k < 1000 && a.robotCount() != 0; k++)
			pp->takeComputerChosenTurn();
		assert(a.robotCount() == 0);
	}
	cout << "Passed all basic tests" << endl;
}

int main()
{
	// Initialize the random number generator.  (You don't need to
	// understand how this works.)
	srand(static_cast<unsigned int>(time(0)));

	// Create a game
	// Use this instead to create a mini-game:   
	//Game g(4, 5, 2);
	
	Game g(15, 18, 15);

	// Play the game
	g.play();
	
	//doBasicTests();

}