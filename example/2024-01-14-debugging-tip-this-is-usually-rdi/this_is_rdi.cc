#include <cstdio>
#include <ctime>
#include <cmath>

#define RAND_MOD 32

struct Foo
{
    Foo( const char offset ) : m_charOffset( offset )
    {
    }

    /* Generate a random char, offsetting
       the value to get a printable character. */
    char
     RandChar( )
    {
        srand( clock( ) );
        char randChar = rand( ) % RAND_MOD;
        return randChar + this->m_charOffset;
    }

    char m_charOffset;
};

int
 main( )
{
    Foo foo{ 0x40 };

    printf( "Generated char is: %c\n", foo.RandChar( ) );

    return 0;
}
