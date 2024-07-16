# About Makefile

The source code and other definitions to be built are written in the Makefile.

| TWELITE                      | Specify the model to be built, BLUE for TWELITE BLUE (TWE-001 Lite) or RED for TWELITE RED.                                                                                                                                |
| ---------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| PROJNAME                     | Decide the name of the generated file. If not specified, use. If not specified, the `../..` folder name will be used.                                                                                                   |
| APPSRC                       | Specify the file to be compiled, using the `+=` operator, as in `APPSRC+=test.c`.                                                                                                                                          |
| TARGET_TYPE                  | Specify the target to generate. When `TARGET_TYPE = bin` is specified, the target is an executable, and when `TARGET_TYPE = a` is specified, the target is a library.。                                                     |
| TOCONET_DEBUG                | If 1, include the debug code for the stack. The output file name will be prefixed with `_TDBG`. See the API description for the stack debug code. 0 means do not include it, and the binary size will be slightly smaller. |
| CFLAGS                       | Add gcc options at compile time. Use the `+=` operator, like `CFLAGS += -DMY_DEBUG`. Do not specify any optimization options, as they are predefined.                                                                      |
| APP_COMMON_SRC_DIR_ADD1 .. 4 | Specify the search path for source and header files. Liks as `APP_COMMON_SRC_DIR_ADD1 = ... /mydir`, relative to the folder where the Makefile is located.                                                              |
| INCFLAGS                     | If you want to specify additional include folders only, use relative paths like `INCLFAGS == -I.../mydir` to specify a relative path.                                                                                  |
| ADDITIONAL_LIBS              | Specify additional libraries (`.a`) to be linked, like `ADDITIONAL_LIBS += . /mylib.a`.                                                                                                                                    |
| ADDITIONAL_OBJS              | Specify additional object files (.o) to be linked, like`ADDITIONAL_OBJS += ../myobj.o`.                                                                                                                                    |
| LDLIBS                       | Specify the compiler libraries (math, spp, etc.) by adding `LDLIBS += m`. In this specification, -lm is passed to the linker.                                                                                              |
| LDFLAGS                      | Additional options to the linker can be specified as `LDFLAGS += -u dummy`. The options required by TWELITE are pre-defined.                                                                                               |
| OBJDIR_SUFF                  | Include the string set in the object storage folder name.                                                                                                                                                               |
| TARGET_SUFF                  | Include the string you set in the target (.bin) file name.                                                                                                                                                                 |

