#include <vector>
#include <string>

#ifndef PACK109_HPP
#define PACK109_HPP

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long u64;
typedef signed char i8;
typedef signed int i32;
typedef signed long i64;
typedef float f32;
typedef double f64;
typedef std::vector<u8> vec;
typedef std::string string;

#define PACK109_TRUE  0xa0
#define PACK109_FALSE 0xa1
#define PACK109_U8    0xa2
#define PACK109_U32   0xa3
#define PACK109_U64   0xa4
#define PACK109_I8    0xa5
#define PACK109_I32   0xa6
#define PACK109_I64   0xa7
#define PACK109_F32   0xa8
#define PACK109_F64   0xa9
#define PACK109_S8    0xaa
#define PACK109_S16   0xab
#define PACK109_A8    0xac
#define PACK109_A16   0xad
#define PACK109_M8    0xae
#define PACK109_M16   0xaf

namespace pack109 {

struct File {
  string name;
  std::vector<unsigned char> bytes;
};

struct Request{
  string name;
};

  void printVec(vec &bytes);

  // Boolean

  vec serialize(bool item);
  bool deserialize_bool(vec bytes);

  // Integers

  vec serialize(u8 item);
  u8 deserialize_u8(vec bytes);

  vec serialize(u32 item);
  u32 deserialize_u32(vec bytes);

  vec serialize(u64 item);
  u64 deserialize_u64(vec bytes);

  vec serialize(i8 item);
  i8 deserialize_i8(vec bytes);

  vec serialize(i32 item);
  i32 deserialize_i32(vec bytes);

  vec serialize(i64 item);
  i64 deserialize_i64(vec bytes);

  // Floats

  vec serialize(f32 item);
  f32 deserialize_f32(vec bytes);

  vec serialize(f64 item);
  f64 deserialize_f64(vec bytes);

  // Strings

  vec serialize(string item);
  string deserialize_string(vec bytes);
  
  // Arrays
  vec serialize(std::vector<u8> item);
  vec serialize(std::vector<u64> item);
  vec serialize(std::vector<f64> item);
  vec serialize(std::vector<string> item);

  std::vector<u8> deserialize_vec_u8(vec bytes);
  std::vector<u64> deserialize_vec_u64(vec bytes);
  std::vector<f64> deserialize_vec_f64(vec bytes);
  std::vector<string> deserialize_vec_string(vec bytes);

  // Maps

  vec serialize(struct File item);
  struct File deserialize_file(vec bytes);

  vec serialize(struct Request item);
  struct Request deserialize_request(vec bytes);

}

#endif