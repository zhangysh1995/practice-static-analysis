//
// Created by yzhanghw on 2020/5/10.
//

#include "InterSignAnalysis.h"
#include <queue>
#include "llvm/ADT/SmallVector.h"
#include <iostream>

/***
 * Register the pass. Modify this part only when necessary
 */

char InterSignAnalysis::ID = 0;
static RegisterPass<InterSignAnalysis> X("intersign", "Interprocedural sign analysis",
                                         true,
                                         true);
static llvm::RegisterStandardPasses Y(
        llvm::PassManagerBuilder::EP_EarlyAsPossible,
        [](const llvm::PassManagerBuilder &Builder,
           llvm::legacy::PassManagerBase &PM) { PM.add(new InterSignAnalysis()); });


void InterSignAnalysis::exractVars(Function *func) {
    for (auto& bb : *func) {
        for (auto& inst: bb) {
            // variable declaration
            if (isa<AllocaInst>(inst)) {
                auto allocaInst = &cast<AllocaInst>(inst);
                // save the variable with an ID
                varMap[allocaInst->getName()] = InterSignAnalysis::varCount++;
            }
        }
    }
}

void InterSignAnalysis::topologicalSort(Function* F, std::vector<BasicBlock*>* bbVector) {
    // implement an algorithm to sort the basicblocks
    // parent should always present before the child
    BasicBlock *bb = &F->getEntryBlock();

    // unprocessed basic blocks
    std::queue<BasicBlock*> bblist;
    bblist.push(bb);

    while(!bblist.empty()) {
        // parent always first
        auto basicblock = bblist.front();
        bblist.pop();

        // multiple predecessors or we have a loop
        if (std::find(bbVector->begin(), bbVector->end(), basicblock) != bbVector->end()) continue;
        else bbVector->push_back(basicblock);

        // put the successors of the currect basic blocks into the list
        auto *last = basicblock->getTerminator();
        for (unsigned i = 0; i < last->getNumSuccessors(); i++) {
            auto succ = last->getSuccessor(i);
            bblist.push(succ);
        }
    }
}

void InterSignAnalysis::updateMap(DenseMap<StringRef, StringRef>* states, Value* operand, StringRef &varName) {
    if (auto value = dyn_cast<ConstantInt>(operand)) {
        auto myValue = value->getSExtValue();

        auto res = myValue + 1;
        if (res == 0) {
            (*states)[varName] = "0";
            errs() << "0";
        }
        else if (res > 0) {
            (*states)[varName] = "+";
            errs() << "+";
        }
        else {
            (*states)[varName] = "-";
            errs() << "-";
        }
        errs() << "\n";
    }
}

DenseMap<StringRef, StringRef>* InterSignAnalysis::intraBBAnalyze(BasicBlock* bb){
    // outer map: basicblock -> states of vars
    // innter map: varname -> state string (e.g "0" in {"0", "+", "-", "⊤", "⊥"})
    auto states = new DenseMap<StringRef, StringRef>();

    auto context = new std::vector<CallInst*>;

    for (auto& instruction : *bb) {
        Value* operand;
        StringRef varName;

        if (auto callInst = dyn_cast<CallInst>(&instruction)) {
            auto callee = callInst->getCalledFunction();
            // on the fly
            // context sensitive analysis
            auto argNo = callInst->getNumArgOperands();

            // only handle `inc`
            if (argNo != 1) continue;
            context->push_back(callInst);

//            errs() << argNo << "\n";

            operand = callInst->getArgOperand(0);
//            errs() << "type: " << operand->getType()->isIntegerTy() << "\n";
//            errs() << "function call" << "\n";

            auto store = callInst->getNextNonDebugInstruction();
            varName = store->getOperand(1)->getName();

            updateMap(states, operand, varName);

            // your code goes here
            // analyze this function
        } else if (auto storeInst = dyn_cast<CallInst>(&instruction)) {
            errs() << "def a variable" << "\n";
            // your code goes here
            // update the state of the vars
        } else if (auto loadInst = dyn_cast<LoadInst>(&instruction)) {
//            loadInst->dump();
//            errs() << *loadInst << '\n';
//            errs() << loadInst->getValueName() << "\n";
//            errs() << loadInst->getName() << "\n";
//            errs() << loadInst->getNumOperands();
//            errs() << loadInst->getOperand(0);

        }

        // your code goes here
        // other instructions??
    }

    return states;
}

/***
 * Analyze a function for signs
 * @param func
 */
// you will need to modify the signature of this function
void InterSignAnalysis::analyzeFunction(Function* func) {
    // extract the variables
    // we save the `id -> varname`
    // you could assume we only have declarations in `main`
    exractVars(func);
    errs() << "extract variables \n";

    // sort the basicblocks in `main`
    auto mainBBVector = new std::vector<BasicBlock*>();
    topologicalSort(func, mainBBVector); // no implementation

    // analyze each basicblock in `main`
    for (auto& bb : *func) {
        // your code goes here
        errs() << bb.getName() << '\n';
        intraBBAnalyze(&bb);
    }
}

bool InterSignAnalysis::runOnModule(Module& M){
    auto cg = &getAnalysis<CallGraphWrapperPass>().getCallGraph();

    // filter the LLVM functions
    std::vector<Function*> functions;

    for (auto& cgn : *cg) {
        auto *func = const_cast<Function *>(cgn.first);
        // functions cannot be null
        if (func == nullptr) continue;
        // remove llvm-inserted functions
        if (func->isIntrinsic()) continue;

        if (func->getName().str() == "inc")
            functions.push_back(func);

        errs() << "function iterator \n";
    }

    // assume the entrance is `main`
    auto name = StringRef("main");
    auto mainFun = M.getFunction(name);
    errs() << "find function " << mainFun->getName() << "\n";

    analyzeFunction(mainFun);

    // your code goes here
    // what should be saved for the results?

    return true;
}

bool InterSignAnalysis::doFinalization(Module &M) {
    // report the analysis result before exit
    reportResult();
    errs() << "Rerporting result: (I'm not implemented)\n";

    return true;
}


void InterSignAnalysis::getAnalysisUsage(AnalysisUsage &AU) const {
    // need the analysis results
    // your code goes here
    AU.addRequired<CallGraphWrapperPass>();
    // not changing the results
    AU.setPreservesAll();
}


void InterSignAnalysis::reportResult() {
    // your code goes here
}
