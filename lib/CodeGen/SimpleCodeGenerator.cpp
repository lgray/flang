//===--- SimpleCodeGenerator.cpp - Build LLVM IR from AST -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the SimpleCodeGenerator interface.
//
//===----------------------------------------------------------------------===//
#include "flang/CodeGen/SimpleCodeGenerator.h"

#include "flang/AST/Expr.h"
#include "flang/AST/Stmt.h"
#include "flang/AST/Type.h"
#include "flang/Basic/LLVM.h"
#include "llvm/Module.h"
#include "llvm/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
using namespace flang;

namespace {
  class StmtVisitor {
  public:
    void visit(StmtResult);

  private:
#define VISIT(STMT) void visit(const STMT *S)
    VISIT(ProgramStmt);
    VISIT(EndProgramStmt);
    //VISIT(UseStmt);
    //VISIT(ImportStmt);
    //VISIT(ImplicitStmt);
    //VISIT(AsynchronousStmt);
    VISIT(AssignmentStmt);
    VISIT(PrintStmt);
#undef VISIT   
  };
} // end anonymous namespace

//SimpleCodeGenerator definitions
SimpleCodeGenerator::SimpleCodeGenerator(llvm::LLVMContext& ctx):
  _ctx(ctx),
  _mod(new llvm::Module("simple module",_ctx)),
  _bldr(new llvm::IRBuilder<>(_ctx))
{}

SimpleCodeGenerator::~SimpleCodeGenerator() {
  delete _mod;
  delete _bldr;
}

void SimpleCodeGenerator::processAST(llvm::ArrayRef<StmtResult> ast) {
  StmtVisitor sv;
  ArrayRef<StmtResult>::iterator i = ast.begin(), e = ast.end();
  for(;i != e; ++i) sv.visit(*i);
}

const llvm::Module* SimpleCodeGenerator::module() const {
  return _mod;
}

void SimpleCodeGenerator::dump() const {
  _mod->dump();
}

//StmtVisitor definitions
void StmtVisitor::visit(StmtResult S) {
#define HANDLE(STMT) \
  if (const STMT *stmt = dyn_cast<STMT>(S.get())) {     \
    visit(stmt);                                        \
    return;                                             \
  }
  HANDLE(ProgramStmt);
  HANDLE(EndProgramStmt);
  //HANDLE(UseStmt);
  //HANDLE(ImportStmt);
  //HANDLE(ImplicitStmt);
  // HANDLE(AsynchronousStmt);
  HANDLE(AssignmentStmt);
  HANDLE(PrintStmt);
#undef HANDLE
}

void StmtVisitor::visit(const ProgramStmt *S) {
  const IdentifierInfo *Name = S->getProgramName();
  llvm::outs() << "(program";
  if (Name) llvm::outs() << ":  '" << Name->getName() << "'";
  llvm::outs() << ")\n";
}
void StmtVisitor::visit(const EndProgramStmt *S) {
  const IdentifierInfo *Name = S->getProgramName();
  llvm::outs() << "(end program";
  if (Name) llvm::outs() << ":  '" << Name->getName() << "'";
  llvm::outs() << ")\n";
}

void StmtVisitor::visit(const AssignmentStmt *S) {
  llvm::outs() << "(assignment:\n  (";
  S->getLHS()->getType().print(llvm::outs());    
  llvm::outs() << "\n  (";
  S->getRHS()->getType().print(llvm::outs());
  llvm::outs() << ") )\n";
}

void StmtVisitor::visit(const PrintStmt *S) {
  llvm::outs() << "(print) : ";
  ArrayRef<ExprResult> arr = S->getIDList();
  ArrayRef<ExprResult>::const_iterator pos = arr.begin();
  ArrayRef<ExprResult>::const_iterator lend = arr.end();
  for(; pos != lend; ++pos) {
    llvm::outs() << "( ";
    pos->get()->getType().print(llvm::outs());
    llvm::outs() << " : ";
    pos->get()->print(llvm::outs());
    llvm::outs() << ' ';
  }  
  llvm::outs() << ")\n";
}
