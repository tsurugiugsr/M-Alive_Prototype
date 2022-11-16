// Protocol Buffers - Google's data interchange format
// Copyright 2014 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#ifndef GOOGLE_PROTOBUF_STUBS_SINGLETON_H__
#define GOOGLE_PROTOBUF_STUBS_SINGLETON_H__

#include <Protobuf/lotus/google/protobuf/stubs/atomicops.h>
#include <Protobuf/lotus/google/protobuf/stubs/common.h>
#include <Protobuf/lotus/google/protobuf/stubs/once.h>

namespace lotus {
namespace google {
namespace protobuf {
namespace internal {
template<typename T>
class Singleton {
 public:
  static T* get() {
    GoogleOnceInit(&once_, &Singleton<T>::Init);
    return instance_;
  }
  static void ShutDown() {
    delete instance_;
    instance_ = NULL;
  }
 private:
  static void Init() {
    instance_ = new T();
  }
  static ProtobufOnceType once_;
  static T* instance_;
};

template<typename T>
ProtobufOnceType Singleton<T>::once_;

template<typename T>
T* Singleton<T>::instance_ = NULL;
}  // namespace internal
}  // namespace protobuf
}  // namespace google
}  // namespace lotus

#endif  // GOOGLE_PROTOBUF_STUBS_SINGLETON_H__
