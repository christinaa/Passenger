/*
 * libkern_test
 * Copyright (c) 2012 Christina Brooks
 *
 * Testing the libkern functionality.
 */

#include "main.h"
#include "OSMetaClass.h"
#include "OSArray.h"
#include "OSObject.h"
#include "OSString.h"
#include "OSSerialize.h"

extern "C" volatile int kmod_start(void);
void libkern_init0();

volatile int kmod_start(void)
{
	libkern_init0();
	
	OSString* one = OSString::withCString("Anal sex");
	OSString* two = OSString::withCString("Cunnilungus");
 
	OSArray* sexthings = OSArray::withCapacity(2);

	sexthings->setObject(one);
	sexthings->setObject(two);
	
	printk("Hello from IOKit!\n");
	
	printk("Put: %p %p\n", one, two);
	printk("OSArray: %p\n", sexthings);
	printk("Get: %p %p\n", sexthings->getObject(0), sexthings->getObject(1));
	
	OSSerialize* ser = OSSerialize::withCapacity(1024);
	sexthings->serialize(ser);

	printk("Serialized: %s \n", ser->text());
	
	return 0;
}