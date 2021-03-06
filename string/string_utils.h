#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

bool EndsWith(const std::string& a, const std::string& b);
std::string ImplodeVector( const std::vector< std::string>& vecstr, const std::string& separator );
std::vector< std::string> ExplodeString( const std::string& str, const std::string& charDelimiters, const bool PreserveDelimiters = false );
std::string Trim( const std::string& trim );
std::string StripString( std::string str, std::string charsToStrip );
std::string StringReplace( std::string result, const std::string& replaceWhat, const std::string& replaceWithWhat);

std::string IntToString( const int i );
std::string CharToString( const char ch );
std::string StringSubstituteNumber( std::string str, const unsigned int number );

template<typename T>
T StringToNumber(const std::string& numberAsString)
{
    T valor;
    
    std::stringstream stream(numberAsString);
    stream >> valor;
    if (stream.fail()) {
        std::runtime_error e(numberAsString);
        throw e;
    }
    return valor;
}


std::vector<std::string> GetRegexMatches( std::string str, std::string pattern );

