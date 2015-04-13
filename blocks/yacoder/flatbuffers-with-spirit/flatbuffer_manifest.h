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

/* NOTE-yacoder-2015-04-12: this header file defines the flatbuffer_manifest 
  structure, which represents the contents of a parsed Flatbuffers IDL file,
  and a number of parsing functions.
*/
#pragma once

#include <string>
#include <vector>

namespace yacoder { namespace flatbuffers {

struct flatbuffer_manifest
{
   struct type_definition
   {
      std::string name; //!< Name of the type from the IDL file.
      
      struct field_definition
      {
          std::string name; //!< Name of the field from the IDL file.

          struct field_type
          {
              std::string name; //!< Name of the type from the IDL file.
              bool is_array;    //!< Whether the field is array type, e.g. true for "[int]".
          }
          type; //!< Type of the field, such as "int", "[bool]", or "MyStruct".
      };

      /* Types have fieds. */
      std::vector<field_definition> field_definitions;
   };

   /* Tables and structs as defined in the IDL file. */
   std::vector<type_definition> type_definitions; 

   /* Flatbuffers IDL file must declare a root type. */
   struct root_type_declaration
   {
      std::string name;
   }
   root_type;
};

/* Attempts to parse the given string as Flatbuffers IDL. 
*/
bool TryParseIdlString(
   const std::string& input_text,
   // std::function<std::string(const std::string& include)>() include_resolver,
   // std::vector<std::string>& errors,
   flatbuffer_manifest& output);

/* Attempts to parse the given string as Flatbuffers IDL. 
*/
bool TryParseIdlFile(
   const std::string& input_file_path,
   // std::vector<std::string>& errors,
   flatbuffer_manifest& output);

}} // END namespace yacoder::flatbuffers 
