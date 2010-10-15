import os

# NOTE: uncomment -save-temps to examine ASM code...
BASE_FLAGS = '-ansi -pedantic -Wall -Wextra' # + ' -save-temps'
DEBUG_FLAGS = BASE_FLAGS + ' -g'
RELEASE_FLAGS = BASE_FLAGS + ' -DNDEBUG -O3'

env = Environment(ENV=os.environ)
Export('env')

# Debug flags: > scons debug=0 to turn debugging off
# Support for multiple debug levels if required
flags = RELEASE_FLAGS
debug_level = ARGUMENTS.get('DEBUG', 1)
if int(debug_level) > 0:
    flags = DEBUG_FLAGS + ' -DDEBUG=' + str(debug_level)
env.Append(CCFLAGS = flags.split())

# Other flags
env.Append(LIBPATH=[ "./", ])
env.Append(CPPPATH=[ "./", "deps", "deps/boost_1_43_0" ])

# Libs
env.StaticLibrary(target = "Indexes", source = env.Glob("indexes/*.cc"))
env.StaticLibrary(target = "CppUnitLite",
    source = env.Glob("deps/CppUnitLite/*.cpp"))

# Unit Tests
tests = env.Program("runtests", LIBS = ["CppUnitLite", "Indexes" ],
    source=[ "tests/TestRunner.cc", env.Glob("tests/*Test.cc")])
# Run the unit tests as part of the build process
env.AddPostAction(tests, "./runtests")
env.AlwaysBuild(tests)
env.Alias('test', tests)

# Program
prog = env.Program("main", LIBS = ["Indexes"], source = ["main.cc"])