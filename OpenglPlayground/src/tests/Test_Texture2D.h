#pragma once
#include "Test.h"

namespace test 
{
	class Texture2D : public Test
	{
	public:
		Texture2D();
		~Texture2D();
		void OnRenderer() override;	
		void OnUserUpdate(float deltaTime) override;
		void OnGuiRenderer() override;
	private:
	};
}