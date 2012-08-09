#ifndef _OS_OSCOLLECTIONITERATOR_H
#define _OS_OSCOLLECTIONITERATOR_H

#include "OSIterator.h"

class OSCollection;

class OSCollectionIterator : public OSIterator
{
    OSDeclareDefaultStructors(OSCollectionIterator)

protected:
    const OSCollection * collection;
    void               * collIterator;
    unsigned int         initialUpdateStamp;
    bool                 valid;

public:
    static OSCollectionIterator * withCollection(const OSCollection * inColl);

    virtual bool initWithCollection(const OSCollection * inColl);
    virtual void free();
    virtual void reset();
    virtual bool isValid();
    virtual OSObject * getNextObject();
};

#endif /* !_OS_OSCOLLECTIONITERATOR_H */
