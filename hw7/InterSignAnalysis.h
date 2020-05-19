//
// Created by yzhanghw on 2020/5/10.
//

#ifndef LLVM_PASS_INTERSIGNANALYSIS_H
#define LLVM_PASS_INTERSIGNANALYSIS_H

#include "llvm/PassAnalysisSupport.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/PassManager.h"

using namespace llvm;

class InterSignAnalysis : public ModulePass {
public:
    static char ID;
    InterSignAnalysis():ModulePass(ID), varCount(0){}
    bool runOnModule(Module& M) override;
    bool doFinalization(Module& M) override;

    void getAnalysisUsage(AnalysisUsage& AU) const override; // your code goes here
    void topologicalSort(Function* F, std::vector<BasicBlock*>* bbVector); // your code goes here
    void analyzeFunction(Function* func); // your code goes here
    DenseMap<StringRef, StringRef>* intraBBAnalyze(BasicBlock* bb); // your code goes here

private:
    void updateMap(DenseMap<StringRef, StringRef>* states, Value* operand, StringRef &varName);
    void exractVars(Function* func);
    void reportResult(); // your code goes here
    DenseMap<StringRef, unsigned> varMap;
    unsigned varCount;
    // you code goes here
    // add variables to save the intermediate result for a function
};


#endif //LLVM_PASS_INTERSIGNANALYSIS_H
