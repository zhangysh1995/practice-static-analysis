//
// Created by Sunshine on 16/3/2020
//

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>
#include <utility>
#include "llvm/PassAnalysisSupport.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "LiveVariableInBranchSkeleton.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/CFG.h"
#include "llvm/Support/Debug.h"

using namespace std;
using namespace llvm;

char LiveVariableInBranch::ID = 0;

//----------------------------------------------------------
// Implementation of SingleBasicBlockLivenessInfo
// Description: Record liveness of variables in each basic block
//----------------------------------------------------------


/*----------------------------------------------------------
* Implementation of LiveVariableInBranch
*----------------------------------------------------------
*/

void LiveVariableInBranch::topologicalSort(vector<BasicBlock *> *blockq, Function &F) {
    BasicBlock *bb = &F.getEntryBlock();

    // unprocessed basic blocks
    queue<BasicBlock*> bblist;
    bblist.push(bb);

    while(!bblist.empty()) {
        // parent always first
        auto basicblock = bblist.front();
        bblist.pop();

        // multiple predecessors or we have a loop
        if (std::find(blockq->begin(), blockq->end(), basicblock) != blockq->end()) continue;
        else blockq->push_back(basicblock);

        // put the successors of the currect basic blocks into the list
        auto *last = basicblock->getTerminator();
        for (unsigned i = 0; i < last->getNumSuccessors(); i++) {
            auto succ = last->getSuccessor(i);
            bblist.push(succ);
        }
    }
}

DenseMap<StringRef, uint>* LiveVariableInBranch::genVarIndexMap(vector<BasicBlock*> *blockq) {
    auto varIndexMap = new DenseMap<StringRef, uint>;
    uint num = 1;

    for (auto it : *blockq) {
        for (auto &inst : *it) {
            if (isa<AllocaInst>(inst)) {
                auto allocaInst = &cast<AllocaInst>(inst);
                if (!allocaInst) continue;
                if (allocaInst->hasName()) {
//                    allocaInstMap[allocaInst] = num;
                    (*varIndexMap)[allocaInst->getName()] = num;
                    num *= 2;
                }
            }
        } // inner loop
    } // outer loop
    return varIndexMap;
}

void LiveVariableInBranch::onTheFlyAnalysis(llvm::Function &F, vector<BasicBlock*> * blockq,
                                            DenseMap<BasicBlock*, uint> &blockToBV) {
    topologicalSort(blockq, F);

    // quick: var -> bit-mask mapping
    auto varIndexMap = genVarIndexMap(blockq);

    // quick: indexing the variables
    std::vector <pair<uint, StringRef>> bitmask;
    for (auto &it : *varIndexMap)
        bitmask.emplace_back(it.getSecond(), it.getFirst());
    std::sort(bitmask.begin(), bitmask.end(),
              [](const pair <uint, StringRef> lhs, const pair <uint8_t, StringRef> rhs) {
                  return lhs.first > rhs.first;
              });

    uint bitvector = 0;

    for (auto rit = blockq->rbegin(); rit != blockq->rend(); rit++) {
        // meet operation
        auto terminator = (*rit)->getTerminator();
        for (int i = 0; i < terminator->getNumSuccessors(); i++)
            bitvector |= blockToBV[terminator->getSuccessor(i)];

        // info
        StringRef name;
        errs() << "Block: " << (*rit)->getName();

        // reverse traversal the instructions, emit result on the fly
        auto instlist = &(*rit)->getInstList();
        for (auto rins = instlist->rbegin(); rins != instlist->rend(); rins++) {
            // USE: generates
            if (isa<LoadInst>(*rins)) {
                errs() << "\n";
                errs() << (*rins).getDebugLoc()->getLine() << ": ";
                if (!(*rins).getOperand(0)->hasName()) continue;
                name = (*rins).getOperand(0)->getName();
                // block result
                bitvector |= (*varIndexMap)[name];
                // block use

                int variables = bitvector;
                for (auto bit : bitmask) {
                    if (variables - bit.first >= 0) {
                        variables -= bit.first;
                        errs() << bit.second << " ";
                    }
                }
            } else if (isa<StoreInst>(*rins)) {
                if (!(*rins).getOperand(0)->hasName()) continue;
                name = (*rins).getOperand(0)->getName();
                // kills a value
                bitvector = (!bitvector) & (*varIndexMap)[name];
            } else continue; // other types, doesn't matter
        }
    }
}

void LiveVariableInBranch::workList(vector<BasicBlock *> const *blockq,
        DenseMap<BasicBlock *, uint>* blockToBV,
        DenseMap<BasicBlock *, uint>* blockToGen,
        DenseMap<BasicBlock *, uint>* blockToKill) {

    auto bbVector = *blockq;

//    int tmp = 0;
    while (!bbVector.empty()) {
//        errs() << ++tmp << " ";
        BasicBlock *bb = bbVector.back();
        bbVector.pop_back();

        uint bitvector = (*blockToBV)[bb];
        auto last = bb->getTerminator();

        // meet operation
        int num = last->getNumSuccessors();
        uint vec = 0;
        for (int i = 0; i < num; i++) {
            auto *succ = last->getSuccessor(i);
            vec |= (*blockToBV)[succ];
        }

        // Transfer function
        // (OUT - DEF) union USE
        (*blockToBV)[bb] = (vec & !(*blockToKill)[bb]) | (*blockToGen)[bb];

        for (auto pred = pred_begin(bb); pred != pred_end(bb); pred++) {
            uint x_j = (*blockToBV)[*pred];
            uint z = std::max((*blockToBV)[bb], x_j);
            if (x_j != z) {
                x_j = z;
                bbVector.push_back(*pred);
            }
        }
    }
}

LiveVariableInBranch::void LiveworkList(vector<BasicBlock *> const *blockq)) {
    map<Instruction*, uint> insToBit;

    for (auto itb = blockq.rbegin(); itb != blockq.rend(); itb++)
        for (auto ins : *itb) insToBit[*ins] = 0;

}

bool LiveVariableInBranch::runOnFunction(llvm::Function &F) {
    // basicblock list
    auto blockq = new vector<BasicBlock*>;
    topologicalSort(blockq, F);

    // quick: var -> bit-mask mapping
    auto varIndexMap = genVarIndexMap(blockq);

    // quick: indexing the variables
    std::vector<pair<uint , StringRef>> bitmask;
    for (auto &it : *varIndexMap)
        bitmask.push_back(make_pair(it.getSecond(), it.getFirst()));
    std::sort(bitmask.begin(), bitmask.end(),
            [](const pair<uint , StringRef> lhs, const pair<uint, StringRef> rhs) {
        return lhs.first > rhs.first;
    });

    // initialize bitvector for all basicblocks
    // 0 => 0000; bit-wise representation
    // x, y, z => 0111
    DenseMap<BasicBlock*, uint> blockToBV, blockToKill, blockToGen;
    for (auto it = blockq->begin(); it != blockq->end(); it ++) {
        blockToBV[*it] = 0;
        blockToKill[*it] = 0;
        blockToGen[*it] = 0;
    }

    /**
     * We do analysis for lines on the fly
     * => only save the result for a block
     */
    uint bitvector = 0;

    for (auto rit = blockq->rbegin(); rit != blockq->rend(); rit++) {
        // meet operation
        auto terminator = (*rit)->getTerminator();
        for (int i = 0; i < terminator->getNumSuccessors(); i++)
            bitvector |= blockToBV[terminator->getSuccessor(i)];

        // info
        StringRef name;
//        errs() << "Block: " << (*rit)->getName();

        // reverse traversal the instructions, emit result on the fly
        auto instlist = &(*rit)->getInstList();
        for (auto rins = instlist->rbegin(); rins != instlist->rend(); rins++) {
            // USE: generates
            if (isa<LoadInst>(*rins)) {
                if (!(*rins).getOperand(0)->hasName()) continue;
                name = (*rins).getOperand(0)->getName();
                // block use
                blockToGen[*rit] |= (*varIndexMap)[name];
            } else if (isa<StoreInst>(*rins)) {
                if (!(*rins).getOperand(0)->hasName()) continue;
                name = (*rins).getOperand(0)->getName();
                // block def
                blockToKill[*rit] |= (*varIndexMap)[name];
            }
            else continue; // other types, doesn't matter
        }
        bitvector = 0;
//        errs() << "\n\n";
    }

    workList(blockq, &blockToBV, &blockToGen, &blockToKill);

    // to print out
    std::error_code EC;
    auto *filestream = new raw_fd_ostream("H5_testcase1_Yushan_20562614.txt",
                                          EC);

//    for (auto rit = blockq->rbegin(); rit != blockq->rend(); rit++) {
//
////        *filestream << "Basicblock: " << (*rit)->getName() << "\n";
//        // meet operation
//        auto terminator = (*rit)->getTerminator();
//        for (int i = 0; i < terminator->getNumSuccessors(); i++)
//            bitvector |= blockToBV[terminator->getSuccessor(i)];
//
//        // info
//        StringRef name;
//
//        // reverse traversal the instructions, emit result on the fly
//        auto instlist = &(*rit)->getInstList();
//        for (auto rins = instlist->rbegin(); rins != instlist->rend(); rins++) {
//            // USE: generates
//            if ((*rins).hasMetadata()) {
//                *filestream << (*rins).getDebugLoc()->getLine() << ": ";
//            }
//
//            if (isa<LoadInst>(*rins)) {
//                if (!(*rins).getOperand(0)->hasName()) continue;
//                name = (*rins).getOperand(0)->getName();
//                // block result
//                bitvector |= (*varIndexMap)[name];
//                // block use
//
//            } else if (isa<StoreInst>(*rins)) {
//                if (!(*rins).getOperand(0)->hasName()) continue;
//                name = (*rins).getOperand(0)->getName();
//                // kills a value
//                bitvector = (!bitvector) & (*varIndexMap)[name];
//            }
//            int variables = bitvector;
//            for (auto bit : bitmask) {
//                if (variables - bit.first >= 0) {
//                    variables -= bit.first;
//                    *filestream << bit.second << " ";
//                }
//            }
//            *filestream << "\n";
//        }
//        bitvector = 0;
//    }
//
//    filestream->flush();
//    filestream->close();

    return true;
}


static RegisterPass<LiveVariableInBranch> X("livevar", "LiveVariableInBranch Pass",
                                        true, // This pass doesn't modify the CFG => true
                                        false // This pass is not a pure analysis pass => false
);

static llvm::RegisterStandardPasses Y(
        llvm::PassManagerBuilder::EP_EarlyAsPossible,
        [](const llvm::PassManagerBuilder &Builder,
           llvm::legacy::PassManagerBase &PM) { PM.add(new LiveVariableInBranch()); });


//            } else if (isa<BinaryOperator>(*rins)) {
//                errs() << "\n";
//                errs() << (*rins).getDebugLoc()->getLine() << " " << rins->getOpcodeName();
//                auto *inst = &cast<BinaryOperator>(*rins);
//                for (auto opt = inst->op_begin(); opt != inst->op_end(); opt++) {
//                    if (!opt->get()->hasName()) continue;
//                    name = opt->get()->getName();
//                    if (varIndexMap.count(name)) bitvector |= varIndexMap[name];
//                    errs() << " " << name;
//                }
//                errs() << " " << unsigned(bitvector);
//            }
