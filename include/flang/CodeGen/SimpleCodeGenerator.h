//===--- CodeGen/SimpleCodeGenerator.h - Build LLVM IR from AST -*- C++ -*-===//
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

#ifndef LLVM_FLANG_CODEGEN_SIMPLECODEGENERATOR_H
#define LLVM_FLANG_CODEGEN_SIMPLECODEGENERATOR_H

#include <map>
#include <string>

namespace llvm {
  class LLVMContext;
  class Module;
  class IRBuilderBase;
  class Value;
  template<typename T> class ArrayRef;
}

namespace flang {
  class Stmt;
  template<typename T> class ActionResult;  
  
  // this is a class that generates code for simple constructs
  // basically trying to see if we can emit IR for hello world
  // that works.
  class SimpleCodeGenerator{
    llvm::LLVMContext& _ctx;
    const llvm::Module* _mod;
    const llvm::IRBuilderBase* _bldr;    
    std::map<std::string,llvm::Value*> _nValues;

  public:
    SimpleCodeGenerator(llvm::LLVMContext&);
    ~SimpleCodeGenerator();    

    /// eat the AST we generated and emit IR
    bool processAST(llvm::ArrayRef<ActionResult<Stmt*> >);    

    // direct access to the module
    const llvm::Module* module() const;

    /// dump out the generated code
    void dump() const;
  };
} // end flang namespace

#endif
