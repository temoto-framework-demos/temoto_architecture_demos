
#ifndef ta_xarm7_manipulate_object_MACROS_H
#define ta_xarm7_manipulate_object_MACROS_H

#define GET_PARAMETER(name, type) getUmrfPtr()->getInputParameters().getParameterData<type>(name)
#define SET_PARAMETER(name, type, value) getUmrfPtr()->getOutputParametersNc().setParameter(name, type, boost::any(value))

#endif
