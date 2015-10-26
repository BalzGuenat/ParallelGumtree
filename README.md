# ParallelGumtree
This is a parallel implementation of the Gumtree algorithm (see github.com/GumTreeDiff/gumtree). This project is done in the context of the DPHPC course at ETH Zurich.
This project was set up with and for QtCreator. Getting started with that should be the easiest.

## Encoding of trees
We currently use very simple trees and use the following encoding to store and load trees to and from files. Every node of the tree is encoded on a single line. The different fields associated with the node are separated by the colon character `:`. The first field is an integer determining the type of the node. The second field is the label of the node. The child-parent relationships are given by the indentation level of a line. This level is given only by the number of white characters so a tab has the same effect on the indentation level as a space. Every tree must have a single root. Here is an example of a simple tree.

```
0:root
 1:node1
 1:node2
  2:inner_node
 3:node3
```
