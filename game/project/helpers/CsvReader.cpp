#include "CsvReader.h"

#include <fstream>


namespace game
{
    CsvReader::CsvReader( const std::string& csvPath )
    {
        load_data_from_file( csvPath );
        data_iterator_ = data_lines_.begin( );
    }

    CsvReader::CsvReader(const std::filesystem::path& csvPath)
        : CsvReader( csvPath.string( ) )
    {
    }


    const std::string& CsvReader::get( const int index ) const
    {
        return data_iterator_->at( index );
    }


    int CsvReader::get_int( const int index ) const
    {
        return std::stoi( get( index ) );
    }


    float CsvReader::get_float( const int index ) const
    {
        return std::stof( get( index ) );
    }


    bool CsvReader::get_boolean( const int index ) const
    {
        return get_int( index );
    }


    int CsvReader::size( ) const
    {
        return static_cast<int>( data_lines_.size( ) );
    }


    bool CsvReader::next( )
    {
        ++data_iterator_;
        return eof( );
    }


    bool CsvReader::eof( ) const
    {
        return data_iterator_ == data_lines_.end( );
    }


    void CsvReader::load_data_from_file( const std::string& csvPath )
    {
        constexpr char csvSeparator{ ';' };

        std::ifstream csv{};
        std::string line;

        size_t separatorCount{};

        csv.open( csvPath );
        if ( !csv )
        {
            throw std::runtime_error( "Couldn't open file at '" + csvPath + "'." );
        }

        while ( std::getline( csv, line ) ) // while there are lines, keep reading
        {
            if ( !csv.good( ) && !csv.eof( ) )
            {
                throw std::runtime_error(
                    "Error while reading csv file at '" + csvPath + "'.\nrdstate: " + std::to_string( csv.rdstate( ) ) );
            }

            if ( line.empty( ) ) continue; // avoid blank lines

            int readerPos{};
            std::string::size_type separatorPos{};
            {
                std::vector<std::string> dataLine{};
                dataLine.reserve( separatorCount );
                data_lines_.push_back( std::move( dataLine ) );
            }

            // if separator exists, put its position in separatorPos and do while's body
            while ( ( separatorPos = line.find( csvSeparator, static_cast<std::string::size_type>( readerPos ) ) ) !=
                    std::string::npos )
            {
                std::string field{ line.substr( readerPos, separatorPos - readerPos ) };

                data_lines_.back( ).push_back( std::move( field ) );

                separatorCount = static_cast<int>( data_lines_.size( ) );
                readerPos      = static_cast<int>( separatorPos ) + 1;
            }
        }
    }

}
