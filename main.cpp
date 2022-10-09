#include <iostream>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

using namespace std;

using ul = unsigned long;

struct End_Node {
  int value;
};

class Node {
  string value;
  unique_ptr<End_Node> end;
  vector<Node> next;

  string intersection(string &st1, string &st2) {
    string *shortest;
    string *longest;

    if (st1.length() < st2.length()) {
      shortest = &st1;
      longest = &st2;
    } else {
      shortest = &st2;
      longest = &st1;
    }

    for (ul i = 0; i < shortest->length(); i++) {
      if (shortest->at(i) != longest->at(i)) {
        return shortest->substr(0, i);
      }
    }
    return *shortest;
  }

  string not_intersection(string common, string st2) {
    return st2.substr(common.length(), st2.length() - common.length());
  }

  Node(string st, unique_ptr<End_Node> &end_node) : value{st} {
    end = move(end_node);
  };

  bool is_string_subset(string const &short_string, string const &long_string) {
    if (short_string.size() == 0){
      return false;
    }

    if (short_string.size() > long_string.size()) {
      return false;
    }

    for (ul i = 0; i < short_string.size(); i++) {
      if (short_string[i] != long_string[i]) {
        return false;
      }
    }
    return true;
  }

  bool is_root_node(string const &st) {
    ul max_val = min(value.size(), st.size());
    for (ul i = 0; i < max_val; i++) {
      if (st[i] != value[i]) {
        return false;
      }
    }
    return true;
  }

  bool is_valid_node(string const &st, Node const &nd) {
    if (st.length() < nd.value.length()){
      return false;
    }
    return is_string_subset(nd.value, st);
  }

  Node *find_end_node(string &wd) {
    Node *nd = this;
    if (!is_valid_node(wd, *nd)) {
      return nd;
    }

    wd = &wd[value.length()];
    bool end = false;
    while (!end) {
      end = true;
      for (Node &n : nd->next) {
        if (is_valid_node(wd, n)) {
          end = false;
          nd = &n;
          wd = &wd[nd->value.length()];
          break;
        }
      }
    }
    return nd;
  }

public:
  Node(string st, int end_value) : value{st} {
    end = make_unique<End_Node>(End_Node{end_value});
  };

  void add(string wd, int end_value) {

    // find last node
    ul j = 0;
    Node *nn = find_end_node(wd);
    string common = intersection(nn->value, wd);

    string v1 = not_intersection(common, nn->value);
    string v2 = not_intersection(common, wd);

    if (common.length() != 0) {
      value = common;
      if (v1.length() != 0) {
        nn->next.push_back(Node(v1, nn->end));
        end = nullptr;
      }
    }

    if (v2.length() != 0) {
      nn->next.push_back(Node(v2, end_value));
    } else {
      if (nn->end == nullptr) {
        nn->end = make_unique<End_Node>(End_Node{end_value});
      } else {
        cout << "error node already allocated: " << nn->end->value << "\n";
      }
    }
  }

void print(int d) {
  cout << "D: " << d;
  cout << " V: " << value;
    if (end != nullptr) {
      cout << " E: " << end->value << " ";
    } else {
      cout << " /";
    }
    cout << "N: ";
    for (Node &n : next) {
      cout << n.value << " ";
    }
    cout << '\n';
    for (Node &n : next) {
      n.print(d+1);
    }
}

  void print() {
    print(0);
  }

  int parse(string st) {
    ul j = 0;
    Node *nn = find_end_node(st);

    if (st.length() == 0){
      return nn->end->value;
    }

    return -1;
  }
};

int test() {

  Node nd{"H", 1};
  nd.add("He", 2);
  nd.add("Hell", 3);
  nd.add("Hello", 4);

  nd.print();

  cout << nd.parse("Hello") << '\n';
  return 0;
}

int main() {

  test();
  return 0;
}