#ifndef MINIGIN_H
#define MINIGIN_H

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <string>
#include <functional>
#include <filesystem>

namespace engine
{
	class Minigin final
	{
	public:
		explicit Minigin( const std::filesystem::path& dataPath );
		~Minigin( ) noexcept;
		
		Minigin( const Minigin& )					= delete;
		Minigin( Minigin&& ) noexcept				= delete;
		Minigin& operator=( const Minigin& ) 		= delete;
		Minigin& operator=( Minigin&& ) noexcept 	= delete;

		void run( const std::function<void( )>& load );
		void run_one_frame( );

	private:
		bool quit_{};

	};
}

#endif // !MINIGIN_H
