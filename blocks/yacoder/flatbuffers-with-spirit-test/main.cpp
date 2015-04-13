#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "yacoder/flatbuffers-with-spirit/flatbuffer_manifest.h"
#include "yacoder/flatbuffers-with-spirit/flatbuffer_model.h"

using namespace std;
using namespace yacoder::flatbuffers;

TEST_CASE( "Parsing root declaration", "" )
{
    flatbuffer_manifest m;
    REQUIRE( TryParseIdlString("root_typer;", m) );
    REQUIRE( m.root_type.name == "r" );
}

TEST_CASE( "Parsing a struct and a table", "" )
{
    string test =
R"s(
struct Test { a:short; b:byte; }
table Stat {
   id:string;
   val:[long];
   count:ushort;
}
root_type Test;
)s";

    flatbuffer_manifest m;
    REQUIRE( TryParseIdlString(test, m) );
    REQUIRE( m.root_type.name == "Test" );

    REQUIRE( m.type_definitions.size() == 2 );
    REQUIRE( m.type_definitions[0].name == "Test" );
    REQUIRE( m.type_definitions[1].name == "Stat" );

    REQUIRE(m.type_definitions[0].is_struct == true);
    REQUIRE(m.type_definitions[1].is_struct == false);

    REQUIRE ( m.type_definitions[0].field_definitions.size() == 2 );
    REQUIRE ( m.type_definitions[0].field_definitions[0].name == "a" );
    REQUIRE ( m.type_definitions[0].field_definitions[0].type.name == "short" );
    REQUIRE ( m.type_definitions[0].field_definitions[0].type.is_array == false );
    REQUIRE ( m.type_definitions[0].field_definitions[1].name == "b" );
    REQUIRE ( m.type_definitions[0].field_definitions[1].type.name == "byte" );
    REQUIRE ( m.type_definitions[0].field_definitions[1].type.is_array == false );

    REQUIRE ( m.type_definitions[1].field_definitions.size() == 3 );
    REQUIRE ( m.type_definitions[1].field_definitions[0].name == "id" );
    REQUIRE ( m.type_definitions[1].field_definitions[0].type.name == "string" );
    REQUIRE ( m.type_definitions[1].field_definitions[0].type.is_array == false );
    REQUIRE ( m.type_definitions[1].field_definitions[1].name == "val" );
    REQUIRE ( m.type_definitions[1].field_definitions[1].type.name == "long" );
    REQUIRE ( m.type_definitions[1].field_definitions[1].type.is_array == true );
    REQUIRE ( m.type_definitions[1].field_definitions[2].name == "count" );
    REQUIRE ( m.type_definitions[1].field_definitions[2].type.name == "ushort" );
    REQUIRE ( m.type_definitions[1].field_definitions[2].type.is_array == false );
}

TEST_CASE( "Generate simple cpp output", "" )
{
    flatbuffer_model m;
    m.vec = { 1, 2, 3 };
    string output;
    REQUIRE( TryGenerateCpp(m, output) );
    REQUIRE( output == "1 , 2 , 3 " );
}
