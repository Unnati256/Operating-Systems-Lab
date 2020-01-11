// RUN: %clang_cc1 -triple x86_64-apple-darwin -O1 -no-struct-path-tbaa -disable-llvm-optzns %s -emit-llvm -o - | FileCheck %s
// RUN: %clang_cc1 -triple x86_64-apple-darwin -O1 -disable-llvm-optzns %s -emit-llvm -o - | FileCheck %s -check-prefix=PATH
// Test TBAA metadata generated by front-end.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
class StructA
{
public:
   uint16_t f16;
   uint32_t f32;
   uint16_t f16_2;
   uint32_t f32_2;
};
class StructB
{
public:
   uint16_t f16;
   StructA a;
   uint32_t f32;
};
class StructC
{
public:
   uint16_t f16;
   StructB b;
   uint32_t f32;
};
class StructD
{
public:
   uint16_t f16;
   StructB b;
   uint32_t f32;
   uint8_t f8;
};

class StructS
{
public:
   uint16_t f16;
   uint32_t f32;
};
class StructS2 : public StructS
{
public:
   uint16_t f16_2;
   uint32_t f32_2;
};

uint32_t g(uint32_t *s, StructA *A, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32:!.*]]
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32:!.*]]
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_A_f32:!.*]]
  *s = 1;
  A->f32 = 4;
  return *s;
}

uint32_t g2(uint32_t *s, StructA *A, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i16 4, i16* %{{.*}}, align 2, !tbaa [[TAG_i16:!.*]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// PATH: store i16 4, i16* %{{.*}}, align 2, !tbaa [[TAG_A_f16:!.*]]
  *s = 1;
  A->f16 = 4;
  return *s;
}

uint32_t g3(StructA *A, StructB *B, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_A_f32]]
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_B_a_f32:!.*]]
  A->f32 = 1;
  B->a.f32 = 4;
  return A->f32;
}

uint32_t g4(StructA *A, StructB *B, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i16 4, i16* %{{.*}}, align 2, !tbaa [[TAG_i16]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_A_f32]]
// PATH: store i16 4, i16* %{{.*}}, align 2, !tbaa [[TAG_B_a_f16:!.*]]
  A->f32 = 1;
  B->a.f16 = 4;
  return A->f32;
}

uint32_t g5(StructA *A, StructB *B, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_A_f32]]
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_B_f32:!.*]]
  A->f32 = 1;
  B->f32 = 4;
  return A->f32;
}

uint32_t g6(StructA *A, StructB *B, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_A_f32]]
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_B_a_f32_2:!.*]]
  A->f32 = 1;
  B->a.f32_2 = 4;
  return A->f32;
}

uint32_t g7(StructA *A, StructS *S, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_A_f32]]
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_S_f32:!.*]]
  A->f32 = 1;
  S->f32 = 4;
  return A->f32;
}

uint32_t g8(StructA *A, StructS *S, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i16 4, i16* %{{.*}}, align 2, !tbaa [[TAG_i16]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_A_f32]]
// PATH: store i16 4, i16* %{{.*}}, align 2, !tbaa [[TAG_S_f16:!.*]]
  A->f32 = 1;
  S->f16 = 4;
  return A->f32;
}

uint32_t g9(StructS *S, StructS2 *S2, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_S_f32]]
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_S_f32:!.*]]
  S->f32 = 1;
  S2->f32 = 4;
  return S->f32;
}

uint32_t g10(StructS *S, StructS2 *S2, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_S_f32]]
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_S2_f32_2:!.*]]
  S->f32 = 1;
  S2->f32_2 = 4;
  return S->f32;
}

uint32_t g11(StructC *C, StructD *D, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_C_b_a_f32:!.*]]
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_D_b_a_f32:!.*]]
  C->b.a.f32 = 1;
  D->b.a.f32 = 4;
  return C->b.a.f32;
}

uint32_t g12(StructC *C, StructD *D, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_i32]]
// TODO: differentiate the two accesses.
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa [[TAG_B_a_f32]]
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa [[TAG_B_a_f32]]
  StructB *b1 = &(C->b);
  StructB *b2 = &(D->b);
  // b1, b2 have different context.
  b1->a.f32 = 1;
  b2->a.f32 = 4;
  return b1->a.f32;
}

// CHECK: [[TYPE_char:!.*]] = !{!"omnipotent char", [[TAG_cxx_tbaa:!.*]],
// CHECK: [[TAG_cxx_tbaa]] = !{!"Simple C/C++ TBAA"}
// CHECK: [[TAG_i32]] = !{[[TYPE_i32:!.*]], [[TYPE_i32]], i64 0}
// CHECK: [[TYPE_i32]] = !{!"int", [[TYPE_char]],
// CHECK: [[TAG_i16]] = !{[[TYPE_i16:!.*]], [[TYPE_i16]], i64 0}
// CHECK: [[TYPE_i16]] = !{!"short", [[TYPE_char]],

// PATH: [[TYPE_CHAR:!.*]] = !{!"omnipotent char", !
// PATH: [[TAG_i32]] = !{[[TYPE_INT:!.*]], [[TYPE_INT]], i64 0}
// PATH: [[TYPE_INT]] = !{!"int", [[TYPE_CHAR]]
// PATH: [[TAG_A_f32]] = !{[[TYPE_A:!.*]], [[TYPE_INT]], i64 4}
// PATH: [[TYPE_A]] = !{!"_ZTS7StructA", [[TYPE_SHORT:!.*]], i64 0, [[TYPE_INT]], i64 4, [[TYPE_SHORT]], i64 8, [[TYPE_INT]], i64 12}
// PATH: [[TYPE_SHORT:!.*]] = !{!"short", [[TYPE_CHAR]]
// PATH: [[TAG_A_f16]] = !{[[TYPE_A]], [[TYPE_SHORT]], i64 0}
// PATH: [[TAG_B_a_f32]] = !{[[TYPE_B:!.*]], [[TYPE_INT]], i64 8}
// PATH: [[TYPE_B]] = !{!"_ZTS7StructB", [[TYPE_SHORT]], i64 0, [[TYPE_A]], i64 4, [[TYPE_INT]], i64 20}
// PATH: [[TAG_B_a_f16]] = !{[[TYPE_B]], [[TYPE_SHORT]], i64 4}
// PATH: [[TAG_B_f32]] = !{[[TYPE_B]], [[TYPE_INT]], i64 20}
// PATH: [[TAG_B_a_f32_2]] = !{[[TYPE_B]], [[TYPE_INT]], i64 16}
// PATH: [[TAG_S_f32]] = !{[[TYPE_S:!.*]], [[TYPE_INT]], i64 4}
// PATH: [[TYPE_S]] = !{!"_ZTS7StructS", [[TYPE_SHORT]], i64 0, [[TYPE_INT]], i64 4}
// PATH: [[TAG_S_f16]] = !{[[TYPE_S]], [[TYPE_SHORT]], i64 0}
// PATH: [[TAG_S2_f32_2]] = !{[[TYPE_S2:!.*]], [[TYPE_INT]], i64 12}
// PATH: [[TYPE_S2]] = !{!"_ZTS8StructS2", [[TYPE_SHORT]], i64 8, [[TYPE_INT]], i64 12}
// PATH: [[TAG_C_b_a_f32]] = !{[[TYPE_C:!.*]], [[TYPE_INT]], i64 12}
// PATH: [[TYPE_C]] = !{!"_ZTS7StructC", [[TYPE_SHORT]], i64 0, [[TYPE_B]], i64 4, [[TYPE_INT]], i64 28}
// PATH: [[TAG_D_b_a_f32]] = !{[[TYPE_D:!.*]], [[TYPE_INT]], i64 12}
// PATH: [[TYPE_D]] = !{!"_ZTS7StructD", [[TYPE_SHORT]], i64 0, [[TYPE_B]], i64 4, [[TYPE_INT]], i64 28, [[TYPE_CHAR]], i64 32}
