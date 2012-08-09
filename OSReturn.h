//
//  OSReturn.h
//  Passenger
//
//  Created by Kristina on 12-07-31.
//
//

#ifndef Passenger_OSReturn_h
#define Passenger_OSReturn_h

#include "runtime.h"

typedef kern_return_t OSReturn;

#ifndef sys_libkern
#define sys_libkern                   err_system(0x37)
#endif /* sys_libkern */

#define sub_libkern_common            err_sub(0)
#define sub_libkern_metaclass         err_sub(1)
#define sub_libkern_reserved          err_sub(-1)

#define libkern_common_err(return)    (sys_libkern|sub_libkern_common|(return))
#define libkern_metaclass_err(return) (sys_libkern|sub_libkern_metaclass|(return))

#endif
