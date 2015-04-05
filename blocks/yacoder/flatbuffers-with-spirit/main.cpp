#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "boost/fusion/adapted/struct/define_struct.hpp"
#include "boost/fusion/include/define_struct.hpp"

#include "boost/fusion/include/std_pair.hpp"

#define BOOST_SPIRIT_DEBUG
#include "boost/spirit/include/qi.hpp"
#include "boost/spirit/include/phoenix.hpp"
//#include "boost/optional.hpp"

#include <map>
#include <vector>
#include <string>
#include <utility>

using namespace std;

namespace fusion = boost::fusion;
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

using boost::optional;

// http://boost-spirit.com/home/articles/qi-example/zero-to-60-mph-in-2-seconds/
// http://stackoverflow.com/questions/10474571/how-to-match-unicode-characters-with-boostspirit
// http://stackoverflow.com/questions/19273882/handling-utf-8-in-boost-spirit-with-utf-32-parser
// https://www.youtube.com/watch?v=aca4JTcDG2E
// http://www.boost.org/doc/libs/1_57_0/libs/fusion/doc/html/fusion/adapted/define_struct.html
// http://www.boost.org/doc/libs/1_57_0/libs/spirit/doc/html/spirit/qi/tutorials/mini_xml___error_handling.html

BOOST_FUSION_DEFINE_STRUCT(
    (),
    field_type_type,
    (string, type_name)
            (bool, is_array)
    );

BOOST_FUSION_DEFINE_STRUCT(
        (),
        field_decl_type,
        (string, field_name)
                (field_type_type, field_type));

BOOST_FUSION_DEFINE_STRUCT(
    (),
    type_decl_type,
    (string, type_name)
    (std::vector<field_decl_type>, fields)
            );

BOOST_FUSION_DEFINE_STRUCT(
    (),
    root_decl_type,
    (string, root_type));

BOOST_FUSION_DEFINE_STRUCT(
    (),
    flatbuffers_manifest_type,
            (std::vector<type_decl_type>, types)
            (root_decl_type, root_decl)
    );

// http://google.github.io/flatbuffers/md__grammar.html
template<typename Iterator>
struct flatbuffers_grammar : qi::grammar<Iterator, flatbuffers_manifest_type(), qi::space_type>
{
    flatbuffers_grammar()
        : flatbuffers_grammar::base_type(flatbuffers_manifest)
    {
        using namespace qi;

        ident = char_("a-zA-Z") >> *char_("a-zA-Z_0-9");
//        primitive_type = qi::string("bool") | qi::string("byte") | qi::string("ubyte")
//                         | qi::string("short") | qi::string("ushort") | qi::string("int")
//                         | qi::string("uint") | qi::string("float") | qi::string("long")
//                         | qi::string("ulong") | qi::string("double") | qi::string("string");

        // http://www.boost.org/doc/libs/1_57_0/libs/spirit/doc/html/spirit/qi/tutorials/employee___parsing_into_structs.html
        quoted_string = lexeme['"' >> +(char_ - '"') >> '"'];

        // "type" definition in flatbuffers grammar allows nested arrays... ?

        field_type = ident [_val = phoenix::construct<field_type_type>(_1, false)]
                     | ('[' >> ident >> ']') [_val = phoenix::construct<field_type_type>(_1, true)];

        field_decl = ident >> ':' >> field_type >> ';';

        //[bind(&type_decl_type::is_table, _val) = true])
        type_decl = (lit("struct") | lit("table"))
                    >> ident
                    >> '{'
                    >> *field_decl
                    >> '}';

        // How to force some whitespace between "root_type" and ident?
        root_decl = "root_type" >> ident >> ';';

        flatbuffers_manifest = *type_decl ^ root_decl;

        //BOOST_SPIRIT_DEBUG_NODES((flatbuffers_manifest)(root_decl)(ident));
    }

    qi::rule<Iterator, string()> ident;
    //qi::rule<Iterator, string()> primitive_type;
    qi::rule<Iterator, string()> quoted_string;

    qi::rule<Iterator, field_type_type(), qi::space_type> field_type;
    qi::rule<Iterator, field_decl_type(), qi::space_type> field_decl;

    qi::rule<Iterator, type_decl_type(), qi::space_type> type_decl;

    qi::rule<Iterator, root_decl_type(), qi::space_type> root_decl;

    // our goal is to parse this manifest structure
    qi::rule<Iterator, flatbuffers_manifest_type(), qi::space_type> flatbuffers_manifest;
};

static bool parse_manifest(const string& input, flatbuffers_manifest_type& m)
{
    flatbuffers_grammar<string::const_iterator> parser;
    return qi::phrase_parse(input.begin(), input.end(), parser, qi::space, m);
}

TEST_CASE( "Parsing root declaration", "" )
{
    flatbuffers_manifest_type m;
    REQUIRE( parse_manifest("root_typer;", m) );
    REQUIRE( m.root_decl.root_type == "r" );
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

    flatbuffers_manifest_type m;
    REQUIRE( parse_manifest(test, m) );
    REQUIRE( m.root_decl.root_type == "Test" );

    REQUIRE( m.types.size() == 2 );
    REQUIRE( m.types[0].type_name == "Test" );
    REQUIRE( m.types[1].type_name == "Stat" );

    REQUIRE ( m.types[0].fields.size() == 2 );
    REQUIRE ( m.types[0].fields[0].field_name == "a" );
    REQUIRE ( m.types[0].fields[0].field_type.type_name == "short" );
    REQUIRE ( m.types[0].fields[0].field_type.is_array == false );
    REQUIRE ( m.types[0].fields[1].field_name == "b" );
    REQUIRE ( m.types[0].fields[1].field_type.type_name == "byte" );
    REQUIRE ( m.types[0].fields[1].field_type.is_array == false );

    REQUIRE ( m.types[1].fields.size() == 3 );
    REQUIRE ( m.types[1].fields[0].field_name == "id" );
    REQUIRE ( m.types[1].fields[0].field_type.type_name == "string" );
    REQUIRE ( m.types[1].fields[0].field_type.is_array == false );
    REQUIRE ( m.types[1].fields[1].field_name == "val" );
    REQUIRE ( m.types[1].fields[1].field_type.type_name == "long" );
    REQUIRE ( m.types[1].fields[1].field_type.is_array == true );
    REQUIRE ( m.types[1].fields[2].field_name == "count" );
    REQUIRE ( m.types[1].fields[2].field_type.type_name == "ushort" );
    REQUIRE ( m.types[1].fields[2].field_type.is_array == false );
}


template<typename Iterator>
struct keys_and_values : qi::grammar<Iterator, map<string, string>()>
{
    keys_and_values()
            : keys_and_values::base_type(query)
    {
        query = kv >> *((qi::lit(';') | '&') >> kv);
        kv = key >> -('=' >> value);
        key = qi::char_("a-zA-Z") >> *qi::char_("a-zA-Z_0-9");
        value = +qi::char_("a-zA-Z_0-9");
    }

    qi::rule<Iterator, map<string, string>()> query;
    qi::rule<Iterator, pair<string, string>()> kv;
    qi::rule<Iterator, string()> key, value;
};

TEST_CASE( "Parsing keys and values", "" )
{
    std::string input("key1=value1&key2=value2&key3=value3");
    keys_and_values<string::iterator> parser;
    map<string, string> m;
    bool result = qi::parse(input.begin(), input.end(), parser, m);

    REQUIRE( result );
    REQUIRE( m["key1"] == "value1" );
    REQUIRE( m["key2"] == "value2" );
}


int ParseInt( const std::string& s )
{
    int i;
    qi::parse(s.begin(), s.end(), qi::int_, i);
    return i;
}
 
TEST_CASE( "Parsing simple ints", "" ) {
    REQUIRE( ParseInt("1") == 1 );
    REQUIRE( ParseInt("2") == 2 );
    REQUIRE( ParseInt("6") == 6 );
    REQUIRE( ParseInt("3628800") == 3628800 );
}

