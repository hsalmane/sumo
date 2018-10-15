/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2007-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    MSDevice.h
/// @author  Michael Behrisch
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @date    Tue, 04 Dec 2007
/// @version $Id$
///
// Abstract in-vehicle device
/****************************************************************************/
#ifndef MSDevice_h
#define MSDevice_h


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <microsim/MSMoveReminder.h>
#include <utils/common/Named.h>
#include <utils/common/UtilExceptions.h>


// ===========================================================================
// class declarations
// ===========================================================================
class OutputDevice;
class SUMOVehicle;
class MSTransportable;
class OptionsCont;
class SUMOSAXAttributes;
class MSVehicleDevice;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class MSDevice
 * @brief Abstract in-vehicle / in-person device
 *
 * The MSDevice-interface brings the following interfaces to a vehicle /person that
 *  may be overwritten by real devices:
 * @arg Building and retrieval of a device id
 */
class MSDevice : public Named {
public:
    /** @brief Inserts options for building devices
     * @param[filled] oc The options container to add the options to
     */
    static void insertOptions(OptionsCont& oc);

    /** @brief check device-specific options
     * @param[filled] oc The options container with the user-defined options
     */
    static bool checkOptions(OptionsCont& oc);


    /** @brief Build devices for the given vehicle, if needed
     *
     * @param[in] v The vehicle for which a device may be built
     * @param[filled] into The vector to store the built device in
     */
    static void buildVehicleDevices(SUMOVehicle& v, std::vector<MSVehicleDevice*>& into);

    static std::mt19937* getEquipmentRNG() {
        return &myEquipmentRNG;
    }

    /// @brief return the name for this type of device
    virtual const std::string deviceName() const = 0;

    /** @brief Determines whether a transportable should get a certain device
     **/
    static bool equippedByParameter(const MSTransportable* t, const std::string& deviceName, bool outputOptionSet);

    /// @brief perform cleanup for all devices
    static void cleanupAll();

public:
    /** @brief Constructor
     *
     * @param[in] holder The vehicle that holds this device
     * @param[in] id The ID of the device
     */
    MSDevice(const std::string& id) : Named(id) {
    }


    /// @brief Destructor
    virtual ~MSDevice() { }


    /** @brief Called on writing tripinfo output
     *
     * The device may write some statistics into the tripinfo output. It
     *  is assumed that the written information is a valid xml-snipplet, which
     *  will be embedded within the vehicle's information.
     *
     * The device should use the openTag / closeTag methods of the OutputDevice
     *  for correct indentation.
     *
     * @exception IOError not yet implemented
     */
    virtual void generateOutput() const {
    }

    /** @brief Saves the state of the device
     *
     * The default implementation writes a warning and does nothing.
     * @param[in] out The OutputDevice to write the information into
     */
    virtual void saveState(OutputDevice& out) const;


    /** @brief Loads the state of the device from the given description
     *
     * The default implementation does nothing.
     * @param[in] attrs XML attributes describing the current state
     */
    virtual void loadState(const SUMOSAXAttributes& attrs);

    /// @brief try to retrieve the given parameter from this device. Throw exception for unsupported key
    virtual std::string getParameter(const std::string& key) const {
        throw InvalidArgument("Parameter '" + key + "' is not supported for device of type '" + deviceName() + "'");
    }

    /// @brief try to set the given parameter for this device. Throw exception for unsupported key
    virtual void setParameter(const std::string& key, const std::string& value) {
        UNUSED_PARAMETER(value);
        throw InvalidArgument("Setting parameter '" + key + "' is not supported for device of type '" + deviceName() + "'");
    }

protected:
    /// @name Helper methods for device assignment
    /// @{

    /** @brief Adds common command options that allow to assign devices to vehicles
     *
     * @param[in] deviceName The name of the device type
     * @param[in] optionsTopic The options topic into which the options shall be added
     * @param[filled] oc The options container to add the options to
     */
    static void insertDefaultAssignmentOptions(const std::string& deviceName, const std::string& optionsTopic, OptionsCont& oc);


    /** @brief Determines whether a vehicle should get a certain device
     *
     * @param[in] oc The options container to get the information about assignment from
     * @param[in] deviceName The name of the device type
     * @param[in] v The vehicle to determine whether it shall be equipped or not
     */
    static bool equippedByDefaultAssignmentOptions(const OptionsCont& oc, const std::string& deviceName, SUMOVehicle& v, bool outputOptionSet);
    /// @}


    /// @name Helper methods for parsing parameters
    /// @{
    static std::string getStringParam(const SUMOVehicle& v, const OptionsCont& oc, std::string paramName, std::string deflt, bool required);
    static double getFloatParam(const SUMOVehicle& v, const OptionsCont& oc, std::string paramName, double deflt, bool required);
    static bool getBoolParam(const SUMOVehicle& v, const OptionsCont& oc, std::string paramName, bool deflt, bool required);
    /// @}

private:
    /// @brief vehicles which explicitly carry a device, sorted by device, first
    static std::map<std::string, std::set<std::string> > myExplicitIDs;

    /// @brief A random number generator used to choose from vtype/route distributions and computing the speed factors
    static std::mt19937 myEquipmentRNG;


private:
    /// @brief Invalidated copy constructor.
    MSDevice(const MSDevice&);

    /// @brief Invalidated assignment operator.
    MSDevice& operator=(const MSDevice&);

};


#endif

/****************************************************************************/
