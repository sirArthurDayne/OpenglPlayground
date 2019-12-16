#include "Test_Texture2D.h"
#include"gtc/matrix_transform.hpp"
#include"../Renderer.h"
#include "../VertexLayout.h"
#include "../imgui/imgui.h"
#include <GLFW/glfw3.h>

test::Texture2D::Texture2D():
	m_proy(glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT), -1.0f, 1.0f)),
	m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0))),
	m_rotate(glm::rotate(glm::mat4(1.0), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f))),
	m_scale(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f))),
	m_translationA(float(WIDTH)/2.0f, 400.0f, 0.0f),m_translationB(220.0f, 450.0f, 0.0f),
	m_scaleVec(1.0f, 1.0f, 1.0f)
{
	float tri_pos[] =
	{
		//POSITION		//TEXTURE
		100.0f, -100.0f,		1.0f, 0.0f,//0
		100.0f, 100.0f,		1.0f, 1.0f,//1
		-100.0f, -100.0f,		0.0f, 0.0f,//2
		-100.0f, 100.0f,		0.0f, 1.0f//3
	};
	unsigned int tri_indices[] =
	{
		2,0,1,//top triangle
		3,2,1,//down triangle
	};
	//BLENDING FOR TEXTURING
	/*ALPHA ECUATION
	 * out_alpha = src_alpha + dest_alpha * (1 - src_alpha)
	   If dest_alpha equals 1, then out_alpha also equals 1
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	/*Core OpenGL requires that we use a VAO so it knows what to do with our vertex inputs.
	If we fail to bind a VAO, OpenGL will most likely refuse to draw anything.*/
	//VAO stores all of the state needed to supply vertex data
	m_VAO = new VertexArray();
	m_VL = new VertexLayout();
	m_VL->Push<float>(2);//pos
	m_VL->Push<float>(2);//texture

	//generates an ID for vertex buffer object 
	m_VBO = new VertexBuffer(tri_pos, 4 * 4 * sizeof(float));
	m_VAO->AddBuffer(*m_VBO, *m_VL);

	//same way, the indices or elements needs a buffer to work, so generate a index buffer
	m_IBO = new IndexBuffer(tri_indices, 6);

	//shader code HELL YEAH!
	//c++17 structured binding	
	m_shader = new Shader("shaders/Base.shader");
	m_shader->SetUniform1i("u_texture0", 0);
	m_shader->SetUniform1i("u_texture1", 1);
	//TEXTURES
	m_texture = new Texture("fabric.png");
	m_texture2 = new Texture("piratelogo.png");
	
	//UNBOUND EVERYTHING
	m_VAO->Unbind();
	m_VBO->Unbind();
	m_IBO->Unbind();
	m_shader->Unbind();
}


void test::Texture2D::OnRenderer() 
{
	Renderer renderer;
	renderer.Clear(0.10f, 0.30f, 0.80f);
	
	//shader binding and uniform sending data
	m_texture->Bind();
	m_texture2->Bind(1);

	m_shader->Bind();
	
	m_shader->SetUniform2f("u_resolution", (float)WIDTH, (float)HEIGHT);
	m_shader->SetUniform1f("u_time", float(glfwGetTime()));
	m_scale = glm::scale(glm::mat4(1.0f), m_scaleVec);
	m_rotate = glm::rotate(glm::mat4(1.0f), -float(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
	
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
	model *= m_rotate * m_scale;
	glm::mat4 mvp = m_proy * m_view * model;
	
	m_shader->SetUniformMat4f("u_mvp", mvp);
	renderer.Draw(m_VAO, m_IBO);
	//second draw call
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
		glm::mat4 mvp = m_proy * m_view * model;
		m_shader->SetUniformMat4f("u_mvp", mvp);
		renderer.Draw(m_VAO, m_IBO);
	}
}

void test::Texture2D::OnGuiRenderer()
{
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImGui::Begin("Move Characters!");
	ImGui::SliderFloat3("coordenadas A", &m_translationA.x, 0.0f, float(WIDTH));
	ImGui::SliderFloat3("scale A", &m_scaleVec.x, 0.50f, 3.0);
	ImGui::SliderFloat3("coordenadas B", &m_translationB.x, 0.0f, float(WIDTH));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

}


test::Texture2D::~Texture2D()
{
	delete m_VBO;
	delete m_IBO;
	delete m_VL;
	delete m_VAO;
	delete m_shader;
	delete m_texture;
	delete m_texture2;
}
