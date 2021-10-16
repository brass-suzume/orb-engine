project( orbEngine )

add_library( ${PROJECT_NAME} OBJECT )

target_sources( ${PROJECT_NAME}
					PRIVATE
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
						private/mesh.cpp

						private/base/gameRuntimeStage.cpp
						private/base/gameInstance.cpp	
						private/base/gameSystem.cpp
						private/base/tickPerformer.cpp
						private/base/tickPerformer.h
						private/base/controller.cpp
						private/base/player.cpp
						private/base/controllerSystem.cpp
						private/base/controllerSystem.h
						private/base/gameMode.cpp
						private/base/cameraSystem.cpp
						private/base/cameraSystem.h
						private/base/physicsSystem.cpp
						private/base/physicsSystem.h

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
						public/engine.h
						public/engineStage.h
						public/engineRuntimeStage.h
						public/engineInitData.h 
						public/rendererInterface.h
						public/rendererTypes.h
						public/level.h
						public/engineTypes.h
						public/resRef.h
						public/mesh.h
						public/inputHelpers.h

						public/base/gameRuntimeStage.h
						public/base/gameInstance.h		
						public/base/gameSystem.h
						public/base/tickRegister.h
						public/base/controller.h
						public/base/player.h
						public/base/controllerSystemInterface.h
						public/base/gameMode.h
						public/base/cameraSystemInterface.h
						public/base/physicsSystemInterface.h

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
								d3d11
								dxguid
								dxgi
)
target_include_directories( ${PROJECT_NAME} 
								PRIVATE
									${ORB_SRC_ROOT_DIR}
									${PROJECT_SOURCE_DIR}/public
									${PROJECT_SOURCE_DIR}/private
									${PROJECT_SOURCE_DIR}/public/base
									${PROJECT_SOURCE_DIR}/private/base
									${PROJECT_SOURCE_DIR}/public/ecs
									${PROJECT_SOURCE_DIR}/private/ecs
									${PROJECT_SOURCE_DIR}/public/components
									${PROJECT_SOURCE_DIR}/private/components
									${PROJECT_SOURCE_DIR}/public/ui
									${PROJECT_SOURCE_DIR}/private/ui
									${PROJECT_SOURCE_DIR}/public/ui/elements
									${PROJECT_SOURCE_DIR}/private/ui/elements
)
target_precompile_headers( ${PROJECT_NAME} 
								PUBLIC
									<functional>
									<vector>
									<algorithm>
									<memory>
									public/rendererTypes.h
									public/engineTypes.h
								PRIVATE
									<Windows.h>
)