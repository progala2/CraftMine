#ifndef _XKS_PLAYER_
#define _XKS_PLAYER_
#include "stdafx.h"
#include "Creature.h"
#include "Camera.h"
#include "Delegate.h"

class Player;
typedef std::tuple<Player*, glm::vec3> DelegatePlayerOnMoveData;
typedef Delegate<DelegatePlayerOnMoveData> DelegatePlayerOnMove;

class Player : public Creature
{
public:
	Player(glm::vec3 pos = glm::vec3(0,0,0));
	~Player();

	void Draw() override;
	void Update(double dt) override;

	void setDelOnMove(DelegatePlayerOnMove& dlgPlr){m_delegateOnMove = dlgPlr;}

	void setPosition(const glm::vec3& pos) { m_position = pos; m_camera->m_pos = pos; m_camera->m_pos.y+=2;} 
	glm::vec3 getPosition() const { return m_position;} 

	void setForce(const glm::vec3& force) { m_force = force;} 
	glm::vec3 getForce() const { return m_force;} 

	void setVelocity(const glm::vec3& velocity) { m_velocity = velocity;} 
	glm::vec3 getVelocity() const { return m_velocity;} 

	Camera* getCamera() const {return m_camera;}

private:
	Camera* m_camera;

	Delegate<DelegatePlayerOnMoveData> m_delegateOnMove; 

	glm::vec3 m_position, m_velocity, m_force;
	float m_speed;
};

#endif