/*
 * Authors: T. Heim <timon.heim@cern.ch>,
 * Date: 2014-Oct-16
 */

#include "Fei4ParameterLoop.h"

Fei4ParameterLoop::Fei4ParameterLoop() {

}

void Fei4ParameterLoop::init() {
    m_done = false;
    cur = min;
    this->writePar();
}

void Fei4ParameterLoop::end() {
   
}

void Fei4ParameterLoop::execPart1() {
    if (verbose)
        std::cout << __PRETTY_FUNCTION__ << " : Parameter Loop at -> " << cur << std::endl;
}

void Fei4ParameterLoop::execPart2() {
    cur += step;
    if (cur > max) m_done = true;
    this->writePar();
}

void Fei4ParameterLoop::writePar() {
    switch (par) {
        case VCAL_PAR:
            g_fe->writeRegister(&Fei4::PlsrDAC, cur);
            break;
        default:
            break;
    }
}

void Fei4ParameterLoop::setRange(unsigned arg_min, unsigned arg_max, unsigned arg_step) {
    min = arg_min;
    max = arg_max;
    step = arg_step;
}
