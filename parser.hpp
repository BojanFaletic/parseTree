#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using ul = unsigned long;

class Node;

class Node {
  static int const UNOWN_VALUE{-1};
  char name;
  vector<Node *> next;
  int end_value = UNOWN_VALUE;

  Node(char ch) : name{ch} {}

  void delete_end(Node *nd) {
    for (Node *nn : nd->next) {
      if (nn->next.size() != 0) {
        delete_end(nn);
      }
    }
    if (nd->next.size() != 0) {
      delete nd->next.back();
      nd->next.pop_back();
    }
  }

public:
  Node() = default;
  Node(Node &) = delete;
  Node(Node &&) = delete;

  void add(string name, int value) {
    Node *nd = this;
    for (char ch : name) {
      bool found_existing = false;

      // search over all possible next nodes
      for (Node *candidate : nd->next) {
        if (ch == candidate->name) {
          found_existing = true;
          nd = candidate;
          break;
        }
      }

      // add new node if not existing
      if (!found_existing) {
        Node *nn = new Node(ch);
        nd->next.push_back(nn);
        nd = nn;
      }
    }
    nd->end_value = value;
  }

  int get(string name) {
    Node *nd = this;
    for (char ch : name) {
      bool found_node = false;
      for (Node *candidate : nd->next) {
        if (ch == candidate->name) {
          found_node = true;
          nd = candidate;
          break;
        }
      }
      if (!found_node) {
        return UNOWN_VALUE;
      }
    }

    return nd->end_value;
  }

  ~Node() { delete_end(this); }
};