// g++ -o type_alignment type_alignment.cc

#include <cxxabi.h>
#include <inttypes.h>
#include <stdio.h>
#include <string>

template < typename T >
void
 print_address_and_alignment( )
{
    T sample;

    /* Store type name in printable form. */
    const size_t nofCharsAllowed = 35;

    const char dots[] = "...";

    char name[ nofCharsAllowed + sizeof(dots) ];
    char * fullName = abi::__cxa_demangle( typeid( T ).name( ), 0, 0, NULL );

    if ( sizeof( name ) <= snprintf( name, sizeof( name ), "%s", fullName ) )
    {
        snprintf( &name[ nofCharsAllowed ], sizeof( dots ), "%s", dots );
    }

    free(fullName);

    /* Printing type and allocation info. */

    printf( "### " );

    printf( "%s\n"
            "Size of  : %zu\n"
            "Align of : %zu\n",
            name,
            sizeof( T ),
            alignof( T ) );

    T aStack[ 4 ];
    printf( "STACK:\n" );
    for ( int i = 0; i < 4; ++i )
    {
        printf( "- Address of %s[%d] : 0x%016" PRIXPTR "\n",
                name,
                i,
                ( void * ) &( aStack[ i ] ) );
    }

    T * aHeap = new T[ 4 ];
    printf( "HEAP:\n" );
    for ( int i = 0; i < 4; ++i )
    {
        printf( "- Address of %s[%d] : 0x%016" PRIXPTR "\n",
                name,
                i,
                ( void * ) &( aHeap[ i ] ) );
    }

    delete[] aHeap;

    printf( "\n" );
}

struct TestStruct0
{
    char * chr;   // 8 bit, 4 bytes 2^3
};

struct TestStruct1
{
    int num;      // 32 bit, 4 bytes 2^5
    int * ptr;    // 64 bit, 8 bytes 2^6
    int * ptr2;   // 64 bit, 8 bytes 2^6
};

struct TestStruct2
{
    int num;       // 32 bit, 4 bytes 2^5
    int * ptr;     // 64 bit, 8 bytes 2^6
    int * ptr2;    // 64 bit, 8 bytes 2^6
    int * ptr3;    // 64 bit, 8 bytes 2^6
    int * ptr4;    // 64 bit, 8 bytes 2^6
    char * name;   // 8 bit, 1 bytes 2^3
};

int
 main( void )
{
    print_address_and_alignment< char >( );
    print_address_and_alignment< int >( );
    print_address_and_alignment< TestStruct0 >( );
    print_address_and_alignment< TestStruct1 >( );
    print_address_and_alignment< TestStruct2 >( );
    print_address_and_alignment< std::string >( );
    return 0;
}
