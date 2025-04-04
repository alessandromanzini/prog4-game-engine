// ReSharper disable CppNonExplicitConvertingConstructor
#ifndef TRANSFORM_H
#define TRANSFORM_H

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
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
        Transform( const glm::vec2& translation, float rotation, const glm::vec2& scale );

        void combine( const Transform& other );

        [[nodiscard]] glm::vec2 get_position( ) const;
        [[nodiscard]] const glm::mat3x3& get_matrix( ) const;

        Transform operator+( const Transform& other ) const;

    private:
        glm::mat3x3 matrix_{ 1.0 };

    };

}

#endif //!TRANSFORM_H
