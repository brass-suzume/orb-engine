
#include "engine/public/engineRuntimeStage.h"

namespace proto
{
	class RuntimeStage : public core::EngineRuntimeStage
	{

	protected:
		virtual bool OnInit( const core::RuntimeStageInitData& initData ) override;
		virtual void OnUninit() override;
		virtual void OnFrameTick( const core::TickInfo& tickInfo ) override;
	};
}