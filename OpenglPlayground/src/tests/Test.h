#pragma once
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
		TestMenu(Test* &testPtr);
		void OnGuiRenderer() override;
		template<class T>
		void AddTest(const std::string& name)
		{
			std::cout << "add test: " << name << std::endl;
			m_TestList.push_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		Test* & m_currentTestPtr;
		std::vector<std::pair<std::string, std::function<Test * ()>>> m_TestList;
	};
}