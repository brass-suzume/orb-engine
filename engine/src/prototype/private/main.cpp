#include <Windows.h>

#include "protoRuntimeStage.h"

#include "engine/public/engine.h"
#include "engine/public/engineInitData.h"


int __stdcall WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	auto protoRuntimeStage = std::make_unique< proto::RuntimeStage >();

	core::EngineInitData initData{ "OrbProto", std::move( protoRuntimeStage ), "level/prototype" };
	core::Engine engine;
	engine.Start( std::move( initData ) );
	return 0;
}