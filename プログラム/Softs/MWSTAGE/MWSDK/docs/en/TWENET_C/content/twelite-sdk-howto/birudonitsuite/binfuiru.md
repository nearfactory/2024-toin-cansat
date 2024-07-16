# bin file naming conventions

The basic form of a bin file name is:

> {Project Name}\_{Variant name}\_{ARCH name}\_{TWENET library version}\_{app version from Version.mk}
>
> e.g.) Samp_PingPong_PingPong_BLUE_L1200\_V0-1-4.bin

| name                        | remark                                                                                                                                         | example       |
| --------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------- | ------------- |
| Project Name                | This is usually the name of the top-level folder in the project.                                                                               | Samp_PingPong |
| Variant Name                | This is usually the name of a subfolder in the project, if you want to create two separate projects, one for the parent and one for the child. | PingPong      |
| ARCH Name                   | BLUE: TWELITE BLUE, RED: TWELITE RED                                                                                                           | BLUE          |
| TWENET lib version          | The library version identifier.                                                                                                                | L1200         |
| app version from Version.mk | The version number as given in the makefile or Version.mk file.                                                                                | V0-1-4        |

