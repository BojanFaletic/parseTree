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

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}