#ifndef _OS_OSCOLLECTION_H
#define _OS_OSCOLLECTION_H

#include "OSObject.h"

class OSDictionary;

class OSCollection : public OSObject
{
    friend class OSCollectionIterator;

    OSDeclareAbstractStructors(OSCollection);

    struct ExpansionData { };
    
protected:
    unsigned int updateStamp;

private:
    unsigned int fOptions;

protected:
    virtual unsigned int iteratorSize() const = 0;
    virtual bool initIterator(void * iterationContext) const = 0;
    virtual bool getNextObjectForIterator(
        void      * iterationContext,
        OSObject ** nextObject) const = 0;

    virtual bool init();

public:
    typedef enum {
        kImmutable  = 0x00000001,
        kMASK       = (unsigned) -1
    } _OSCollectionFlags;

    void haveUpdated();
    virtual unsigned int getCount() const = 0;
    virtual unsigned int getCapacity() const = 0;
    virtual unsigned int getCapacityIncrement() const = 0;
    virtual unsigned int setCapacityIncrement(unsigned increment) = 0;
    virtual unsigned int ensureCapacity(unsigned int newCapacity) = 0;
    virtual void flushCollection() = 0;
    virtual unsigned setOptions(
        unsigned   options,
        unsigned   mask,
        void     * context = 0);
	
    OSMetaClassDeclareReservedUsed(OSCollection, 0);
    virtual OSCollection *copyCollection(OSDictionary * cycleDict = 0);
    OSMetaClassDeclareReservedUsed(OSCollection, 1);


    OSMetaClassDeclareReservedUnused(OSCollection, 2);
    OSMetaClassDeclareReservedUnused(OSCollection, 3);
    OSMetaClassDeclareReservedUnused(OSCollection, 4);
    OSMetaClassDeclareReservedUnused(OSCollection, 5);
    OSMetaClassDeclareReservedUnused(OSCollection, 6);
    OSMetaClassDeclareReservedUnused(OSCollection, 7);
};

#endif /* !_OS_OSCOLLECTION_H */
