#include <iostream>
#include "tree.h"

using namespace std;

int main()
{
  Tree t(0, "Hello World!");
  cout << t.label() << endl;
  return 0;
}

