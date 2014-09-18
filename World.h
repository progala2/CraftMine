#ifndef _XKS_WORLD_H_
#define _XKS_WORLD_H_
#include "stdafx.h"
#include "CubeShader.h"
#include "Chunk.h"
#include "Utility.h"
#include "Camera.h"
#include "Application.h"
#include "ResourceManager.h"
#include "WQueue.h"
#include "Player.h"

	class World
	{
	public:
		~World();

		void Load();
		void Unload();
		void Update(double dt);
		void Draw();

		void GenerateWorld(std::pair<const glm::ivec3, Chunk*>& chunk );
		glm::ivec3 transformPositionToChunk(const glm::vec3& pos) const;
		glm::ivec3 transformPositionToBlock(const glm::vec3& pos) const;

		float getGravityAcceleration() const {return m_gravityAcceleration;}

		void onPlayerMove(std::tuple<Player*, glm::vec3> info);

		void UpdateProjectionMatrix();
	private:

		void MoveToGraphic();

		CubeShader *m_program;

		GLuint m_texturesID;
		std::map<glm::ivec3, Chunk* > m_chunks;
		Player* m_player;
		glm::mat4 m_viewMatrix, m_projectionMatrix;
		GLuint m_seed;
		GLint m_distance;
		std::thread* m_threadBuilding[3], *m_threadUpdate;
		WQueue<Chunk*>  m_chunkUpdateQueue, m_chunkTransferQueue;
		WQueue<std::pair<const glm::ivec3, Chunk*> > m_chunksBuildingQueue;

		float m_gravityAcceleration;
	};
#endif