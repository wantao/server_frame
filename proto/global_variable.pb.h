// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: global_variable.proto

#ifndef PROTOBUF_global_5fvariable_2eproto__INCLUDED
#define PROTOBUF_global_5fvariable_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace protoc {
namespace global_var {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_global_5fvariable_2eproto();
void protobuf_AssignDesc_global_5fvariable_2eproto();
void protobuf_ShutdownFile_global_5fvariable_2eproto();


enum GLOBAL_VARIABLE {
  MAX_NET_PACKET_SIZE = 65536,
  MAX_LARGE_NET_PACKET_SIZE = 512000,
  MAX_SERVER_ID_LEN = 32,
  MAX_ACCOUNT_LEN = 64,
  MAX_SESSION_KEY_LEN = 64,
  MAX_PLAYER_NAME_LEN = 32,
  MAX_PLAYER_MSG_CONTENT_LEN = 64,
  E_MAIL_CONTENT_MAX_LEN = 540,
  E_MAIL_TITLE_MAX_LEN = 60,
  E_MAIL_AWARD_MAX_LEN = 255
};
bool GLOBAL_VARIABLE_IsValid(int value);
const GLOBAL_VARIABLE GLOBAL_VARIABLE_MIN = MAX_SERVER_ID_LEN;
const GLOBAL_VARIABLE GLOBAL_VARIABLE_MAX = MAX_LARGE_NET_PACKET_SIZE;
const int GLOBAL_VARIABLE_ARRAYSIZE = GLOBAL_VARIABLE_MAX + 1;

const ::google::protobuf::EnumDescriptor* GLOBAL_VARIABLE_descriptor();
inline const ::std::string& GLOBAL_VARIABLE_Name(GLOBAL_VARIABLE value) {
  return ::google::protobuf::internal::NameOfEnum(
    GLOBAL_VARIABLE_descriptor(), value);
}
inline bool GLOBAL_VARIABLE_Parse(
    const ::std::string& name, GLOBAL_VARIABLE* value) {
  return ::google::protobuf::internal::ParseNamedEnum<GLOBAL_VARIABLE>(
    GLOBAL_VARIABLE_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

}  // namespace global_var
}  // namespace protoc

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protoc::global_var::GLOBAL_VARIABLE>() {
  return ::protoc::global_var::GLOBAL_VARIABLE_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_global_5fvariable_2eproto__INCLUDED