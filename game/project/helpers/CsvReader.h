#ifndef CSVREADER_H
#define CSVREADER_H

#include <list>
#include <unordered_map>


namespace game
{
    class CsvReader final
    {
    public:
        explicit CsvReader( const std::string& csvPath );

        const std::string& get( int index ) const;
        int get_int( int index ) const;
        float get_float( int index ) const;
        bool get_boolean( int index ) const;

        int size( ) const;

        bool next( );
        bool eof( ) const;

    private:
        std::list<std::vector<std::string>> data_lines_{};
        std::list<std::vector<std::string>>::iterator data_iterator_{};

        void load_data_from_file( const std::string& csvPath );

    };

}


#endif //!CSVREADER_H
