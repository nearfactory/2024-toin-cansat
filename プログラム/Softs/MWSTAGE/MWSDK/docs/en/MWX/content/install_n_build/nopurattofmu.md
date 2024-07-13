---
title: "Other platforms"
author: "Mono Wireless Inc."
description: "Other platforms"
---

# Other platforms

Build definitions are provided so that some features ([serparser](../api-reference/classes/ser\_parser.md), [pktparser](../api-reference/classes/pktparser/), Serial object for console) can be built on other platforms. Only the necessary files are cut out.

The build definitions are stored in the {mwx library storage}/stdio folder. See [README.md](https://github.com/monowireless/mwx/tree/master/stdio) (link is on GitHub) for build instructions.

* Must be able to compile in C++11.
* Ability to use C++11 standard library headers (utility, algorithm, functional, iterator, etc.)
* new/delete/virtual are not used.
* Memory allocation by new may be used in exceptional cases.
  * In serparser/pktparser, alloc\_heap which uses new operator is processed by delete.
    * (Reference) However, the mwx library has been designed on the assumption that delete is not taken into account.

