#define _CRT_SECURE_NO_WARNINGS
#define FLOATING_ANIMATION 0.5f * (float)sin(glfwGetTime())

#include "Window.h"

#include "Shader.h"
#include "Buffer.h"
#include "Pieces.h"
#include "World.h"

#include <cglm/cglm.h>
#include <time.h>

mat4 projection = GLM_MAT4_IDENTITY_INIT,
		 object = GLM_MAT4_IDENTITY_INIT,
		   view = GLM_MAT4_IDENTITY_INIT;

f64 t = 0.0;
u32 vao, vbo, ibo;

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "lib/Nuklear-master/nuklear.h"
#include "lib/Nuklear-master/nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 1024 * 512
#define MAX_ELEMENT_BUFFER 1024 * 128

void InputMgr(Window* window, vec3 v) 
{	
	if (InputIsKeyPressed(&window->input, GLFW_KEY_UP)) 
	{
		v[1] += 1.0f;
	}

	if (InputIsKeyPressed(&window->input, GLFW_KEY_DOWN))
	{
		v[1] -= 1.0f;
	}

	if (InputIsKeyPressed(&window->input, GLFW_KEY_RIGHT))
	{
		v[0] += 1.0f;
	}

	if (InputIsKeyPressed(&window->input, GLFW_KEY_LEFT))
	{
		v[0] -= 1.0f;
	}
}

int main(void) 
{	
	struct nk_glfw glfw = { 0 };
	struct nk_context* ctx;
	struct nk_colorf bg;

	WindowInit();

	Window window;
	NewWindow(&window, 1280, 720, "Chess", false);

	u32 count = NewModel(&vao, &vbo, &ibo);
	u32 shader = NewShader("resources/shaders/Vertex.glsl", "resources/shaders/Fragment.glsl");
	
	i32 proj_loc = glGetUniformLocation(shader, "projection");
	i32 obj_loc = glGetUniformLocation(shader, "object");
	i32 view_loc = glGetUniformLocation(shader, "view");

	Pieces p;
	NewPieces(&p, &object, &obj_loc, &count);

	World w;
	NewWorld(&w, 8, 8, &object, &obj_loc, &count);
	
	vec3 selecPos = { 0, 0, 0 };

	printf("Loading time: %lfs\n", glfwGetTime());

	ctx = nk_glfw3_init(&glfw, window.window, NK_GLFW3_INSTALL_CALLBACKS);
	struct nk_font_atlas* atlas;
	nk_glfw3_font_stash_begin(&glfw, &atlas);
	nk_glfw3_font_stash_end(&glfw);

	bg.r = 0.9f, bg.g = 0.9f, bg.b = 0.9f, bg.a = 1.0f;

	while(WindowIsRunning(&window)) 
	{
		InputMgr(&window, selecPos);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(bg.r, bg.g, bg.b, bg.a);

		glm_ortho((float)-window.width / 125.0f, (float)window.width / 125.0f, (float)-window.height / 125.0f, (float)window.height / 125.0f, +1.0f, -1.0f, projection);

		{
			glUseProgram(shader);

			ModelBeginDraw(vao);

			glUniformMatrix4fv(proj_loc, 1, GL_FALSE, (float*)projection);
			glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float*)view);
			
			WorldRender(&w);
			WorldSelectionRender(&w, selecPos);

			PiecesRender(&p, selecPos);

			ModelEndDraw();

			glUseProgram(0);
		}

		/*
		{
			nk_glfw3_new_frame(&glfw);
			int w = 400, h = 280;
			int wm = (window.width - w) / 2, hm = (window.height - h) / 2;
			if (nk_begin(ctx, "Menu", nk_rect(wm, hm, w, h), 
				NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
			{
				
				nk_layout_row_dynamic(ctx, 64, 1);
				if (nk_button_label(ctx, "Start Game"))
					fprintf(stdout, "Button pressed!\n");
				nk_button_set_behavior(ctx, NK_BUTTON_DEFAULT);

				nk_layout_row_dynamic(ctx, 64, 1);
				if (nk_button_label(ctx, "Options"))
					fprintf(stdout, "Button pressed!\n");
				nk_button_set_behavior(ctx, NK_BUTTON_DEFAULT);

				nk_layout_row_dynamic(ctx, 64, 1);
				if (nk_button_label(ctx, "About"))
					fprintf(stdout, "Button pressed!\n");
				nk_button_set_behavior(ctx, NK_BUTTON_DEFAULT);

				nk_layout_row_dynamic(ctx, 64, 1);
				if (nk_button_label(ctx, "Exit"))
					glfwSetWindowShouldClose(window.window, GLFW_TRUE);
				nk_button_set_behavior(ctx, NK_BUTTON_DEFAULT);
            }
			nk_end(ctx);
			nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
		}
		*/
		if (glfwGetTime() - t > 0.5) 
		{
			printf("\rDisplay %dx%d, Fps ~%d\t", window.width, window.height, (int)(1.0 / deltaTime));
			t = glfwGetTime();
		}
	}

	VBO_Delete(vbo);
	IBO_Delete(ibo);
	VAO_Delete(vao);

	glDeleteProgram(shader);

	WorldTerminate(&w);
	WindowTerminate(&window);

	glfwTerminate();

	return 0;
}
