#pragma once

#include <cglm/cglm.h>

#include "Texture.h"

typedef struct {

	Texture white, black, selection;
	int x, y;

	mat4* object_ptr;
	int* obj_loc_ptr;
	size_t* model_count_ptr;


} World;

void NewWorld(World* self, int x, int y, mat4* object_ptr, int* obj_loc_ptr, size_t* model_count_ptr)
{
	NewTexture(&self->white, "resources/isometric.png");
	NewTexture(&self->black, "resources/isometric2.png");
	NewTexture(&self->selection, "resources/selection.png");

	self->x = x;
	self->y = y;

	self->object_ptr = object_ptr;
	self->obj_loc_ptr = obj_loc_ptr;
	self->model_count_ptr = model_count_ptr;
}


void WorldRender(World* self) 
{
	for (int i = self->x; i > 0; i--)
	{
		for (int j = self->y; j > 0; j--)
		{
			float x = (float)(i - j);
			float y = (float)(i + j) * 0.5f;

			if ((i + j) % 2 == 0)
			{
				TextureBind(self->white, 0);
			}
			else
			{
				TextureBind(self->black, 0);
			}

			glm_translate_make(*self->object_ptr, (vec3) { x, y + FLOATING_ANIMATION - 5.0f, 0 });

			glUniformMatrix4fv(*self->obj_loc_ptr, 1, GL_FALSE, (float*)*self->object_ptr);

			glDrawElements(GL_TRIANGLES, *self->model_count_ptr, GL_UNSIGNED_BYTE, NULL);

			
		}

	}

	TextureUnbind();
}

void WorldSelectionRender(World* self, vec3 selectPos)
{
	float x = (float)(selectPos[0] - selectPos[1]);
	float y = (float)(selectPos[0] + selectPos[1]) * 0.5f;

	TextureBind(self->selection, 0);
	glm_translate_make(*self->object_ptr, (vec3) { x, y + FLOATING_ANIMATION - 4.0f, 0 });
	glUniformMatrix4fv(*self->obj_loc_ptr, 1, GL_FALSE, (float*)*self->object_ptr);
	glDrawElements(GL_TRIANGLES, *self->model_count_ptr, GL_UNSIGNED_BYTE, NULL);
	TextureUnbind();
}

void WorldTerminate(World* self) 
{
	TextureDelete(self->white);
	TextureDelete(self->black);
	TextureDelete(self->selection);
}
