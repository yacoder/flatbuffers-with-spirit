# flatbuffers-with-spirit

Google flatbuffers implementation using Boost.Spirit grammars and Biicode dependency manager.

Blog post: http://yacoder.net/blog/2015/04/05/boost-spirit-flatbuffers-catch-biicode-clion-github/

This is only an experimental project. Flatbuffers specification does not seem to fully describe the binary layout, and so I have no way of guaranteeing 100% compatibility in all cases.

# How to build

Instruction verified on Windows, but should not be very much different on OS X.

- Have fresh biicode and CMake installed.
- Clone this repo.
- `bii init` in the repo folder (`bii init -l clion` if you wish to work with the project using CLion IDE)
- `bii configure` with the right parameters (see *.cmd for examples, but you may need to change paths in there)
- `bii build` (or build from VS or CLion using the files produced in the previous step)
- Run `bin/yacoder_flatbuffers-with-spirit-test_main.exe`, all test cases should pass

