#ifndef FONT_H
#define FONT_H

#include <string>

struct _TTF_Font;
namespace engine
{
	/**
	 * Simple RAII wrapper for a _TTF_Font
	 */
	class Font final
	{
	public:
		explicit Font( const std::string& fullPath, unsigned int size );
		~Font( );

		Font( const Font& ) = delete;
		Font( Font&& ) = delete;
		Font& operator= ( const Font& ) = delete;
		Font& operator= ( const Font&& ) = delete;

		_TTF_Font* get_font( ) const;

	private:
		_TTF_Font* font_ptr_;
	};
}

#endif // !FONT_H
