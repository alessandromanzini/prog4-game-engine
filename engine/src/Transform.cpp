#include "Transform.h"

#include <stdexcept>

using namespace engine;

Transform::Transform( const glm::vec2& translation )
{
	matrix_ = glm::translate( matrix_, translation );
}

Transform::Transform( const glm::mat3x3& matrix )
	: matrix_{ matrix }
{
}

Transform::Transform( const glm::vec2& translation, float rotation, const glm::vec2& scale )
{
	matrix_ = glm::translate( matrix_, translation )
		* glm::rotate(matrix_, rotation)
		* glm::scale( matrix_, scale );
}

void Transform::combine( const Transform& other )
{
	matrix_ = matrix_ * other.matrix_;
}

glm::vec2 Transform::get_position( ) const
{
	return matrix_[2];
}

const glm::mat3x3& Transform::get_matrix( ) const
{
	return matrix_;
}

Transform Transform::operator+( const Transform& other ) const
{
	Transform result{ *this };
	result.combine( other );
	return result;
}
