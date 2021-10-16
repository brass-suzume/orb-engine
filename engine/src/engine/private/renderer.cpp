#include "renderer.h"

#include "DirectXTK/CommonStates.h"
#include "DirectXTK/Effects.h"
#include "DirectXTK/SpriteFont.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/BufferHelpers.h"
#include "DirectXTK/VertexTypes.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/DirectXHelpers.h"


#define SAFE_RELEASE_RES( x ) do{ if( x ) { x->Release(); x = nullptr; } } while( 0 );

using namespace DirectX;

namespace core
{
	namespace
	{
		// value which was provided to font gen
		constexpr float c_swFontSize = 64.0f;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	RenderProxy::RenderProxy( RenderProxyType proxyType, RenderLayer renderLayer )
		: m_proxyType{ proxyType }
		, m_renderLayer{ renderLayer }
	{
	}

	void RenderProxy::SetWorldTransform( const Vector2& pos, float rotation )
	{
		m_world = Matrix::CreateScale( m_scale ) * Matrix::CreateRotationZ( rotation ) * Matrix::CreateTranslation( pos.x, pos.y, 0.0f );
	}

	void RenderProxy::SetScale( float scale )
	{
		m_scale = scale;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	ModelRenderProxy::ModelRenderProxy( RenderLayer renderLayer, ID3D11Buffer* vb, ID3D11Buffer* ib, size_t indicesCount, bool solid )
		: Super{ RenderProxyType::Model, renderLayer }
		, m_vertexBuffer{ vb }
		, m_indexBuffer{ ib }
		, m_indicesCount{ indicesCount }
		, m_solid{ solid }
	{
	}

	ModelRenderProxy::~ModelRenderProxy()
	{
		SAFE_RELEASE_RES( m_vertexBuffer );
		SAFE_RELEASE_RES( m_indexBuffer );
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	TextRenderProxy::TextRenderProxy( RenderLayer renderLayer )
		: Super{ RenderProxyType::Text, renderLayer }
	{
	}

	TextRenderProxy::~TextRenderProxy() = default;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	Skybox::Skybox( ID3D11Device* device, ID3D11DeviceContext* deviceCtx )
		: m_device{ device }
		, m_deviceCtx{ deviceCtx }
	{
		const VertexPositionTexture vertices[] =
		{
			{ Vector3{ -1,-1, 0}, Vector2{0, 0} },
			{ Vector3{ 1,-1, 0}, Vector2{1, 0} },
			{ Vector3{ -1,1, 0}, Vector2{0, 1} },
			{ Vector3{ 1, 1, 0}, Vector2{1, 1} }
		};

		const uint16_t indices[] =
		{
			0, 1, 2,
			1, 2, 3
		};

		CreateStaticBuffer( device, vertices, std::size( vertices ), D3D11_BIND_VERTEX_BUFFER, &m_vertexBuffer );
		CreateStaticBuffer( device, indices, std::size( indices ), D3D11_BIND_INDEX_BUFFER, &m_indexBuffer );
	}

	Skybox::~Skybox()
	{
		SAFE_RELEASE_RES( m_vertexBuffer );
		SAFE_RELEASE_RES( m_indexBuffer );
	}

	void Skybox::LoadFromFile( const std::string& path )
	{
		if( m_loadedSkyboxPath == path )
		{
			return;
		}

		m_loadedSkyboxPath = path;

		SAFE_RELEASE_RES( m_skyboxTextureResource );
		SAFE_RELEASE_RES( m_skyboxTextureResourceView );

		const std::wstring wstrPath = ToWString( path );
		ORB_ASSERT( SUCCEEDED( CreateDDSTextureFromFile( m_device, m_deviceCtx, wstrPath.c_str(), &m_skyboxTextureResource, &m_skyboxTextureResourceView ) ) );
	}

	void Skybox::Draw( DirectX::BasicEffect& effect )
	{
		m_deviceCtx->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		effect.SetProjection( Matrix::Identity );
		effect.SetView( Matrix::Identity );
		effect.SetWorld( Matrix::Identity );
		effect.SetTexture( m_skyboxTextureResourceView );
		effect.Apply( m_deviceCtx );

		UINT stride = sizeof( DirectX::VertexPositionTexture ), offset = 0;
		m_deviceCtx->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &stride, &offset );
		m_deviceCtx->IASetIndexBuffer( m_indexBuffer, DXGI_FORMAT_R16_UINT, 0 );

		m_deviceCtx->DrawIndexed( 6, 0, 0 );
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	void RenderLayerSet::AddEntry( float depth, RenderProxy* proxy )
	{
		Entry newEntry{ depth, proxy };
		auto it = std::upper_bound( m_entries.begin(), m_entries.end(), newEntry, []( const Entry& lhs, const Entry& rhs )
		{
			return lhs.m_depth > rhs.m_depth;
		} );

		m_entries.insert( it, newEntry );
	}

	void RenderLayerSet::RemoveEntry( RenderProxy* proxy )
	{
		auto it = std::find_if( m_entries.begin(), m_entries.end(), [proxy]( const Entry& entry )
		{
			return entry.m_proxy == proxy;
		} );

		m_entries.erase( it );
	}

	void RenderLayerSet::Clear()
	{
		m_entries.clear();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	Renderer::Renderer() = default;

	Renderer::~Renderer()
	{
		ORB_ASSERT( !m_isInitialized, "Renderer is still initialized" );
		ORB_ASSERT( !m_device, "Device wasn't uninitialized" );
		ORB_ASSERT( !m_deviceCtx, "Device Context wasn't uninitialized" );
		ORB_ASSERT( !m_swapChain, "Swap chain wasn't uninitialized" );
		ORB_ASSERT( !m_renderTarget, "Render target wasn't uninitialized" );
		ORB_ASSERT( !m_wireframeRasterizer, "Wireframe rasterizer wasn't uninitialized" );
		ORB_ASSERT( !m_solidRasterizer, "Solid rasterizer wasn't uninitialized" );
	}

	void Renderer::Render()
	{
		m_deviceCtx->ClearRenderTargetView( m_renderTarget, Colors::Black );

		DrawSkybox();
		DrawWorld();

		m_swapChain->Present( 0, 0 );
	}

	RenderProxyID Renderer::CreateQuadProxy( const RenderQuadData& quadData )
	{
		DirectX::VertexPositionColor rectVertices[] =
		{
			{ Vector3{ -1,	-1,	-1.0f }, Colors::White },
			{ Vector3{ -1,	1,	-1.0f }, Colors::White },
			{ Vector3{	1,	1,	-1.0f }, Colors::White },
			{ Vector3{	1,	-1,	-1.0f }, Colors::White },
		};

		rectVertices[ 0 ].position = Vector3{ quadData.m_quadMin.x, quadData.m_quadMin.y, -0.1f };
		rectVertices[ 0 ].color = quadData.m_color;

		rectVertices[ 1 ].position = Vector3{ quadData.m_quadMin.x, quadData.m_quadMax.y, -0.1f };
		rectVertices[ 1 ].color = quadData.m_color;

		rectVertices[ 2 ].position = Vector3{ quadData.m_quadMax.x, quadData.m_quadMax.y, -0.1f };
		rectVertices[ 2 ].color = quadData.m_color;

		rectVertices[ 3 ].position = Vector3{ quadData.m_quadMax.x, quadData.m_quadMin.y, -0.1f };
		rectVertices[ 3 ].color = quadData.m_color;

		ID3D11Buffer* vb;
		CreateStaticBuffer( m_device, rectVertices, std::size( rectVertices ), D3D11_BIND_VERTEX_BUFFER, &vb );

		const uint16_t indices[] =
		{
			0, 1, 2,
			0, 2, 3,
		};

		ID3D11Buffer* ib;
		CreateStaticBuffer( m_device, indices, std::size( indices ), D3D11_BIND_INDEX_BUFFER, &ib );

		auto modelProxy = std::make_unique< ModelRenderProxy >( quadData.m_layer, vb, ib, std::size( indices ), quadData.m_solid );
		modelProxy->SetVisible( quadData.m_visible );
		return AddProxy( std::move( modelProxy ), quadData.m_layer, quadData.m_depth );
	}

	RenderProxyID Renderer::CreateTextProxy( const RenderTextData& textData )
	{
		auto textProxy = std::make_unique< TextRenderProxy >( textData.m_layer );
		textProxy->SetWorldTransform( textData.m_position, 0.0f );
		textProxy->SetVisible( textData.m_visible );
		textProxy->SetColor( textData.m_color );
		textProxy->SetText( textData.m_text );
		textProxy->SetScale( textData.m_fontSize / c_swFontSize );
		return AddProxy( std::move( textProxy ), textData.m_layer, textData.m_depth );
	}

	RenderProxyID Renderer::CreatePrimitiveProxy( const RenderPrimitiveData& primitiveData )
	{
		std::vector< DirectX::VertexPositionColor > vertexColorArr;
		vertexColorArr.resize( primitiveData.m_verticesCount );
		for( size_t i = 0; i < primitiveData.m_verticesCount; ++i )
		{
			vertexColorArr[ i ].position.x = primitiveData.m_vertices[ i ].x;
			vertexColorArr[ i ].position.y = primitiveData.m_vertices[ i ].y;
			vertexColorArr[ i ].position.z = 0;
			vertexColorArr[ i ].color = primitiveData.m_color;
		}

		ID3D11Buffer* vb;
		CreateStaticBuffer( m_device, vertexColorArr, D3D11_BIND_VERTEX_BUFFER, &vb );

		ID3D11Buffer* ib;
		CreateStaticBuffer( m_device, primitiveData.m_indices, primitiveData.m_indicesCount, D3D11_BIND_INDEX_BUFFER, &ib );

		auto modelProxy = std::make_unique< ModelRenderProxy >( primitiveData.m_layer, vb, ib, primitiveData.m_indicesCount, true );
		modelProxy->SetScale( primitiveData.m_scale );
		modelProxy->SetVisible( primitiveData.m_visible );
		return AddProxy( std::move( modelProxy ), primitiveData.m_layer, primitiveData.m_depth );
	}

	void Renderer::DestroyProxy( RenderProxyID proxyId )
	{
		RemoveProxy( proxyId );
	}

	Vector2 Renderer::GetScreenSpaceCoords( const Vector2& uiCoords ) const
	{
		return Vector2{
			uiCoords.x - m_viewPort.Width / 2.0f,
			m_viewPort.Height / 2.0f - uiCoords.y
		};
	}

	Vector2 Renderer::GetNearPlaneWorldPositionFromScreenSpace( const Vector2& ssPos ) const
	{
		const float halfWidth = m_viewPort.Width * 0.5f;
		const float halfHeight = m_viewPort.Height * 0.5f;
		const Vector3 ndc{
			( ssPos.x - halfWidth ) / halfWidth,
			( halfHeight - ssPos.y ) / halfHeight,
			0.0f
		};

		const Matrix invVP = ( m_worldView * m_worldProjection ).Invert();
		const Vector3 nearPlanePos = Vector3::Transform( ndc, invVP );
		return Vector2{ nearPlanePos.x, nearPlanePos.y };
	}

	void Renderer::SetSkybox( const std::string& skyboxTexturePath )
	{
		m_skybox->LoadFromFile( skyboxTexturePath );
	}

	void Renderer::SetView( const Matrix& view )
	{
		m_worldView = view;
		RefreshView();
	}

	void Renderer::SetProjection( const Matrix& projectionMatrix )
	{
		m_worldProjection = projectionMatrix;
		RefreshProjection();
	}

	void Renderer::UpdateTransform( RenderProxyID proxyId, const Vector2& position, float rotation )
	{
		const std::unique_ptr< RenderProxy >& proxy = m_proxyRegistry.at( proxyId );
		proxy->SetWorldTransform( position, rotation );
	}

	void Renderer::SetVisibility( RenderProxyID proxyId, bool isVisible )
	{
		const std::unique_ptr< RenderProxy >& proxy = m_proxyRegistry.at( proxyId );
		proxy->SetVisible( isVisible );
	}

	void Renderer::UpdateText( RenderProxyID proxyId, const std::string& text )
	{
		const std::unique_ptr< RenderProxy >& proxy = m_proxyRegistry.at( proxyId );
		auto* textProxy = dynamic_cast< TextRenderProxy* >( proxy.get() );
		textProxy->SetText( text );
	}

	bool Renderer::Init( HWND windowHandle )
	{
		m_isInitialized = true;

		if( !InitDevice() )
		{
			ORB_FAIL( "Fatal state" );
			return false;
		}

		if( !InitSwapChain( windowHandle ) )
		{
			ORB_FAIL( "Fatal state" );
			return false;
		}

		if( !InitRenderTarget() )
		{
			ORB_FAIL( "Fatal state" );
			return false;
		}

		if( !InitViewport() )
		{
			ORB_FAIL( "Fatal state" );
			return false;
		}

		if( !InitRasterizer() )
		{
			ORB_FAIL( "Fatal state" );
			return false;
		}

		if( !InitEffects() )
		{
			ORB_FAIL( "Fatal state" );
			return false;
		}

		if( !InitStructures() )
		{
			ORB_FAIL( "Fatal state" );
			return false;
		}

		return true;
	}

	void Renderer::Uninit()
	{
		m_skybox.reset();

		m_layers.clear();
		m_proxyRegistry.clear();

		m_effect.reset();
		m_spriteFont.reset();
		m_spriteBatch.reset();
		m_commonStates.reset();

		SAFE_RELEASE_RES( m_texturedEffectIL );
		SAFE_RELEASE_RES( m_vertexColorEffectIL );
		SAFE_RELEASE_RES( m_solidRasterizer );
		SAFE_RELEASE_RES( m_wireframeRasterizer );
		SAFE_RELEASE_RES( m_renderTarget );
		SAFE_RELEASE_RES( m_swapChain );
		SAFE_RELEASE_RES( m_deviceCtx );
		SAFE_RELEASE_RES( m_device );
		m_isInitialized = false;
	}

	bool Renderer::InitDevice()
	{
		HRESULT res = D3D11CreateDevice( nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
#ifdef _DEBUG
			D3D11_CREATE_DEVICE_DEBUG,
#else
			0,
#endif
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&m_device,
			nullptr,
			&m_deviceCtx );

		return SUCCEEDED( res );
	}

	bool Renderer::InitSwapChain( HWND windowHandle )
	{
		IDXGIFactory* pFactory;
		if( FAILED( CreateDXGIFactory( __uuidof( IDXGIFactory ), ( void** )( &pFactory ) ) ) )
		{
			return false;
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = windowHandle;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;

		if( FAILED( pFactory->CreateSwapChain( m_device, &swapChainDesc, &m_swapChain ) ) )
		{
			return false;
		}

		pFactory->Release();

		return true;
	}

	bool Renderer::InitRenderTarget()
	{
		ID3D11Texture2D* backBuffer;
		if( FAILED( m_swapChain->GetBuffer( 0, IID_ID3D11Texture2D, ( void** )&backBuffer ) ) )
		{
			return false;
		}

		if( FAILED( m_device->CreateRenderTargetView( backBuffer, nullptr, &m_renderTarget ) ) )
		{
			return false;
		}

		m_deviceCtx->OMSetRenderTargets( 1, &m_renderTarget, nullptr );

		return true;
	}

	bool Renderer::InitViewport()
	{
		D3D11_VIEWPORT vp;
		vp.Width = 1600;
		vp.Height = 900;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_deviceCtx->RSSetViewports( 1, &vp );

		m_viewPort = vp;

		return true;
	}

	bool Renderer::InitRasterizer()
	{
		D3D11_RASTERIZER_DESC desc;

		ZeroMemory( &desc, sizeof( D3D11_RASTERIZER_DESC ) );

		desc.FrontCounterClockwise = TRUE;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.DepthClipEnable = TRUE;

		if( FAILED( m_device->CreateRasterizerState( &desc, &m_wireframeRasterizer ) ) )
		{
			return false;
		}

		desc.FillMode = D3D11_FILL_SOLID;
		if( FAILED( m_device->CreateRasterizerState( &desc, &m_solidRasterizer ) ) )
		{
			return false;
		}
		
		return true;
	}

	bool Renderer::InitEffects()
	{
		m_effect = std::make_unique< DirectX::BasicEffect >( m_device );
		m_effect->SetFogEnabled( false );
		m_effect->SetLightingEnabled( false );
		m_effect->SetTextureEnabled( false );
		m_effect->SetVertexColorEnabled( true );
		if( FAILED( DirectX::CreateInputLayoutFromEffect< DirectX::VertexPositionColor >( m_device, m_effect.get(), &m_vertexColorEffectIL ) ) )
		{
			return false;
		}

		m_effect->SetVertexColorEnabled( false );
		m_effect->SetTextureEnabled( true );
		if( FAILED( DirectX::CreateInputLayoutFromEffect< DirectX::VertexPositionTexture >( m_device, m_effect.get(), &m_texturedEffectIL ) ) )
		{
			return false;
		}

		return true;
	}

	bool Renderer::InitStructures()
	{
		m_layers.resize( static_cast< size_t >( RenderLayer::_COUNT ) );

		m_worldProjection = Matrix::CreateOrthographic( 1.0f, 1.0f, 0.0f, 1.0f );
		m_uiProjection = Matrix::CreateOrthographic( m_viewPort.Width, m_viewPort.Height, 0.0f, 3.0f );
		RefreshProjection();

		m_worldView = Matrix::Identity;
		RefreshView();

		m_commonStates = std::make_unique< DirectX::CommonStates >( m_device );
		m_spriteBatch = std::make_unique< DirectX::SpriteBatch >( m_deviceCtx );
		m_spriteFont = std::make_unique< DirectX::SpriteFont >( m_device, L"data/font/orbFont.spritefont" );

		m_skybox = std::make_unique< Skybox >( m_device, m_deviceCtx );

		return true;
	}

	void Renderer::RefreshProjection()
	{
		m_layers[ static_cast< size_t >( RenderLayer::Background ) ].SetProjection( m_worldProjection );
		m_layers[ static_cast< size_t >( RenderLayer::World ) ].SetProjection( m_worldProjection );
		m_layers[ static_cast< size_t >( RenderLayer::UI ) ].SetProjection( m_uiProjection );
	}

	void Renderer::RefreshView()
	{
		m_layers[ static_cast< size_t >( RenderLayer::Background ) ].SetView( m_worldView );
		m_layers[ static_cast< size_t >( RenderLayer::World ) ].SetView( m_worldView );
		m_layers[ static_cast< size_t >( RenderLayer::UI ) ].SetView( Matrix::Identity );
	}

	RenderProxyID Renderer::AddProxy( std::unique_ptr< RenderProxy >&& proxy, RenderLayer layer, float depth )
	{
		const RenderProxyID newId = ++m_idGen;
		RenderProxy* proxyPtr = proxy.get();
		m_proxyRegistry[ newId ] = std::move( proxy );

		m_layers[ static_cast< size_t >( layer ) ].AddEntry( depth, proxyPtr );
		return newId;
	}

	void Renderer::RemoveProxy( RenderProxyID proxyId )
	{
		std::unique_ptr< RenderProxy >& proxy = m_proxyRegistry.at( proxyId );

		m_layers[ static_cast< size_t >( proxy->GetLayer() ) ].RemoveEntry( proxy.get() );
		m_proxyRegistry.erase( proxyId );
	}

	void Renderer::DrawWorld()
	{
		m_effect->SetVertexColorEnabled( true );
		m_effect->SetTextureEnabled( false );

		std::vector< const RenderProxy* > sameTypeProxies;
		RenderProxyType currentProxyType = RenderProxyType::Empty;

		for( const RenderLayerSet& layerSet : m_layers )
		{
			layerSet.ForEach( [&]( const RenderProxy* proxy )
			{
				if( proxy->IsVisible() )
				{
					if( proxy->GetProxyType() != currentProxyType )
					{
						DrawProxyList( currentProxyType, sameTypeProxies, layerSet.GetView(), layerSet.GetProjection() );
						currentProxyType = proxy->GetProxyType();
						sameTypeProxies.clear();
					}

					sameTypeProxies.push_back( proxy );
				}
			} );

			DrawProxyList( currentProxyType, sameTypeProxies, layerSet.GetView(), layerSet.GetProjection() );
			sameTypeProxies.clear();
		}
	}

	void Renderer::DrawSkybox()
	{
		m_effect->SetVertexColorEnabled( false );
		m_effect->SetTextureEnabled( true );
		m_deviceCtx->IASetInputLayout( m_texturedEffectIL );
		m_deviceCtx->RSSetState( m_solidRasterizer );
		m_skybox->Draw( *m_effect );
	}

	void Renderer::DrawProxyList( RenderProxyType proxyType, const std::vector< const RenderProxy* >& proxyList, const Matrix& view, const Matrix& projection )
	{
		switch( proxyType )
		{

		case RenderProxyType::Model:
		{
			UINT stride = sizeof( DirectX::VertexPositionColor );
			UINT offset = 0;

			m_effect->SetProjection( projection );
			m_effect->SetView( view );
			m_deviceCtx->OMSetDepthStencilState( nullptr, 0 );
			m_deviceCtx->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			m_deviceCtx->IASetInputLayout( m_vertexColorEffectIL );

			for( const RenderProxy* proxy : proxyList )
			{
				auto modelProxy = static_cast< const ModelRenderProxy* >( proxy );

				m_effect->SetWorld( modelProxy->GetWorldTransform() );
				m_effect->Apply( m_deviceCtx );

				ID3D11Buffer* vb = modelProxy->GetVertexBuffer();
				m_deviceCtx->IASetVertexBuffers( 0, 1, &vb, &stride, &offset );
				m_deviceCtx->IASetIndexBuffer( modelProxy->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0 );

				if( modelProxy->IsSolid() )
				{
					m_deviceCtx->RSSetState( m_solidRasterizer );
					m_deviceCtx->DrawIndexed( modelProxy->GetIndicesCount(), 0, 0 );
				}

				m_deviceCtx->RSSetState( m_wireframeRasterizer );
				m_deviceCtx->DrawIndexed( modelProxy->GetIndicesCount(), 0, 0 );
			}
		}
		break;

		case RenderProxyType::Text:
		{
			m_spriteBatch->Begin( DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, m_commonStates->DepthDefault() );
			for( const RenderProxy* proxy : proxyList )
			{
				auto textProxy = static_cast< const TextRenderProxy* >( proxy );
				m_spriteFont->DrawString( 
					m_spriteBatch.get(),
					textProxy->GetText(),
					textProxy->GetWorldTransform().Translation(),
					textProxy->GetColor(),
					0,
					Vector2::Zero,
					textProxy->GetScale(),
					DirectX::SpriteEffects::SpriteEffects_None,
					0.1f );
			}
			m_spriteBatch->End();


		}
		break;

		case RenderProxyType::Empty:
			break;

		default:
			ORB_ASSERT( false, "Unsupported type" );
			break;

		}
	}
}