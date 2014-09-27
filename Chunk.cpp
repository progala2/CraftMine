#include "stdafx.h"
#include "Chunk.h"
#include "World.h"

const glm::vec3 Chunk::m_Normals[DIR_LAST+1] =
{
	glm::vec3(0,0,1),
	glm::vec3(0,0,-1),
	glm::vec3(0,1,0),
	glm::vec3(0,-1,0),
	glm::vec3(-1,0,0),
	glm::vec3(1,0,0)
};

CubeShader* Chunk::m_program = nullptr;

const glm::ivec3 Chunk::ms_chunkSize(16, 255, 16);

Chunk::Chunk(CubeShader* shader)
	:m_canDrawing(false), m_modified(false), m_facesToDraw(VSBL_FCS_ALL)
{
	if(m_program == nullptr)
		m_program = shader;
	GLenum err = glGetError();
	int attrib[2] = {m_program->GetAttrib("in_Position"), m_program->GetAttrib("vertexUV") };
	GLsizei vSize = sizeof( VertPosTexCompressed);
	for (int i = 0; i <= DIR_LAST; ++i)
	{
		m_vertex[i].reserve(15000);
		m_lastSize[i] = m_vertex[i].size();
		m_Neighbours[i] = nullptr;
		glGenVertexArrays(1, &m_vertexVAO[i]);
		glBindVertexArray(m_vertexVAO[i] );
		glGenBuffers(1, &m_vertexVBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO[i]);
		for (unsigned j = 0; j < 2; ++j)
		{
			glEnableVertexAttribArray(attrib[j]);
			glVertexAttribPointer(attrib[j], VertPosTexCompressed::Attributes[j].size, VertPosTexCompressed::Attributes[j].type, 
									VertPosTexCompressed::Attributes[j].normalized, vSize, VertPosTexCompressed::Attributes[j].offset);
			
		}
		
	}
	glBindVertexArray(0);	
	m_chunkData.set_empty_key(Vec2ub(255,255));
	m_chunkData.set_deleted_key(Vec2ub(254,255));
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		throw std::exception("Blad tworzenia buforow!");
	}
}

Chunk::~Chunk()
{
	glDeleteBuffers(DIR_LAST + 1, m_vertexVBO);
	for (int i = 0; i <= DIR_LAST; ++i)
	{
		glBindVertexArray(m_vertexVAO[i] );
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
	glBindVertexArray(0);
	glDeleteVertexArrays(DIR_LAST + 1, m_vertexVAO);
}

void Chunk::Draw()
{
	if(!m_canDrawing) return;
	int offset = 0;
	for (int i = 0; i <= DIR_LAST; ++i)
	{
		if(m_lastSize[i] != 0 && (m_facesToDraw & (1<<i) ))
		{
			m_program->SetNormal(m_Normals[i]);
			glBindVertexArray(m_vertexVAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, m_lastSize[i]);
		}
		offset += m_lastSize[i];
	}
}

void Chunk::MoveToGraphic()
{
	GLsizeiptr vSize = sizeof(VertPosTexCompressed);
	for (int i = 0; i <= DIR_LAST; ++i)
	{
		if(!m_vertex[i].empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO[i]);
			glBufferData(GL_ARRAY_BUFFER, vSize*m_vertex[i].size(), &m_vertex[i][0] , GL_DYNAMIC_DRAW);			
		}
		m_lastSize[i] = m_vertex[i].size();
		m_vertex[i].clear();
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_canDrawing = true;
}

void Chunk::LoadChunkData(ChunkData& data)
{
	m_chunkData.swap(data); 
}

void Chunk::SetNeighbour(DIRECTIONS flag, Chunk* obj)
{
	m_Neighbours[flag] = obj;
}

void Chunk::Set(GLubyte x, GLubyte y, GLubyte z, BlockTypeNumber tex)
{
	m_chunkData[Vec2ub(x,y,z)] = tex;
}

void Chunk::Delete(GLubyte x, GLubyte y, GLubyte z)
{
	m_chunkData.erase(Vec2ub(x,y,z));
	if(x == ms_chunkSize.x-1)
		if(m_Neighbours[DIR_RIGHT] != nullptr)
			m_Neighbours[DIR_RIGHT]->ChunkModified();
	else if(x == 0)
		if(m_Neighbours[DIR_LEFT] != nullptr)
			m_Neighbours[DIR_LEFT]->ChunkModified();
	if(z == ms_chunkSize.z-1)
		if(m_Neighbours[DIR_FRONT] != nullptr)
			m_Neighbours[DIR_FRONT]->ChunkModified();
	else if(z == 0)
		if(m_Neighbours[DIR_BACK] != nullptr)
			m_Neighbours[DIR_BACK]->ChunkModified();
}

void Chunk::UpdateVertexes()
{
	for(int i = 0; i <= DIR_LAST; ++i)
	{
		m_vertex[i].clear();
		m_vertex[i].reserve(m_lastSize[i]+1);
	}
	auto end = m_chunkData.end();
	for(auto it = m_chunkData.begin(); it!=end; ++it)
	{
		GLubyte fl = VSBL_FCS_NOTHING;
		BlockTypeNumber tex[6];
		GLubyte z = it->first.xz >> 4;
		GLubyte x = it->first.xz - (z<<4), y = it->first.y;

		if(z < Chunk::ms_chunkSize.z - 1)
		{
			if(m_chunkData.find(Vec2ub(x, y, z + 1)) == end)
			{
				fl += VSBL_FCS_FRONT;
				tex[DIR_FRONT] = it->second;
			}
		// We check neighbours
		}else if(m_Neighbours[DIR_FRONT] == nullptr || !m_Neighbours[DIR_FRONT]->isSolid(x, y, 0))
		{
			fl += VSBL_FCS_FRONT;
			tex[DIR_FRONT] = it->second;
		}
		if(z > 0)
		{
			if(m_chunkData.find(Vec2ub(x, y, z - 1)) == end)
			{
				fl += VSBL_FCS_BACK;
				tex[DIR_BACK] = it->second;
			}
		}else if(m_Neighbours[DIR_BACK] == nullptr || !m_Neighbours[DIR_BACK]->isSolid(x , y, Chunk::ms_chunkSize.z - 1))
		{
			fl += VSBL_FCS_BACK;
			tex[DIR_BACK] = it->second;
		}
		if(y < Chunk::ms_chunkSize.y - 1)
		{
			if(m_chunkData.find(Vec2ub(x, y + 1, z)) == end)
			{
				fl += VSBL_FCS_UP;
				tex[DIR_UP] = it->second;
			}
		}else if(m_Neighbours[DIR_UP] == nullptr || !m_Neighbours[DIR_UP]->isSolid(x, 0, z))
		{
			fl += VSBL_FCS_UP;
			tex[DIR_UP] = it->second;
		}
		if(y > 0)
		{
			if(m_chunkData.find(Vec2ub(x, y - 1, z)) == end)
			{
				fl += VSBL_FCS_DOWN;
				tex[DIR_DOWN] = it->second;
			}
		}/*else if(m_Neighbours[DIR_DOWN] == nullptr || !m_Neighbours[DIR_DOWN]->isSolid(Vec3ub(x, Chunk::ms_chunkSize.y - 1, z)))
		{
			fl += VSBL_FCS_DOWN;
			tex[DIR_DOWN] = it->second;
		}*/
		if(x < Chunk::ms_chunkSize.x - 1)
		{
			if(m_chunkData.find(Vec2ub(x + 1, y, z)) == end)
			{
				fl += VSBL_FCS_RIGHT;
				tex[DIR_RIGHT] = it->second;
			}
		}else if(m_Neighbours[DIR_RIGHT] == nullptr || !m_Neighbours[DIR_RIGHT]->isSolid(0, y, z))
		{
			fl += VSBL_FCS_RIGHT;
			tex[DIR_RIGHT] = it->second;
		}
		if(x > 0)
		{
			if(m_chunkData.find(Vec2ub(x - 1, y, z)) == end)
			{
				fl += VSBL_FCS_LEFT;
				tex[DIR_LEFT] = it->second;
			}
		}else if(m_Neighbours[DIR_LEFT] == nullptr || !m_Neighbours[DIR_LEFT]->isSolid(Chunk::ms_chunkSize.x - 1, y, z))
		{
			fl += VSBL_FCS_LEFT;
			tex[DIR_LEFT] = it->second;
		}
		AddFace(x, y, z, fl, tex);
	}
	for(int i = 0; i <= DIR_LAST; ++i)
	{
		m_vertex[i].shrink_to_fit();
	}
}

bool Chunk::isSolid(GLubyte x, GLubyte y, GLubyte z) const
{
	return m_chunkData.find(Vec2ub(x,y,z)) != m_chunkData.end();
}

void Chunk::AddFace(GLubyte x, GLubyte y, GLubyte z, GLubyte flag, BlockTypeNumber tex[6])
{
	GLubyte x1 = x+1, y1 = y+1, z1 = z+1;
	if(flag & VSBL_FCS_FRONT)
	{	
		VertPosTexCompressed two(Vec3ub(x, y, z1), Vec2us(2, tex[DIR_FRONT]));
		VertPosTexCompressed three(Vec3ub(x1, y, z1), Vec2us(3, tex[DIR_FRONT]));
		VertPosTexCompressed one(Vec3ub(x1, y1, z1), Vec2us(1, tex[DIR_FRONT]));
		VertPosTexCompressed zero(Vec3ub(x, y1, z1), Vec2us(0, tex[DIR_FRONT]));
		m_vertex[DIR_FRONT].push_back(two);
		m_vertex[DIR_FRONT].push_back(three);
		m_vertex[DIR_FRONT].push_back(one);	
		m_vertex[DIR_FRONT].push_back(two);
		m_vertex[DIR_FRONT].push_back(one);
		m_vertex[DIR_FRONT].push_back(zero);	
	}
	if(flag & VSBL_FCS_BACK)
	{
		VertPosTexCompressed two(Vec3ub(x1, y, z), Vec2us(2, tex[DIR_BACK]));
		VertPosTexCompressed three(Vec3ub(x, y, z), Vec2us(3, tex[DIR_BACK]));
		VertPosTexCompressed zero(Vec3ub(x1, y1, z), Vec2us(0, tex[DIR_BACK]));
		VertPosTexCompressed one(Vec3ub(x, y1, z), Vec2us(1, tex[DIR_BACK]));
		m_vertex[DIR_BACK].push_back(two);
		m_vertex[DIR_BACK].push_back(three);
		m_vertex[DIR_BACK].push_back(zero);
		m_vertex[DIR_BACK].push_back(three);
		m_vertex[DIR_BACK].push_back(one);
		m_vertex[DIR_BACK].push_back(zero);
	}
	
	if(flag & VSBL_FCS_UP)
	{
		VertPosTexCompressed two(Vec3ub(x, y1, z1), Vec2us(2, tex[DIR_UP]));
		VertPosTexCompressed three(Vec3ub(x1, y1, z1), Vec2us(3, tex[DIR_UP]));
		VertPosTexCompressed zero(Vec3ub(x, y1, z), Vec2us(0, tex[DIR_UP]));
		VertPosTexCompressed one(Vec3ub(x1, y1, z), Vec2us(1, tex[DIR_UP]));

		m_vertex[DIR_UP].push_back(two);
		m_vertex[DIR_UP].push_back(three);
		m_vertex[DIR_UP].push_back(zero);
		m_vertex[DIR_UP].push_back(three);
		m_vertex[DIR_UP].push_back(one);
		m_vertex[DIR_UP].push_back(zero);
	}
	if(flag & VSBL_FCS_DOWN)
	{
		VertPosTexCompressed two(Vec3ub(x, y, z), Vec2us(2, tex[DIR_DOWN]));
		VertPosTexCompressed three(Vec3ub(x1, y, z), Vec2us(3, tex[DIR_DOWN]));
		VertPosTexCompressed zero(Vec3ub(x, y, z1), Vec2us(0, tex[DIR_DOWN]));
		VertPosTexCompressed one(Vec3ub(x1, y, z1), Vec2us(1, tex[DIR_DOWN]));

		m_vertex[DIR_DOWN].push_back(two);
		m_vertex[DIR_DOWN].push_back(three);
		m_vertex[DIR_DOWN].push_back(zero);
		m_vertex[DIR_DOWN].push_back(three);
		m_vertex[DIR_DOWN].push_back(one);
		m_vertex[DIR_DOWN].push_back(zero);
	}
	if(flag & VSBL_FCS_LEFT)
	{
		VertPosTexCompressed two(Vec3ub(x, y, z), Vec2us(2, tex[DIR_LEFT]));
		VertPosTexCompressed three(Vec3ub(x, y, z1), Vec2us(3, tex[DIR_LEFT]));
		VertPosTexCompressed zero(Vec3ub(x, y1, z), Vec2us(0, tex[DIR_LEFT]));
		VertPosTexCompressed one(Vec3ub(x, y1, z1), Vec2us(1, tex[DIR_LEFT]));

		m_vertex[DIR_LEFT].push_back(two);
		m_vertex[DIR_LEFT].push_back(three);
		m_vertex[DIR_LEFT].push_back(zero);
		m_vertex[DIR_LEFT].push_back(three);
		m_vertex[DIR_LEFT].push_back(one);
		m_vertex[DIR_LEFT].push_back(zero);
	}
	if(flag & VSBL_FCS_RIGHT)
	{
		VertPosTexCompressed two(Vec3ub(x1, y, z1), Vec2us(2, tex[DIR_RIGHT]));
		VertPosTexCompressed three(Vec3ub(x1, y, z), Vec2us(3, tex[DIR_RIGHT]));
		VertPosTexCompressed zero(Vec3ub(x1, y1, z1), Vec2us(0, tex[DIR_RIGHT]));
		VertPosTexCompressed one(Vec3ub(x1, y1, z), Vec2us(1, tex[DIR_RIGHT]));

		m_vertex[DIR_RIGHT].push_back(two);
		m_vertex[DIR_RIGHT].push_back(three);
		m_vertex[DIR_RIGHT].push_back(zero);
		m_vertex[DIR_RIGHT].push_back(three);
		m_vertex[DIR_RIGHT].push_back(one);
		m_vertex[DIR_RIGHT].push_back(zero);
	}
}