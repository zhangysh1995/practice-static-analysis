#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/FormatVariadic.h"
#include "llvm/Support/Process.h"
#include <string>


using namespace llvm;
using std::string;

namespace {
  struct Farg : public ModulePass {
    static char ID;
    Farg() : ModulePass(ID) {}

    // floating-points in LLVM has type-id of 3
      bool runOnModule(Module &M)  override{
        string s;

        for (auto &it: M) {
            if (it.isIntrinsic()) continue;

            int num = 0;
            int fnum = 0;

            for (auto &arg: it.args()) {
                if (arg.getType()->getTypeID() == Type::TypeID::FloatTyID) fnum ++;
                num ++;
            }

            s += formatv("{0}\t{1}\t{2}\n", it.getName(), num, fnum);
        }

        // retrieve the path from environment variable
        auto pathvar = sys::Process::GetEnv("FOLDER");
        auto prog = sys::Process::GetEnv("PROG");

        if (!pathvar || !prog) {
            errs() << "\n ====== You need to specify "
                      "the output file with: export FOLDER=xxx && export PROG=xxx =====\n\n";
            std::exit(-1);
        }

        // output the content to file
        std::error_code EC;
        auto *filestream = new raw_fd_ostream(pathvar.getValue() +
                "H1_" + prog.getValue() + "_Yushan_20562614.txt",
                EC);
        *filestream << s;
        filestream->flush();
        filestream->close();
      return true;
    }
  };
}

char Farg::ID = 0;

static RegisterPass<Farg> X("farg", "Count args and the number of floating-point args",
                            false /* Only looks at CFG */,
                            false /* Analysis Pass */);

static llvm::RegisterStandardPasses Y(
        llvm::PassManagerBuilder::EP_EarlyAsPossible,
        [](const llvm::PassManagerBuilder &Builder,
           llvm::legacy::PassManagerBase &PM) { PM.add(new Farg()); });