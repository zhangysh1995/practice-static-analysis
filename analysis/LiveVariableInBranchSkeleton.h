//
// Created by Sunshine on 26/3/2020.
//

#ifndef TUTORIALPASS_LIVEVARIABLEINBRANCH_H
#define TUTORIALPASS_LIVEVARIABLEINBRANCH_H

#include <utility>
#include <queue>
#include <map>
#include <vector>
#include <set>
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/PassAnalysisSupport.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/DenseMap.h"

using namespace std;
using namespace llvm;

/*
 * The skeleton is for reference only, and you can use the data structure as you like
 */

//------------------------------------------------------------------------------
// PM interface
//------------------------------------------------------------------------------

class LiveVariableInBranch : public llvm::FunctionPass {
    public:
        static char ID;
        LiveVariableInBranch() : llvm::FunctionPass(ID) {}
        bool runOnFunction(llvm::Function &F) override;
        void topologicalSort(vector<BasicBlock *> *blockq, Function &F);

        static DenseMap<StringRef, uint>* genVarIndexMap(vector<BasicBlock*> *blockq);
        // basic block analysis
        void workList(vector<BasicBlock *> const *blockq,
                                        DenseMap<BasicBlock *, uint>* blockToBV,
                                        DenseMap<BasicBlock *, uint>* blockToGen,
                                        DenseMap<BasicBlock *, uint>* blockToKill);

        // instruction analysis
        void workList(vector<BasicBlock *> const *blockq);

        void onTheFlyAnalysis(llvm::Function &F,
                vector<BasicBlock*> * blockq, DenseMap<BasicBlock*, uint> &blockToBV);

};

#endif //TUTORIALPASS_LIVEVARIABLEINBRANCH_H
