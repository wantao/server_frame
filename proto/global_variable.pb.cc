// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: global_variable.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "global_variable.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace protoc {
namespace global_var {

namespace {

const ::google::protobuf::EnumDescriptor* GLOBAL_VARIABLE_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_global_5fvariable_2eproto() {
  protobuf_AddDesc_global_5fvariable_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "global_variable.proto");
  GOOGLE_CHECK(file != NULL);
  GLOBAL_VARIABLE_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_global_5fvariable_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_global_5fvariable_2eproto() {
}

void protobuf_AddDesc_global_5fvariable_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\025global_variable.proto\022\021protoc.global_v"
    "ar*\235\002\n\017GLOBAL_VARIABLE\022\031\n\023MAX_NET_PACKET"
    "_SIZE\020\200\200\004\022\037\n\031MAX_LARGE_NET_PACKET_SIZE\020\200"
    "\240\037\022\025\n\021MAX_SERVER_ID_LEN\020 \022\023\n\017MAX_ACCOUNT"
    "_LEN\020@\022\027\n\023MAX_SESSION_KEY_LEN\020@\022\027\n\023MAX_P"
    "LAYER_NAME_LEN\020 \022\036\n\032MAX_PLAYER_MSG_CONTE"
    "NT_LEN\020@\022\033\n\026E_MAIL_CONTENT_MAX_LEN\020\234\004\022\030\n"
    "\024E_MAIL_TITLE_MAX_LEN\020<\022\031\n\024E_MAIL_AWARD_"
    "MAX_LEN\020\377\001", 330);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "global_variable.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_global_5fvariable_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_global_5fvariable_2eproto {
  StaticDescriptorInitializer_global_5fvariable_2eproto() {
    protobuf_AddDesc_global_5fvariable_2eproto();
  }
} static_descriptor_initializer_global_5fvariable_2eproto_;
const ::google::protobuf::EnumDescriptor* GLOBAL_VARIABLE_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return GLOBAL_VARIABLE_descriptor_;
}
bool GLOBAL_VARIABLE_IsValid(int value) {
  switch(value) {
    case 32:
    case 60:
    case 64:
    case 255:
    case 540:
    case 65536:
    case 512000:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace global_var
}  // namespace protoc

// @@protoc_insertion_point(global_scope)
