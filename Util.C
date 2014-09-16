#include "Util.H"


void find_and_replace(string& source, string const& find, string const& replace)
{
    for(string::size_type i = 0; (i = source.find(find, i)) != string::npos;)
    {
            source.replace(i, find.length(), replace);
            i += replace.length() - find.length() + 1;
        }
}
