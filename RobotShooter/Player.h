#ifndef PLAYER_H
#define PLAYER_H

class Arena;

class Player {
private:
	Arena * m_arena;
	int    m_row;
	int    m_col;
	int    m_age;
	bool   m_dead;

public:
	// Constructor
	Player(Arena *ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	int  age() const;
	bool isDead() const;

	bool nearbyRobot();
	bool canMoveUp();
	bool canMoveDown();
	bool canMoveLeft();
	bool canMoveRight();

	// Mutators
	std::string takeComputerChosenTurn();
	void   stand();
	void   move(int dir);
	bool   shoot(int dir);
	void   setDead();
};

#endif

