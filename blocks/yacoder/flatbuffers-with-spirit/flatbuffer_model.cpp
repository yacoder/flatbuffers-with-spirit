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

/* NOTE-yacoder-2015-04-13: implementation of flatbuffers handlers generation with Boost.Karma.
*/

#include "flatbuffer_model.h"

// Boost headers produce a dozen of C4459	declaration of 'attr' hides global declaration
#pragma warning(push)
#pragma warning(disable: 4459) 

#include "boost/spirit/include/karma.hpp"

#pragma warning(pop)

using namespace std;
using namespace yacoder::flatbuffers;

    // http://www.boost.org/doc/libs/1_57_0/libs/spirit/example/karma/num_list2.cpp
    template <typename OutputIterator, typename Container>
    bool generate_numbers(OutputIterator& sink, Container const& v)
    {
        using boost::spirit::karma::int_;
        using boost::spirit::karma::generate_delimited;
        using boost::spirit::ascii::space;

        bool r = generate_delimited(
            sink,                           // destination: output iterator
            int_ % ',',                     // the generator
            space,                          // the delimiter-generator
            v                               // the data to output 
        );
        return r;
    }

namespace yacoder { namespace flatbuffers {

bool TryGenerateCpp(
   const flatbuffer_model& input_model,
   string& output)
{
   std::back_insert_iterator<std::string> sink(output);
   return generate_numbers(sink, input_model.vec);
}


}} // END namespace yacoder::flatbuffers 
