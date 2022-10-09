## Parser

Efficent string matching alg. Implemented with parse tree, runs in linear time.

``` c++
  Node nd{"H", 1};
  nd.add("He", 2);
  nd.add("Hell", 3);
  nd.add("Hello", 4);

  std::string input{"He"};
  switch (nd.parse(input)){
    case 1:
        std::cout << "H \n";
        break;
    case 2:
        std::cout << "HE \n";
        break;
    case 3:
        std::cout << "HELL \n";
        break;
    case 4:
        std::cout << "HELLO \n";
        break;
    default:
        std::cout << "ERROR \n";
        break;
  }

```