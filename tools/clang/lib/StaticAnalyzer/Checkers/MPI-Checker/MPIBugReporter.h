//===-- MPIBugReporter.h - bug reporter -----------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file defines prefabricated reports which are emitted in
/// case of MPI related bugs, detected by path-sensitive analysis.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_STATICANALYZER_CHECKERS_MPICHECKER_MPIBUGREPORTER_H
#define LLVM_CLANG_LIB_STATICANALYZER_CHECKERS_MPICHECKER_MPIBUGREPORTER_H

#include "MPITypes.h"
#include "MPITypes_2.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"

namespace clang {
namespace ento {
namespace mpi {

class MPIBugReporter {
public:
  MPIBugReporter(const CheckerBase &CB) {
    UnmatchedWaitBugType.reset(new BugType(&CB, "Unmatched wait", MPIError));
    DoubleNonblockingBugType.reset(
        new BugType(&CB, "Double nonblocking", MPIError));
    MissingWaitBugType.reset(new BugType(&CB, "Missing wait", MPIError));
    DoubleCloseBugType.reset(new BugType(&CB, "Double Close", MPIError));
    FileLeakBugType.reset(
        new BugType(&CB, "File has not been closed after open", MPIError));
    DoubleOpenBugType.reset(new BugType(&CB, "Double Open", MPIError));
  }

  /// Report duplicate request use by nonblocking calls without intermediate
  /// wait.
  ///
  /// \param MPICallEvent MPI call that caused the double nonblocking
  /// \param Req request that was used by two nonblocking calls in sequence
  /// \param RequestRegion memory region of the request
  /// \param ExplNode node in the graph the bug appeared at
  /// \param BReporter bug reporter for current context
  void reportDoubleNonblocking(const CallEvent &MPICallEvent,
                               const Request &Req,
                               const MemRegion *const RequestRegion,
                               const ExplodedNode *const ExplNode,
                              BugReporter &BReporter) const;

  /// Report a missing wait for a nonblocking call.
  ///
  /// \param Req request that is not matched by a wait
  /// \param RequestRegion memory region of the request
  /// \param ExplNode node in the graph the bug appeared at
  /// \param BReporter bug reporter for current context
  void reportMissingWait(const Request &Req,
                         const MemRegion *const RequestRegion,
                         const ExplodedNode *const ExplNode,
                         BugReporter &BReporter) const;

  /// Report a wait on a request that has not been used at all before.
  ///
  /// \param CE wait call that uses the request
  /// \param RequestRegion memory region of the request
  /// \param ExplNode node in the graph the bug appeared at
  /// \param BReporter bug reporter for current context
  void reportUnmatchedWait(const CallEvent &CE,
                           const MemRegion *const RequestRegion,
                           const ExplodedNode *const ExplNode,
                           BugReporter &BReporter) const;

  void reportDoubleClose(const CallEvent &MPICallEvent,
                          const MPIFile &fh,
                          const MemRegion *const MPIFileRegion,
                          const ExplodedNode *const ExplNode,
                          BugReporter &BReporter) const;

  void reportFileLeak(const MPIFile &fh, const MemRegion *const MPIFileRegion,
                      const ExplodedNode *const ExplNode,
                      BugReporter &BReporter) const;

  void reportDoubleOpen(const CallEvent &MPICallEvent, const ento::mpi::MPIFile &Fh,
                  const MemRegion *const MPIFileRegion,
                  const ExplodedNode *const ExplNode,
                  BugReporter &BReporter) const;

private:
  const std::string MPIError = "MPI Error";

  // path-sensitive bug types
  std::unique_ptr<BugType> UnmatchedWaitBugType;
  std::unique_ptr<BugType> MissingWaitBugType;
  std::unique_ptr<BugType> DoubleNonblockingBugType;
  std::unique_ptr<BugType> DoubleCloseBugType;
  std::unique_ptr<BugType> FileLeakBugType;
  std::unique_ptr<BugType> DoubleOpenBugType;

  /// Bug visitor class to find the node where the request region was
  /// previously used in order to include it into the BugReport path.
  class RequestNodeVisitor : public BugReporterVisitorImpl<RequestNodeVisitor> {
  public:
    RequestNodeVisitor(const MemRegion *const MemoryRegion,
                       const std::string &ErrText)
        : RequestRegion(MemoryRegion), ErrorText(ErrText) {}

    void Profile(llvm::FoldingSetNodeID &ID) const override {
      static int X = 0;
      ID.AddPointer(&X);
      ID.AddPointer(RequestRegion);
    }

    std::shared_ptr<PathDiagnosticPiece> VisitNode(const ExplodedNode *N,
                                                   const ExplodedNode *PrevN,
                                                   BugReporterContext &BRC,
                                                   BugReport &BR) override;

  private:
    const MemRegion *const RequestRegion;
    bool IsNodeFound = false;
    std::string ErrorText;
    };

    // class to observe the path of the used file for the bugreport
    class MPIFileNodeVisitor
        : public BugReporterVisitorImpl<MPIFileNodeVisitor> {
    public:
      MPIFileNodeVisitor(const MemRegion *const MemoryRegion,
                         const std::string &ErrText)
          : MPIFileRegion(MemoryRegion), ErrorText(ErrText) {}

      void Profile(llvm::FoldingSetNodeID &ID) const override {
        static int X = 0;
        ID.AddPointer(&X);
        ID.AddPointer(MPIFileRegion);
      }

      std::shared_ptr<PathDiagnosticPiece> VisitNode(const ExplodedNode *N,
                                                     const ExplodedNode *PrevN,
                                                     BugReporterContext &BRC,
                                                     BugReport &BR) override;

    private:
      const MemRegion *const MPIFileRegion;
      bool IsNodeFound = false;
      std::string ErrorText;
    };
};

} // end of namespace: mpi
} // end of namespace: ento
} // end of namespace: clang

#endif
