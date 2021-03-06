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

/* NOTE-yacoder-2015-04-12: this header file defines the flatbuffer_model 
  structure, which describes the layout of a particular flatbuffer, and
  a number of functions that take the model and generate code that can
  handle flatbuffers of this model in various languages.
*/
#pragma once

#include <string>
#include <vector>

namespace yacoder { namespace flatbuffers {

struct flatbuffer_model
{
   std::vector<int> vec;
};

/* Generates C++ code to handle the given flatbuffer model. 
*/
bool TryGenerateCpp(
   const flatbuffer_model& input_model,
   std::string& output);

}} // END namespace yacoder::flatbuffers 
