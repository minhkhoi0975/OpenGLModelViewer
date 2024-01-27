#pragma once

struct RendererSettings
{
	bool enableWireframeMode = false;
	bool enableDepthTesting = true;
	bool enableFaceCulling = true;
	bool isCulledFaceBack = true; // Only applied if enableFaceCulling is true, true = cull back face, false = cull front face
	bool isFrontFaceOrderCCW = true; // true = counter-clockwise, false = clockwise.
};