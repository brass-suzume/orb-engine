#pragma once

namespace core
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class EngineStage : private NoCopy
	{

	public:
		virtual ~EngineStage() = 0;
		virtual void Uninit() = 0;
	};
}