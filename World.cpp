#include "stdafx.h"
#include "World.h"

//extern ResourceManager* resourceManager;

World::~World()
{
	Unload();
}

void World::UpdateProjectionMatrix() 
{
	Application* application = Application::getInstance();
	m_projectionMatrix = glm::perspective( application->GetFoV(), application->GetAspect(), 0.1f, application->GetClippingDistance() );
	m_gravityAcceleration = 9.83f;
}

void World::GenerateWorld(std::pair<const glm::ivec3, Chunk*>& chunk )
{
	std::srand(m_seed);
	glm::ivec3 pos(chunk.first.x * Chunk::ms_chunkSize.x, chunk.first.y * Chunk::ms_chunkSize.y, chunk.first.z * Chunk::ms_chunkSize.z);
	float p = std::rand()/float(RAND_MAX), r = std::rand()/float(RAND_MAX), 
		s = std::rand()/float(RAND_MAX), t = std::rand()/float(RAND_MAX);

	for(GLubyte x = 0; x < Chunk::ms_chunkSize.x; ++x)
	{
		for(GLubyte z = 0; z < Chunk::ms_chunkSize.z; ++z)
		{
			glm::vec3 pos2(pos.x + x, pos.z + z, t);
			pos2 *= (s + r) * (p + t);
			int max = 40 + int(75*glm::perlin(pos2));
			pos2.z = s;
			TextureTypeNumber block = TextureTypeNumber(4 * glm::perlin(pos2));
			for(GLubyte y = 0; y < max; ++y)
			{	
				chunk.second->Set(x, y, z, block);		
			}
		}
	}
}

glm::ivec3 World::transformPositionToChunk(const glm::vec3& pos) const
{ 

	return glm::ivec3(pos.x < 0 ? int((pos.x - Chunk::ms_chunkSize.x)/Chunk::ms_chunkSize.x) : int(pos.x/Chunk::ms_chunkSize.x), int(pos.y/Chunk::ms_chunkSize.y),
		pos.z < 0 ? int((pos.z - Chunk::ms_chunkSize.z)/Chunk::ms_chunkSize.z) : int(pos.z/Chunk::ms_chunkSize.z));
}

glm::ivec3 World::transformPositionToBlock(const glm::vec3& pos) const
{ 
	return glm::ivec3(pos.x < 0 ? 15 - std::abs((int(pos.x) - Chunk::ms_chunkSize.x)%Chunk::ms_chunkSize.x) : int(pos.x)%Chunk::ms_chunkSize.x, int(pos.y)%Chunk::ms_chunkSize.y,
		pos.z < 0 ? 15 - std::abs((int(pos.z) - Chunk::ms_chunkSize.z)%Chunk::ms_chunkSize.z) : int(pos.z)%Chunk::ms_chunkSize.z);
}

void World::Load()
{
	Application* application = Application::getInstance();
	ResourceManager* resourceManager = ResourceManager::getInstance();

	double time = glfwGetTime();

	UpdateProjectionMatrix();
	m_program = new CubeShader();
	m_program->Attach();
	m_seed = unsigned(glfwGetTime());
	m_texturesID = resourceManager->getTextureArrID();

	m_player = new Player(glm::vec3(0, 84, 0));
	m_player->setDelOnMove(DelegatePlayerOnMove::from_method<World, &World::onPlayerMove>(const_cast<World*>(this)));

	glm::ivec3 a;
	a.y = 0;
	GLint distance = GLint(application->GetClippingDistance()/Chunk::ms_chunkSize.z) + 4;
	m_distance = distance;

	for(a.z = -distance; a.z < distance; ++a.z)
	{
		for(a.x = -distance; a.x < distance; ++a.x)
		{
			m_chunks[a]= new Chunk(m_program);
		}
	}


	for(auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
	{
		glm::ivec3 pos(it->first);
		Chunk* obj = it->second;

		--pos.x;
		auto temp = m_chunks.find(pos);
		if(temp != m_chunks.end())
			obj->SetNeighbour(DIR_LEFT, temp->second);

		pos.x += 2;
		temp = m_chunks.find(pos);
		if(temp != m_chunks.end())
			obj->SetNeighbour(DIR_RIGHT, temp->second);

		pos.x = it->first.x;
		++pos.z;
		temp = m_chunks.find(pos);
		if(temp != m_chunks.end())
			obj->SetNeighbour(DIR_FRONT, temp->second);

		pos.z -= 2;
		temp = m_chunks.find(pos);
		if(temp != m_chunks.end())
			obj->SetNeighbour(DIR_BACK, temp->second);

		pos.z = it->first.z;
		--pos.y;
		temp = m_chunks.find(pos);
		if(temp != m_chunks.end())
			obj->SetNeighbour(DIR_DOWN, temp->second);

		pos.y += 2;
		temp = m_chunks.find(pos);
		if(temp != m_chunks.end())
			obj->SetNeighbour(DIR_UP, temp->second);
		m_chunksBuildingQueue.Add(*it);
	}
	for(int i = 0; i < 3; ++i) 
		m_threadBuilding[i] = new std::thread([this] {
			for(;;)
			{
				
				double time = glfwGetTime();
				while(m_chunksBuildingQueue.Size() != 0){
					auto obj = m_chunksBuildingQueue.Remove(); 
					GenerateWorld(obj);
					m_chunkUpdateQueue.Add(obj.second);
				}
				while(m_chunkUpdateQueue.Size() != 0){
					Chunk* obj = m_chunkUpdateQueue.Remove(); 
					obj->UpdateVertexes();
					m_chunkTransferQueue.Add(obj);
				}
				std::this_thread::sleep_for(std::chrono::duration<int>(1));
			}
			
	});
	for(int i = 0; i < 3; ++i)
	{
		m_threadBuilding[i]->detach();
	}
	printf(" %Lf s", glfwGetTime()-time);
}

void World::Unload()
{
	auto end = m_chunks.end();
	for(auto it = m_chunks.begin(); it != end; ++it)
	{
		delete it->second;
	}
	m_chunks.clear();
	delete m_player;
	delete m_program;
}

void World::Update(double dt)
{
	while(m_chunkTransferQueue.Size() > 0){
			Chunk* obj = m_chunkTransferQueue.Remove();
			obj->MoveToGraphic();
		}
	for(auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
		if(it->second->isModified()) m_chunkUpdateQueue.Add(it->second);

	m_player->Update(dt);
}

void World::Draw()
{
	Application* application = Application::getInstance();
	GLenum err = glGetError();
	GLubyte flag = VSBL_FCS_ALL;
	Camera* camera = m_player->getCamera();
	auto dir = camera->m_dir;

	GLfloat aspect = application->GetAspect();
	aspect -= (aspect-1)*0.5f;
	if(dir.x < -0.5 * aspect) 
		flag ^= VSBL_FCS_LEFT;
	if(dir.x > 0.5 * aspect) 
		flag ^= VSBL_FCS_RIGHT;
	if(dir.y < -0.5) 
		flag ^= VSBL_FCS_DOWN;
	if(dir.y > 0.5) 
		flag ^= VSBL_FCS_UP;
	if(dir.z < -0.5 * aspect) 
		flag ^= VSBL_FCS_BACK;
	if(dir.z > 0.5 * aspect) 
		flag ^= VSBL_FCS_FRONT;
	m_program->Attach();
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texturesID);

	glm::mat4 mvp = camera->LookAt();
	m_program->SetVision(mvp );
	m_program->SetProjection(m_projectionMatrix );
	m_program->SetLight(camera->m_pos);

	glm::vec3 cameraPos(camera->m_pos);
	for(auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
	{
		glm::vec3 pos = glm::vec3((it->first.x)*Chunk::ms_chunkSize.x, 0, (it->first.z)*Chunk::ms_chunkSize.z);
		glm::vec3 cpVec = pos - cameraPos;
		GLfloat length = std::max(cpVec.x < 0 ? -(cpVec.x + 15) : cpVec.x, cpVec.z < 0 ? -(cpVec.z + 15) : cpVec.z);
		if(length > application->GetClippingDistance()+7) continue;

		GLubyte cflag = VSBL_FCS_ALL;
		if(pos.x < cameraPos.x - Chunk::ms_chunkSize.x) cflag ^= VSBL_FCS_LEFT;
		if(pos.x > cameraPos.x) cflag ^= VSBL_FCS_RIGHT;
		if(pos.z < cameraPos.z - Chunk::ms_chunkSize.z) cflag ^= VSBL_FCS_BACK;
		if(pos.z > cameraPos.z) cflag ^= VSBL_FCS_FRONT;
		it->second->SetFacesToDraw(flag & cflag);

		m_program->SetWorldPos(pos);
		it->second->Draw();
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glBindVertexArray(0);
}

void World::onPlayerMove(DelegatePlayerOnMoveData info)
{
	glm::vec3 newPos = m_player->getPosition();
	glm::vec3& oldPos = std::get<1>(info);
	glm::ivec3 chunkPos = transformPositionToChunk(newPos);
	glm::ivec3 blockPos = transformPositionToBlock(newPos);

	if(m_chunks.find(chunkPos) != m_chunks.end() && m_chunks[chunkPos]->isSolid(blockPos.x, blockPos.y+1, blockPos.z))
	{
		if(!m_chunks[chunkPos]->isSolid(blockPos.x, blockPos.y+2, blockPos.z))
			m_player->setForce(glm::vec3(0, 0, 0)); 
		m_player->setVelocity(glm::vec3(0, 0, 0));
		m_player->setPosition(oldPos);
	}else
	{
		m_player->setForce(glm::vec3(0, -m_gravityAcceleration, 0)); 
	}
}