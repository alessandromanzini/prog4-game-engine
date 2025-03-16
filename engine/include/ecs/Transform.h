#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm.hpp>
#include <gtx/matrix_transform_2d.inl>

namespace engine
{
	class Transform final
	{
	public:
		Transform( ) = default;
		Transform( const glm::vec2& translation );
		Transform( const glm::mat3x3& matrix );
		explicit Transform( const glm::vec2& translation, float rotation, const glm::vec2& scale );

		void combine( const Transform& other );

		glm::vec2 get_position( ) const;
		const glm::mat3x3& get_matrix( ) const;

		Transform operator+( const Transform& other ) const;

	private:
		glm::mat3x3 matrix_{ 1.0 };

	};

}

#endif // !TRANSFORM_H
