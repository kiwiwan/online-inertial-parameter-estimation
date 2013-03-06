/*
 * Copyright (C) 2007-2011 RobotCub Consortium, European Commission FP6 Project IST-004370
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

#ifndef LM_PORTABLE_TEMPLATE__
#define LM_PORTABLE_TEMPLATE__

#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>

#include <yarp/os/Portable.h>
#include <yarp/os/Bottle.h>

#include "iCub/learningMachine/FactoryT.h"

namespace iCub {
namespace learningmachine {

/**
 * A templated portable class intended to wrap abstract base classes. This
 * template depends on an associated FactoryT for the specified type.
 *
 * \see iCub::learningmachine::MachinePortable
 * \see iCub::learningmachine::TransformerPortable
 *
 * \author Arjan Gijsberts
 *
 */

template<class T>
class PortableT : public yarp::os::Portable {
private:
    /**
     * A pointer to the actual wrapped base class.
     */
    T* wrapped;

public:
    /**
     * Constructor.
     *
     * @param w initial wrapped object
     */
    PortableT(T* w = (T*) 0) : wrapped(w) { }

    /**
     * Constructor.
     *
     * @param name name specifier of the wrapped object
     * @throw runtime error if no object exists with the given key
     */
    PortableT(std::string name) : wrapped((T*) 0) {
        this->setWrapped(name);
    }

    /**
     * Copy constructor.
     */
    PortableT(const PortableT<T>& other) : wrapped(other.wrapped->clone()) { }

    /**
     * Destructor.
     */
    virtual ~PortableT() {
        delete(this->wrapped);
    }

    /**
     * Assignment operator.
     */
    PortableT<T>& operator=(const PortableT<T>& other) {
        if(this == &other) return *this;

        // clone method is a safer bet than copy constructor or assignment
        // operator in our case.
        this->setWrapped(other.wrapped->clone(), true);

        return *this;
    }

    /**
     * Writes a wrapped object to a connection.
     *
     * @param connection the connection
     * @return true on success
     */
    bool write(yarp::os::ConnectionWriter& connection) {
        // return false directly if there is no machine. If not, we end up
        // up writing things on the port, after which an exception will be
        // thrown when accessing the machine.
        if(!this->hasWrapped()) {
            return false;
        }
        connection.appendInt(BOTTLE_TAG_LIST);
        connection.appendInt(2);
        yarp::os::Bottle nameBottle;
        nameBottle.addString(this->wrapped->getName().c_str());
        nameBottle.write(connection);
        this->getWrapped().write(connection);

        // for text readers
        connection.convertTextMode();
        return true;
    }

    /**
     * Reads a wrapped object from a connection.
     *
     * @param connection the connection
     * @return true on success
     */
    bool read(yarp::os::ConnectionReader& connection) {
        if(!connection.isValid()) {
            return false;
        }

        connection.convertTextMode();
        // check headers for the pair (name + actual object serialization)
        int header = connection.expectInt();
        int len = connection.expectInt();
        if(header != BOTTLE_TAG_LIST || len != 2) {
            return false;
        }

        // read machine identifier and use it to create object
        yarp::os::Bottle nameBottle;
        nameBottle.read(connection);
        std::string name = nameBottle.get(0).asString().c_str();
        this->setWrapped(name);
        if(this->wrapped == (T *) 0) {
            return false;
        }

        // call read method to construct specific object
        bool ok = this->getWrapped().read(connection);
        return ok;
    }

    /**
     * Writes a wrapped object to a file.
     *
     * @param filename the filename
     * @return true on success
     */
    bool writeToFile(std::string filename) {
        std::ofstream stream(filename.c_str());

        if(!stream.is_open()) {
            throw std::runtime_error(std::string("Could not open file '") + filename + "'");
        }

        stream << this->getWrapped().getName() << std::endl;
        stream << this->getWrapped().toString();

        stream.close();

        return true;
    }

    /**
     * Reads a wrapped object from a file.
     *
     * @param filename the filename
     * @return true on success
     */
    bool readFromFile(std::string filename) {
        std::ifstream stream(filename.c_str());

        if(!stream.is_open()) {
            throw std::runtime_error(std::string("Could not open file '") + filename + "'");
        }

        std::string name;
        stream >> name;

        this->setWrapped(name);
        std::stringstream strstr;
        strstr << stream.rdbuf();
        this->getWrapped().fromString(strstr.str());

        return true;
    }

    /**
     * Returns true iff if there is a wrapped object.
     *
     * @return true iff there is a wrapped machine
     */
    bool hasWrapped() {
        return (this->wrapped != (T*) 0);
    }

    /**
     * The accessor for the wrapped object.
     *
     * @return a reference to the wrapped object
     * @throw runtime error if no wrapped object exists
     */
    T& getWrapped() {
        if(!this->hasWrapped()) {
            throw std::runtime_error("Attempt to retrieve inexistent wrapped object!");
        }
        return *(this->wrapped);
    }

    /**
     * The mutator for the wrapped object.
     *
     * @param w a pointer to object to wrap
     * @param wipe boolean whether the previous wrapped object has to be deleted
     */
    void setWrapped(T* w, bool wipe = true) {
        if(wipe && this->hasWrapped()) {
            delete this->wrapped;
            this->wrapped = (T*) 0;
        }
        this->wrapped = w;
    }

    /**
     * The mutator for the wrapped object.
     *
     * @param name The name specifier for the wrapped object
     * @param wipe boolean whether the previous wrapped object has to be deleted
     */
    void setWrapped(std::string name, bool wipe = true) {
        if(wipe && this->hasWrapped()) {
            delete this->wrapped;
            this->wrapped = (T*) 0;
        }
        this->wrapped = FactoryT<std::string, T>::instance().create(name);
    }


};

} // learningmachine
} // iCub

#endif
