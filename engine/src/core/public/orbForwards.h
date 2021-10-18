#pragma once

#include <Mouse.h>
#include <Keyboard.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

namespace core
{
	using MouseState = DirectX::Mouse::State;
	using KeyboardState = DirectX::Keyboard::State;

	namespace Colors = DirectX::Colors;

	using BoundingSphere = DirectX::BoundingSphere;
}