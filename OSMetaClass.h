/*
 * OSMetaClass
 * Copyright (c) 2012 Christina Brooks
 *
 * ... this is so very sad ... 
 */

/* @APPLE_OSREFERENCE_LICENSE_HEADER@ */

#ifndef Passenger_OSMetaClass_h
#define Passenger_OSMetaClass_h

#include "runtime.h"
#include "OSReturn.h"

class OSSerialize;
class OSMetaClass;
class OSSymbol;
class OSDictionary;
class OSObject;
class OSString;

#define APPLE_KEXT_PAD_METHOD  static
#define APPLE_KEXT_PAD_IMPL(index)

#define OSTypeAlloc(type)   ((type *) ((type::metaClass)->alloc()))

#define OSTypeID(type)   (type::metaClass)

#define OSTypeIDInst(typeinst)   ((typeinst)->getMetaClass())

#define OSDynamicCast(type, inst)   \
((type *) OSMetaClassBase::safeMetaCast((inst), OSTypeID(type)))

#define OSCheckTypeInst(typeinst, inst) \
OSMetaClassBase::checkTypeInst(inst, typeinst)

#define OSSafeRelease(inst)       do { if (inst) (inst)->release(); } while (0)

#define OSSafeReleaseNULL(inst)   do { if (inst) (inst)->release(); (inst) = NULL; } while (0)

class OSMetaClassBase
{
public:
	OSMetaClassBase();
    virtual ~OSMetaClassBase();
private:
    void operator =(OSMetaClassBase &src);
    OSMetaClassBase(OSMetaClassBase &src);
public:
	virtual void release(int freeWhen) const = 0;
	virtual int getRetainCount() const = 0;
	virtual void retain() const = 0;
    virtual void release() const = 0;
	virtual bool serialize(OSSerialize * serializer) const = 0;
    virtual const OSMetaClass * getMetaClass() const = 0;
	virtual bool isEqualTo(const OSMetaClassBase * anObject) const;
	OSMetaClassBase * metaCast(const OSMetaClass * toMeta) const;
	OSMetaClassBase * metaCast(const OSSymbol * toMeta) const;
	OSMetaClassBase * metaCast(const OSString * toMeta) const;
	OSMetaClassBase * metaCast(const char * toMeta) const;
    static OSMetaClassBase * safeMetaCast(
										  const OSMetaClassBase * anObject,
										  const OSMetaClass     * toMeta);
	static bool checkTypeInst(
							  const OSMetaClassBase * inst,
							  const OSMetaClassBase * typeinst);
	static void initialize(void);
	
	virtual void taggedRetain(const void * tag = 0) const = 0;
	virtual void taggedRelease(const void * tag = 0) const = 0;
	
	virtual void _RESERVEDOSMetaClassBase2();
private:
    // Virtual Padding
    virtual void _RESERVEDOSMetaClassBase3();
    virtual void _RESERVEDOSMetaClassBase4();
    virtual void _RESERVEDOSMetaClassBase5();
    virtual void _RESERVEDOSMetaClassBase6();
    virtual void _RESERVEDOSMetaClassBase7();
};

class OSMetaClass : private OSMetaClassBase
{
private:
	static void * operator new(size_t size);
    struct ExpansionData { };
    ExpansionData *reserved;
    const OSMetaClass *superClassLink;
    const OSSymbol *className;
    unsigned int classSize;
    mutable unsigned int instanceCount;
    OSMetaClass();
    static void logError(OSReturn result);
public:
    static const OSMetaClass * getMetaClassWithName(const OSSymbol * name);
protected:
    virtual void retain() const;
    virtual void release() const;
    virtual void release(int freeWhen) const;
    virtual void taggedRetain(const void * tag = 0) const;
    virtual void taggedRelease(const void * tag = 0) const;
    virtual void taggedRelease(
							   const void * tag,
							   const int    freeWhen) const;

    virtual int getRetainCount() const;
    virtual const OSMetaClass * getMetaClass() const;
    OSMetaClass(const char * className,
				const OSMetaClass  * superclass,
				unsigned int         classSize);
    virtual ~OSMetaClass();
    void operator delete(void *, size_t) { };
public:
    static const OSMetaClass * const metaClass;
	static void * preModLoad(const char * kextID);
    static bool checkModLoad(void * loadHandle);
    static OSReturn postModLoad(void * loadHandle);
	static bool modHasInstance(const char * kextID);
    static void reportModInstances(const char * kextID);
    static void considerUnloads();
	static OSObject * allocClassWithName(const OSSymbol * name);
    static OSObject * allocClassWithName(const OSString * name);
    static OSObject * allocClassWithName(const char * name);
	
    static OSMetaClassBase * checkMetaCastWithName(
												   const OSSymbol        * className,
												   const OSMetaClassBase * object);

    static OSMetaClassBase * checkMetaCastWithName(
												   const OSString        * className,
												   const OSMetaClassBase * object);
	
    static OSMetaClassBase * checkMetaCastWithName(
												   const char            * className,
												   const OSMetaClassBase * object);
    void instanceConstructed() const;
    void instanceDestructed() const;
    OSMetaClassBase * checkMetaCast(const OSMetaClassBase * object) const;
    unsigned int getInstanceCount() const;
    const OSMetaClass * getSuperClass() const;
    const OSSymbol * getKmodName() const;
    const char * getClassName() const;
    unsigned int getClassSize() const;
    virtual OSObject * alloc() const = 0;
	
#define OSDeclareCommonStructors(className)                     \
private:                                                    \
static const OSMetaClass * const superClass;                \
public:                                                     \
static const OSMetaClass * const metaClass;                 \
static class MetaClass : public OSMetaClass {           \
public:                                                 \
MetaClass();                                        \
virtual OSObject *alloc() const;                    \
} gMetaClass;                                           \
friend class className ::MetaClass;                     \
virtual const OSMetaClass * getMetaClass() const;       \
protected:                                                  \
className (const OSMetaClass *);                            \
virtual ~ className ()
	
#define OSDeclareDefaultStructors(className)    \
OSDeclareCommonStructors(className);        \
public:                                     \
className ();                               \
protected:
	
#define OSDeclareAbstractStructors(className)                          \
OSDeclareCommonStructors(className);                               \
private:                                                           \
className (); /* Make primary constructor private in abstract */   \
protected:
	
#define OSDeclareFinalStructors(className)                              \
OSDeclareDefaultStructors(className)                            \
private:                                                            \
void __OSFinalClass(void);                                      \
protected:
	
#define OSDefineMetaClassWithInit(className, superclassName, init)            \
/* Class global data */                                                   \
className ::MetaClass className ::gMetaClass;                             \
const OSMetaClass * const className ::metaClass =                         \
& className ::gMetaClass;                                             \
const OSMetaClass * const className ::superClass =                        \
& superclassName ::gMetaClass;                                        \
/* Class member functions */                                              \
className :: className(const OSMetaClass *meta)                           \
: superclassName (meta) { }                                           \
className ::~ className() { }                                             \
const OSMetaClass * className ::getMetaClass() const                      \
{ return &gMetaClass; }                                               \
/* The ::MetaClass constructor */                                         \
className ::MetaClass::MetaClass()                                        \
: OSMetaClass(#className, className::superClass, sizeof(className))   \
{ init; }
	
#define OSDefineAbstractStructors(className, superclassName)        \
OSObject * className ::MetaClass::alloc() const { return 0; }
	
#define OSDefineDefaultStructors(className, superclassName)     \
OSObject * className ::MetaClass::alloc() const             \
{ return new className; }                                   \
className :: className () : superclassName (&gMetaClass)    \
{ gMetaClass.instanceConstructed(); }
	
#define OSDefineMetaClass(className, superclassName)            \
OSDefineMetaClassWithInit(className, superclassName, )
	
#define OSMetaClassDeclareReservedUsed(className, index)
	
#define OSMetaClassDeclareReservedUnused(className, index)        \
private:                                                      \
APPLE_KEXT_PAD_METHOD void _RESERVED ## className ## index ()

#define OSDefineFinalStructors(className, superclassName)               \
OSDefineDefaultStructors(className, superclassName)                 \
void className ::__OSFinalClass(void) { }
	
#define OSDefineMetaClassAndStructorsWithInit(className, superclassName, init) \
OSDefineMetaClassWithInit(className, superclassName, init)        \
OSDefineDefaultStructors(className, superclassName)
	
#define OSDefineMetaClassAndAbstractStructorsWithInit(className, superclassName, init) \
OSDefineMetaClassWithInit(className, superclassName, init)        \
OSDefineAbstractStructors(className, superclassName)
	
#define OSDefineMetaClassAndFinalStructorsWithInit(className, superclassName, init) \
OSDefineMetaClassWithInit(className, superclassName, init)                      \
OSDefineFinalStructors(className, superclassName)

#define OSDefineMetaClass(className, superclassName)            \
OSDefineMetaClassWithInit(className, superclassName, )
	
#define OSDefineMetaClassAndStructors(className, superclassName)    \
OSDefineMetaClassAndStructorsWithInit(className, superclassName, )
	
#define OSMetaClassDefineReservedUsed(className, index)
	
#define OSMetaClassDefineReservedUnused(className, index)       \
void className ::_RESERVED ## className ## index ()             \
{ APPLE_KEXT_PAD_IMPL(index); }
	
#define OSDefineMetaClassAndAbstractStructors(className, superclassName) \
OSDefineMetaClassAndAbstractStructorsWithInit (className, superclassName, )
	 void reservedCalled(int ind) const;
	
private:
    // Obsolete APIs
    static OSDictionary* getClassDictionary();
    virtual bool serialize(OSSerialize * serializer) const;
	
	OSMetaClassDeclareReservedUnused(OSMetaClass, 0);
    OSMetaClassDeclareReservedUnused(OSMetaClass, 1);
    OSMetaClassDeclareReservedUnused(OSMetaClass, 2);
    OSMetaClassDeclareReservedUnused(OSMetaClass, 3);
    OSMetaClassDeclareReservedUnused(OSMetaClass, 4);
    OSMetaClassDeclareReservedUnused(OSMetaClass, 5);
    OSMetaClassDeclareReservedUnused(OSMetaClass, 6);
    OSMetaClassDeclareReservedUnused(OSMetaClass, 7);
};

#endif
