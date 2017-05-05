; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-unknown-unknown | FileCheck %s --check-prefix=X32
; RUN: llc < %s -mtriple=x86_64-unknown-unknown | FileCheck %s --check-prefix=X64 --check-prefix=SSE2
; RUN: llc < %s -mtriple=x86_64-unknown-unknown -mattr=AVX2 | FileCheck %s --check-prefix=X64 --check-prefix=AVX2

; This tests codegen time inlining/optimization of memcmp
; rdar://6480398

@.str = private constant [65 x i8] c"0123456789012345678901234567890123456789012345678901234567890123\00", align 1

declare i32 @memcmp(i8*, i8*, i64)

define i1 @length2(i8* %X, i8* %Y, i32* nocapture %P) nounwind {
; X32-LABEL: length2:
; X32:       # BB#0:
; X32-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X32-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X32-NEXT:    movzwl (%ecx), %ecx
; X32-NEXT:    cmpw (%eax), %cx
; X32-NEXT:    sete %al
; X32-NEXT:    retl
;
; X64-LABEL: length2:
; X64:       # BB#0:
; X64-NEXT:    movzwl (%rdi), %eax
; X64-NEXT:    cmpw (%rsi), %ax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* %Y, i64 2) nounwind
  %c = icmp eq i32 %m, 0
  ret i1 %c
}

define i1 @length2_const(i8* %X, i32* nocapture %P) nounwind {
; X32-LABEL: length2_const:
; X32:       # BB#0:
; X32-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X32-NEXT:    movzwl (%eax), %eax
; X32-NEXT:    cmpl $12849, %eax # imm = 0x3231
; X32-NEXT:    setne %al
; X32-NEXT:    retl
;
; X64-LABEL: length2_const:
; X64:       # BB#0:
; X64-NEXT:    movzwl (%rdi), %eax
; X64-NEXT:    cmpl $12849, %eax # imm = 0x3231
; X64-NEXT:    setne %al
; X64-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str, i32 0, i32 1), i64 2) nounwind
  %c = icmp ne i32 %m, 0
  ret i1 %c
}

define i1 @length2_nobuiltin_attr(i8* %X, i8* %Y, i32* nocapture %P) nounwind {
; X32-LABEL: length2_nobuiltin_attr:
; X32:       # BB#0:
; X32-NEXT:    pushl $0
; X32-NEXT:    pushl $2
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    calll memcmp
; X32-NEXT:    addl $16, %esp
; X32-NEXT:    testl %eax, %eax
; X32-NEXT:    sete %al
; X32-NEXT:    retl
;
; X64-LABEL: length2_nobuiltin_attr:
; X64:       # BB#0:
; X64-NEXT:    pushq %rax
; X64-NEXT:    movl $2, %edx
; X64-NEXT:    callq memcmp
; X64-NEXT:    testl %eax, %eax
; X64-NEXT:    sete %al
; X64-NEXT:    popq %rcx
; X64-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* %Y, i64 2) nounwind nobuiltin
  %c = icmp eq i32 %m, 0
  ret i1 %c
}

define i1 @length4(i8* %X, i8* %Y, i32* nocapture %P) nounwind {
; X32-LABEL: length4:
; X32:       # BB#0:
; X32-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X32-NEXT:    movl {{[0-9]+}}(%esp), %ecx
; X32-NEXT:    movl (%ecx), %ecx
; X32-NEXT:    cmpl (%eax), %ecx
; X32-NEXT:    setne %al
; X32-NEXT:    retl
;
; X64-LABEL: length4:
; X64:       # BB#0:
; X64-NEXT:    movl (%rdi), %eax
; X64-NEXT:    cmpl (%rsi), %eax
; X64-NEXT:    setne %al
; X64-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* %Y, i64 4) nounwind
  %c = icmp ne i32 %m, 0
  ret i1 %c
}

define i1 @length4_const(i8* %X, i32* nocapture %P) nounwind {
; X32-LABEL: length4_const:
; X32:       # BB#0:
; X32-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X32-NEXT:    cmpl $875770417, (%eax) # imm = 0x34333231
; X32-NEXT:    sete %al
; X32-NEXT:    retl
;
; X64-LABEL: length4_const:
; X64:       # BB#0:
; X64-NEXT:    cmpl $875770417, (%rdi) # imm = 0x34333231
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str, i32 0, i32 1), i64 4) nounwind
  %c = icmp eq i32 %m, 0
  ret i1 %c
}

define i1 @length8(i8* %X, i8* %Y, i32* nocapture %P) nounwind {
; X32-LABEL: length8:
; X32:       # BB#0:
; X32-NEXT:    pushl $0
; X32-NEXT:    pushl $8
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    calll memcmp
; X32-NEXT:    addl $16, %esp
; X32-NEXT:    testl %eax, %eax
; X32-NEXT:    sete %al
; X32-NEXT:    retl
;
; X64-LABEL: length8:
; X64:       # BB#0:
; X64-NEXT:    movq (%rdi), %rax
; X64-NEXT:    cmpq (%rsi), %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* %Y, i64 8) nounwind
  %c = icmp eq i32 %m, 0
  ret i1 %c
}

define i1 @length8_const(i8* %X, i32* nocapture %P) nounwind {
; X32-LABEL: length8_const:
; X32:       # BB#0:
; X32-NEXT:    pushl $0
; X32-NEXT:    pushl $8
; X32-NEXT:    pushl $.L.str
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    calll memcmp
; X32-NEXT:    addl $16, %esp
; X32-NEXT:    testl %eax, %eax
; X32-NEXT:    setne %al
; X32-NEXT:    retl
;
; X64-LABEL: length8_const:
; X64:       # BB#0:
; X64-NEXT:    movabsq $3978425819141910832, %rax # imm = 0x3736353433323130
; X64-NEXT:    cmpq %rax, (%rdi)
; X64-NEXT:    setne %al
; X64-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str, i32 0, i32 0), i64 8) nounwind
  %c = icmp ne i32 %m, 0
  ret i1 %c
}

define i1 @length16(i8* %x, i8* %y) nounwind {
; X32-LABEL: length16:
; X32:       # BB#0:
; X32-NEXT:    pushl $0
; X32-NEXT:    pushl $16
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    calll memcmp
; X32-NEXT:    addl $16, %esp
; X32-NEXT:    testl %eax, %eax
; X32-NEXT:    setne %al
; X32-NEXT:    retl
;
; SSE2-LABEL: length16:
; SSE2:       # BB#0:
; SSE2-NEXT:    movdqu (%rsi), %xmm0
; SSE2-NEXT:    movdqu (%rdi), %xmm1
; SSE2-NEXT:    pcmpeqb %xmm0, %xmm1
; SSE2-NEXT:    pmovmskb %xmm1, %eax
; SSE2-NEXT:    cmpl $65535, %eax # imm = 0xFFFF
; SSE2-NEXT:    setne %al
; SSE2-NEXT:    retq
;
; AVX2-LABEL: length16:
; AVX2:       # BB#0:
; AVX2-NEXT:    vmovdqu (%rdi), %xmm0
; AVX2-NEXT:    vpcmpeqb (%rsi), %xmm0, %xmm0
; AVX2-NEXT:    vpmovmskb %xmm0, %eax
; AVX2-NEXT:    cmpl $65535, %eax # imm = 0xFFFF
; AVX2-NEXT:    setne %al
; AVX2-NEXT:    retq
  %call = tail call i32 @memcmp(i8* %x, i8* %y, i64 16) nounwind
  %cmp = icmp ne i32 %call, 0
  ret i1 %cmp
}

define i1 @length16_const(i8* %X, i32* nocapture %P) nounwind {
; X32-LABEL: length16_const:
; X32:       # BB#0:
; X32-NEXT:    pushl $0
; X32-NEXT:    pushl $16
; X32-NEXT:    pushl $.L.str
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    calll memcmp
; X32-NEXT:    addl $16, %esp
; X32-NEXT:    testl %eax, %eax
; X32-NEXT:    sete %al
; X32-NEXT:    retl
;
; SSE2-LABEL: length16_const:
; SSE2:       # BB#0:
; SSE2-NEXT:    movdqu (%rdi), %xmm0
; SSE2-NEXT:    pcmpeqb {{.*}}(%rip), %xmm0
; SSE2-NEXT:    pmovmskb %xmm0, %eax
; SSE2-NEXT:    cmpl $65535, %eax # imm = 0xFFFF
; SSE2-NEXT:    sete %al
; SSE2-NEXT:    retq
;
; AVX2-LABEL: length16_const:
; AVX2:       # BB#0:
; AVX2-NEXT:    vmovdqu (%rdi), %xmm0
; AVX2-NEXT:    vpcmpeqb {{.*}}(%rip), %xmm0, %xmm0
; AVX2-NEXT:    vpmovmskb %xmm0, %eax
; AVX2-NEXT:    cmpl $65535, %eax # imm = 0xFFFF
; AVX2-NEXT:    sete %al
; AVX2-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str, i32 0, i32 0), i64 16) nounwind
  %c = icmp eq i32 %m, 0
  ret i1 %c
}

define i1 @length32(i8* %x, i8* %y) nounwind {
; X32-LABEL: length32:
; X32:       # BB#0:
; X32-NEXT:    pushl $0
; X32-NEXT:    pushl $32
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    calll memcmp
; X32-NEXT:    addl $16, %esp
; X32-NEXT:    testl %eax, %eax
; X32-NEXT:    sete %al
; X32-NEXT:    retl
;
; SSE2-LABEL: length32:
; SSE2:       # BB#0:
; SSE2-NEXT:    pushq %rax
; SSE2-NEXT:    movl $32, %edx
; SSE2-NEXT:    callq memcmp
; SSE2-NEXT:    testl %eax, %eax
; SSE2-NEXT:    sete %al
; SSE2-NEXT:    popq %rcx
; SSE2-NEXT:    retq
;
; AVX2-LABEL: length32:
; AVX2:       # BB#0:
; AVX2-NEXT:    vmovdqu (%rdi), %ymm0
; AVX2-NEXT:    vpcmpeqb (%rsi), %ymm0, %ymm0
; AVX2-NEXT:    vpmovmskb %ymm0, %eax
; AVX2-NEXT:    cmpl $-1, %eax
; AVX2-NEXT:    sete %al
; AVX2-NEXT:    vzeroupper
; AVX2-NEXT:    retq
  %call = tail call i32 @memcmp(i8* %x, i8* %y, i64 32) nounwind
  %cmp = icmp eq i32 %call, 0
  ret i1 %cmp
}

define i1 @length32_const(i8* %X, i32* nocapture %P) nounwind {
; X32-LABEL: length32_const:
; X32:       # BB#0:
; X32-NEXT:    pushl $0
; X32-NEXT:    pushl $32
; X32-NEXT:    pushl $.L.str
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    calll memcmp
; X32-NEXT:    addl $16, %esp
; X32-NEXT:    testl %eax, %eax
; X32-NEXT:    setne %al
; X32-NEXT:    retl
;
; SSE2-LABEL: length32_const:
; SSE2:       # BB#0:
; SSE2-NEXT:    pushq %rax
; SSE2-NEXT:    movl $.L.str, %esi
; SSE2-NEXT:    movl $32, %edx
; SSE2-NEXT:    callq memcmp
; SSE2-NEXT:    testl %eax, %eax
; SSE2-NEXT:    setne %al
; SSE2-NEXT:    popq %rcx
; SSE2-NEXT:    retq
;
; AVX2-LABEL: length32_const:
; AVX2:       # BB#0:
; AVX2-NEXT:    vmovdqu (%rdi), %ymm0
; AVX2-NEXT:    vpcmpeqb {{.*}}(%rip), %ymm0, %ymm0
; AVX2-NEXT:    vpmovmskb %ymm0, %eax
; AVX2-NEXT:    cmpl $-1, %eax
; AVX2-NEXT:    setne %al
; AVX2-NEXT:    vzeroupper
; AVX2-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str, i32 0, i32 0), i64 32) nounwind
  %c = icmp ne i32 %m, 0
  ret i1 %c
}

define i1 @length64(i8* %x, i8* %y) nounwind {
; X32-LABEL: length64:
; X32:       # BB#0:
; X32-NEXT:    pushl $0
; X32-NEXT:    pushl $64
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    calll memcmp
; X32-NEXT:    addl $16, %esp
; X32-NEXT:    testl %eax, %eax
; X32-NEXT:    setne %al
; X32-NEXT:    retl
;
; X64-LABEL: length64:
; X64:       # BB#0:
; X64-NEXT:    pushq %rax
; X64-NEXT:    movl $64, %edx
; X64-NEXT:    callq memcmp
; X64-NEXT:    testl %eax, %eax
; X64-NEXT:    setne %al
; X64-NEXT:    popq %rcx
; X64-NEXT:    retq
  %call = tail call i32 @memcmp(i8* %x, i8* %y, i64 64) nounwind
  %cmp = icmp ne i32 %call, 0
  ret i1 %cmp
}

define i1 @length64_const(i8* %X, i32* nocapture %P) nounwind {
; X32-LABEL: length64_const:
; X32:       # BB#0:
; X32-NEXT:    pushl $0
; X32-NEXT:    pushl $64
; X32-NEXT:    pushl $.L.str
; X32-NEXT:    pushl {{[0-9]+}}(%esp)
; X32-NEXT:    calll memcmp
; X32-NEXT:    addl $16, %esp
; X32-NEXT:    testl %eax, %eax
; X32-NEXT:    sete %al
; X32-NEXT:    retl
;
; X64-LABEL: length64_const:
; X64:       # BB#0:
; X64-NEXT:    pushq %rax
; X64-NEXT:    movl $.L.str, %esi
; X64-NEXT:    movl $64, %edx
; X64-NEXT:    callq memcmp
; X64-NEXT:    testl %eax, %eax
; X64-NEXT:    sete %al
; X64-NEXT:    popq %rcx
; X64-NEXT:    retq
  %m = tail call i32 @memcmp(i8* %X, i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str, i32 0, i32 0), i64 64) nounwind
  %c = icmp eq i32 %m, 0
  ret i1 %c
}
