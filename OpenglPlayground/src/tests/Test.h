#pragma once
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<functional>
#include<iostream>

/*USE ORDER
 * 1. OnUserUpdate
 * 2. OnRenderer
 * 3. OnGuiRenderer
 */
namespace test
{
	class Test
	{
	public:
		Test() { }
		virtual ~Test() {  }
		virtual void OnRenderer() {};
		virtual void OnUserUpdate(float deltaTime) {};
		virtual void OnGuiRenderer() {};
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test* &testPtr, GLFWwindow*& win);
		void OnGuiRenderer() override;
		template<class T>
		void AddTest(const std::string& name)
		{
			std::cout << "add scene: " << name << std::endl;
			m_TestList.push_back(std::make_pair(name, [&]() { return new T(this->m_win); }));
		}
	private:
		Test* & m_currentTestPtr;
		GLFWwindow* & m_win;
		std::vector<std::pair<std::string, std::function<Test * (void)>>> m_TestList;
	};
}