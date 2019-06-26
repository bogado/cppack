//
// Created by Mike Loomis on 6/22/2019.
//

#include <catch2/catch.hpp>

#include <msgpack/msgpack.hpp>

struct Example {
  std::map<int, int> x;
//  uint8_t y;
//  std::string z;
//  std::vector<std::string> array;

  template<class Packager>
  void pack(Packager &packager) {
    packager.process(x);
  }
};

TEST_CASE("Nil type packing") {
  auto null_obj = std::nullptr_t{};
  auto packer = msgpack::Packer{};
  packer.process(null_obj);
  REQUIRE(packer.vector() == std::vector<uint8_t>{0xc0});
}

TEST_CASE("Boolean type packing") {
  auto bool_obj = false;
  auto packer = msgpack::Packer{};
  packer.process(bool_obj);
  REQUIRE(packer.vector() == std::vector<uint8_t>{0xc2});
  bool_obj = true;
  packer.clear();
  packer.process(bool_obj);
  REQUIRE(packer.vector() == std::vector<uint8_t>{0xc3});
}

TEST_CASE("Integer type packing") {
  auto packer = msgpack::Packer{};
  auto unpacker = msgpack::Unpacker{};

  for (auto i = 0U; i < 10; ++i) {
    uint8_t test_num = i * std::numeric_limits<uint8_t>::max() / 10;
    packer.clear();
    packer.process(test_num);
    uint8_t x = 0U;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }

  for (auto i = 0U; i < 10; ++i) {
    uint16_t test_num = i * std::numeric_limits<uint16_t>::max() / 10;
    packer.clear();
    packer.process(test_num);
    uint16_t x = 0U;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }

  for (auto i = 0U; i < 10; ++i) {
    uint32_t test_num = i * std::numeric_limits<uint32_t>::max() / 10;
    packer.clear();
    packer.process(test_num);
    uint32_t x = 0U;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }

  for (auto i = 0U; i < 10; ++i) {
    uint64_t test_num = i * std::numeric_limits<uint64_t>::max() / 10;
    packer.clear();
    packer.process(test_num);
    uint64_t x = 0U;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }

  for (auto i = -5; i < 5; ++i) {
    int8_t test_num = i * std::numeric_limits<int8_t>::max() / 5;
    packer.clear();
    packer.process(test_num);
    int8_t x = 0;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }

  for (auto i = -5; i < 5; ++i) {
    int16_t test_num = i * std::numeric_limits<int16_t>::max() / 5;
    packer.clear();
    packer.process(test_num);
    int16_t x = 0;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }

  for (auto i = -5; i < 5; ++i) {
    int32_t test_num = i * std::numeric_limits<int32_t>::max() / 5;
    packer.clear();
    packer.process(test_num);
    int32_t x = 0;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }

  for (auto i = -5; i < 5; ++i) {
    int64_t test_num = i * std::numeric_limits<int64_t>::max() / 5;
    packer.clear();
    packer.process(test_num);
    int64_t x = 0;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }
}

TEST_CASE("Float type packing") {
  auto packer = msgpack::Packer{};
  auto unpacker = msgpack::Unpacker{};

  for (auto i = -5; i < 5; ++i) {
    float test_num = 5.0f + float(i) * 12345.67f / 4.56f;
    packer.clear();
    packer.process(test_num);
    float x = 0.0f;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }

  for (auto i = -5; i < 5; ++i) {
    double test_num = 5.0 + double(i) * 12345.67 / 4.56;
    packer.clear();
    packer.process(test_num);
    double x = 0.0;
    unpacker.set_data(packer.vector().data());
    unpacker.process(x);
    REQUIRE(x == test_num);
  }
}

TEST_CASE("String type packing") {
  auto str1 = std::string("test");
  auto packer = msgpack::Packer{};
  packer.process(str1);
  REQUIRE(packer.vector() == std::vector<uint8_t>{0b10100000 | 4, 't', 'e', 's', 't'});
}

TEST_CASE("Byte array type packing") {
  auto vec1 = std::vector<uint8_t>{1, 2, 3, 4};
  auto packer = msgpack::Packer{};
  packer.process(vec1);
  REQUIRE(packer.vector() == std::vector<uint8_t>{0xc4, 4, 1, 2, 3, 4});
}

TEST_CASE("Array type packing") {
  auto list1 = std::list<std::string>{"one", "two", "three"};
  auto packer = msgpack::Packer{};
  packer.process(list1);
  REQUIRE(packer.vector() == std::vector<uint8_t>{0b10010000 | 3, 0b10100000 | 3, 'o', 'n', 'e',
                                                  0b10100000 | 3, 't', 'w', 'o',
                                                  0b10100000 | 5, 't', 'h', 'r', 'e', 'e'});
}

TEST_CASE("Map type packing") {
  auto map1 = std::map<uint8_t, std::string>{std::make_pair(0, "zero"), std::make_pair(1, "one")};
  auto packer = msgpack::Packer{};
  packer.process(map1);
  REQUIRE(packer.vector() == std::vector<uint8_t>{0b10000000 | 2, 0, 0b10100000 | 4, 'z', 'e', 'r', 'o',
                                                  1, 0b10100000 | 3, 'o', 'n', 'e'});
}