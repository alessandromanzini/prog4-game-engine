#include "Transform.h"
#include <stdexcept>

using namespace engine;

// +--------------------------------+
// | Transform						|
// +--------------------------------+
void Transform::set_parent( Transform* parent )
{
	parent_ptr_ = parent;
}

GlobalProxy Transform::global( )
{
	return GlobalProxy( this );
}

const GlobalProxyReadOnly Transform::global( ) const
{
	return GlobalProxyReadOnly( this );
}

LocalProxy Transform::local( )
{
	return LocalProxy( this );
}

const LocalProxyReadOnly Transform::local( ) const
{
	return LocalProxyReadOnly( this );
}

// +--------------------------------+
// | Global Proxy					|
// +--------------------------------+
GlobalProxy::GlobalProxy( Transform* pTransform ) : Proxy( pTransform )
{
	if ( pTransform == nullptr )
	{
		throw std::invalid_argument( "Transform cannot be null!" );
	}
}

glm::vec2 GlobalProxy::get_position( ) const
{
	if ( transform_ptr_->parent_ptr_ != nullptr )
	{
		return transform_ptr_->parent_ptr_->global( ).get_position( ) + transform_ptr_->position_;
	}
	return transform_ptr_->position_;
}

void GlobalProxy::set_position( const glm::vec2& pos )
{
	if ( transform_ptr_->parent_ptr_ != nullptr )
	{
		transform_ptr_->position_ = pos - transform_ptr_->parent_ptr_->global( ).get_position( );
	}
	else
	{
		transform_ptr_->position_ = pos;
	}
}

// +--------------------------------+
// | Local Proxy					|
// +--------------------------------+
LocalProxy::LocalProxy( Transform* pTransform ) : Proxy( pTransform )
{
	if ( pTransform == nullptr )
	{
		throw std::invalid_argument( "Transform cannot be null!" );
	}
}

glm::vec2 LocalProxy::get_position( ) const
{
	return transform_ptr_->position_;
}

void LocalProxy::set_position( const glm::vec2& pos )
{
	transform_ptr_->position_ = pos;
}

// +--------------------------------+
// | Global Proxy Read Only			|
// +--------------------------------+
GlobalProxyReadOnly::GlobalProxyReadOnly( const Transform* pTransform )
	: ProxyReadOnly( pTransform )
{
	if ( pTransform == nullptr )
	{
		throw std::invalid_argument( "Transform cannot be null!" );
	}
}

glm::vec2 GlobalProxyReadOnly::get_position( ) const
{
	if ( transform_ptr_->parent_ptr_ != nullptr )
	{
		return transform_ptr_->parent_ptr_->global( ).get_position( ) + transform_ptr_->position_;
	}
	return transform_ptr_->position_;
}

// +--------------------------------+
// | Local Proxy Read Only			|
// +--------------------------------+
LocalProxyReadOnly::LocalProxyReadOnly( const Transform* pTransform )
	: ProxyReadOnly( pTransform )
{
	if ( pTransform == nullptr )
	{
		throw std::invalid_argument( "Transform cannot be null!" );
	}
}

glm::vec2 LocalProxyReadOnly::get_position( ) const
{
	return transform_ptr_->position_;
}
