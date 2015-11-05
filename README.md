# ParallelGumtree
This is a parallel implementation of the Gumtree algorithm (see github.com/GumTreeDiff/gumtree).
This project is done in the context of the DPHPC course at ETH Zurich.
This project was set up with and for QtCreator.
Getting started with that should be the easiest.

## Encoding of trees
We currently use very simple trees and use the following encoding to store and load trees to and from files.
Every node of the tree is encoded on a single line.
The different fields associated with the node are separated by the colon character `:`.
The first field is an integer determining the type of the node.
The second field is the label of the node.
The child-parent relationships are given by the indentation level of a line.
This level is given only by the number of white characters so a tab has the same effect on the indentation level as a space.
Every tree must have a single root. Here is an example of a simple tree.

```
0:root
 1:node1
 1:node2
  2:inner_node
 3:node3
```

## Reference implementation
To ensure the equivalence of our implementation to the original one, we compare the results as follows.
We have implemented a tree generator for the original implementation that reads the same kind of test trees as our parallel implementation.
A corresponding client then dumps the matching in terms of line numbers to a file.
The code for this can be found [here](https://github.com/BalzGuenat/gumtree/tree/prallel-gt-reference).
To generate a reference mapping, execute the runGumtree.sh script and pass as arguments two files A and B encoding trees in the above format.
A file with the same name as B plus the file ending ".mtch" is generated in the parent directory of B.
Each line is formatted `a -> b` indicating that the node on line number `a` in file A was matched with the node on line number `b` in file B.
