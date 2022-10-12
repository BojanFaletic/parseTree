#include "parser.hpp"
using namespace std;

bool check(string const test, int expected, Node &nd) {
  int result = nd.get(test);
  if (result != expected) {
    printf("Test failed %s: %d != %d\n", test.c_str(), expected, result);
    return -1;
  }
  return 0;
};

int test() {

  Node nd;
  nd.add("Hello", 2);
  nd.add("He", 4);

  int status = 0;
  status |= check("He", 4, nd);

  return !(status == -1);
}

int main() {

  test();

  return 0;
}