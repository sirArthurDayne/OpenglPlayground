#pragma once

namespace test
{
	class Test
	{
	public:
		Test() { }
		~Test() {  }
		virtual void OnRenderer();
		virtual void OnUserUpdate(float deltaTime);
		virtual void OnGuiRenderer();
	};
}