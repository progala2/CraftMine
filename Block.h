#ifndef _XKS_BLOCK_H_
#define _XKS_BLOCK_H_
#include "stdafx.h"
#include "ResourceManager.h"

enum BlockFlags {
	BLOCK_FLAG_IS_SOLID = 1
};
class Block {
	GLuint m_flags;
	GLuint m_textureID;
	static Block m_blockTable[BLCK_LAST+1];
public:
	explicit Block(GLuint flags = 1, GLuint textureID = 1)
		: m_flags(flags), m_textureID(textureID)	{}

	bool isSolid() const { return m_flags & BLOCK_FLAG_IS_SOLID; }
	GLuint getTextureID() const { return m_textureID; }

	static Block* getBlockTable() { return m_blockTable; }
};

#endif