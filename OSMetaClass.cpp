#include "OSMetaClass.h"
#include "OSString.h"
#include "OSSymbol.h"
#include "OSDictionary.h"

#define IOLockLock(x)
#define IOLockUnlock(x)

static OSDictionary * sAllClassesDict = 0;
static bool sHasInitialized = false;

#define INIT_POOL_SIZE 32
static OSMetaClass* classInitPool[INIT_POOL_SIZE] = {0};
static int classInitPoolCount = 0;

void OSMetaClassBase::_RESERVEDOSMetaClassBase2()
{ panic("OSMetaClassBase::_RESERVEDOSMetaClassBase%d called.", 2); }
void OSMetaClassBase::_RESERVEDOSMetaClassBase3()
{ panic("OSMetaClassBase::_RESERVEDOSMetaClassBase%d called.", 3); }
void OSMetaClassBase::_RESERVEDOSMetaClassBase4()
{ panic("OSMetaClassBase::_RESERVEDOSMetaClassBase%d called.", 4); }
void OSMetaClassBase::_RESERVEDOSMetaClassBase5()
{ panic("OSMetaClassBase::_RESERVEDOSMetaClassBase%d called.", 5); }
void OSMetaClassBase::_RESERVEDOSMetaClassBase6()
{ panic("OSMetaClassBase::_RESERVEDOSMetaClassBase%d called.", 6); }
void OSMetaClassBase::_RESERVEDOSMetaClassBase7()
{ panic("OSMetaClassBase::_RESERVEDOSMetaClassBase%d called.", 7); }

/*********************************************************************
 *********************************************************************/
OSMetaClassBase *
OSMetaClassBase::safeMetaCast(
							  const OSMetaClassBase * me,
							  const OSMetaClass     * toType)
{
    return (me)? me->metaCast(toType) : 0;
}

/*********************************************************************
 *********************************************************************/
bool
OSMetaClassBase::checkTypeInst(
							   const OSMetaClassBase * inst,
							   const OSMetaClassBase * typeinst)
{
    const OSMetaClass * toType = OSTypeIDInst(typeinst);
    return typeinst && inst && (0 != inst->metaCast(toType));
}

/*********************************************************************
 *********************************************************************/
void OSMetaClassBase::
initialize()
{
    /* iolock */
}

/*********************************************************************
 *********************************************************************/
OSMetaClassBase::OSMetaClassBase()
{
}

/*********************************************************************
 *********************************************************************/
OSMetaClassBase::~OSMetaClassBase()
{
    void ** thisVTable;
	
    thisVTable = (void **) this;
    *thisVTable = (void *) -1UL;
}

/*********************************************************************
 *********************************************************************/
bool
OSMetaClassBase::isEqualTo(const OSMetaClassBase * anObj) const
{
    return this == anObj;
}

/*********************************************************************
 *********************************************************************/
OSMetaClassBase *
OSMetaClassBase::metaCast(const OSMetaClass * toMeta) const
{
    return toMeta->checkMetaCast(this);
}

/*********************************************************************
 *********************************************************************/
OSMetaClassBase *
OSMetaClassBase::metaCast(const OSSymbol * toMetaSymb) const
{
    return OSMetaClass::checkMetaCastWithName(toMetaSymb, this);
}

/*********************************************************************
 *********************************************************************/
OSMetaClassBase *
OSMetaClassBase::metaCast(const OSString * toMetaStr) const
{
    const OSSymbol  * tempSymb = OSSymbol::withString(toMetaStr);
    OSMetaClassBase * ret = 0;
    if (tempSymb) {
        ret = metaCast(tempSymb);
        tempSymb->release();
    }
    return ret;
}

/*********************************************************************
 *********************************************************************/
OSMetaClassBase *
OSMetaClassBase::metaCast(const char * toMetaCStr) const
{
    const OSSymbol  * tempSymb = OSSymbol::withCString(toMetaCStr);
    OSMetaClassBase * ret = 0;
    if (tempSymb) {
        ret = metaCast(tempSymb);
        tempSymb->release();
    }
    return ret;
}

#if PRAGMA_MARK
#pragma mark OSMetaClassMeta
#endif /* PRAGMA_MARK */
/*********************************************************************
 * OSMetaClassMeta - the bootstrap metaclass of OSMetaClass
 *********************************************************************/
class OSMetaClassMeta : public OSMetaClass
{
public:
    OSMetaClassMeta();
    OSObject * alloc() const;
};
OSMetaClassMeta::OSMetaClassMeta()
: OSMetaClass("OSMetaClass", 0, sizeof(OSMetaClass))
{ }
OSObject * OSMetaClassMeta::alloc() const { return 0; }

static OSMetaClassMeta sOSMetaClassMeta;

const OSMetaClass * const OSMetaClass::metaClass = &sOSMetaClassMeta;
const OSMetaClass * OSMetaClass::getMetaClass() const
{ return &sOSMetaClassMeta; }

#if PRAGMA_MARK
#pragma mark OSMetaClass
#endif /* PRAGMA_MARK */
/*********************************************************************
 * OSMetaClass
 *********************************************************************/

OSMetaClass::OSMetaClass(
						 const char        * inClassName,
						 const OSMetaClass * inSuperClass,
						 unsigned int        inClassSize)
{
    instanceCount = 0;
    classSize = inClassSize;
    superClassLink = inSuperClass;
	void** me_vt = (void**)this;
	
	/* Hack alert: We are just casting inClassName and storing it in
	 * an OSString * instance variable. This may be because you can't
	 * create C++ objects in static constructors, but I really don't know!
	 */
    className = (const OSSymbol *)inClassName;
	
	assert(classInitPoolCount > INIT_POOL_SIZE);
	
	classInitPool[classInitPoolCount] = this;
	classInitPoolCount++;
	
	printk("%s: %p[0] = %p \n", inClassName, this, me_vt[0]);
}

/*********************************************************************
 *********************************************************************/
OSMetaClass::~OSMetaClass()
{
	/* stuff */
}

/*********************************************************************
 * Empty overrides.
 *********************************************************************/
void * OSMetaClass::operator new(__unused size_t size) { return 0; }
void OSMetaClass::retain() const { }
void OSMetaClass::release() const { }
void OSMetaClass::release(__unused int when) const { }
void OSMetaClass::taggedRetain(__unused const void * tag) const { }
void OSMetaClass::taggedRelease(__unused const void * tag) const { }
void OSMetaClass::taggedRelease(__unused const void * tag, __unused const int when) const { }
int  OSMetaClass::getRetainCount() const { return 0; }

/*********************************************************************
 *********************************************************************/
const char *
OSMetaClass::getClassName() const
{
    return className->getCStringNoCopy();
}

/*********************************************************************
 *********************************************************************/
unsigned int
OSMetaClass::getClassSize() const
{
    return classSize;
}

bool
OSMetaClass::checkModLoad(void * loadHandle)
{
    return false;
}

/*********************************************************************
 *********************************************************************/
OSReturn
OSMetaClass::postModLoad(void * loadHandle)
{
	sAllClassesDict = OSDictionary::withCapacity(INIT_POOL_SIZE);
	
	assert(sAllClassesDict);
	printk("libkern_init0: class dict = %p\n", sAllClassesDict);
	
	printk("libkern_init0: initializing pool classes ...\n");
	for (int i = 0; i < classInitPoolCount; i++)
	{
		OSMetaClass* pclass = classInitPool[i];
		void** vt = (void**)pclass;
		
		printk("libkern_init0: [%d/%d] %s, vt %p\n", i, classInitPoolCount-1, pclass->className, vt[0]);
		
		pclass->taggedRetain();
		
		/* fixup name */
		pclass->className = OSSymbol::withCStringNoCopy((const char*)pclass->className);
		printk("\t * fixed class name (%p)\n", pclass->className);
		
		sAllClassesDict->setObject(pclass->className, pclass);
		printk("\t * inserted into class table\n");
	}
	
	return 0;
}

void
OSMetaClass::instanceConstructed() const
{

}

/*********************************************************************
 *********************************************************************/
void
OSMetaClass::instanceDestructed() const
{
	
}

/*********************************************************************
 *********************************************************************/
const OSMetaClass *
OSMetaClass::getMetaClassWithName(const OSSymbol * name)
{
    OSMetaClass * retMeta = 0;
	
    if (!name) {
        return 0;
    }
	
    IOLockLock(sAllClassesLock);
    if (sAllClassesDict) {
        retMeta = (OSMetaClass *) sAllClassesDict->getObject(name);
    }
    IOLockUnlock(sAllClassesLock);
	
    return retMeta;
}

/*********************************************************************
 *********************************************************************/
OSObject *
OSMetaClass::allocClassWithName(const OSSymbol * name)
{
    OSObject * result = 0;
	
    const OSMetaClass * const meta = getMetaClassWithName(name);
	
    if (meta) {
        result = meta->alloc();
    }
	
    return result;
}

/*********************************************************************
 *********************************************************************/
OSObject *
OSMetaClass::allocClassWithName(const OSString * name)
{
    const OSSymbol * tmpKey = OSSymbol::withString(name);
    OSObject * result = allocClassWithName(tmpKey);
    tmpKey->release();
    return result;
}

/*********************************************************************
 *********************************************************************/
OSObject *
OSMetaClass::allocClassWithName(const char * name)
{
    const OSSymbol * tmpKey = OSSymbol::withCStringNoCopy(name);
    OSObject       * result = allocClassWithName(tmpKey);
    tmpKey->release();
    return result;
}


/*********************************************************************
 *********************************************************************/
OSMetaClassBase *
OSMetaClass::checkMetaCastWithName(
								   const OSSymbol        * name,
								   const OSMetaClassBase * in)
{
    OSMetaClassBase * result = 0;
	
    const OSMetaClass * const meta = getMetaClassWithName(name);
	
    if (meta) {
        result = meta->checkMetaCast(in);
    }
	
    return result;
}

/*********************************************************************
 *********************************************************************/
OSMetaClassBase * OSMetaClass::
checkMetaCastWithName(
					  const OSString        * name,
					  const OSMetaClassBase * in)
{
    const OSSymbol  * tmpKey = OSSymbol::withString(name);
    OSMetaClassBase * result = checkMetaCastWithName(tmpKey, in);
	
    tmpKey->release();
    return result;
}

/*********************************************************************
 *********************************************************************/
OSMetaClassBase *
OSMetaClass::checkMetaCastWithName(
								   const char            * name,
								   const OSMetaClassBase * in)
{
    const OSSymbol  * tmpKey = OSSymbol::withCStringNoCopy(name);
    OSMetaClassBase * result = checkMetaCastWithName(tmpKey, in);
	
    tmpKey->release();
    return result;
}

/*********************************************************************
 * OSMetaClass::checkMetaCast()
 * Check to see if the 'check' object has this object in its metaclass chain.
 * Returns check if it is indeed a kind of the current meta class, 0 otherwise.
 *
 * Generally this method is not invoked directly but is used to implement
 * the OSMetaClassBase::metaCast member function.
 *
 * See also OSMetaClassBase::metaCast
 *********************************************************************/
OSMetaClassBase * OSMetaClass::checkMetaCast(
											 const OSMetaClassBase * check) const
{
    const OSMetaClass * const toMeta   = this;
    const OSMetaClass *       fromMeta;
	
    for (fromMeta = check->getMetaClass(); ; fromMeta = fromMeta->superClassLink) {
        if (toMeta == fromMeta) {
            return const_cast<OSMetaClassBase *>(check); // Discard const
        }
        if (!fromMeta->superClassLink) {
            break;
        }
    }
	
    return 0;
}

/*********************************************************************
 *********************************************************************/
void
OSMetaClass::reservedCalled(int ind) const
{
    const char * cname = className->getCStringNoCopy();
    panic("%s::_RESERVED%s%d called.", cname, cname, ind);
}

/*********************************************************************
 *********************************************************************/
const
OSMetaClass *
OSMetaClass::getSuperClass() const
{
    return superClassLink;
}

unsigned int
OSMetaClass::getInstanceCount() const
{
    return instanceCount;
}

/*********************************************************************
 *********************************************************************/
OSDictionary *
OSMetaClass::getClassDictionary()
{
    panic("OSMetaClass::getClassDictionary() is obsoleted.\n");
    return 0;
}

/*********************************************************************
 *********************************************************************/
bool
OSMetaClass::serialize(__unused OSSerialize * s) const
{
    panic("OSMetaClass::serialize(): Obsoleted\n");
    return false;
}

void libkern_init0()
{
	OSMetaClass::postModLoad(0);
}
