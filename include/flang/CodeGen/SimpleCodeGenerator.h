//===--- CodeGen/SimpleCodeGenerator.h - Build LLVM from ASTs ---*- C++ -*-===//
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

namespace llvm {
  class LLVMContext;
  class Module;
  class IRBuilderBase;
}

namespace flang {
  // this is a class that generates code for simple constructs
  class SimpleCodeGenerator{
    llvm::LLVMContext& _ctx;
    llvm::IRBuilderBase* _bldr;
    llvm::Module* _mod;

  public:
    SimpleCodeGenerator(llvm::LLVMContext&);
    ~SimpleCodeGenerator();    

    bool processAST();

    /// dump out the generated code
    void dump();
  };
} // end flang namespace

#endif
