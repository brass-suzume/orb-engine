#pragma once

#include "rendererInterface.h"

#include <d3d11.h>


class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;
class ID3D11RasterizerState;
class ID3D11Buffer;
class ID3D11InputLayout;
class ID3D11Texture2D;

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
	class CommonStates;
	class BasicEffect;
}

namespace core
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	enum class RenderProxyType
	{
		Empty,
		Model,
		Text,
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class RenderProxy
	{

	public:
		RenderProxy( RenderProxyType proxyType, RenderLayer renderLayer );
		virtual ~RenderProxy() = 0 {};

		RenderProxyType GetProxyType() const { return m_proxyType; }
		RenderLayer GetLayer() const { return m_renderLayer; }

		void SetWorldTransform( const Vector2& pos, float rotation );
		void SetScale( float scale );
		const Matrix& GetWorldTransform() const { return m_world; }

		bool IsVisible() const { return m_visible; }
		void SetVisible( bool v ) { m_visible = v; }

	private:
		RenderProxyType m_proxyType = RenderProxyType::Empty;
		RenderLayer m_renderLayer = RenderLayer::Invalid;
		Matrix m_world;
		float m_scale;
		bool m_visible = true;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class ModelRenderProxy : public RenderProxy
	{
		ORB_SUPER_CLASS( RenderProxy );

	public:
		ModelRenderProxy( RenderLayer renderLayer, ID3D11Buffer* vb, ID3D11Buffer* ib, size_t indicesCount, bool solid );
		virtual ~ModelRenderProxy();

		ID3D11Buffer* GetVertexBuffer() const { return m_vertexBuffer; }
		ID3D11Buffer* GetIndexBuffer() const { return m_indexBuffer; }
		bool IsSolid() const { return m_solid; }
		size_t GetIndicesCount() const { return m_indicesCount; }

	private:
		ID3D11Buffer* m_vertexBuffer = nullptr;
		ID3D11Buffer* m_indexBuffer = nullptr;
		size_t m_indicesCount = 0;
		bool m_solid = true;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class TextRenderProxy : public RenderProxy
	{
		ORB_SUPER_CLASS( RenderProxy );

	public:
		TextRenderProxy( RenderLayer renderLayer );
		virtual ~TextRenderProxy();

		const char* GetText() const { return m_text.c_str(); }
		void SetText( const std::string& text ) { m_text = text; }

		float GetScale() const { return m_scale; }
		void SetScale( float s ) { m_scale = s; }

		const Color& GetColor() const { return m_color; }
		void SetColor( const Color& color ) { m_color = color; }

	private:
		std::string m_text;
		float m_scale = 1.0f;
		Color m_color = Colors::White;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class Skybox final
	{

	public:
		Skybox( ID3D11Device* device, ID3D11DeviceContext* deviceCtx );
		~Skybox();

		void LoadFromFile( const std::string& path );
		void Draw( DirectX::BasicEffect& effect );

	private:
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceCtx;
		std::string m_loadedSkyboxPath;

		ID3D11Buffer* m_vertexBuffer = nullptr;
		ID3D11Buffer* m_indexBuffer = nullptr;

		ID3D11Resource* m_skyboxTextureResource = nullptr;
		ID3D11ShaderResourceView* m_skyboxTextureResourceView = nullptr;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class RenderLayerSet
	{

	private:
		struct Entry
		{
			float m_depth;
			RenderProxy* m_proxy;
		};

	public:
		void AddEntry( float depth, RenderProxy* proxy );
		void RemoveEntry( RenderProxy* proxy );
		void Clear();

		template< class TFunc >
		void ForEach( TFunc&& functor ) const
		{
			for( const Entry& entry : m_entries )
			{
				functor( entry.m_proxy );
			}
		}

		const Matrix& GetView() const { return m_view; }
		void SetView( const Matrix& mat ) { m_view = mat; }

		const Matrix& GetProjection() const { return m_projection; }
		void SetProjection( const Matrix& mat ) { m_projection = mat; }

	private:
		std::vector< Entry > m_entries;
		Matrix m_view;
		Matrix m_projection;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class Renderer final : public IRenderer
	{

	public:
		Renderer();
		virtual ~Renderer();

		virtual void Render() override;

		virtual RenderProxyID CreateQuadProxy( const RenderQuadData& quadData ) override;
		virtual RenderProxyID CreateTextProxy( const RenderTextData& textData ) override;
		virtual RenderProxyID CreatePrimitiveProxy( const RenderPrimitiveData& primitiveData ) override;

		virtual void DestroyProxy( RenderProxyID proxyId ) override;

		virtual Vector2 GetScreenSpaceCoords( const Vector2& uiCoords ) const override;
		virtual Vector2 GetNearPlaneWorldPositionFromScreenSpace( const Vector2& ssPos ) const override;

		virtual void SetSkybox( const std::string& skyboxTexturePath ) override;

		virtual void SetView( const Matrix& view ) override;
		virtual void SetProjection( const Matrix& projectionMatrix ) override;

		virtual void UpdateTransform( RenderProxyID proxyId, const Vector2& position, float rotation ) override;

		virtual void SetVisibility( RenderProxyID proxyId, bool isVisible ) override;
		virtual void UpdateText( RenderProxyID proxyId, const std::string& text ) override;

		bool Init( HWND windowHandle );
		void Uninit();

		bool IsInitialized() const { return m_isInitialized; }

	private:
		bool InitDevice();
		bool InitSwapChain( HWND windowHandle );
		bool InitRenderTarget();
		bool InitViewport();
		bool InitRasterizer();
		bool InitEffects();
		bool InitStructures();

		void RefreshProjection();
		void RefreshView();

		RenderProxyID AddProxy( std::unique_ptr< RenderProxy >&& proxy, RenderLayer layer, float depth );
		void RemoveProxy( RenderProxyID proxyId );

		void DrawSkybox();
		void DrawWorld();

		void DrawProxyList( RenderProxyType proxyType, const std::vector< const RenderProxy* >& proxyList, const Matrix& view, const Matrix& projection );

	private:
		bool m_isInitialized = false;

		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_deviceCtx = nullptr;
		IDXGISwapChain* m_swapChain = nullptr;
		ID3D11RenderTargetView* m_renderTarget = nullptr;

		ID3D11RasterizerState* m_wireframeRasterizer = nullptr;
		ID3D11RasterizerState* m_solidRasterizer = nullptr;

		D3D11_VIEWPORT m_viewPort;

		Matrix m_worldView;
		Matrix m_worldProjection;
		Matrix m_uiProjection;

		std::unique_ptr< DirectX::CommonStates > m_commonStates;
		std::unique_ptr< DirectX::SpriteBatch > m_spriteBatch;
		std::unique_ptr< DirectX::SpriteFont > m_spriteFont;
		std::unique_ptr< DirectX::BasicEffect > m_effect;
		
		ID3D11InputLayout* m_vertexColorEffectIL = nullptr;
		ID3D11InputLayout* m_texturedEffectIL = nullptr;

		std::vector< RenderLayerSet > m_layers;
		RenderProxyID m_idGen = 0;
		std::unordered_map< RenderProxyID, std::unique_ptr< RenderProxy > > m_proxyRegistry;

		std::unique_ptr< Skybox > m_skybox;
	};
}