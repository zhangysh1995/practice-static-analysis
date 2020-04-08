#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

namespace {
  struct farg : public ModulePass {
    static char ID;
    farg() : ModulePass(ID) {}


    virtual bool runOnModule(Module &M)  {
        for (auto &it: M) {
//            if (it == NULL) {
//                dbgs() << "Error: function is null";
//            }
//            if (it.isIntrinsic()) continue;
            errs() << "name: " << it.getName() << "\n";
        }

      return true;
    }

    virtual bool doFinalization(Module &M) {
        report();
    }

  private:
      static void  report() {
        errs() << "I report the result;";
    }

  };
}

char farg::ID = 0;

static RegisterPass<farg> X("farg", "Count args and the number of floating-point args",
                            false /* Only looks at CFG */,
                            false /* Analysis Pass */);

static llvm::RegisterStandardPasses Y(
        llvm::PassManagerBuilder::EP_EarlyAsPossible,
        [](const llvm::PassManagerBuilder &Builder,
           llvm::legacy::PassManagerBase &PM) { PM.add(new farg()); });

