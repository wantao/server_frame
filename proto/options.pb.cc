// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: options.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "options.pb.h"

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

namespace {


}  // namespace


void protobuf_AssignDesc_options_2eproto() {
  protobuf_AddDesc_options_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "options.proto");
  GOOGLE_CHECK(file != NULL);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_options_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_options_2eproto() {
}

void protobuf_AddDesc_options_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::protobuf_AddDesc_google_2fprotobuf_2fdescriptor_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\roptions.proto\032 google/protobuf/descrip"
    "tor.proto:=\n\014as3_bindable\022\037.google.proto"
    "buf.MessageOptions\030\261I \001(\010:\005false:7\n\ras3_"
    "amf_alias\022\037.google.protobuf.MessageOptio"
    "ns\030\262I \001(\t:C\n\022as3_amf_auto_alias\022\037.google"
    ".protobuf.MessageOptions\030\263I \001(\010:\005falseB\032"
    "\n\030com.netease.protocGenAs3", 266);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "options.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::ExtensionSet::RegisterExtension(
    &::google::protobuf::MessageOptions::default_instance(),
    9393, 8, false, false);
  ::google::protobuf::internal::ExtensionSet::RegisterExtension(
    &::google::protobuf::MessageOptions::default_instance(),
    9394, 9, false, false);
  ::google::protobuf::internal::ExtensionSet::RegisterExtension(
    &::google::protobuf::MessageOptions::default_instance(),
    9395, 8, false, false);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_options_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_options_2eproto {
  StaticDescriptorInitializer_options_2eproto() {
    protobuf_AddDesc_options_2eproto();
  }
} static_descriptor_initializer_options_2eproto_;
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MessageOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< bool >, 8, false >
  as3_bindable(kAs3BindableFieldNumber, false);
const ::std::string as3_amf_alias_default("");
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MessageOptions,
    ::google::protobuf::internal::StringTypeTraits, 9, false >
  as3_amf_alias(kAs3AmfAliasFieldNumber, as3_amf_alias_default);
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MessageOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< bool >, 8, false >
  as3_amf_auto_alias(kAs3AmfAutoAliasFieldNumber, false);

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)