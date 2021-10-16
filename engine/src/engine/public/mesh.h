#pragma once

namespace core
{
	class Mesh : public Serializable
	{
		ORB_DECLARE_TYPE( Mesh );
		ORB_SUPER_CLASS( Serializable );

	public:
		virtual bool Deserialize( const json& data ) override;

		const std::vector< Vector2 >& GetVertices() const { return m_vertices; }
		const std::vector< uint16_t >& GetIndices() const { return m_indices; }
		const std::vector< Color >& GetTriangleColors() const { return m_colorPerTriangle; }
		float GetBoundingSphere() const { return m_boundingSphere; }
		const Color& GetColor() const { return m_color; }

	private:
		std::string m_name;
		std::vector< Vector2 > m_vertices;
		std::vector< uint16_t > m_indices;
		std::vector< Color > m_colorPerTriangle;
		float m_boundingSphere = 0.0f;
		Color m_color;
	};
}