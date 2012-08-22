//===-- Main.cpp - LLVM-based Fortran Compiler ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// The Fortran Compiler.
//
//===----------------------------------------------------------------------===//

#include "flang/Frontend/TextDiagnosticPrinter.h"
#include "flang/Parse/Parser.h"
#include "flang/Sema/Sema.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/Timer.h"
#include "llvm/Support/system_error.h"
#include "llvm/ADT/OwningPtr.h"
using namespace llvm;
using namespace flang;

// driver diags here for now
namespace flang {
  namespace diag {
    enum {
#define DIAG(ENUM,FLAGS,DEFAULT_MAPPING,DESC,GROUP,	\
             SFINAE,ACCESS,CATEGORY,BRIEF,FULL) ENUM,
#include "flang/Basic/DiagnosticDriverKinds.inc"
#undef DIAG
      NUM_BUILTIN_DRIVER_DIAGNOSTICS
    };
  }
}

llvm::sys::Path GetExecutablePath(const char *Argv0, bool CanonicalPrefixes) {
  if (!CanonicalPrefixes)
    return llvm::sys::Path(Argv0);

  // This just needs to be some symbol in the binary; C++ doesn't
  // allow taking the address of ::main however.
  void *P = (void*) (intptr_t) GetExecutablePath;
  return llvm::sys::Path::GetMainExecutable(Argv0, P);
}

//===----------------------------------------------------------------------===//
// Command line options.
//===----------------------------------------------------------------------===//

namespace {

  cl::opt<std::string>
  InputFilename(cl::Positional, cl::desc("<input file>"), cl::init(""));

  cl::list<std::string>
  IncludeDirs("I", cl::desc("Directory of include files"),
              cl::value_desc("directory"), cl::Prefix);

  cl::opt<bool>
  ReturnComments("C", cl::desc("Do not discard comments"), cl::init(false));

} // end anonymous namespace

static bool ParseFile(const std::string &Filename,
                      const std::vector<std::string> &IncludeDirs) {  
  llvm::OwningPtr<llvm::MemoryBuffer> MB;

  // Record the location of the include directory so that the lexer can find it
  // later.
  SourceMgr SrcMgr;
  SrcMgr.setIncludeDirs(IncludeDirs);  

  LangOptions Opts;
  Opts.ReturnComments = ReturnComments;
  TextDiagnosticPrinter TDP(SrcMgr);
  Diagnostic Diag(&SrcMgr, &TDP, false);

  if (llvm::error_code ec = llvm::MemoryBuffer::getFile(Filename.c_str(),
							MB)) {
    llvm::errs() << "Could not open input file '" << Filename << "': " 
                 << ec.message() <<"\n";
    return true;
  }

  // Tell SrcMgr about this buffer, which is what Parser will pick up.
  SrcMgr.AddNewSourceBuffer(MB.take(), SMLoc());

#if 0
  PrintAction PA(Diag);
#endif
  ASTContext Context(SrcMgr);
  Sema SA(Context, Diag);
  Parser P(SrcMgr, Opts, Diag, SA);
  return P.ParseProgramUnits();
}

int main(int argc, char **argv) {
  sys::PrintStackTraceOnErrorSignal();
  PrettyStackTraceProgram X(argc, argv);
  cl::ParseCommandLineOptions(argc, argv, "LLVM Fortran compiler");

  bool CanonicalPrefixes = true;
  for (int i = 1; i < argc; ++i)
    if (llvm::StringRef(argv[i]) == "-no-canonical-prefixes") {
      CanonicalPrefixes = false;
      break;
    }

  llvm::sys::Path Path = GetExecutablePath(argv[0], CanonicalPrefixes);

  // Parse the input file.
  int Res = ParseFile(InputFilename, IncludeDirs);

  // If any timers were active but haven't been destroyed yet, print their
  // results now. This happens in -disable-free mode.
  llvm::TimerGroup::printAll(llvm::errs());

  llvm::llvm_shutdown();
  return Res;
}
