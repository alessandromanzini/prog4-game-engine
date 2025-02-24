#ifndef DELETER_H
#define DELETER_H

#include <concepts>
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace engine
{
	template <typename deletable_t>
	class Deleter final
	{
	public:
		void mark_index_for_deletion( size_t index )
		{
			indices_to_destroy_.insert( index );
		}

		void cleanup( std::vector<std::shared_ptr<deletable_t>>& deletables )
		{
			// Remove elements in one pass using erase-remove idiom
			// 'i' is the index of the object in the vector
			// 'i' gets checked against the indices_to_destroy_ set, returning 0 for no match or 1 for match
			std::erase_if( deletables,
				[this, i = uint64_t( 0 )]( auto ) mutable { return indices_to_destroy_.count( i++ ); } );
			indices_to_destroy_.clear( );
		}

		void cleanup( std::unordered_multimap<uint64_t, std::unique_ptr<deletable_t>>& deletables )
		{
			for ( auto index : indices_to_destroy_ )
			{
				auto range = deletables.equal_range( index );
				for ( auto it = range.first; it != range.second;)
				{
					if ( it->second->is_marked_for_deletion( ) )
					{
						it = deletables.erase( it );
					}
					else
					{
						++it;
					}
				}
			}
		}

		[[nodiscard]] bool is_cleanup_needed( ) const
		{
			return !indices_to_destroy_.empty( );
		}

	private:
		// We choose a set because we don't want duplicates and we retain O(1) complexity for 
		// insertion, deletion, and search.
		std::unordered_set<uint64_t> indices_to_destroy_{};

	};
}

#endif // !DELETER_H