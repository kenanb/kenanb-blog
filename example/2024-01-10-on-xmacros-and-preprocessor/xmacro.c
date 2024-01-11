#include <stdio.h>
#include <stdlib.h>

/* /////////////////////////////////////////////////////// */
/* UTILITY                                                 */

#define NSX( NS, X ) NS##X
#define NS( NS, X ) NSX( NS, X )

#define STRX( X ) #X
#define STR( X ) STRX( X )

/* /////////////////////////////////////////////////////// */
/* CONDITION HANDLING                                      */

/* Namespacing the condition values. */
#define COND_NS FOO_CONDITION_

/* Used to refer to enums. */
#define cond_( X ) NS( COND_NS, X )

/* Get enum symbol as string. */
#define COND_STR( X ) STR( cond_( X ) )

/* Condition enum. */
typedef enum Condition
{
    cond_( SUCCESS ) = 0,

#define CONDITION( C, M ) cond_( C ),
#include "error.codes"
#undef CONDITION

    /* Non-error. Value to use in loop expressions. */
    cond_( END ),
} FooCondition_e;

/* Generate error message for given condition. */
const char *
 fooReport( FooCondition_e cond )
{
    switch ( cond )
    {

#define CONDITION( C, M )                                   \
    case cond_( C ):                                        \
        return M " ( Code: " COND_STR( C ) " )\n";
#include "error.codes"
#undef CONDITION

        default:
            return "Success.";
    }
}

/* /////////////////////////////////////////////////////// */
/* MAIN                                                    */

/* Example function returning a condition. */
FooCondition_e
 fooTest( )
{
    return cond_( GLFW_WINDOW_FAILED );
}

int
 main( )
{
    FooCondition_e cond = fooTest( );

    /* Condition usage example. */
    if ( cond )
    {
        fprintf( stderr, "%s\n", fooReport( cond ) );

        /* Handle relevant conditions. */
        switch ( cond )
        {
            case cond_( GLFW_WINDOW_FAILED ):
            {
                fprintf( stderr, "Printing extra information here.\n" );
                break;
            }
            case cond_( UNSUPPORTED_EXTENSION ):
            {
                fprintf( stderr, "Doing other things here.\n" );
                break;
            }
            default:
                break;
        }

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
