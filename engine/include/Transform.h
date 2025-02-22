#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm.hpp>

namespace engine
{
	class GlobalProxy;
	class LocalProxy;
	class GlobalProxyReadOnly;
	class LocalProxyReadOnly;

	class Transform final
	{
	public:
		void set_parent( Transform* parent );

		GlobalProxy global( );
		const GlobalProxyReadOnly global( ) const;

		LocalProxy local( );
		const LocalProxyReadOnly local( ) const;

		friend class GlobalProxy;
		friend class LocalProxy;
		friend class GlobalProxyReadOnly;
		friend class LocalProxyReadOnly;

	private:
		Transform* parent_ptr_{ nullptr };
		glm::vec2 position_{};

	};

	// +--------------------------------+
	// | PROXY							|
	// +--------------------------------+
	class Proxy
	{
	public:
		explicit Proxy( Transform* pTransform ) : transform_ptr_{ pTransform }
		{
		};

		virtual glm::vec2 get_position( ) const = 0;

		virtual void set_position( const glm::vec2& pos ) = 0;
		void set_position( float x, float y )
		{
			set_position( glm::vec2{ x, y } );
		}

	protected:
		Transform* transform_ptr_;

	};

	class GlobalProxy final : public Proxy
	{
	public:
		explicit GlobalProxy( Transform* pTransform );

		glm::vec2 get_position( ) const override;
		void set_position( const glm::vec2& pos ) override;
	};

	class LocalProxy final : public Proxy
	{
	public:
		explicit LocalProxy( Transform* pTransform );

		glm::vec2 get_position( ) const override;
		void set_position( const glm::vec2& pos ) override;
	};

	// +--------------------------------+
	// | READONLY PROXY					|
	// +--------------------------------+
	class ProxyReadOnly
	{
	public:
		explicit ProxyReadOnly( const Transform* pTransform ) : transform_ptr_{ pTransform }
		{
		};

		virtual glm::vec2 get_position( ) const = 0;

	protected:
		const Transform* transform_ptr_;

	};

	class GlobalProxyReadOnly final : public ProxyReadOnly
	{
	public:
		explicit GlobalProxyReadOnly( const Transform* pTransform );

		glm::vec2 get_position( ) const override;
	};

	class LocalProxyReadOnly final : public ProxyReadOnly
	{
	public:
		explicit LocalProxyReadOnly( const Transform* pTransform );

		glm::vec2 get_position( ) const override;
	};
}

#endif // !TRANSFORM_H
