
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __java_nio_DoubleViewBufferImpl__
#define __java_nio_DoubleViewBufferImpl__

#pragma interface

#include <java/nio/DoubleBuffer.h>
extern "Java"
{
  namespace java
  {
    namespace nio
    {
        class ByteBuffer;
        class ByteOrder;
        class DoubleBuffer;
        class DoubleViewBufferImpl;
    }
  }
}

class java::nio::DoubleViewBufferImpl : public ::java::nio::DoubleBuffer
{

public: // actually package-private
  DoubleViewBufferImpl(::java::nio::ByteBuffer *, jint);
public:
  DoubleViewBufferImpl(::java::nio::ByteBuffer *, jint, jint, jint, jint, jint, jboolean, ::java::nio::ByteOrder *);
  jdouble get();
  jdouble get(jint);
  ::java::nio::DoubleBuffer * put(jdouble);
  ::java::nio::DoubleBuffer * put(jint, jdouble);
  ::java::nio::DoubleBuffer * compact();
  ::java::nio::DoubleBuffer * slice();
public: // actually package-private
  ::java::nio::DoubleBuffer * duplicate(jboolean);
public:
  ::java::nio::DoubleBuffer * duplicate();
  ::java::nio::DoubleBuffer * asReadOnlyBuffer();
  jboolean isReadOnly();
  jboolean isDirect();
  ::java::nio::ByteOrder * order();
private:
  jint __attribute__((aligned(__alignof__( ::java::nio::DoubleBuffer)))) offset;
  ::java::nio::ByteBuffer * bb;
  jboolean readOnly;
  ::java::nio::ByteOrder * endian;
public:
  static ::java::lang::Class class$;
};

#endif // __java_nio_DoubleViewBufferImpl__