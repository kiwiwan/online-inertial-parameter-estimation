/*
 * Copyright (C) 2007-2010 RobotCub Consortium, European Commission FP6 Project IST-004370
 * author:  Arjan Gijsberts
 * email:   arjan.gijsberts@iit.it
 * website: www.robotcub.org
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

#ifndef LM_MACHINECATALOGUE__
#define LM_MACHINECATALOGUE__

#include "iCub/learningMachine/IMachineLearner.h"
#include "iCub/learningMachine/DummyLearner.h"
#include "iCub/learningMachine/RLSLearner.h"
#include "iCub/learningMachine/LinearGPRLearner.h"
#include "iCub/learningMachine/LSSVMLearner.h"
#include "iCub/learningMachine/DatasetRecorder.h"


namespace iCub {
namespace learningmachine {

void registerMachines() {
    FactoryT<std::string, IMachineLearner>::instance().registerPrototype(new DummyLearner());
    FactoryT<std::string, IMachineLearner>::instance().registerPrototype(new RLSLearner());
    FactoryT<std::string, IMachineLearner>::instance().registerPrototype(new LinearGPRLearner());
    FactoryT<std::string, IMachineLearner>::instance().registerPrototype(new LSSVMLearner());
    FactoryT<std::string, IMachineLearner>::instance().registerPrototype(new DatasetRecorder());
}

} // learningmachine
} // iCub

#endif
