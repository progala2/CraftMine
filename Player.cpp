#include "stdafx.h"
#include "Player.h"
#include "Application.h"

Player::Player(glm::vec3 pos):Creature(), m_position(pos), m_speed(10.0f), m_force(0,0,0), m_velocity(0,0,0)
{ 
	m_camera = new Camera(pos, 45.0f);
	m_camera->m_pos.y += 2;	
}

Player::~Player()
{
	delete m_camera;
}

void Player::Update(double dt)
{
	Application* application = Application::getInstance();
	double xpos, ypos;
	int width, height;
	float mouseSpeed = application->GetMouseSpeed();
	application->GetCursorPos(xpos, ypos);
	application->GetWindowWidthHeight(width, height);
	application->SetCursorPos( width/2, height/2);

	if(m_position.y < 0)
	{
		m_position.y = 0;
		m_velocity.y = 0;
	}else
	{
		m_velocity += m_force*float(dt);
	}

	// Compute new orientation
	m_camera->m_hAngle += mouseSpeed * float(dt * (width/2 - xpos ));
	float y = m_camera->m_vAngle + mouseSpeed * float(dt * ( height/2 - ypos ));
	if(y > -1.52 && y < 1.52)
		m_camera->m_vAngle   = y;

	glm::vec3 direction = glm::normalize(m_camera->GetDirection());
	m_camera->m_dir = direction;
	glm::vec3 right = m_camera->GetRightVec();

	glm::vec3 newPos(0,0,0);
	if (application->GetKey(GLFW_KEY_SPACE ) == GLFW_PRESS && m_velocity.y == 0){
		m_velocity.y = 15.0f;
	}
	// Move backward
	/*if (application->GetKey(GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){
		newPos += glm::vec3(0, -1, 0) * float(dt * m_speed);
	}*/
	direction.y = 0;
	direction = glm::normalize(direction);
	if (application->GetKey(GLFW_KEY_UP ) == GLFW_PRESS){
		newPos += direction * float(dt * m_speed);
	}
	// Move backward
	else if (application->GetKey(GLFW_KEY_DOWN ) == GLFW_PRESS){
		newPos -= direction * float(dt * m_speed);
	}
	// Strafe right
	if (application->GetKey(GLFW_KEY_RIGHT ) == GLFW_PRESS){
		newPos += right * float(dt * m_speed);
	}
	// Strafe left
	if (application->GetKey(GLFW_KEY_LEFT ) == GLFW_PRESS){
		newPos -= right * float(dt * m_speed);
	}
	if (application->GetKey(GLFW_KEY_1 ) == GLFW_PRESS){
		printf("\n%f %f %f \n", m_camera->m_dir.x, m_camera->m_dir.y, m_camera->m_dir.z);
	}
	if (application->GetKey(GLFW_KEY_2 ) == GLFW_PRESS){
		printf("\n%f %f %f \n", m_camera->m_pos.x, m_camera->m_pos.y, m_camera->m_pos.z);
	}

	newPos += m_velocity * float(dt);
	
	this->setPosition(m_position + newPos);

	m_delegateOnMove(std::make_tuple(this, m_position - newPos));
	if (application->GetKey(GLFW_KEY_3) == GLFW_PRESS)
	{
		setForce(glm::vec3(0, -9.83f, 0));
	}
}

void Player::Draw()
{

}