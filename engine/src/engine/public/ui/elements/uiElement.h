#pragma once

namespace core
{
	class IRenderer;
	class GameInstance;
	class InputState;

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct UIElementInitContext
	{
		GameInstance* m_gameInstance = nullptr;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	class UIElement : public Serializable
	{
		ORB_DECLARE_TYPE( UIElement );
		ORB_SUPER_CLASS( Serializable );

	public:
		virtual ~UIElement();

		virtual bool Deserialize( const json& data ) override;
		virtual bool HandleInput( const InputState& state );

		void Init( const UIElementInitContext& initCtx );
		void Uninit();

		void SetDesiredVisibility( bool isVisible );

		bool IsInitialized() const { return m_isInitialized; }
		bool IsVisible() const { return m_isVisible; }

		Vector2 GetPosition() const { return m_position; }
		Vector2 GetSize() const { return m_size; }

	protected:
		std::shared_ptr< IRenderer > GetRenderer() const;

		virtual void OnInit( const UIElementInitContext& initCtx );
		virtual void OnUninit();

		virtual bool HasDesireForVisibility() const;
		void EvaluateVisibility();

		virtual void OnShow();
		virtual void OnHide();

	protected:
		GameInstance* m_gameInstance = nullptr;

	private:
		bool m_isInitialized = false;
		bool m_isVisible = true;
		bool m_desiredVisibility = true;

		Vector2 m_position;
		Vector2 m_size;
	};
}