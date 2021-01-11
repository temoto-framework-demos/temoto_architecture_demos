
#ifndef ta_initialize_robot_MACROS_H
#define ta_initialize_robot_MACROS_H

#define GET_PARAMETER(name, type) getUmrfPtr()->getInputParameters().getParameterData<type>(name)
#define SET_PARAMETER(name, type, value) getUmrfPtr()->getOutputParametersNc().setParameter(name, type, boost::any(value))

#endif
