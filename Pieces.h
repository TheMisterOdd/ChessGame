#pragma once

#include "Texture.h"

#include <stdint.h>
#include <cglm/cglm.h>

#define FORWARD_MOVEMENT   (1 << 0)
#define DIAGONAL_MOVEMENT  (1 << 1)
#define L_MOVEMENT         (1 << 2)
#define SIDE_MOVEMENT      (1 << 3)

#define PEON			   (1 << 4)
#define TOWER			   (1 << 5)
#define HORSE              (1 << 6)
#define ALFIL              (1 << 7)
#define KING               (1 << 8)
#define QUEEN              (1 << 9)

#define WHITE              (1 << 10)
#define BLACK              (1 << 11)

#define INFINITE_MOVEMENTS (1 << 12)
#define ONE_MOVEMENT	   (1 << 13)
#define DOUBLE_MOVEMENT	   (1 << 13)

#define JUMP_OVER_PIECES   (1 << 14)

#define ABS(x) (int)(x * -1.0f)

typedef struct
{
	Texture* textures;
	vec3* positions;

	u16* flags;
	u8 totalPieces;

	// Pointers
	mat4* object_ptr;
	i32* obj_loc_ptr;
	u32* model_count_ptr;

} Pieces;

void NewPieces(Pieces* self, mat4* object_ptr, int* obj_loc_ptr, u32* model_count_ptr)
{
	self->textures = (Texture*)malloc(sizeof(Texture) * 6 * 2);

	self->positions = (vec3*)malloc(sizeof(vec3) * 32);
	self->flags = (u16*)malloc(sizeof(u16) * 32);
	self->totalPieces = 32;

	self->object_ptr = object_ptr;
	self->obj_loc_ptr = obj_loc_ptr;
	self->model_count_ptr = model_count_ptr;

	// Peons: 
	{
		NewTexture(&self->textures[0], "resources/peon1.png");
		NewTexture(&self->textures[1], "resources/peon2.png");

		for (int i = 0; i < 8; i++)
		{
			self->flags[i] = PEON | WHITE | FORWARD_MOVEMENT | DIAGONAL_MOVEMENT | DOUBLE_MOVEMENT;
			self->flags[i + 8] = PEON | BLACK | FORWARD_MOVEMENT | DIAGONAL_MOVEMENT | DOUBLE_MOVEMENT;

			glm_vec3_copy((vec3) { (float)i, 1, 0 }, self->positions[i]);
			glm_vec3_copy((vec3) { (float)i, 6, 0 }, self->positions[i + 8]);
		}
	}

	// Towers:
	{
		NewTexture(&self->textures[2], "resources/tower1.png");
		NewTexture(&self->textures[3], "resources/tower2.png");

		self->flags[16] = TOWER | WHITE | FORWARD_MOVEMENT | SIDE_MOVEMENT | INFINITE_MOVEMENTS;
		self->flags[17] = TOWER | WHITE | FORWARD_MOVEMENT | SIDE_MOVEMENT | INFINITE_MOVEMENTS;
		self->flags[18] = TOWER | BLACK | FORWARD_MOVEMENT | SIDE_MOVEMENT | INFINITE_MOVEMENTS;
		self->flags[19] = TOWER | BLACK | FORWARD_MOVEMENT | SIDE_MOVEMENT | INFINITE_MOVEMENTS;

		glm_vec3_copy((vec3) { 0, 0, 0 }, self->positions[16]);
		glm_vec3_copy((vec3) { 7, 0, 0 }, self->positions[17]);

		glm_vec3_copy((vec3) { 0, 7, 0 }, self->positions[18]);
		glm_vec3_copy((vec3) { 7, 7, 0 }, self->positions[19]);
	}

	// Horses:
	{
		NewTexture(&self->textures[4], "resources/horse1.png");
		NewTexture(&self->textures[5], "resources/horse2.png");

		self->flags[20] = HORSE | WHITE | L_MOVEMENT | ONE_MOVEMENT;
		self->flags[21] = HORSE | WHITE | L_MOVEMENT | ONE_MOVEMENT;
		self->flags[22] = HORSE | BLACK | L_MOVEMENT | ONE_MOVEMENT;
		self->flags[23] = HORSE | BLACK | L_MOVEMENT | ONE_MOVEMENT;

		glm_vec3_copy((vec3) { 1, 0, 0 }, self->positions[20]);
		glm_vec3_copy((vec3) { 6, 0, 0 }, self->positions[21]);

		glm_vec3_copy((vec3) { 1, 7, 0 }, self->positions[22]);
		glm_vec3_copy((vec3) { 6, 7, 0 }, self->positions[23]);
	}

	// Alfils:
	{
		NewTexture(&self->textures[6], "resources/alfil1.png");
		NewTexture(&self->textures[7], "resources/alfil2.png");

		self->flags[24] = ALFIL | WHITE | DIAGONAL_MOVEMENT;
		self->flags[25] = ALFIL | WHITE | DIAGONAL_MOVEMENT;
		self->flags[26] = ALFIL | BLACK | DIAGONAL_MOVEMENT;
		self->flags[27] = ALFIL | BLACK | DIAGONAL_MOVEMENT;

		glm_vec3_copy((vec3) { 2, 0, 0 }, self->positions[24]);
		glm_vec3_copy((vec3) { 5, 0, 0 }, self->positions[25]);

		glm_vec3_copy((vec3) { 2, 7, 0 }, self->positions[26]);
		glm_vec3_copy((vec3) { 5, 7, 0 }, self->positions[27]);
	}

	// Kings:
	{
		NewTexture(&self->textures[8], "resources/king1.png");
		NewTexture(&self->textures[9], "resources/king2.png");

		self->flags[28] = KING | WHITE | FORWARD_MOVEMENT | DIAGONAL_MOVEMENT | SIDE_MOVEMENT | ONE_MOVEMENT;
		self->flags[29] = KING | BLACK | FORWARD_MOVEMENT | DIAGONAL_MOVEMENT | SIDE_MOVEMENT | ONE_MOVEMENT;

		glm_vec3_copy((vec3) { 3, 0, 0 }, self->positions[28]);
		glm_vec3_copy((vec3) { 4, 7, 0 }, self->positions[29]);
	}

	// Queens:
	{
		NewTexture(&self->textures[10], "resources/queen1.png");
		NewTexture(&self->textures[11], "resources/queen2.png");

		self->flags[30] = QUEEN | WHITE | FORWARD_MOVEMENT | DIAGONAL_MOVEMENT | SIDE_MOVEMENT | ONE_MOVEMENT;
		self->flags[31] = QUEEN | BLACK | FORWARD_MOVEMENT | DIAGONAL_MOVEMENT | SIDE_MOVEMENT | ONE_MOVEMENT;

		glm_vec3_copy((vec3) { 4, 0, 0 }, self->positions[30]);
		glm_vec3_copy((vec3) { 3, 7, 0 }, self->positions[31]);
	}
}


void PiecesRender(Pieces* self, vec3 selectPos)
{
	for (i8 i = self->totalPieces; i >= 0; --i) {
		
		if (self->flags[i] & PEON)
		{
			if (self->flags[i] & WHITE)
			{
				TextureBind(self->textures[0], 0);
			}
			else
			{
				TextureBind(self->textures[1], 0);
			}
		}
		else if (self->flags[i] & TOWER)
		{
			if (self->flags[i] & WHITE)
			{
				TextureBind(self->textures[2], 0);
			}
			else
			{
				TextureBind(self->textures[3], 0);
			}
		}
		else if (self->flags[i] & HORSE)
		{
			if (self->flags[i] & WHITE)
			{
				TextureBind(self->textures[4], 0);
			}
			else
			{
				TextureBind(self->textures[5], 0);
			}
		}
		else if (self->flags[i] & ALFIL) 
		{
			if (self->flags[i] & WHITE)
			{
				TextureBind(self->textures[6], 0);
			}
			else
			{
				TextureBind(self->textures[7], 0);
			}
		}
		else if (self->flags[i] & KING) 
		{
			if (self->flags[i] & WHITE)
			{
				TextureBind(self->textures[8], 0);
			}
			else
			{
				TextureBind(self->textures[9], 0);
			}
		}
		else if (self->flags[i] & QUEEN)
		{
			if (self->flags[i] & WHITE)
			{
				TextureBind(self->textures[10], 0);
			}
			else
			{
				TextureBind(self->textures[11], 0);
			}
		}

		float x = (float)(self->positions[i][0] - self->positions[i][1]);
		float y = (float)(self->positions[i][0] + self->positions[i][1]) * 0.5f;

		
		if (
			(int)self->positions[i][0] == (int)selectPos[0] &&
			(int)self->positions[i][1] == (int)selectPos[1]
		)
		{
			glm_translate_make(*self->object_ptr, (vec3) { x, y + 0.25f * FLOATING_ANIMATION - 2.5f, 0 });

		} else 
		{
			glm_translate_make(*self->object_ptr, (vec3) { x, y + FLOATING_ANIMATION - 3.0f, 0 });
		}


		glUniformMatrix4fv(*self->obj_loc_ptr, 1, GL_FALSE, (float*)*self->object_ptr);
		glDrawElements(GL_TRIANGLES, *self->model_count_ptr, GL_UNSIGNED_BYTE, NULL);

		TextureUnbind();
	}
}