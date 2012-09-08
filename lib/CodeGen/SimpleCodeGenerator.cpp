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

SimpleCodeGenerator::SimpleCodeGenerator(llvm::LLVMContext& ctx):
  _ctx(ctx),
  _mod(new llvm::Module("simple module",_ctx)),
  _bldr(new llvm::IRBuilder<>(_ctx))
{}

SimpleCodeGenerator::~SimpleCodeGenerator() {
  delete _mod;
  delete _bldr;
}

bool SimpleCodeGenerator::processAST(llvm::ArrayRef<StmtResult> ast) {
  return false;
}

const llvm::Module* SimpleCodeGenerator::module() const {
  return _mod;
}

void SimpleCodeGenerator::dump() const {
  _mod->dump();
}
