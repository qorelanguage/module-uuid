/* indent-tabs-mode: nil -*- */
/*
  uuid Qore module

  Copyright (C) 2010 - 2012 David Nichols

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

static QoreStringNode *uuid_module_init();
static void uuid_module_ns_init(QoreNamespace *rns, QoreNamespace *qns);
static void uuid_module_delete();

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

DLLLOCAL QoreClass* initUUIDClass(QoreNamespace &ns);

QoreNamespace UNS("UUID");

QoreStringNode *uuid_module_init() {
   // setup namespace
   QoreClass *QC_UUID = initUUIDClass(UNS);
   UNS.addSystemClass(QC_UUID);
   return 0;
}

void uuid_module_ns_init(QoreNamespace *rns, QoreNamespace *qns) {
   qns->addNamespace(UNS.copy());
}

void uuid_module_delete() {
}
