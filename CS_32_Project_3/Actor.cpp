#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

///////////////////////////////     ACTOR      ///////////////////////////////
Actor::Actor(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, double size, int dir, int depth, double speed, bool collision, bool beSprayed)
	:GraphObject(imageID, x, y, dir, size, depth)
{
	m_vert_speed = speed;
	m_world = sw;
	m_car = car;
	m_collision = collision;
	m_be_sprayed = beSprayed;
	m_alive = true;
}

void Actor::standardMove() {
	double new_X = getX();
	double new_Y = getY() + getVertSpeed() - car()->getVertSpeed();
	moveTo(new_X, new_Y);
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		setDead();
	}
}

bool Actor::overlap(Actor* a, Actor* b) {
	double delta_x = abs(a->getX() - b->getX());
	double delta_y = abs(a->getY() - b->getY());
	double rad_sum = a->getRadius() + b->getRadius();
	return (delta_x < rad_sum * 0.25 && delta_y < rad_sum * 0.6);
}


///////////////////////////////     BORDERLINE      ///////////////////////////////
BorderLine::BorderLine(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, bool isYellow) 
	:Actor(sw, car, imageID, x, y, 2.0, 0, 2, -4.0, false, false)
{
	m_isYellow = isYellow;
}

void BorderLine::doSomething() {
	standardMove();
}


///////////////////////////////    AGENT      ///////////////////////////////
Agent::Agent(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, double size, int dir, int hp, double speed, bool beSprayed)
	:Actor(sw, car, imageID, x, y, size, dir, 0, speed, true, beSprayed) 
{
	m_hp = hp;
}

bool Agent::beSprayedIfAppropriate() {
	if (!getBeSprayed()) {
		return false;
	}
	takeDamageAndPossiblyDie(1);
	return true;
}

bool Agent::takeDamageAndPossiblyDie(int hp) {
		m_hp -= hp;
		world()->playSound(soundWhenHurt());
		if (getHP() <= 0) {
			world()->playSound(soundWhenDie());
			deathRattle();
			this->setDead();
		}
		return getAlive();
	}

int Agent::getHP() const {
	return m_hp;
}


///////////////////////////////    GHOSTRACER      ///////////////////////////////
GhostRacer::GhostRacer(StudentWorld* sw)
	:Agent(sw, nullptr, IID_GHOST_RACER, 128, 32, 4.0, 90, 100, 0, false) 
{
	m_sprays = 10;
}

void GhostRacer::doSomething() {
	//if not alive
	if (getAlive() == false) {
		return;
	}

	//if at the boundary
	if (getX() <= ROAD_CENTER - ROAD_WIDTH / 2) {
		if (getDirection() > 90) {
			setDirection(82);
			world()->playSound(SOUND_VEHICLE_CRASH);
			if (!takeDamageAndPossiblyDie(10)) {
				return;
			}
			move();
			return;
		}
	}
	if (getX() >= ROAD_CENTER + ROAD_WIDTH / 2) {
		if (getDirection() < 90) {
			setDirection(98);
			world()->playSound(SOUND_VEHICLE_CRASH);
			if (!takeDamageAndPossiblyDie(10)) {
				return;
			}
			move();
			return;
		}
	}

	int key;
	if (world()->getKey(key)) {
		switch (key) {
		case KEY_PRESS_LEFT:
			if (this->getDirection() < 114) {
				this->setDirection(this->getDirection() + 8);
			}
			break;

		case KEY_PRESS_RIGHT:
			if (this->getDirection() > 66) {
				this->setDirection(this->getDirection() - 8);
			}
			break;

		case KEY_PRESS_UP:
			if (this->getVertSpeed() < 5) {
				this->setVertSpeed(this->getVertSpeed() + 1);
			}
			break;

		case KEY_PRESS_DOWN:
			if (this->getVertSpeed() > -1) {
				this->setVertSpeed(this->getVertSpeed() - 1);
			}
			break;

		case KEY_PRESS_SPACE:
			if (getNumSprays() > 0) {
				world()->addSpray();
				increaseSprays(-1);
			}
			break;

		default:
			break;
		}
	}
	move();
	return;
}

void GhostRacer::move() {
	double max_shift_per_tick = 4.0;
	int direction = this->getDirection();
	double delta_x = cos(direction * 3.14159 / 180) * max_shift_per_tick;
	double cur_x = this->getX();
	double cur_y = this->getY();
	this->moveTo(cur_x + delta_x, cur_y);
}

void GhostRacer::spin()
{
	int direction = randInt(1, 2);
	if (direction == 1) {
		int newDirection = getDirection() + randInt(5, 20);
		if (newDirection < 120) {
			setDirection(newDirection);
		}
		else {
			setDirection(120);
		}
	}
	else {
		int newDirection = getDirection() - randInt(5, 20);
		if (newDirection > 60) {
			setDirection(newDirection);
		}
		else {
			setDirection(60);
		}
	}
}


///////////////////////////////    PEDESTRIAN      ///////////////////////////////
Pedestrian::Pedestrian(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, double size)
	:Agent(sw, car, imageID, x, y, size, 0, 2, -4, true)  
{
	m_hori_speed = 0;
}

void Pedestrian::standardMove() {
	double new_X = getX() + getHorizSpeed();
	double new_Y = getY() + getVertSpeed() - car()->getVertSpeed();
	moveTo(new_X, new_Y);
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		setDead();
	}
}

void Pedestrian::moveAndPossiblyPickPlan()
{
	standardMove();
	m_movementPlanDistance--;
	if (m_movementPlanDistance > 0) {
		return;
	}

	do
	{
		setHorizSpeed(randInt(-3, 3));
	} while (getHorizSpeed() == 0);
	setMovementPlanDistance(randInt(4, 32));
	if (getHorizSpeed() > 0) {
		setDirection(0);
	}
	else if (getHorizSpeed() < 0) {
		setDirection(180);
	}
}


///////////////////////////////   HUMANPED      ///////////////////////////////
HumanPedestrian::HumanPedestrian(StudentWorld* sw, GhostRacer* car, double x, double y)
	:Pedestrian(sw, car, IID_HUMAN_PED, x, y, 2.0)
{

}

void HumanPedestrian::doSomething() {
	if (overlap(this, car())) {
		car()->setDead();
		return;
	}

	moveAndPossiblyPickPlan();
}

bool HumanPedestrian::beSprayedIfAppropriate() {
	setHorizSpeed(getHorizSpeed() * -1);
	if (getDirection() == 0) {
		setDirection(180);
	}
	else if (getDirection() == 180) {
		setDirection(0);
	}
	world()->playSound(soundWhenHurt());
	return true;
}


///////////////////////////////   ZOMBIEPED      ///////////////////////////////
ZombiePedestrian::ZombiePedestrian(StudentWorld* sw, GhostRacer* car, double x, double y)
	:Pedestrian(sw, car, IID_ZOMBIE_PED, x, y, 3.0)
{
	tickBeforeGrunt = 20;
}

void ZombiePedestrian::doSomething() {
	if (overlap(this, car())) {
		car()->takeDamageAndPossiblyDie(5);
		world()->playSound(soundWhenDie());
		setDead();
		return;
	}
	if (abs(getX() - car()->getX()) <= 30 && getY() > car()->getY()) {
		setDirection(270);
		if (getX() < car()->getX()) {
			setHorizSpeed(1);
		}
		else if (getX() > car()->getX()) {
			setHorizSpeed(-1);
		}
		else if (getX() == car()->getX()) {
			setHorizSpeed(0);
		}
		tickBeforeGrunt--;
		if (tickBeforeGrunt <= 0) {
			world()->playSound(SOUND_ZOMBIE_ATTACK);
			tickBeforeGrunt = 20;
		}
	}

	moveAndPossiblyPickPlan();
}

void ZombiePedestrian::deathRattle()
{
	int Chance = randInt(1, 5);
 	if (Chance == 1) {
		world()->addHG(this->getX(), this->getY());
	}
	return;
}

///////////////////////////////    SPRAY    ///////////////////////////////
Spray::Spray(StudentWorld* sw, GhostRacer* car, double x, double y, int dir)
	:Actor(sw, car, IID_HOLY_WATER_PROJECTILE, x, y, 1.0, dir, 1, 0, false, false) 
{
	m_activated = false;
	m_travel_distance = 0;
}

void Spray::doSomething() {
	attemptToActivate();
	moveForward(SPRITE_HEIGHT);
	m_travel_distance += SPRITE_HEIGHT;
	attemptToActivate();
	if (m_travel_distance == 160) {
		setDead();
	}
}

void Spray::attemptToActivate() {
	int size = world()->list().size();
	for (int i = 0; i < size; i++) {
		if (overlap(this, world()->list()[i])) {
			if (world()->list()[i]->beSprayedIfAppropriate() && !m_activated) {
				m_activated = true;
				setDead();
				return;
			}
		}
	}
}


///////////////////////////////   ZOMBIECAB   /////////////////////////
ZombieCab::ZombieCab(StudentWorld* sw, GhostRacer* car, double x, double y)
	: Agent(sw, car, IID_ZOMBIE_CAB, x, y, 4.0, 90, 3, -4, true)
{
	damaged = false;
	m_hori_speed = 0;
	m_movementPlan = 0;
}

void ZombieCab::doSomething() {
	if (overlap(this, car())) {
		if (!damaged) {
			world()->playSound(SOUND_VEHICLE_CRASH);
			car()->takeDamageAndPossiblyDie(20);
			if (getX() <= car()->getX()) {
				setHorizSpeed(-5);
				setDirection(120 + randInt(0, 19));
			}
			else if (getX() > car()->getX()) {
				setHorizSpeed(5);
				setDirection(60 - randInt(0, 19));
			}
			damaged = true;
		}
	}

	double new_X = getX() + getHorizSpeed();
	double new_Y = getY() + getVertSpeed() - car()->getVertSpeed();
	moveTo(new_X, new_Y);
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		setDead();
	}

	if (getVertSpeed() > car()->getVertSpeed()) {
		int size = world()->list().size();
		for (int i = 0; i < size; i++) {
			if (world()->list()[i]->isCollisionAvoidanceWorthy() &&
				getX() == world()->list()[i]->getX() &&
				getY() < world()->list()[i]->getY()){
				if (abs(world()->list()[i]->getY() - getY()) < 96) {
					setVertSpeed(getVertSpeed() - 0.5);
					return;
				}
			}
		}
	}

	if (getVertSpeed() <= car()->getVertSpeed()) {
		int size = world()->list().size();
		for (int i = 0; i < size; i++) {
			if (world()->list()[i]->isCollisionAvoidanceWorthy() && 
				getX() == world()->list()[i]->getX() &&
				getY() > world()->list()[i]->getY()) {
				if (abs(world()->list()[i]->getY() - getY()) < 96) {
					setVertSpeed(getVertSpeed() + 0.5);
					return;
				}
			}
		}
	}
	m_movementPlan--;
	if (m_movementPlan < 0) {
		m_movementPlan = randInt(4, 32);
		setVertSpeed(getVertSpeed() + randInt(-2, 2));
	}
}

void ZombieCab::deathRattle()
{
	int Chance = randInt(1, 5);
	if (Chance == 1) {
		world()->addOS(this->getX(), this->getY());
	}
	return;
}

///////////////////////////////   GRAO    ///////////////////////////////
GhostRacerActivatedObject::GhostRacerActivatedObject(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, double size, int dir)
	:Actor(sw, car, imageID, x, y, size, dir, 2, -4, false, false) 
{
}

void GhostRacerActivatedObject::doSomething() {
	standardMove();
	if (getScoreIncrease() == 100) {
		if (getDirection() >= 350) {
			setDirection(getDirection() - 350);
		}
		else {
			setDirection(getDirection() + 10);
		}
	}
	if (overlap(this, car())) {
		world()->playSound(getSound());
		world()->increaseScore(getScoreIncrease());
		doActivity();
		if (selfDestructs()) {
			setDead();
		}
	}
}


OilSlick::OilSlick(StudentWorld* sw, GhostRacer* car, double x, double y)
	:GhostRacerActivatedObject(sw, car, IID_OIL_SLICK, x, y, randInt(2, 5), 0)
{
}

void OilSlick::doActivity() {
	car()->spin();
}


HealingGoodie::HealingGoodie(StudentWorld* sw, GhostRacer* car, double x, double y)
	:GhostRacerActivatedObject(sw, car, IID_HEAL_GOODIE, x, y, 1.0, 0) 
{
}

void HealingGoodie::doActivity() {
	if (car()->getHP() > 90) {
		car()->increaseHP(100 - car()->getHP());
	}
	else {
		car()->increaseHP(10);
	}
}


HolyWaterGoodie::HolyWaterGoodie(StudentWorld* sw, GhostRacer* car, double x, double y)
	:GhostRacerActivatedObject(sw, car, IID_HOLY_WATER_GOODIE, x, y, 2.0, 90) 
{
}

void HolyWaterGoodie::doActivity() {
	car()->increaseSprays(10);
}


SoulGoodie::SoulGoodie(StudentWorld* sw, GhostRacer* car, double x, double y)
	: GhostRacerActivatedObject(sw,car, IID_SOUL_GOODIE, x, y, 4.0, 0)
{
}

void SoulGoodie::doActivity() {
	world()->recordSoulSaved();
}
