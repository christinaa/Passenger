//
//  OSObject.h
//  Passenger
//
//  Created by Kristina on 12-07-31.
//
//

#ifndef Passenger_OSObject_h
#define Passenger_OSObject_h

#include "OSMetaClass.h"

class OSObject : public OSMetaClassBase
{
    OSDeclareAbstractStructors(OSObject)
private:
    mutable int retainCount;
	
protected:
	virtual void release(int freeWhen) const;
	virtual void taggedRelease(const void * tag, const int freeWhen) const;
	virtual bool init();
	virtual void free();
	static void operator delete(void * mem, size_t size);
	
public:
	static void * operator new(size_t size);
	virtual int getRetainCount() const;
	virtual void retain() const;
	virtual void release() const;
	virtual void taggedRetain(const void * tag = 0) const;
	virtual void taggedRelease(const void * tag = 0) const;
	
	virtual bool serialize(OSSerialize * serializer) const;
	
    OSMetaClassDeclareReservedUnused(OSObject,  0);
    OSMetaClassDeclareReservedUnused(OSObject,  1);
    OSMetaClassDeclareReservedUnused(OSObject,  2);
    OSMetaClassDeclareReservedUnused(OSObject,  3);
    OSMetaClassDeclareReservedUnused(OSObject,  4);
    OSMetaClassDeclareReservedUnused(OSObject,  5);
    OSMetaClassDeclareReservedUnused(OSObject,  6);
    OSMetaClassDeclareReservedUnused(OSObject,  7);
    OSMetaClassDeclareReservedUnused(OSObject,  8);
    OSMetaClassDeclareReservedUnused(OSObject,  9);
    OSMetaClassDeclareReservedUnused(OSObject, 10);
    OSMetaClassDeclareReservedUnused(OSObject, 11);
    OSMetaClassDeclareReservedUnused(OSObject, 12);
    OSMetaClassDeclareReservedUnused(OSObject, 13);
    OSMetaClassDeclareReservedUnused(OSObject, 14);
    OSMetaClassDeclareReservedUnused(OSObject, 15);
};

#endif
