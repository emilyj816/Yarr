#ifndef RD53APARAMETERLOOP_H
#define RD53APARAMETERLOOP_H
// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: Parameter Loop for RD53A
// # Date: 03/2018
// ################################

#include <iostream>

#include "LoopActionBase.h"
#include "Rd53a.h"

class Rd53aParameterLoop : public LoopActionBase {
    public:
        Rd53aParameterLoop();
        Rd53aParameterLoop(Rd53aReg Rd53aGlobalCfg::*ref);

        void writeConfig(json &j);
        void loadConfig(json &j);

    private:
        Rd53aReg Rd53aGlobalCfg::*parPtr;
        std::string parName;
        std::vector<std::string> parNameMultiple;
        void writePar(Rd53aReg Rd53aGlobalCfg::*p, uint32_t m);
        std::vector<unsigned> m_cur;
        signed add = 0;
        bool logDiff = false;
        int m_curToLog = 0;
        
        void init();
        void end();
        void execPart1();
        void execPart2();
};

#endif
