#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"

using namespace std;

class StudentWorld;

class GhostRacer;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, double size, int dir, int depth, double speed, bool collision, bool beSprayed);

    // Action to perform for each tick.
    virtual void doSomething() = 0;

    // Mark this actor as dead.
    void setDead() {
        m_alive = false;
    }

    // Is this actor dead?
    bool getAlive() const{
        return m_alive;
    }

    virtual bool isWhiteBorder() const {
        return false;
    }

    // Get this actor's world
    StudentWorld* world() const {
        return m_world;
    }

    // Get the car related to this actor
    GhostRacer* car() const {
        return m_car;
    }

    // Get this actor's vertical speed.
    double getVertSpeed() const {
        return m_vert_speed;
    }

    // Set this actor's vertical speed.
    void setVertSpeed(double speed) {
        m_vert_speed = speed;
    }

    bool getBeSprayed() {
        return m_be_sprayed;
    }

    // If this actor is affected by holy water projectiles, then inflict that
    // affect on it and return true; otherwise, return false.
    virtual bool beSprayedIfAppropriate() {
        return false;
    }
        
    // Does this object affect zombie cab placement and speed?
    virtual bool isCollisionAvoidanceWorthy() {
        return m_collision;
    }
    
    virtual void standardMove();
    bool overlap(Actor* a, Actor* b);

private:
    StudentWorld* m_world;
    GhostRacer* m_car;
    double m_vert_speed;
    bool m_alive;
    bool m_collision;
    bool m_be_sprayed;
};

class BorderLine : public Actor
{
public:
    BorderLine(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, bool isYellow);

protected:
    virtual void doSomething();

    virtual bool isWhiteBorder() const {
        return ! m_isYellow;
    }

private:
    bool m_isYellow;
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, double size, int dir, int hp, double speed, bool beSprayed);
    virtual bool isCollisionAvoidanceWorthy() {
        return true;
    }

    // Get hit points.
    int getHP() const;

    // Increase hit points by hp.
    void increaseHP(int hp) {
        m_hp += hp;
    }

    virtual bool beSprayedIfAppropriate();

    // Do what the spec says happens when hp units of damage is inflicted.
    // Return true if this agent dies as a result, otherwise false.
    virtual bool takeDamageAndPossiblyDie(int hp);

    // What sound should play when this agent is damaged but does not die?
    virtual int soundWhenHurt() {
        return SOUND_PED_HURT;
    }

    // What sound should play when this agent is damaged and dies?
    virtual int soundWhenDie() {
        return SOUND_PED_DIE;
    }

    virtual void deathRattle() {
        return;
    }

private:
    int m_hp;
};

class GhostRacer : public Agent
{
public:
    GhostRacer(StudentWorld* sw);

    virtual void doSomething();
    virtual int soundWhenDie(){
        return SOUND_NONE;
    }

    // How many holy water projectiles does the object have?
    int getNumSprays() const {
        return m_sprays;
    }

    // Increase the number of holy water projectiles the object has.
    void increaseSprays(int amt) {
        m_sprays += amt;
    }

    void move();
    void spin();

private:
    int m_sprays;
};

class Pedestrian : public Agent
{
public:
    Pedestrian(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, double size);

    // Get the pedestrian's horizontal speed
    double getHorizSpeed() const {
        return m_hori_speed;
    }

    // Set the pedestrian's horizontal speed
    void setHorizSpeed(double s) {
        m_hori_speed = s;
    }

    virtual void standardMove();

    void moveAndPossiblyPickPlan();

    virtual int soundWhenHurt() {
        return SOUND_PED_HURT;
    }

    virtual int soundWhenDie() {
        return SOUND_PED_DIE;
    }

    void setMovementPlanDistance(int value) {
        m_movementPlanDistance = value;
    }

    int getMovementPlanDistance() {
        return m_movementPlanDistance;
    }

private:
    int m_movementPlanDistance;
    double m_hori_speed;
};

class HumanPedestrian : public Pedestrian
{
public:
    HumanPedestrian(StudentWorld* sw, GhostRacer* car, double x, double y);

protected:
    virtual void doSomething();

    virtual bool beSprayedIfAppropriate();
};

class ZombiePedestrian : public Pedestrian
{
public:
    ZombiePedestrian(StudentWorld* sw, GhostRacer* car, double x, double y);

protected:
    virtual void doSomething();

    virtual void deathRattle();

private:
    int tickBeforeGrunt;
};

class ZombieCab : public Agent
{
public:
    ZombieCab(StudentWorld* sw, GhostRacer* car, double x, double y);

protected:
    virtual void doSomething();

    virtual int soundWhenHurt() {
        return SOUND_VEHICLE_HURT;
    }

    virtual int soundWhenDie() {
        return SOUND_VEHICLE_DIE;
    }

    double getHorizSpeed() const {
        return m_hori_speed;
    }

    virtual void deathRattle();

    // Set the pedestrian's horizontal speed
    void setHorizSpeed(double s) {
        m_hori_speed = s;
    }

    bool isCollisionAvoidanceWorthy(){
        return true;
    }

private:
    bool damaged;
    int m_hori_speed;
    int m_movementPlan;
};

class Spray : public Actor
{
public:
    Spray(StudentWorld* sw, GhostRacer* car, double x, double y, int dir);

protected:
    virtual void doSomething();
    
    bool getActivated() {
        return m_activated;
    }

    void attemptToActivate();

private:
    double m_travel_distance;
    bool m_activated;
};

class GhostRacerActivatedObject : public Actor
{
public:
    GhostRacerActivatedObject(StudentWorld* sw, GhostRacer* car, int imageID, double x, double y, double size, int dir);

    virtual void doSomething();
    
    // Do the object's special activity (increase health, spin Ghostracer, etc.)
    virtual void doActivity() = 0;

    // Return the object's increase to the score when activated.
    virtual int getScoreIncrease() const = 0;

    // Return the sound to be played when the object is activated.
    virtual int getSound() const {
        return SOUND_GOT_GOODIE;
    }

    // Return whether the object dies after activation.
    virtual bool selfDestructs() const {
        return true;
    }

    // Return whether the object is affected by a holy water projectile.
    virtual bool isSprayable() const {
        return false;
    }

    virtual bool isCollisionAvoidanceWorthy() {
        return false;
    }
};

class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(StudentWorld* sw, GhostRacer* car, double x, double y);

protected:
    virtual void doActivity();

    virtual int getScoreIncrease() const {
        return 0;
    }

    virtual int getSound() const {
        return SOUND_OIL_SLICK;
    }

    virtual bool selfDestructs() const {
        return false;
    }
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(StudentWorld* sw, GhostRacer* car, double x, double y);

protected:
    virtual void doActivity();

    virtual int getScoreIncrease() const {
        return 250;
    }
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(StudentWorld* sw, GhostRacer* car, double x, double y);

protected:
    virtual void doActivity();

    virtual int getScoreIncrease() const {
        return 50;
    }
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(StudentWorld* sw, GhostRacer* car, double x, double y);

protected:
    virtual void doActivity();

    virtual int getScoreIncrease() const {
        return 100;
    }

    virtual int getSound() const {
        return SOUND_GOT_SOUL;
    }
};

#endif // ACTOR_INCLUDED