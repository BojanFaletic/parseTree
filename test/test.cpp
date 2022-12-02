#include <gtest/gtest.h>

#include "loader.h"
#include "parser.h"

TEST(Parser, read_list) {
  parser_t *list;
  json_obj_t *json;

  parser_init(&list);
  parser_load_json("test_out.json", list, &json);

  EXPECT_EQ(parser_parse("SKFOJSC", list), 1453);

  parser_free_json(json);
  parser_free(list);
}

TEST(Parser, nb_of_nodes) {
  parser_t *list;
  json_obj_t *json;

  parser_init(&list);
  parser_load_json("test_out.json", list, &json);

  EXPECT_EQ(parser_parse("SKFOJSC", list), 1453);

  parser_free_json(json);
  parser_free(list);
}

TEST(Parser, add_new) {
  const char *test_filename = "test_cases/add_new.json";
  parser_t *list;
  json_obj_t *json;

  parser_init(&list);
  parser_load_json(test_filename, list, &json);

  list_holder_t *tmp = json;
  do {
    char *name = (char*)tmp->data;
    EXPECT_NE(parser_parse(name, list), -1);
    tmp = tmp->next;
  } while (tmp != NULL);

  parser_free(list);
  parser_free_json(json);
}

TEST(Parser, assign) {
  const char *test_filename = "test_cases/assign.json";
  parser_t *list;
  json_obj_t *json;

  parser_init(&list);
  parser_load_json(test_filename, list, &json);

  list_holder_t *tmp = json;
  do {
    char *name = (char*)tmp->data;
    EXPECT_NE(parser_parse(name, list), -1);
    tmp = tmp->next;
  } while (tmp != NULL);

  parser_free(list);
  parser_free_json(json);
}

TEST(Parser, branch) {
  const char *test_filename = "test_cases/branch.json";
  parser_t *list;
  json_obj_t *json;

  parser_init(&list);
  parser_load_json(test_filename, list, &json);

  list_holder_t *tmp = json;
  do {
    char *name = (char*)tmp->data;
    EXPECT_NE(parser_parse(name, list), -1);
    tmp = tmp->next;
  } while (tmp != NULL);

  parser_free(list);
  parser_free_json(json);
}

TEST(Parser, full) {
  const char *test_filename = "test_cases/full.json";
  parser_t *list;
  json_obj_t *json;

  parser_init(&list);
  parser_load_json(test_filename, list, &json);

  list_holder_t *tmp = json;
  do {
    char *name = (char*)tmp->data;
    EXPECT_NE(parser_parse(name, list), -1);
    tmp = tmp->next;
  } while (tmp != NULL);

  parser_free(list);
  parser_free_json(json);
}

TEST(Parser, insert_new) {
  const char *test_filename = "test_cases/insert_new.json";
  parser_t *list;
  json_obj_t *json;

  parser_init(&list);
  parser_load_json(test_filename, list, &json);

  list_holder_t *tmp = json;
  do {
    char *name = (char*)tmp->data;
    EXPECT_NE(parser_parse(name, list), -1);
    tmp = tmp->next;
  } while (tmp != NULL);

  parser_free(list);
  parser_free_json(json);
}

TEST(Parser, large) {
  const char *test_filename = "test_out.json";
  parser_t *list;
  json_obj_t *json;

  parser_init(&list);
  parser_load_json(test_filename, list, &json);

  list_holder_t *tmp = json;
  do {
    char *name = (char*)tmp->data;
    EXPECT_NE(parser_parse(name, list), -1);
    tmp = tmp->next;
  } while (tmp != NULL);

  parser_free(list);
  parser_free_json(json);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}