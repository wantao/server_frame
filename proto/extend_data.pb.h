// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: extend_data.proto

#ifndef PROTOBUF_extend_5fdata_2eproto__INCLUDED
#define PROTOBUF_extend_5fdata_2eproto__INCLUDED

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
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace extend_data {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_extend_5fdata_2eproto();
void protobuf_AssignDesc_extend_5fdata_2eproto();
void protobuf_ShutdownFile_extend_5fdata_2eproto();

class extend_data_v1;
class state_v1;
class state_list_v1;

enum extend_data_version {
  V1 = 1
};
bool extend_data_version_IsValid(int value);
const extend_data_version extend_data_version_MIN = V1;
const extend_data_version extend_data_version_MAX = V1;
const int extend_data_version_ARRAYSIZE = extend_data_version_MAX + 1;

const ::google::protobuf::EnumDescriptor* extend_data_version_descriptor();
inline const ::std::string& extend_data_version_Name(extend_data_version value) {
  return ::google::protobuf::internal::NameOfEnum(
    extend_data_version_descriptor(), value);
}
inline bool extend_data_version_Parse(
    const ::std::string& name, extend_data_version* value) {
  return ::google::protobuf::internal::ParseNamedEnum<extend_data_version>(
    extend_data_version_descriptor(), name, value);
}
// ===================================================================

class extend_data_v1 : public ::google::protobuf::Message {
 public:
  extend_data_v1();
  virtual ~extend_data_v1();

  extend_data_v1(const extend_data_v1& from);

  inline extend_data_v1& operator=(const extend_data_v1& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const extend_data_v1& default_instance();

  void Swap(extend_data_v1* other);

  // implements Message ----------------------------------------------

  extend_data_v1* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const extend_data_v1& from);
  void MergeFrom(const extend_data_v1& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 today_error_input_award_key_num = 1;
  inline bool has_today_error_input_award_key_num() const;
  inline void clear_today_error_input_award_key_num();
  static const int kTodayErrorInputAwardKeyNumFieldNumber = 1;
  inline ::google::protobuf::int32 today_error_input_award_key_num() const;
  inline void set_today_error_input_award_key_num(::google::protobuf::int32 value);

  // optional int64 new_day_time = 2;
  inline bool has_new_day_time() const;
  inline void clear_new_day_time();
  static const int kNewDayTimeFieldNumber = 2;
  inline ::google::protobuf::int64 new_day_time() const;
  inline void set_new_day_time(::google::protobuf::int64 value);

  // optional bool today_login_log = 3;
  inline bool has_today_login_log() const;
  inline void clear_today_login_log();
  static const int kTodayLoginLogFieldNumber = 3;
  inline bool today_login_log() const;
  inline void set_today_login_log(bool value);

  // optional int32 today_send_email_num = 4;
  inline bool has_today_send_email_num() const;
  inline void clear_today_send_email_num();
  static const int kTodaySendEmailNumFieldNumber = 4;
  inline ::google::protobuf::int32 today_send_email_num() const;
  inline void set_today_send_email_num(::google::protobuf::int32 value);

  // optional int32 today_ask_friend_num = 5;
  inline bool has_today_ask_friend_num() const;
  inline void clear_today_ask_friend_num();
  static const int kTodayAskFriendNumFieldNumber = 5;
  inline ::google::protobuf::int32 today_ask_friend_num() const;
  inline void set_today_ask_friend_num(::google::protobuf::int32 value);

  // optional int64 last_get_action_count_time = 6;
  inline bool has_last_get_action_count_time() const;
  inline void clear_last_get_action_count_time();
  static const int kLastGetActionCountTimeFieldNumber = 6;
  inline ::google::protobuf::int64 last_get_action_count_time() const;
  inline void set_last_get_action_count_time(::google::protobuf::int64 value);

  // optional int32 today_buy_action_count_times = 7;
  inline bool has_today_buy_action_count_times() const;
  inline void clear_today_buy_action_count_times();
  static const int kTodayBuyActionCountTimesFieldNumber = 7;
  inline ::google::protobuf::int32 today_buy_action_count_times() const;
  inline void set_today_buy_action_count_times(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:extend_data.extend_data_v1)
 private:
  inline void set_has_today_error_input_award_key_num();
  inline void clear_has_today_error_input_award_key_num();
  inline void set_has_new_day_time();
  inline void clear_has_new_day_time();
  inline void set_has_today_login_log();
  inline void clear_has_today_login_log();
  inline void set_has_today_send_email_num();
  inline void clear_has_today_send_email_num();
  inline void set_has_today_ask_friend_num();
  inline void clear_has_today_ask_friend_num();
  inline void set_has_last_get_action_count_time();
  inline void clear_has_last_get_action_count_time();
  inline void set_has_today_buy_action_count_times();
  inline void clear_has_today_buy_action_count_times();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 new_day_time_;
  ::google::protobuf::int32 today_error_input_award_key_num_;
  bool today_login_log_;
  ::google::protobuf::int32 today_send_email_num_;
  ::google::protobuf::int32 today_ask_friend_num_;
  ::google::protobuf::int64 last_get_action_count_time_;
  ::google::protobuf::int32 today_buy_action_count_times_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(7 + 31) / 32];

  friend void  protobuf_AddDesc_extend_5fdata_2eproto();
  friend void protobuf_AssignDesc_extend_5fdata_2eproto();
  friend void protobuf_ShutdownFile_extend_5fdata_2eproto();

  void InitAsDefaultInstance();
  static extend_data_v1* default_instance_;
};
// -------------------------------------------------------------------

class state_v1 : public ::google::protobuf::Message {
 public:
  state_v1();
  virtual ~state_v1();

  state_v1(const state_v1& from);

  inline state_v1& operator=(const state_v1& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const state_v1& default_instance();

  void Swap(state_v1* other);

  // implements Message ----------------------------------------------

  state_v1* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const state_v1& from);
  void MergeFrom(const state_v1& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional uint32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::uint32 id() const;
  inline void set_id(::google::protobuf::uint32 value);

  // optional uint64 end_time = 2;
  inline bool has_end_time() const;
  inline void clear_end_time();
  static const int kEndTimeFieldNumber = 2;
  inline ::google::protobuf::uint64 end_time() const;
  inline void set_end_time(::google::protobuf::uint64 value);

  // optional uint64 res1 = 3;
  inline bool has_res1() const;
  inline void clear_res1();
  static const int kRes1FieldNumber = 3;
  inline ::google::protobuf::uint64 res1() const;
  inline void set_res1(::google::protobuf::uint64 value);

  // optional uint32 res2 = 4;
  inline bool has_res2() const;
  inline void clear_res2();
  static const int kRes2FieldNumber = 4;
  inline ::google::protobuf::uint32 res2() const;
  inline void set_res2(::google::protobuf::uint32 value);

  // optional uint32 res3 = 5;
  inline bool has_res3() const;
  inline void clear_res3();
  static const int kRes3FieldNumber = 5;
  inline ::google::protobuf::uint32 res3() const;
  inline void set_res3(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:extend_data.state_v1)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_end_time();
  inline void clear_has_end_time();
  inline void set_has_res1();
  inline void clear_has_res1();
  inline void set_has_res2();
  inline void clear_has_res2();
  inline void set_has_res3();
  inline void clear_has_res3();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint64 end_time_;
  ::google::protobuf::uint32 id_;
  ::google::protobuf::uint32 res2_;
  ::google::protobuf::uint64 res1_;
  ::google::protobuf::uint32 res3_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_extend_5fdata_2eproto();
  friend void protobuf_AssignDesc_extend_5fdata_2eproto();
  friend void protobuf_ShutdownFile_extend_5fdata_2eproto();

  void InitAsDefaultInstance();
  static state_v1* default_instance_;
};
// -------------------------------------------------------------------

class state_list_v1 : public ::google::protobuf::Message {
 public:
  state_list_v1();
  virtual ~state_list_v1();

  state_list_v1(const state_list_v1& from);

  inline state_list_v1& operator=(const state_list_v1& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const state_list_v1& default_instance();

  void Swap(state_list_v1* other);

  // implements Message ----------------------------------------------

  state_list_v1* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const state_list_v1& from);
  void MergeFrom(const state_list_v1& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .extend_data.state_v1 state_list = 1;
  inline int state_list_size() const;
  inline void clear_state_list();
  static const int kStateListFieldNumber = 1;
  inline const ::extend_data::state_v1& state_list(int index) const;
  inline ::extend_data::state_v1* mutable_state_list(int index);
  inline ::extend_data::state_v1* add_state_list();
  inline const ::google::protobuf::RepeatedPtrField< ::extend_data::state_v1 >&
      state_list() const;
  inline ::google::protobuf::RepeatedPtrField< ::extend_data::state_v1 >*
      mutable_state_list();

  // @@protoc_insertion_point(class_scope:extend_data.state_list_v1)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::extend_data::state_v1 > state_list_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_extend_5fdata_2eproto();
  friend void protobuf_AssignDesc_extend_5fdata_2eproto();
  friend void protobuf_ShutdownFile_extend_5fdata_2eproto();

  void InitAsDefaultInstance();
  static state_list_v1* default_instance_;
};
// ===================================================================


// ===================================================================

// extend_data_v1

// optional int32 today_error_input_award_key_num = 1;
inline bool extend_data_v1::has_today_error_input_award_key_num() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void extend_data_v1::set_has_today_error_input_award_key_num() {
  _has_bits_[0] |= 0x00000001u;
}
inline void extend_data_v1::clear_has_today_error_input_award_key_num() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void extend_data_v1::clear_today_error_input_award_key_num() {
  today_error_input_award_key_num_ = 0;
  clear_has_today_error_input_award_key_num();
}
inline ::google::protobuf::int32 extend_data_v1::today_error_input_award_key_num() const {
  return today_error_input_award_key_num_;
}
inline void extend_data_v1::set_today_error_input_award_key_num(::google::protobuf::int32 value) {
  set_has_today_error_input_award_key_num();
  today_error_input_award_key_num_ = value;
}

// optional int64 new_day_time = 2;
inline bool extend_data_v1::has_new_day_time() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void extend_data_v1::set_has_new_day_time() {
  _has_bits_[0] |= 0x00000002u;
}
inline void extend_data_v1::clear_has_new_day_time() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void extend_data_v1::clear_new_day_time() {
  new_day_time_ = GOOGLE_LONGLONG(0);
  clear_has_new_day_time();
}
inline ::google::protobuf::int64 extend_data_v1::new_day_time() const {
  return new_day_time_;
}
inline void extend_data_v1::set_new_day_time(::google::protobuf::int64 value) {
  set_has_new_day_time();
  new_day_time_ = value;
}

// optional bool today_login_log = 3;
inline bool extend_data_v1::has_today_login_log() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void extend_data_v1::set_has_today_login_log() {
  _has_bits_[0] |= 0x00000004u;
}
inline void extend_data_v1::clear_has_today_login_log() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void extend_data_v1::clear_today_login_log() {
  today_login_log_ = false;
  clear_has_today_login_log();
}
inline bool extend_data_v1::today_login_log() const {
  return today_login_log_;
}
inline void extend_data_v1::set_today_login_log(bool value) {
  set_has_today_login_log();
  today_login_log_ = value;
}

// optional int32 today_send_email_num = 4;
inline bool extend_data_v1::has_today_send_email_num() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void extend_data_v1::set_has_today_send_email_num() {
  _has_bits_[0] |= 0x00000008u;
}
inline void extend_data_v1::clear_has_today_send_email_num() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void extend_data_v1::clear_today_send_email_num() {
  today_send_email_num_ = 0;
  clear_has_today_send_email_num();
}
inline ::google::protobuf::int32 extend_data_v1::today_send_email_num() const {
  return today_send_email_num_;
}
inline void extend_data_v1::set_today_send_email_num(::google::protobuf::int32 value) {
  set_has_today_send_email_num();
  today_send_email_num_ = value;
}

// optional int32 today_ask_friend_num = 5;
inline bool extend_data_v1::has_today_ask_friend_num() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void extend_data_v1::set_has_today_ask_friend_num() {
  _has_bits_[0] |= 0x00000010u;
}
inline void extend_data_v1::clear_has_today_ask_friend_num() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void extend_data_v1::clear_today_ask_friend_num() {
  today_ask_friend_num_ = 0;
  clear_has_today_ask_friend_num();
}
inline ::google::protobuf::int32 extend_data_v1::today_ask_friend_num() const {
  return today_ask_friend_num_;
}
inline void extend_data_v1::set_today_ask_friend_num(::google::protobuf::int32 value) {
  set_has_today_ask_friend_num();
  today_ask_friend_num_ = value;
}

// optional int64 last_get_action_count_time = 6;
inline bool extend_data_v1::has_last_get_action_count_time() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void extend_data_v1::set_has_last_get_action_count_time() {
  _has_bits_[0] |= 0x00000020u;
}
inline void extend_data_v1::clear_has_last_get_action_count_time() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void extend_data_v1::clear_last_get_action_count_time() {
  last_get_action_count_time_ = GOOGLE_LONGLONG(0);
  clear_has_last_get_action_count_time();
}
inline ::google::protobuf::int64 extend_data_v1::last_get_action_count_time() const {
  return last_get_action_count_time_;
}
inline void extend_data_v1::set_last_get_action_count_time(::google::protobuf::int64 value) {
  set_has_last_get_action_count_time();
  last_get_action_count_time_ = value;
}

// optional int32 today_buy_action_count_times = 7;
inline bool extend_data_v1::has_today_buy_action_count_times() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void extend_data_v1::set_has_today_buy_action_count_times() {
  _has_bits_[0] |= 0x00000040u;
}
inline void extend_data_v1::clear_has_today_buy_action_count_times() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void extend_data_v1::clear_today_buy_action_count_times() {
  today_buy_action_count_times_ = 0;
  clear_has_today_buy_action_count_times();
}
inline ::google::protobuf::int32 extend_data_v1::today_buy_action_count_times() const {
  return today_buy_action_count_times_;
}
inline void extend_data_v1::set_today_buy_action_count_times(::google::protobuf::int32 value) {
  set_has_today_buy_action_count_times();
  today_buy_action_count_times_ = value;
}

// -------------------------------------------------------------------

// state_v1

// optional uint32 id = 1;
inline bool state_v1::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void state_v1::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void state_v1::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void state_v1::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 state_v1::id() const {
  return id_;
}
inline void state_v1::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// optional uint64 end_time = 2;
inline bool state_v1::has_end_time() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void state_v1::set_has_end_time() {
  _has_bits_[0] |= 0x00000002u;
}
inline void state_v1::clear_has_end_time() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void state_v1::clear_end_time() {
  end_time_ = GOOGLE_ULONGLONG(0);
  clear_has_end_time();
}
inline ::google::protobuf::uint64 state_v1::end_time() const {
  return end_time_;
}
inline void state_v1::set_end_time(::google::protobuf::uint64 value) {
  set_has_end_time();
  end_time_ = value;
}

// optional uint64 res1 = 3;
inline bool state_v1::has_res1() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void state_v1::set_has_res1() {
  _has_bits_[0] |= 0x00000004u;
}
inline void state_v1::clear_has_res1() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void state_v1::clear_res1() {
  res1_ = GOOGLE_ULONGLONG(0);
  clear_has_res1();
}
inline ::google::protobuf::uint64 state_v1::res1() const {
  return res1_;
}
inline void state_v1::set_res1(::google::protobuf::uint64 value) {
  set_has_res1();
  res1_ = value;
}

// optional uint32 res2 = 4;
inline bool state_v1::has_res2() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void state_v1::set_has_res2() {
  _has_bits_[0] |= 0x00000008u;
}
inline void state_v1::clear_has_res2() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void state_v1::clear_res2() {
  res2_ = 0u;
  clear_has_res2();
}
inline ::google::protobuf::uint32 state_v1::res2() const {
  return res2_;
}
inline void state_v1::set_res2(::google::protobuf::uint32 value) {
  set_has_res2();
  res2_ = value;
}

// optional uint32 res3 = 5;
inline bool state_v1::has_res3() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void state_v1::set_has_res3() {
  _has_bits_[0] |= 0x00000010u;
}
inline void state_v1::clear_has_res3() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void state_v1::clear_res3() {
  res3_ = 0u;
  clear_has_res3();
}
inline ::google::protobuf::uint32 state_v1::res3() const {
  return res3_;
}
inline void state_v1::set_res3(::google::protobuf::uint32 value) {
  set_has_res3();
  res3_ = value;
}

// -------------------------------------------------------------------

// state_list_v1

// repeated .extend_data.state_v1 state_list = 1;
inline int state_list_v1::state_list_size() const {
  return state_list_.size();
}
inline void state_list_v1::clear_state_list() {
  state_list_.Clear();
}
inline const ::extend_data::state_v1& state_list_v1::state_list(int index) const {
  return state_list_.Get(index);
}
inline ::extend_data::state_v1* state_list_v1::mutable_state_list(int index) {
  return state_list_.Mutable(index);
}
inline ::extend_data::state_v1* state_list_v1::add_state_list() {
  return state_list_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::extend_data::state_v1 >&
state_list_v1::state_list() const {
  return state_list_;
}
inline ::google::protobuf::RepeatedPtrField< ::extend_data::state_v1 >*
state_list_v1::mutable_state_list() {
  return &state_list_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace extend_data

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::extend_data::extend_data_version>() {
  return ::extend_data::extend_data_version_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_extend_5fdata_2eproto__INCLUDED
