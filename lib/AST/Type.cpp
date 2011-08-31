//===-- Type.cpp - Fortran Type Interface ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// The Fortran type interface.
//
//===----------------------------------------------------------------------===//

#include "flang/AST/Type.h"
#include "flang/AST/ASTContext.h"
#include "flang/AST/Expr.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
using namespace fortran;

//===----------------------------------------------------------------------===//
//                             Subtype Methods
//===----------------------------------------------------------------------===//

void BuiltinType::print(llvm::raw_ostream &O) const {
  switch (getTypeSpec()) {
  default: assert(false && "Invalid built-in type!");
  case BuiltinType::Integer:
    O << "integer";
    break;
  case BuiltinType::Real:
    O << "real";
    break;
  case BuiltinType::DoublePrecision:
    O << "double_precision";
    break;
  case BuiltinType::Complex:
    O << "complex";
    break;
  case BuiltinType::Logical:
    O << "logical";
    break;
  }

  if (Kind) {
    O << " kind=\"";
    Kind->print(O);
    O << "\"";
  }
}

void CharacterBuiltinType::print(llvm::raw_ostream &O) const {
  O << "character";

  if (Len) {
    O << " length=\"";
    Len->print(O);
    O << "\"";
  }

  if (Kind) {
    O << " kind=\"";
    Kind->print(O);
    O << "\"";
  }
}
