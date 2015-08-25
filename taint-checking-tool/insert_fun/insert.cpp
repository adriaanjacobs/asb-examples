#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ADT/ArrayRef.h"

#include "llvm/IR/IRBuilder.h"



using namespace llvm;

namespace{
    struct bishe_insert : public ModulePass{
        static char ID;  
        Function *hook;

        bishe_insert() : ModulePass(ID) {}

        virtual bool runOnModule(Module &M)
        {
            Constant *hookFunc;
/* Here, we define the hook function that will be inserted after each cast instruction,
FunctionType:: is the return type of print. Every type declared after this one is an argument type*/
            hookFunc = M.getOrInsertFunction("print", FunctionType::getVoidTy(M.getContext()),Type::getInt64Ty(M.getContext()), (Type*)0); 
              
            hook= cast<Function>(hookFunc);
            for(Module::iterator F = M.begin(), E = M.end(); F!= E; ++F)/*Going though each module*/
            {
                for(Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB)/*going through each function*/
                {
		    bishe_insert::runOnBasicBlock(BB);
                }
            }
            return false;
        }
        
	virtual bool runOnBasicBlock(Function::iterator &BB)
        {
            for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI)/*going through each instruction*/          
            {
                    if(CastInst *CI = dyn_cast<CastInst>(BI) ) /*If BI is a CastInst, cast it and return TRUE, otherwise return false*/
                    {
                       if ( CI->getSrcTy()->isPointerTy() && CI->getDestTy()->isIntegerTy() )/*check the source and destinationt types of the instruction*/
			   {
			      /*Value *operand = CI->getOperand(0);
			      operand->printAsOperand(errs());
			      errs() << "\n";*/
			      CI->getType()->print(errs());
/*Important, the return value if an instruction is the instruction itself. Here, we use CI (the variable containing the cast ptr) as an argument in our print() function */
			      errs() << "\n";
			      ArrayRef<Value *> args = ArrayRef<Value *>(CI);
/*To create an instruction, the arguments have to be passed in an ArrayRef. If there are no arguments, you can create an empty ArrayRef or use CallInst::Create(hook,"") */
			      Instruction *newInst = CallInst::Create(hook,args, "");
                              BB->getInstList().insertAfter((Instruction*)CI, newInst);                      
                    	   }
		    }
                  
            }
            return true;
        }
    };
}
char bishe_insert::ID = 0;
static RegisterPass<bishe_insert> X("bishe_insert", "test function exist", false, false);
