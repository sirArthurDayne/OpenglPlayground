
#include"Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/Test_Texture2D.h"
#include "tests/Texture3D.h"
#include"tests/MeshTest.h"

/*openGL--> is an implementation that runs & works on your GPU
openGL works as a state machine.
example for drawing a triangle:
1. generate a buffer
1.1 load data(anticlockwise)
1.2 generate an index buffer
2. select a shader 
3. draw a triangle with that data.
*/

bool BACK = false;
static void keyCallBack(GLFWwindow* win, int key, int scanCode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, GL_TRUE);
	//change render mode
	if (key ==  GLFW_KEY_1 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (key ==  GLFW_KEY_2 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
		BACK = true;
}

int main(void)
{
	GLFWwindow* mainWindow;
	//if doesnt work
	if (!glfwInit())
		return EXIT_FAILURE;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica con OpenGL", NULL, NULL);

	if (!mainWindow)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(mainWindow);

	if (glewInit() != GLEW_OK) std::cout << "GLEW FAILED\n";
	std::cout << glGetString(GL_VERSION) << std::endl;

	// Setup imGUI Platform/Renderer bindings
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	
	float delta_time = 0.0f;
	float lastFrame = 0.0f;

	
	//TEST FRAMEWORK SETUP
	test::Test* currentTest = nullptr;
	test::TestMenu* test_menu = new test::TestMenu(currentTest, mainWindow);
	currentTest = test_menu;
	test_menu->AddTest <test::Texture2D>("2D Orthographic + Texture");
	test_menu->AddTest<test::Texture3D>("3D Perspective + Texture");
	test_menu->AddTest<test::MeshTest>("Model Loading + Phong/Gouldard Lighting");
	
	Renderer renderer;
	while (!glfwWindowShouldClose(mainWindow))
	{
		renderer.Clear(0.0f, 0.0f, 0.0f, 1.0f);
		//get dt
		const float current_frame = float(glfwGetTime());
		delta_time = current_frame - lastFrame;
		lastFrame = current_frame;
		
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//changes in testFrame
		if (currentTest != nullptr)
		{
			currentTest->OnUserUpdate(delta_time);
			currentTest->OnRenderer();
			currentTest->OnGuiRenderer();
			ImGui::Begin("Tests List");
			if ((ImGui::Button("<-RETURN") || BACK) && currentTest != test_menu)
			{
				delete currentTest;
				currentTest = test_menu;
				BACK = false;
			}
			ImGui::End();
		}		
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		//swap buffers before draw 
		glfwSwapBuffers(mainWindow);
		//event manager
		glfwPollEvents();
		glfwSetKeyCallback(mainWindow, keyCallBack);
	}
	delete currentTest;
	if (currentTest != test_menu)
		delete test_menu;
	// Cleanup Imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return EXIT_SUCCESS;
}