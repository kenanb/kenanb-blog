---
layout: post
title:  "Observing Type Size and Alignment"
date:   2024-01-11 21:00:00 +1300
categories: code c-cpp
published: true
---

A few lines of code to check size and alignment of various types for C/C++.

[ *Check out all posts in "c-cpp" series [here][series-c-cpp].* ]

[series-c-cpp]: {% link series.md %}#c-cpp

Yesterday, I wrote about [X-macros and the preprocessor][int-prev]. So I decided to continue with more C++.

I found this old file among my notes. It is a super simple program to observe:
- size and alignment of various types in C/C++.
- Print sample heap allocation and stack addresses for each, as hints into what kind of hex address patterns to expect for the type.

Necessary includes:
``` cpp
#include <cxxabi.h>
#include <inttypes.h>
#include <stdio.h>
#include <string>
```

We have a `print_address_and_alignment` function template:
``` cpp
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
```

A few test structs:
``` cpp
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
```

And main:
```
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
```

Running this will produce an output like so:

```
### char
Size of  : 1
Align of : 1
STACK:
- Address of char[0] : 0x00007FFCC422AE6C
- Address of char[1] : 0x00007FFCC422AE6D
- Address of char[2] : 0x00007FFCC422AE6E
- Address of char[3] : 0x00007FFCC422AE6F
HEAP:
- Address of char[0] : 0x00005558B19922B0
- Address of char[1] : 0x00005558B19922B1
- Address of char[2] : 0x00005558B19922B2
- Address of char[3] : 0x00005558B19922B3

### int
Size of  : 4
Align of : 4
STACK:
- Address of int[0] : 0x00007FFCC422AE50
- Address of int[1] : 0x00007FFCC422AE54
- Address of int[2] : 0x00007FFCC422AE58
- Address of int[3] : 0x00007FFCC422AE5C
HEAP:
- Address of int[0] : 0x00005558B19922B0
- Address of int[1] : 0x00005558B19922B4
- Address of int[2] : 0x00005558B19922B8
- Address of int[3] : 0x00005558B19922BC
```
...

```
### TestStruct2
Size of  : 48
Align of : 8
STACK:
- Address of TestStruct2[0] : 0x00007FFC152050F0
- Address of TestStruct2[1] : 0x00007FFC15205120
- Address of TestStruct2[2] : 0x00007FFC15205150
- Address of TestStruct2[3] : 0x00007FFC15205180
HEAP:
- Address of TestStruct2[0] : 0x00005635DC971780
- Address of TestStruct2[1] : 0x00005635DC9717B0
- Address of TestStruct2[2] : 0x00005635DC9717E0
- Address of TestStruct2[3] : 0x00005635DC971810

### std::__cxx11::basic_string<char, std::ch...
Size of  : 32
Align of : 8
STACK:
- Address of std::__cxx11::basic_string<char, st...[0] : 0x00007FFC15205110
- Address of std::__cxx11::basic_string<char, st...[1] : 0x00007FFC15205130
- Address of std::__cxx11::basic_string<char, st...[2] : 0x00007FFC15205150
- Address of std::__cxx11::basic_string<char, st...[3] : 0x00007FFC15205170
HEAP:
- Address of std::__cxx11::basic_string<char, st...[0] : 0x00005635DC971858
- Address of std::__cxx11::basic_string<char, st...[1] : 0x00005635DC971878
- Address of std::__cxx11::basic_string<char, st...[2] : 0x00005635DC971898
- Address of std::__cxx11::basic_string<char, st...[3] : 0x00005635DC9718B8
```

That's all for today. The source is [shared in blog's repository][source]. If you find technical errors, please report in the blog's [Issues page][report].

Thanks for reading!

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-prev]: {% post_url 2024-01-10-on-xmacros-and-preprocessor %}

[source]: https://github.com/kenanb/kenanb-blog/tree/main/example/2024-01-11-observing-type-size-and-alignment
