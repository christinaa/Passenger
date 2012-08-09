/*
 * Copyright (c) 2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/* IOArray.h created by rsulack on Thu 11-Sep-1997 */
/* IOArray.h converted to C++ by gvdl on Fri 1998-10-30 */

#ifndef _OS_OSARRAY_H
#define _OS_OSARRAY_H

#include "OSCollection.h"

class OSSerialize;

class OSArray : public OSCollection
{
    friend class OSSet;
	
    OSDeclareDefaultStructors(OSArray)
	
protected:
    const OSMetaClassBase ** array;
    unsigned int             count;
    unsigned int             capacity;
    unsigned int             capacityIncrement;
	
    struct ExpansionData { };
    
	/* Reserved for future use. (Internal use only) */
    ExpansionData          * reserved;
	
	/* OSCollectionIterator interfaces. */
    virtual unsigned int iteratorSize() const;
    virtual bool initIterator(void * iterator) const;
    virtual bool getNextObjectForIterator(void * iterator, OSObject ** ret) const;
	
public:
    static OSArray * withCapacity(unsigned int capacity);
    static OSArray * withObjects(
								 const OSObject * objects[],
								 unsigned int     count,
								 unsigned int     capacity = 0);
    static OSArray * withArray(
							   const OSArray * array,
							   unsigned int    capacity = 0);
    virtual bool initWithCapacity(unsigned int capacity);
    virtual bool initWithObjects(
								 const OSObject * objects[],
								 unsigned int     count,
								 unsigned int     capacity = 0);
    virtual bool initWithArray(
							   const OSArray * anArray,
							   unsigned int    capacity = 0);
    virtual void free();
    virtual unsigned int getCount() const;
    virtual unsigned int getCapacity() const;
    virtual unsigned int getCapacityIncrement() const;
    virtual unsigned int setCapacityIncrement(unsigned increment);
    virtual unsigned int ensureCapacity(unsigned int newCapacity);
    virtual void flushCollection();
    virtual bool setObject(const OSMetaClassBase * anObject);
    virtual bool setObject(
						   unsigned int            index,
						   const OSMetaClassBase * anObject);
    virtual bool merge(const OSArray * otherArray);
    virtual void replaceObject(
							   unsigned int            index,
							   const OSMetaClassBase * anObject);
    virtual void removeObject(unsigned int index);
    virtual bool isEqualTo(const OSArray * anArray) const;
    virtual bool isEqualTo(const OSMetaClassBase * anObject) const;
    virtual OSObject * getObject(unsigned int index) const;
    virtual OSObject * getLastObject() const;
    virtual unsigned int getNextIndexOfObject(
											  const OSMetaClassBase * anObject,
											  unsigned int            index) const;
	
    virtual bool serialize(OSSerialize * serializer) const;
    virtual unsigned setOptions(
								unsigned   options,
								unsigned   mask,
								void     * context = 0);

    OSCollection * copyCollection(OSDictionary * cycleDict = 0);
	
    OSMetaClassDeclareReservedUnused(OSArray, 0);
    OSMetaClassDeclareReservedUnused(OSArray, 1);
    OSMetaClassDeclareReservedUnused(OSArray, 2);
    OSMetaClassDeclareReservedUnused(OSArray, 3);
    OSMetaClassDeclareReservedUnused(OSArray, 4);
    OSMetaClassDeclareReservedUnused(OSArray, 5);
    OSMetaClassDeclareReservedUnused(OSArray, 6);
    OSMetaClassDeclareReservedUnused(OSArray, 7);
};

#endif /* !_OS_OSARRAY_H */
