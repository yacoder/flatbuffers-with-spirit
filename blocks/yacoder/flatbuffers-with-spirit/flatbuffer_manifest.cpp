/*
The MIT License (MIT)

Copyright (c) 2015 Maksim Galkin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* NOTE-yacoder-2015-04-12: implementation of flatbuffers IDL parsing with Boost.Qi.
*/

#include "flatbuffer_manifest.h"

// Boost headers produce a dozen of C4459	declaration of 'attr' hides global declaration
#pragma warning(push)
#pragma warning(disable: 4459) 

#include "boost/fusion/adapted/struct/adapt_struct.hpp"
#include "boost/fusion/include/adapt_struct.hpp"

#define BOOST_SPIRIT_DEBUG
#include "boost/spirit/include/qi.hpp"
#include "boost/spirit/include/phoenix.hpp"

#pragma warning(pop)

using namespace std;
using namespace yacoder::flatbuffers;

namespace fusion  = boost::fusion;
namespace qi      = boost::spirit::qi;
namespace phoenix = boost::phoenix;

// http://boost-spirit.com/home/articles/qi-example/zero-to-60-mph-in-2-seconds/
// http://stackoverflow.com/questions/10474571/how-to-match-unicode-characters-with-boostspirit
// http://stackoverflow.com/questions/19273882/handling-utf-8-in-boost-spirit-with-utf-32-parser
// https://www.youtube.com/watch?v=aca4JTcDG2E
// http://www.boost.org/doc/libs/1_57_0/libs/fusion/doc/html/fusion/adapted/define_struct.html
// http://www.boost.org/doc/libs/1_57_0/libs/spirit/doc/html/spirit/qi/tutorials/mini_xml___error_handling.html

using field_type = flatbuffer_manifest::type_definition::field_definition::field_type;
using field_definition = flatbuffer_manifest::type_definition::field_definition;
using type_definition = flatbuffer_manifest::type_definition;
using root_type_declaration = flatbuffer_manifest::root_type_declaration;


BOOST_FUSION_ADAPT_STRUCT
(
   field_type,
   (string, name)
   (bool, is_array)
);

BOOST_FUSION_ADAPT_STRUCT
(
   field_definition,
   (string, name)
   (field_type, type)
);

BOOST_FUSION_ADAPT_STRUCT
(
   type_definition,
   (string, name)
   (std::vector<field_definition>, field_definitions)
);

BOOST_FUSION_ADAPT_STRUCT
(
   root_type_declaration,
   (string, name)
);

BOOST_FUSION_ADAPT_STRUCT
(
   flatbuffer_manifest,
   (std::vector<type_definition>, type_definitions)
   (root_type_declaration, root_type)
);


// http://google.github.io/flatbuffers/md__grammar.html
template<typename Iterator>
struct flatbuffers_grammar : qi::grammar<Iterator, flatbuffer_manifest(), qi::space_type>
{
    flatbuffers_grammar()
        : flatbuffers_grammar::base_type(flatbuffer_manifest)
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
        field_type_rule = (ident >> attr(false))
                     | ('[' >> ident >> attr(true) >> ']');

        field_decl = ident >> ':' >> field_type_rule >> ';';

        //[bind(&type_decl_type::is_table, _val) = true])
        type_decl = (lit("struct") | lit("table"))
                    >> ident
                    >> '{'
                    >> *field_decl
                    >> '}';

        // How to force some whitespace between "root_type" and ident?
        root_decl = "root_type" >> ident >> ';';

        flatbuffer_manifest = *type_decl ^ root_decl;

        //BOOST_SPIRIT_DEBUG_NODES((flatbuffer_manifest)(root_decl)(ident));
    }

    qi::rule<Iterator, string()> ident;
    //qi::rule<Iterator, string()> primitive_type;
    qi::rule<Iterator, string()> quoted_string;

    qi::rule<Iterator, field_type(), qi::space_type> field_type_rule;
    qi::rule<Iterator, field_definition(), qi::space_type> field_decl;
    qi::rule<Iterator, type_definition(), qi::space_type> type_decl;
    qi::rule<Iterator, root_type_declaration(), qi::space_type> root_decl;

    // our goal is to parse this manifest structure
    qi::rule<Iterator, flatbuffer_manifest(), qi::space_type> flatbuffer_manifest;
};


namespace yacoder { namespace flatbuffers {


bool TryParseIdlString(const string& input, flatbuffer_manifest& output)
{
    flatbuffers_grammar<string::const_iterator> parser;
    return qi::phrase_parse(input.begin(), input.end(), parser, qi::space, output);
}


}} // END namespace yacoder::flatbuffers 
