# B-Tree #
## Overview ##
A simple <em>B-Tree</em> implementation in C based on the description from <em>Introduction to Algorithms</em> T.Cormen.
Tree is defined by a tree degree <em>t</em>, which serves balancing purposes.
A minimum degree value is 2.

Properties:
- Node, other than root, has no less than t-1 keys.
- Node, if not leaf, has childs number of keys number plus one.
- All nodes have at maximum 2*t-1 keys and 2*t childs.
- All leafs have the same depth.

Implemented operations:
- insert
- search
- delete
 
TODO:
- bulk insert
- payload in nodes
- move everything to the header(?)
- io(?)

## Usage ##
For linux, remove <em>EXE_EXT</em> definition from Makefile.
To compile <em>main.c</em>, which has a little bit of usage examples, use
~~~
make
~~~
To compile and run, use
~~~
make run
~~~
To run tests, use
~~~
make tests
~~~
Tree usage examples can be found in <em>./tests/tests.c</em>.


