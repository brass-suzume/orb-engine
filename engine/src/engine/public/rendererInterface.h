#pragma once

namespace core
{
	constexpr float c_baseLayerDepth = 0.0f;

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct RenderTextData
	{
		RenderLayer m_layer = RenderLayer::Invalid;
		float m_depth = c_baseLayerDepth;
		Vector2 m_position;
		uint8_t m_fontSize = 16.0f;
		Color m_color;
		const char* m_text = nullptr;
		bool m_visible = true;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct RenderQuadData
	{
		RenderLayer m_layer = RenderLayer::Invalid;
		float m_depth = c_baseLayerDepth;
		Vector2 m_quadMin;
		Vector2 m_quadMax;
		Color m_color;
		bool m_solid = true;
		bool m_visible = true;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct RenderPrimitiveData
	{
		RenderLayer m_layer = RenderLayer::Invalid;
		float m_depth = c_baseLayerDepth;
		Vector2 m_position;
		float m_rotation = 0.0f;
		float m_scale = 1.0f;
		const Vector2* m_vertices = nullptr;
		size_t m_verticesCount = 0;
		const uint16_t* m_indices = nullptr;
		size_t m_indicesCount = 0;
		Color m_color;
		bool m_visible = true;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	class IRenderer : private NoCopy
	{

	public:
		virtual ~IRenderer() = 0 {};

		virtual void Render() = 0;

		virtual RenderProxyID CreateQuadProxy( const RenderQuadData& quadData ) = 0;
		virtual RenderProxyID CreateTextProxy( const RenderTextData& textData ) = 0;
		virtual RenderProxyID CreatePrimitiveProxy( const RenderPrimitiveData& primitiveData ) = 0;

		virtual void DestroyProxy( RenderProxyID proxyId ) = 0;

		virtual Vector2 GetScreenSpaceCoords( const Vector2& uiCoords ) const = 0;
		virtual Vector2 GetNearPlaneWorldPositionFromScreenSpace( const Vector2& ssPos ) const = 0;

		virtual void SetSkybox( const std::string& skyboxTexturePath ) = 0;

		virtual void SetView( const Matrix& view ) = 0;
		virtual void SetProjection( const Matrix& projection ) = 0;

		virtual void UpdateTransform( RenderProxyID proxyId, const Vector2& position, float rotation ) = 0;

		virtual void SetVisibility( RenderProxyID proxyId, bool isVisible ) = 0;
		virtual void UpdateText( RenderProxyID proxyId, const std::string& text ) = 0;
	};
}