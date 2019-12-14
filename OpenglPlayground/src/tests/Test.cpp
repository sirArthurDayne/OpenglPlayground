#include "Test.h"
#include "../imgui/imgui.h"

test::TestMenu::TestMenu(Test* & testPtr):
	m_currentTestPtr(testPtr)
{}

void test::TestMenu::OnGuiRenderer()
{
	for (auto& test : m_TestList)
	{
		if (ImGui::Button(test.first.c_str()))
			m_currentTestPtr = test.second();
	}	
}