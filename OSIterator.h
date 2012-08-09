#ifndef _OS_OSITERATOR_H
#define _OS_OSITERATOR_H

#include "OSObject.h"

class OSIterator : public OSObject
{
    OSDeclareAbstractStructors(OSIterator)

public:
    virtual void reset() = 0;
    virtual bool isValid() = 0;
    virtual OSObject *getNextObject() = 0;

    OSMetaClassDeclareReservedUnused(OSIterator, 0);
    OSMetaClassDeclareReservedUnused(OSIterator, 1);
    OSMetaClassDeclareReservedUnused(OSIterator, 2);
    OSMetaClassDeclareReservedUnused(OSIterator, 3);
};

#endif /* ! _OS_OSITERATOR_H */
