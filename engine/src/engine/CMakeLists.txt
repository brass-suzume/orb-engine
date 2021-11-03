project( orbEngine )

add_library( ${PROJECT_NAME} )

target_sources( ${PROJECT_NAME}
					PRIVATE
						private/dataCenter.cpp
						private/engine.cpp
						private/engineStage.cpp
						private/engineWindowInitStage.cpp
						private/engineWindowInitStage.h
						private/engineRenderingInitStage.cpp
						private/engineRenderingInitStage.h
						private/engineRuntimeStage.cpp
						private/engineInitData.cpp
						private/renderer.h
						private/renderer.cpp
						private/level.cpp
						private/resRef.cpp
						private/mathSerialization.cpp
						private/mesh.cpp
						private/serializable.cpp
						private/serializationHelpers.cpp

						private/gameBase/gameRuntimeStage.cpp
						private/gameBase/gameInstance.cpp	
						private/gameBase/gameSystem.cpp
						private/gameBase/tickPerformer.cpp
						private/gameBase/tickPerformer.h
						private/gameBase/controller.cpp
						private/gameBase/player.cpp
						private/gameBase/controllerSystem.cpp
						private/gameBase/controllerSystem.h
						private/gameBase/gameMode.cpp
						private/gameBase/cameraSystem.cpp
						private/gameBase/cameraSystem.h
						private/gameBase/physicsSystem.cpp
						private/gameBase/physicsSystem.h

						private/ecs/entity.cpp
						private/ecs/entityRef.cpp
						private/ecs/component.cpp
						private/ecs/positionComponent.cpp
						private/ecs/entitySystem.cpp
						private/ecs/entitySystem.h

						private/components/meshComponent.cpp
						private/components/inputComponent.cpp
						private/components/cameraComponent.cpp
						private/components/physicsComponent.cpp

						private/ui/uiSystem.cpp
						private/ui/uiSystem.h
		
						private/ui/elements/uiRootView.cpp
						private/ui/elements/uiElement.cpp	
						private/ui/elements/uiText.cpp
						private/ui/elements/uiButton.cpp

					PUBLIC
						public/callbackRegistry.h
						public/dataCenter.h
						public/engine.h						
						public/engineRuntimeStage.h
						public/engineInitData.h
						public/inputHelpers.h
						public/engineStage.h
						public/engineTypes.h
						public/level.h
						public/mathSerialization.h
						public/mesh.h
						public/orbForwards.h
						public/rendererInterface.h
						public/rendererTypes.h
						public/resRef.h
						public/serializable.h
						public/serializationHelpers.h
						public/type.h
						public/typeRegistrar.h

						public/gameBase/gameRuntimeStage.h
						public/gameBase/gameInstance.h		
						public/gameBase/gameSystem.h
						public/gameBase/tickRegister.h
						public/gameBase/controller.h
						public/gameBase/player.h
						public/gameBase/controllerSystemInterface.h
						public/gameBase/gameMode.h
						public/gameBase/cameraSystemInterface.h
						public/gameBase/physicsSystemInterface.h

						public/ecs/entity.h
						public/ecs/entityRef.h	
						public/ecs/component.h
						public/ecs/positionComponent.h
						public/ecs/entitySystemInterface.h

						public/components/meshComponent.h
						public/components/inputComponent.h
						public/components/cameraComponent.h
						public/components/physicsComponent.h

						public/ui/uiSystemInterface.h

						public/ui/elements/uiRootView.h
						public/ui/elements/uiElement.h		
						public/ui/elements/uiText.h
						public/ui/elements/uiButton.h )

target_link_libraries( ${PROJECT_NAME}
							PUBLIC 
								orbCore
								orbMath
								d3d11
								dxguid
								dxgi
								Microsoft::DirectXTK
								nlohmann_json::nlohmann_json )

target_include_directories( ${PROJECT_NAME} 
								PRIVATE
									${ORB_SRC_ROOT_DIR}
									${PROJECT_SOURCE_DIR}/public
									${PROJECT_SOURCE_DIR}/private
									${PROJECT_SOURCE_DIR}/public/gameBase
									${PROJECT_SOURCE_DIR}/private/gameBase
									${PROJECT_SOURCE_DIR}/public/ecs
									${PROJECT_SOURCE_DIR}/private/ecs
									${PROJECT_SOURCE_DIR}/public/components
									${PROJECT_SOURCE_DIR}/private/components
									${PROJECT_SOURCE_DIR}/public/ui
									${PROJECT_SOURCE_DIR}/private/ui
									${PROJECT_SOURCE_DIR}/public/ui/elements
									${PROJECT_SOURCE_DIR}/private/ui/elements )

target_precompile_headers( ${PROJECT_NAME} 
								PUBLIC
									<functional>
									<vector>
									<algorithm>
									<nlohmann/json.hpp>
									public/rendererTypes.h
									public/engineTypes.h
									public/orbForwards.h
								PRIVATE
									<Windows.h> )