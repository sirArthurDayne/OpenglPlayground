#pragma once

namespace test
{
	class Test
	{
	public:
		Test() { }
		~Test() {  }
		virtual void OnRenderer() = 0;
		virtual void OnUserUpdate(float deltaTime) = 0;
		virtual void OnGuiRenderer() = 0;
	};
}