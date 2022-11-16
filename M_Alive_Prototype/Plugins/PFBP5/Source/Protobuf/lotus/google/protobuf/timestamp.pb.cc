// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/timestamp.proto

#include <Protobuf/lotus/google/protobuf/timestamp.pb.h>

#include <algorithm>

#include <Protobuf/lotus/google/protobuf/stubs/common.h>
#include <Protobuf/lotus/google/protobuf/stubs/port.h>
#include <Protobuf/lotus/google/protobuf/stubs/once.h>
#include <Protobuf/lotus/google/protobuf/io/coded_stream.h>
#include <Protobuf/lotus/google/protobuf/wire_format_lite_inl.h>
#include <Protobuf/lotus/google/protobuf/descriptor.h>
#include <Protobuf/lotus/google/protobuf/generated_message_reflection.h>
#include <Protobuf/lotus/google/protobuf/reflection_ops.h>
#include <Protobuf/lotus/google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace lotus {
namespace google {
namespace protobuf {
class TimestampDefaultTypeInternal {
 public:
  ::lotus::google::protobuf::internal::ExplicitlyConstructed<Timestamp>
      _instance;
} _Timestamp_default_instance_;
}  // namespace protobuf
}  // namespace google
}  // namespace lotus
namespace protobuf_google_2fprotobuf_2ftimestamp_2eproto {
void InitDefaultsTimestampImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::lotus::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::lotus::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::lotus::google::protobuf::_Timestamp_default_instance_;
    new (ptr) ::lotus::google::protobuf::Timestamp();
    ::lotus::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::lotus::google::protobuf::Timestamp::InitAsDefaultInstance();
}

void InitDefaultsTimestamp() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::lotus::google::protobuf::GoogleOnceInit(&once, &InitDefaultsTimestampImpl);
}

::lotus::google::protobuf::Metadata file_level_metadata[1];

const ::lotus::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::lotus::google::protobuf::Timestamp, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::lotus::google::protobuf::Timestamp, seconds_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::lotus::google::protobuf::Timestamp, nanos_),
};
static const ::lotus::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::lotus::google::protobuf::Timestamp)},
};

static ::lotus::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::lotus::google::protobuf::Message*>(&::lotus::google::protobuf::_Timestamp_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::lotus::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "google/protobuf/timestamp.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::lotus::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::lotus::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\037google/protobuf/timestamp.proto\022\017googl"
      "e.protobuf\"+\n\tTimestamp\022\017\n\007seconds\030\001 \001(\003"
      "\022\r\n\005nanos\030\002 \001(\005B~\n\023com.google.protobufB\016"
      "TimestampProtoP\001Z+github.com/golang/prot"
      "obuf/ptypes/timestamp\370\001\001\242\002\003GPB\252\002\036Google."
      "Protobuf.WellKnownTypesb\006proto3"
  };
  ::lotus::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 231);
  ::lotus::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "google/protobuf/timestamp.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::lotus::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_google_2fprotobuf_2ftimestamp_2eproto
namespace lotus {
namespace google {
namespace protobuf {

// ===================================================================

void Timestamp::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Timestamp::kSecondsFieldNumber;
const int Timestamp::kNanosFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Timestamp::Timestamp()
  : ::lotus::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_google_2fprotobuf_2ftimestamp_2eproto::InitDefaultsTimestamp();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:google.protobuf.Timestamp)
}
Timestamp::Timestamp(::lotus::google::protobuf::Arena* arena)
  : ::lotus::google::protobuf::Message(),
  _internal_metadata_(arena) {
  ::protobuf_google_2fprotobuf_2ftimestamp_2eproto::InitDefaultsTimestamp();
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:google.protobuf.Timestamp)
}
Timestamp::Timestamp(const Timestamp& from)
  : ::lotus::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&seconds_, &from.seconds_,
    static_cast<size_t>(reinterpret_cast<char*>(&nanos_) -
    reinterpret_cast<char*>(&seconds_)) + sizeof(nanos_));
  // @@protoc_insertion_point(copy_constructor:google.protobuf.Timestamp)
}

void Timestamp::SharedCtor() {
  ::memset(&seconds_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&nanos_) -
      reinterpret_cast<char*>(&seconds_)) + sizeof(nanos_));
  _cached_size_ = 0;
}

Timestamp::~Timestamp() {
  // @@protoc_insertion_point(destructor:google.protobuf.Timestamp)
  SharedDtor();
}

void Timestamp::SharedDtor() {
  GOOGLE_DCHECK(GetArenaNoVirtual() == NULL);
}

void Timestamp::ArenaDtor(void* object) {
  Timestamp* _this = reinterpret_cast< Timestamp* >(object);
  (void)_this;
}
void Timestamp::RegisterArenaDtor(::lotus::google::protobuf::Arena* arena) {
}
void Timestamp::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::lotus::google::protobuf::Descriptor* Timestamp::descriptor() {
  ::protobuf_google_2fprotobuf_2ftimestamp_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_google_2fprotobuf_2ftimestamp_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Timestamp& Timestamp::default_instance() {
  ::protobuf_google_2fprotobuf_2ftimestamp_2eproto::InitDefaultsTimestamp();
  return *internal_default_instance();
}

Timestamp* Timestamp::New(::lotus::google::protobuf::Arena* arena) const {
  return ::lotus::google::protobuf::Arena::CreateMessage<Timestamp>(arena);
}

void Timestamp::Clear() {
// @@protoc_insertion_point(message_clear_start:google.protobuf.Timestamp)
  ::lotus::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&seconds_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&nanos_) -
      reinterpret_cast<char*>(&seconds_)) + sizeof(nanos_));
  _internal_metadata_.Clear();
}

bool Timestamp::MergePartialFromCodedStream(
    ::lotus::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::lotus::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:google.protobuf.Timestamp)
  for (;;) {
    ::std::pair< ::lotus::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::lotus::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int64 seconds = 1;
      case 1: {
        if (static_cast< ::lotus::google::protobuf::uint8>(tag) ==
            static_cast< ::lotus::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::lotus::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::lotus::google::protobuf::int64, ::lotus::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &seconds_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 nanos = 2;
      case 2: {
        if (static_cast< ::lotus::google::protobuf::uint8>(tag) ==
            static_cast< ::lotus::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {

          DO_((::lotus::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::lotus::google::protobuf::int32, ::lotus::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &nanos_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::lotus::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:google.protobuf.Timestamp)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:google.protobuf.Timestamp)
  return false;
#undef DO_
}

void Timestamp::SerializeWithCachedSizes(
    ::lotus::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:google.protobuf.Timestamp)
  ::lotus::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 seconds = 1;
  if (this->seconds() != 0) {
    ::lotus::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->seconds(), output);
  }

  // int32 nanos = 2;
  if (this->nanos() != 0) {
    ::lotus::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->nanos(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::lotus::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::lotus::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::lotus::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:google.protobuf.Timestamp)
}

::lotus::google::protobuf::uint8* Timestamp::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::lotus::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:google.protobuf.Timestamp)
  ::lotus::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 seconds = 1;
  if (this->seconds() != 0) {
    target = ::lotus::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->seconds(), target);
  }

  // int32 nanos = 2;
  if (this->nanos() != 0) {
    target = ::lotus::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->nanos(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::lotus::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::lotus::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::lotus::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:google.protobuf.Timestamp)
  return target;
}

size_t Timestamp::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:google.protobuf.Timestamp)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::lotus::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::lotus::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::lotus::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // int64 seconds = 1;
  if (this->seconds() != 0) {
    total_size += 1 +
      ::lotus::google::protobuf::internal::WireFormatLite::Int64Size(
        this->seconds());
  }

  // int32 nanos = 2;
  if (this->nanos() != 0) {
    total_size += 1 +
      ::lotus::google::protobuf::internal::WireFormatLite::Int32Size(
        this->nanos());
  }

  int cached_size = ::lotus::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Timestamp::MergeFrom(const ::lotus::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:google.protobuf.Timestamp)
  GOOGLE_DCHECK_NE(&from, this);
  const Timestamp* source =
      ::lotus::google::protobuf::internal::DynamicCastToGenerated<const Timestamp>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:google.protobuf.Timestamp)
    ::lotus::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:google.protobuf.Timestamp)
    MergeFrom(*source);
  }
}

void Timestamp::MergeFrom(const Timestamp& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:google.protobuf.Timestamp)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::lotus::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.seconds() != 0) {
    set_seconds(from.seconds());
  }
  if (from.nanos() != 0) {
    set_nanos(from.nanos());
  }
}

void Timestamp::CopyFrom(const ::lotus::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:google.protobuf.Timestamp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Timestamp::CopyFrom(const Timestamp& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:google.protobuf.Timestamp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Timestamp::IsInitialized() const {
  return true;
}

void Timestamp::Swap(Timestamp* other) {
  if (other == this) return;
  if (GetArenaNoVirtual() == other->GetArenaNoVirtual()) {
    InternalSwap(other);
  } else {
    Timestamp* temp = New(GetArenaNoVirtual());
    temp->MergeFrom(*other);
    other->CopyFrom(*this);
    InternalSwap(temp);
    if (GetArenaNoVirtual() == NULL) {
      delete temp;
    }
  }
}
void Timestamp::UnsafeArenaSwap(Timestamp* other) {
  if (other == this) return;
  GOOGLE_DCHECK(GetArenaNoVirtual() == other->GetArenaNoVirtual());
  InternalSwap(other);
}
void Timestamp::InternalSwap(Timestamp* other) {
  using std::swap;
  swap(seconds_, other->seconds_);
  swap(nanos_, other->nanos_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::lotus::google::protobuf::Metadata Timestamp::GetMetadata() const {
  protobuf_google_2fprotobuf_2ftimestamp_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_google_2fprotobuf_2ftimestamp_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace protobuf
}  // namespace google
}  // namespace lotus

// @@protoc_insertion_point(global_scope)
