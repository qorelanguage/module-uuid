/* indent-tabs-mode: nil -*- */
/*
  uuid Qore module

  Copyright (C) 2010 David Nichols, all rights reserved

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "uuid-module.h"

QoreStringNode *uuid_module_init();
void uuid_module_ns_init(QoreNamespace *rns, QoreNamespace *qns);
void uuid_module_delete();

// qore module symbols
DLLEXPORT char qore_module_name[] = "uuid";
DLLEXPORT char qore_module_version[] = PACKAGE_VERSION;
DLLEXPORT char qore_module_description[] = "uuid module";
DLLEXPORT char qore_module_author[] = "David Nichols";
DLLEXPORT char qore_module_url[] = "http://qore.org";
DLLEXPORT int qore_module_api_major = QORE_MODULE_API_MAJOR;
DLLEXPORT int qore_module_api_minor = QORE_MODULE_API_MINOR;
DLLEXPORT qore_module_init_t qore_module_init = uuid_module_init;
DLLEXPORT qore_module_ns_init_t qore_module_ns_init = uuid_module_ns_init;
DLLEXPORT qore_module_delete_t qore_module_delete = uuid_module_delete;
DLLEXPORT qore_license_t qore_module_license = QL_LGPL;

qore_classid_t CID_UUID;

// UUID::constructor(int $flag = UUID::None)
static void UUID_constructor_int(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink) {
   self->setPrivate(CID_UUID, new QoreUUID((int)HARD_QORE_INT(params, 0)));
}

// UUID::constructor(string $uuid_str)
static void UUID_constructor_str(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink) {
   SimpleRefHolder<QoreUUID> uuid(new QoreUUID(*(HARD_QORE_STRING(params, 0)), xsink));
   if (*xsink)
      return;
 
   self->setPrivate(CID_UUID, uuid.release());
}

static void UUID_copy(QoreObject *self, QoreObject *old, QoreUUID *uuid, ExceptionSink *xsink) {
   self->setPrivate(CID_UUID, new QoreUUID(*uuid));
}

// UUID::toString(int $flags = UUID::None) returns string
static AbstractQoreNode *UUID_toString(QoreObject *self, QoreUUID *uuid, const QoreListNode *params, ExceptionSink *xsink) {
   return uuid->toString((int)HARD_QORE_INT(params, 0));
}

// UUID::isNull() returns bool
static AbstractQoreNode *UUID_isNull(QoreObject *self, QoreUUID *uuid, const QoreListNode *params, ExceptionSink *xsink) {
   return get_bool_node(uuid->isNull());
}

// UUID::clear() returns nothing
static AbstractQoreNode *UUID_clear(QoreObject *self, QoreUUID *uuid, const QoreListNode *params, ExceptionSink *xsink) {
   uuid->clear();
   return 0;
}

// UUID::generate(int $flags = UUID::None) returns nothing
static AbstractQoreNode *UUID_generate(QoreObject *self, QoreUUID *uuid, const QoreListNode *params, ExceptionSink *xsink) {
   uuid->generate((int)HARD_QORE_INT(params, 0));
   return 0;
}

// UUID::compare(UUID $other) returns int
static AbstractQoreNode *UUID_compare(QoreObject *self, QoreUUID *uuid, const QoreListNode *params, ExceptionSink *xsink) {
   HARD_QORE_OBJ_DATA(other, QoreUUID, params, 0, CID_UUID, "UUID", "UUID::compare", xsink);
   if (*xsink)
      return 0;
   assert(other);

   SimpleRefHolder<QoreUUID> holder(other);

   return new QoreBigIntNode(uuid->compare(*other));
}

// UUID::set(string $uuid_str) returns nothing
static AbstractQoreNode *UUID_set(QoreObject *self, QoreUUID *uuid, const QoreListNode *params, ExceptionSink *xsink) {
   uuid->set(*(HARD_QORE_STRING(params, 0)), xsink);
   return 0;
}

// static methods
// static UUID::getString(int $gen_flags = UUID::None, int $string_flags = UUID::None) returns string
static AbstractQoreNode *f_UUID_get(const QoreListNode *params, ExceptionSink *xsink) {
   return QoreUUID::get((int)HARD_QORE_INT(params, 0), (int)HARD_QORE_INT(params, 1));
}

QoreNamespace UNS("UUID");

QoreStringNode *uuid_module_init() {
   // setup namespace

   QoreClass *QC_UUID = new QoreClass("UUID");
   CID_UUID = QC_UUID->getID();

   // UUID::constructor(int $flags = UUID::None)
   QC_UUID->setConstructorExtended(UUID_constructor_int, false, QC_NO_FLAGS, QDOM_DEFAULT, 1, bigIntTypeInfo, new QoreBigIntNode(QUF_NONE));

   // UUID::constructor(string $uuid_str)
   QC_UUID->setConstructorExtended(UUID_constructor_str, false, QC_NO_FLAGS, QDOM_DEFAULT, 1, stringTypeInfo, QORE_PARAM_NO_ARG);

   QC_UUID->setCopy((q_copy_t)UUID_copy);

   // UUID::toString(int $flags = UUID::None) returns string
   QC_UUID->addMethodExtended("toString", (q_method_t)UUID_toString, false, QC_RET_VALUE_ONLY, QDOM_DEFAULT, stringTypeInfo, 1, bigIntTypeInfo, new QoreBigIntNode(QUF_NONE));

   // UUID::isNull() returns bool
   QC_UUID->addMethodExtended("isNull", (q_method_t)UUID_isNull, false, QC_RET_VALUE_ONLY, QDOM_DEFAULT, boolTypeInfo);

   // UUID::clear() returns nothing
   QC_UUID->addMethodExtended("clear", (q_method_t)UUID_clear, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo);

   // UUID::generate(int $flags = UUID::None) returns nothing
   QC_UUID->addMethodExtended("generate", (q_method_t)UUID_generate, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, bigIntTypeInfo, new QoreBigIntNode(QUF_NONE));

   // UUID::compare(UUID $other) returns int
   QC_UUID->addMethodExtended("compare", (q_method_t)UUID_compare, false, QC_RET_VALUE_ONLY, QDOM_DEFAULT, bigIntTypeInfo, 1, QC_UUID->getTypeInfo(), QORE_PARAM_NO_ARG);

   // UUID::set(string $uuid_str) returns nothing
   QC_UUID->addMethodExtended("set", (q_method_t)UUID_set, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, stringTypeInfo, QORE_PARAM_NO_ARG);

   // static methods
   // static UUID::getString(int $gen_flags = UUID::None, int $string_flags = UUID::None) returns string
   QC_UUID->addStaticMethodExtended("get", f_UUID_get, false, QC_RET_VALUE_ONLY, QDOM_DEFAULT, stringTypeInfo, 2, bigIntTypeInfo, new QoreBigIntNode(QUF_NONE), bigIntTypeInfo, new QoreBigIntNode(QUF_NONE));

   UNS.addSystemClass(QC_UUID);

   UNS.addConstant("None",      new QoreBigIntNode(QUF_NONE));
   UNS.addConstant("UpperCase", new QoreBigIntNode(QUF_UPPER_CASE));
   UNS.addConstant("LowerCase", new QoreBigIntNode(QUF_LOWER_CASE));
   UNS.addConstant("Random",    new QoreBigIntNode(QUF_RANDOM));
   UNS.addConstant("Time",      new QoreBigIntNode(QUF_TIME));
   UNS.addConstant("Empty",     new QoreBigIntNode(QUF_EMPTY));

#ifdef HAVE_UUID_UNPARSE_CASE
   UNS.addConstant("HAVE_UNPARSE_CASE", &True);
#else
   UNS.addConstant("HAVE_UNPARSE_CASE", &False);
#endif

   return 0;
}

void uuid_module_ns_init(QoreNamespace *rns, QoreNamespace *qns) {
   qns->addNamespace(UNS.copy());
}

void uuid_module_delete() {
}
