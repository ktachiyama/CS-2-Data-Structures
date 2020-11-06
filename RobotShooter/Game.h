#ifndef GAME_H
#define GAME_H

class Arena;

class Game {
private:
	Arena * m_arena;
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRobots);
	~Game();

	// Mutators
	void play();

};


#endif