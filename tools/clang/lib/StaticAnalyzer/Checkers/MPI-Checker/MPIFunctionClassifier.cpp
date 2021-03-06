//===-- MPIFunctionClassifier.cpp - classifies MPI functions ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file defines functionality to identify and classify MPI functions.
///
//===----------------------------------------------------------------------===//

#include "clang/StaticAnalyzer/Checkers/MPIFunctionClassifier.h"
#include "llvm/ADT/STLExtras.h"

namespace clang {
namespace ento {
namespace mpi {

void MPIFunctionClassifier::identifierInit(ASTContext &ASTCtx) {
  // Initialize function identifiers.
  initPointToPointIdentifiers(ASTCtx);
  initCollectiveIdentifiers(ASTCtx);
  initIOIdentifiers(ASTCtx);
  initAdditionalIdentifiers(ASTCtx);
}

void MPIFunctionClassifier::initPointToPointIdentifiers(ASTContext &ASTCtx) {
  // Copy identifiers into the correct classification containers.
  IdentInfo_MPI_Send = &ASTCtx.Idents.get("MPI_Send");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Send);
  MPIType.push_back(IdentInfo_MPI_Send);
  assert(IdentInfo_MPI_Send);

  IdentInfo_MPI_Isend = &ASTCtx.Idents.get("MPI_Isend");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Isend);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Isend);
  MPIType.push_back(IdentInfo_MPI_Isend);
  assert(IdentInfo_MPI_Isend);

  IdentInfo_MPI_Ssend = &ASTCtx.Idents.get("MPI_Ssend");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Ssend);
  MPIType.push_back(IdentInfo_MPI_Ssend);
  assert(IdentInfo_MPI_Ssend);

  IdentInfo_MPI_Issend = &ASTCtx.Idents.get("MPI_Issend");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Issend);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Issend);
  MPIType.push_back(IdentInfo_MPI_Issend);
  assert(IdentInfo_MPI_Issend);

  IdentInfo_MPI_Bsend = &ASTCtx.Idents.get("MPI_Bsend");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Bsend);
  MPIType.push_back(IdentInfo_MPI_Bsend);
  assert(IdentInfo_MPI_Bsend);

  IdentInfo_MPI_Ibsend = &ASTCtx.Idents.get("MPI_Ibsend");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Ibsend);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Ibsend);
  MPIType.push_back(IdentInfo_MPI_Ibsend);
  assert(IdentInfo_MPI_Ibsend);

  IdentInfo_MPI_Rsend = &ASTCtx.Idents.get("MPI_Rsend");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Rsend);
  MPIType.push_back(IdentInfo_MPI_Rsend);
  assert(IdentInfo_MPI_Rsend);

  IdentInfo_MPI_Irsend = &ASTCtx.Idents.get("MPI_Irsend");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Irsend);
  MPIType.push_back(IdentInfo_MPI_Irsend);
  assert(IdentInfo_MPI_Irsend);

  IdentInfo_MPI_Recv = &ASTCtx.Idents.get("MPI_Recv");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Recv);
  MPIType.push_back(IdentInfo_MPI_Recv);
  assert(IdentInfo_MPI_Recv);

  IdentInfo_MPI_Irecv = &ASTCtx.Idents.get("MPI_Irecv");
  MPIPointToPointTypes.push_back(IdentInfo_MPI_Irecv);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Irecv);
  MPIType.push_back(IdentInfo_MPI_Irecv);
  assert(IdentInfo_MPI_Irecv);
}

void MPIFunctionClassifier::initCollectiveIdentifiers(ASTContext &ASTCtx) {
  // Copy identifiers into the correct classification containers.
  IdentInfo_MPI_Scatter = &ASTCtx.Idents.get("MPI_Scatter");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Scatter);
  MPIPointToCollTypes.push_back(IdentInfo_MPI_Scatter);
  MPIType.push_back(IdentInfo_MPI_Scatter);
  assert(IdentInfo_MPI_Scatter);

  IdentInfo_MPI_Iscatter = &ASTCtx.Idents.get("MPI_Iscatter");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Iscatter);
  MPIPointToCollTypes.push_back(IdentInfo_MPI_Iscatter);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Iscatter);
  MPIType.push_back(IdentInfo_MPI_Iscatter);
  assert(IdentInfo_MPI_Iscatter);

  IdentInfo_MPI_Gather = &ASTCtx.Idents.get("MPI_Gather");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Gather);
  MPICollToPointTypes.push_back(IdentInfo_MPI_Gather);
  MPIType.push_back(IdentInfo_MPI_Gather);
  assert(IdentInfo_MPI_Gather);

  IdentInfo_MPI_Igather = &ASTCtx.Idents.get("MPI_Igather");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Igather);
  MPICollToPointTypes.push_back(IdentInfo_MPI_Igather);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Igather);
  MPIType.push_back(IdentInfo_MPI_Igather);
  assert(IdentInfo_MPI_Igather);

  IdentInfo_MPI_Allgather = &ASTCtx.Idents.get("MPI_Allgather");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Allgather);
  MPICollToCollTypes.push_back(IdentInfo_MPI_Allgather);
  MPIType.push_back(IdentInfo_MPI_Allgather);
  assert(IdentInfo_MPI_Allgather);

  IdentInfo_MPI_Iallgather = &ASTCtx.Idents.get("MPI_Iallgather");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Iallgather);
  MPICollToCollTypes.push_back(IdentInfo_MPI_Iallgather);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Iallgather);
  MPIType.push_back(IdentInfo_MPI_Iallgather);
  assert(IdentInfo_MPI_Iallgather);

  IdentInfo_MPI_Bcast = &ASTCtx.Idents.get("MPI_Bcast");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Bcast);
  MPIPointToCollTypes.push_back(IdentInfo_MPI_Bcast);
  MPIType.push_back(IdentInfo_MPI_Bcast);
  assert(IdentInfo_MPI_Bcast);

  IdentInfo_MPI_Ibcast = &ASTCtx.Idents.get("MPI_Ibcast");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Ibcast);
  MPIPointToCollTypes.push_back(IdentInfo_MPI_Ibcast);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Ibcast);
  MPIType.push_back(IdentInfo_MPI_Ibcast);
  assert(IdentInfo_MPI_Ibcast);

  IdentInfo_MPI_Reduce = &ASTCtx.Idents.get("MPI_Reduce");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Reduce);
  MPICollToPointTypes.push_back(IdentInfo_MPI_Reduce);
  MPIType.push_back(IdentInfo_MPI_Reduce);
  assert(IdentInfo_MPI_Reduce);

  IdentInfo_MPI_Ireduce = &ASTCtx.Idents.get("MPI_Ireduce");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Ireduce);
  MPICollToPointTypes.push_back(IdentInfo_MPI_Ireduce);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Ireduce);
  MPIType.push_back(IdentInfo_MPI_Ireduce);
  assert(IdentInfo_MPI_Ireduce);

  IdentInfo_MPI_Allreduce = &ASTCtx.Idents.get("MPI_Allreduce");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Allreduce);
  MPICollToCollTypes.push_back(IdentInfo_MPI_Allreduce);
  MPIType.push_back(IdentInfo_MPI_Allreduce);
  assert(IdentInfo_MPI_Allreduce);

  IdentInfo_MPI_Iallreduce = &ASTCtx.Idents.get("MPI_Iallreduce");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Iallreduce);
  MPICollToCollTypes.push_back(IdentInfo_MPI_Iallreduce);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Iallreduce);
  MPIType.push_back(IdentInfo_MPI_Iallreduce);
  assert(IdentInfo_MPI_Iallreduce);

  IdentInfo_MPI_Alltoall = &ASTCtx.Idents.get("MPI_Alltoall");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Alltoall);
  MPICollToCollTypes.push_back(IdentInfo_MPI_Alltoall);
  MPIType.push_back(IdentInfo_MPI_Alltoall);
  assert(IdentInfo_MPI_Alltoall);

  IdentInfo_MPI_Ialltoall = &ASTCtx.Idents.get("MPI_Ialltoall");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Ialltoall);
  MPICollToCollTypes.push_back(IdentInfo_MPI_Ialltoall);
  MPINonBlockingTypes.push_back(IdentInfo_MPI_Ialltoall);
  MPIType.push_back(IdentInfo_MPI_Ialltoall);
  assert(IdentInfo_MPI_Ialltoall);
}

// identifiers for IO operations
void MPIFunctionClassifier::initIOIdentifiers(ASTContext &ASTCtx){

  IdentInfo_MPI_File_open = &ASTCtx.Idents.get("MPI_File_open");
  MPIIOTypes.push_back(IdentInfo_MPI_File_open);
  MPIType.push_back(IdentInfo_MPI_File_open);
  assert(IdentInfo_MPI_File_open);

  IdentInfo_MPI_File_close = &ASTCtx.Idents.get("MPI_File_close");
  MPIIOTypes.push_back(IdentInfo_MPI_File_close);
  MPIType.push_back(IdentInfo_MPI_File_close);
  assert(IdentInfo_MPI_File_close);

  IdentInfo_MPI_File_read = &ASTCtx.Idents.get("MPI_File_read");
  MPIIOTypes.push_back(IdentInfo_MPI_File_read);
  MPIType.push_back(IdentInfo_MPI_File_read);
  assert(IdentInfo_MPI_File_read);

  IdentInfo_MPI_File_write = &ASTCtx.Idents.get("MPI_File_write");
  MPIIOTypes.push_back(IdentInfo_MPI_File_write);
  MPIType.push_back(IdentInfo_MPI_File_write);
  assert(IdentInfo_MPI_File_write);

  IdentInfo_MPI_File_read_at = &ASTCtx.Idents.get("MPI_File_read_at");
  MPIIOTypes.push_back(IdentInfo_MPI_File_read_at);
  MPIType.push_back(IdentInfo_MPI_File_read_at);
  assert(IdentInfo_MPI_File_read_at);

  IdentInfo_MPI_File_write_at = &ASTCtx.Idents.get("MPI_File_write_at");
  MPIIOTypes.push_back(IdentInfo_MPI_File_write_at);
  MPIType.push_back(IdentInfo_MPI_File_write_at);
  assert(IdentInfo_MPI_File_write_at);

  IdentInfo_MPI_File_set_view = &ASTCtx.Idents.get("MPI_File_set_view");
  MPIIOTypes.push_back(IdentInfo_MPI_File_set_view);
  MPIType.push_back(IdentInfo_MPI_File_set_view);
  assert(IdentInfo_MPI_File_set_view);

  IdentInfo_MPI_File_seek = &ASTCtx.Idents.get("MPI_File_seek");
  MPIIOTypes.push_back(IdentInfo_MPI_File_seek);
  MPIType.push_back(IdentInfo_MPI_File_seek);
  assert(IdentInfo_MPI_File_seek);

  IdentInfo_MPI_File_seek_shared = &ASTCtx.Idents.get("MPI_File_seek_shared");
  MPIIOTypes.push_back(IdentInfo_MPI_File_seek_shared);
  MPIType.push_back(IdentInfo_MPI_File_seek_shared);
  assert(IdentInfo_MPI_File_seek_shared);

  IdentInfo_MPI_Type_create_subarray =
      &ASTCtx.Idents.get("MPI_Type_create_subarray");
  MPIIOTypes.push_back(IdentInfo_MPI_Type_create_subarray);
  MPIType.push_back(IdentInfo_MPI_Type_create_subarray);
  assert(IdentInfo_MPI_Type_create_subarray);

  IdentInfo_MPI_File_iread = &ASTCtx.Idents.get("MPI_File_iread");
  MPIIOTypes.push_back(IdentInfo_MPI_File_iread);
  MPIType.push_back(IdentInfo_MPI_File_iread);
  assert(IdentInfo_MPI_File_iread);

  IdentInfo_MPI_File_iwrite = &ASTCtx.Idents.get("MPI_File_iwrite");
  MPIIOTypes.push_back(IdentInfo_MPI_File_iwrite);
  MPIType.push_back(IdentInfo_MPI_File_iwrite);
  assert(IdentInfo_MPI_File_iwrite);

  IdentInfo_MPI_File_iread_at = &ASTCtx.Idents.get("MPI_File_iread_at");
  MPIIOTypes.push_back(IdentInfo_MPI_File_iread_at);
  MPIType.push_back(IdentInfo_MPI_File_iread_at);
  assert(IdentInfo_MPI_File_iread_at);

  IdentInfo_MPI_File_iwrite_at = &ASTCtx.Idents.get("MPI_File_iwrite_at");
  MPIIOTypes.push_back(IdentInfo_MPI_File_iwrite_at);
  MPIType.push_back(IdentInfo_MPI_File_iwrite_at);
  assert(IdentInfo_MPI_File_iwrite_at);

  IdentInfo_MPI_File_read_shared = &ASTCtx.Idents.get("MPI_File_read_shared");
  MPIIOTypes.push_back(IdentInfo_MPI_File_read_shared);
  MPIType.push_back(IdentInfo_MPI_File_read_shared);
  assert(IdentInfo_MPI_File_read_shared);
  
  IdentInfo_MPI_File_write_shared = &ASTCtx.Idents.get("MPI_File_write_shared");
  MPIIOTypes.push_back(IdentInfo_MPI_File_write_shared);
  MPIType.push_back(IdentInfo_MPI_File_write_shared);
  assert(IdentInfo_MPI_File_write_shared);

  IdentInfo_MPI_File_iread_shared = &ASTCtx.Idents.get("MPI_File_iread_shared");
  MPIIOTypes.push_back(IdentInfo_MPI_File_iread_shared);
  MPIType.push_back(IdentInfo_MPI_File_iread_shared);
  assert(IdentInfo_MPI_File_iread_shared);
  
  IdentInfo_MPI_File_iwrite_shared = &ASTCtx.Idents.get("MPI_File_iwrite_shared");
  MPIIOTypes.push_back(IdentInfo_MPI_File_iwrite_shared);
  MPIType.push_back(IdentInfo_MPI_File_iwrite_shared);
  assert(IdentInfo_MPI_File_iwrite_shared);

}

void MPIFunctionClassifier::initAdditionalIdentifiers(ASTContext &ASTCtx) {
  IdentInfo_MPI_Comm_rank = &ASTCtx.Idents.get("MPI_Comm_rank");
  MPIType.push_back(IdentInfo_MPI_Comm_rank);
  assert(IdentInfo_MPI_Comm_rank);

  IdentInfo_MPI_Comm_size = &ASTCtx.Idents.get("MPI_Comm_size");
  MPIType.push_back(IdentInfo_MPI_Comm_size);
  assert(IdentInfo_MPI_Comm_size);

  IdentInfo_MPI_Wait = &ASTCtx.Idents.get("MPI_Wait");
  MPIType.push_back(IdentInfo_MPI_Wait);
  assert(IdentInfo_MPI_Wait);

  IdentInfo_MPI_Waitall = &ASTCtx.Idents.get("MPI_Waitall");
  MPIType.push_back(IdentInfo_MPI_Waitall);
  assert(IdentInfo_MPI_Waitall);

  IdentInfo_MPI_Barrier = &ASTCtx.Idents.get("MPI_Barrier");
  MPICollectiveTypes.push_back(IdentInfo_MPI_Barrier);
  MPIType.push_back(IdentInfo_MPI_Barrier);
  assert(IdentInfo_MPI_Barrier);

  IdentInfo_MPI_Get_count = &ASTCtx.Idents.get("MPI_Get_count");
  MPIType.push_back(IdentInfo_MPI_Get_count);
  assert(IdentInfo_MPI_Get_count);
}

// general identifiers
bool MPIFunctionClassifier::isMPIType(const IdentifierInfo *IdentInfo) const {
  return llvm::is_contained(MPIType, IdentInfo);
}

bool MPIFunctionClassifier::isNonBlockingType(
    const IdentifierInfo *IdentInfo) const {
  return llvm::is_contained(MPINonBlockingTypes, IdentInfo);
}

// point-to-point identifiers
bool MPIFunctionClassifier::isPointToPointType(
    const IdentifierInfo *IdentInfo) const {
  return llvm::is_contained(MPIPointToPointTypes, IdentInfo);
}

// collective identifiers
bool MPIFunctionClassifier::isCollectiveType(
    const IdentifierInfo *IdentInfo) const {
  return llvm::is_contained(MPICollectiveTypes, IdentInfo);
}

bool MPIFunctionClassifier::isCollToColl(
    const IdentifierInfo *IdentInfo) const {
  return llvm::is_contained(MPICollToCollTypes, IdentInfo);
}

bool MPIFunctionClassifier::isScatterType(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_Scatter ||
         IdentInfo == IdentInfo_MPI_Iscatter;
}

bool MPIFunctionClassifier::isGatherType(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_Gather ||
         IdentInfo == IdentInfo_MPI_Igather ||
         IdentInfo == IdentInfo_MPI_Allgather ||
         IdentInfo == IdentInfo_MPI_Iallgather;
}

bool MPIFunctionClassifier::isAllgatherType(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_Allgather ||
         IdentInfo == IdentInfo_MPI_Iallgather;
}

bool MPIFunctionClassifier::isAlltoallType(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_Alltoall ||
         IdentInfo == IdentInfo_MPI_Ialltoall;
}

bool MPIFunctionClassifier::isBcastType(const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_Bcast || IdentInfo == IdentInfo_MPI_Ibcast;
}

bool MPIFunctionClassifier::isReduceType(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_Reduce ||
         IdentInfo == IdentInfo_MPI_Ireduce ||
         IdentInfo == IdentInfo_MPI_Allreduce ||
         IdentInfo == IdentInfo_MPI_Iallreduce;
}

// io identifiers
bool MPIFunctionClassifier::isMPIIO_Type(
  const IdentifierInfo *IdentInfo) const {
  	return llvm::is_contained(MPIIOTypes, IdentInfo);
}

bool MPIFunctionClassifier::isMPI_File_open(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_File_open;
}

bool MPIFunctionClassifier::isMPI_File_close(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_File_close;
}

// file manipulation
bool MPIFunctionClassifier::isMPIIO_file_manipulation(
    const IdentifierInfo *IdentInfo) const {
    return IdentInfo == IdentInfo_MPI_File_open ||
     IdentInfo == IdentInfo_MPI_File_close;
}

// data access routines
bool MPIFunctionClassifier::isMPIIO_collective(
  const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_File_read ||
         IdentInfo == IdentInfo_MPI_File_write ||
         IdentInfo == IdentInfo_MPI_File_read_at ||
         IdentInfo == IdentInfo_MPI_File_write_at ||
         IdentInfo == IdentInfo_MPI_File_iread ||
         IdentInfo == IdentInfo_MPI_File_iwrite ||
         IdentInfo == IdentInfo_MPI_File_iread_at ||
         IdentInfo == IdentInfo_MPI_File_iwrite_at ||
         IdentInfo == IdentInfo_MPI_File_read_shared ||
         IdentInfo == IdentInfo_MPI_File_write_shared ||
         IdentInfo == IdentInfo_MPI_File_iread_shared ||
         IdentInfo == IdentInfo_MPI_File_iwrite_shared; 
}

// blocking
bool MPIFunctionClassifier::isMPIIO_blocking(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_File_read ||
         IdentInfo == IdentInfo_MPI_File_write ||
         IdentInfo == IdentInfo_MPI_File_read_at ||
         IdentInfo == IdentInfo_MPI_File_write_at ||
         IdentInfo == IdentInfo_MPI_File_read_shared ||
         IdentInfo == IdentInfo_MPI_File_write_shared;
}

// nonblocking
bool MPIFunctionClassifier::isMPIIO_nonblocking(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_File_iread ||
         IdentInfo == IdentInfo_MPI_File_iwrite ||
         IdentInfo == IdentInfo_MPI_File_iread_at ||
         IdentInfo == IdentInfo_MPI_File_iwrite_at ||
         IdentInfo == IdentInfo_MPI_File_iread_shared ||
         IdentInfo == IdentInfo_MPI_File_iwrite_shared;
}

// explicit offsets 
bool MPIFunctionClassifier::isMPIIO_explicit_offset(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_File_read_at ||
         IdentInfo == IdentInfo_MPI_File_write_at ||
         IdentInfo == IdentInfo_MPI_File_iread_at ||
         IdentInfo == IdentInfo_MPI_File_iwrite_at;
}

// individual file pointers
bool MPIFunctionClassifier::isMPIIO_individual_file_pointers(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_File_read ||
         IdentInfo == IdentInfo_MPI_File_write ||
         IdentInfo == IdentInfo_MPI_File_iread ||
         IdentInfo == IdentInfo_MPI_File_iwrite;
}
// shared file pointer
bool MPIFunctionClassifier::isMPIIO_shared_file_pointer(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_File_read_shared ||
         IdentInfo == IdentInfo_MPI_File_write_shared ||
         IdentInfo == IdentInfo_MPI_File_iread_shared ||
         IdentInfo == IdentInfo_MPI_File_iwrite_shared;
}

// additional identifiers
bool MPIFunctionClassifier::isMPI_Wait(const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_Wait;
}

bool MPIFunctionClassifier::isMPI_Waitall(
    const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_Waitall;
}

bool MPIFunctionClassifier::isWaitType(const IdentifierInfo *IdentInfo) const {
  return IdentInfo == IdentInfo_MPI_Wait || IdentInfo == IdentInfo_MPI_Waitall;
}

} // end of namespace: mpi
} // end of namespace: ento
} // end of namespace: clang
