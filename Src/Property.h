#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"

bool operator== (const API_Property& i_this, const API_Property& i_other);
inline bool operator!= (const API_Property& i_this, const API_Property& i_other) { return !operator==(i_this, i_other); };

#endif // !PROPERTY_HPP

