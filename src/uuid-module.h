/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
  uuid-module.h
  
  Qore Programming Language

  Copyright 2010 David Nichols

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

#ifndef _QORE_UUID_MODULE_H
#define _QORE_UUID_MODULE_H

#include "../config.h"

#include <qore/Qore.h>

#include <uuid.h>

#ifndef _UUID_STRING_T
typedef char* uuid_string_t;
#endif

// uuid flags
#define QUF_NONE        0
#define QUF_UPPER_CASE  1
#define QUF_LOWER_CASE  2
#define QUF_RANDOM      3
#define QUF_TIME        4
#define QUF_EMPTY       5

class QoreUUID : public AbstractPrivateData {
protected:
   uuid_t uuid;

   DLLLOCAL static QoreStringNode *getStringIntern(uuid_t uuid, int flags = QUF_NONE) {
      QoreStringNode *str = new QoreStringNode;
      str->allocate(37);

      if (flags == QUF_NONE)
         flags = QUF_LOWER_CASE;

#ifdef HAVE_UUID_UNPARSE_CASE
      if (flags & QUF_LOWER_CASE)
         uuid_unparse_lower(uuid, (char *)str->getBuffer());
      else
         uuid_unparse_upper(uuid, (char *)str->getBuffer());
      str->terminate(36);
#else
      uuid_unparse(uuid, (char *)str->getBuffer());
      str->terminate(36);
      if (flags & QUF_UPPER_CASE)
         str->toupr();
      else
         str->tolwr();
#endif

      return str;
   }

   DLLLOCAL static void generateIntern(uuid_t uuid, int flags = QUF_NONE) {
      if (flags & QUF_RANDOM)
         uuid_generate_random(uuid);
      else if (flags & QUF_TIME)
         uuid_generate_time(uuid);
      else if (flags & QUF_EMPTY)
         uuid_clear(uuid);
      else
         uuid_generate(uuid);
   }

public:
   DLLLOCAL QoreUUID(int flags = QUF_NONE) {
      generateIntern(uuid, flags);
   }

   DLLLOCAL QoreUUID(const QoreString &uuid_str, ExceptionSink *xsink) {
      set(uuid_str, xsink);
   }

   DLLLOCAL QoreUUID(const QoreUUID &old) {
      uuid_copy(uuid, const_cast<QoreUUID &>(old).uuid);
   }

   DLLLOCAL void generate(int flags = QUF_NONE) {
      generateIntern(uuid, flags);
   }

   DLLLOCAL int set(const QoreString &uuid_str, ExceptionSink *xsink) {
      if (uuid_parse((char *)uuid_str.getBuffer(), uuid)) {
         QoreStringNode *desc = new QoreStringNode("cannot parse string argument '");
         desc->concat(&uuid_str, xsink);
         desc->concat("' as a UUID string (must be in format like ex: '1b4e28ba-2fa1-11d2-883f-b9a761bde3fb')");
         if (desc->strlen() > 255) {
            desc->terminate(255);
            desc->concat("...");
         }
         xsink->raiseException("UUID-CONSTRUCTOR-ERROR", desc);
         return -1;
      }
      return 0;
   }

   DLLLOCAL QoreStringNode *toString(int flags = QUF_NONE) {
      return getStringIntern(uuid, flags);
   }

   DLLLOCAL bool isNull() const {
      return uuid_is_null(const_cast<QoreUUID *>(this)->uuid);
   }

   DLLLOCAL void clear() {
      uuid_clear(uuid);
   }

   DLLLOCAL int compare(const QoreUUID &other) {
      return uuid_compare(uuid, const_cast<QoreUUID &>(other).uuid);
   }

   DLLLOCAL static QoreStringNode *get(int string_flags = QUF_NONE, int gen_flags = QUF_NONE) {
      uuid_t uuid;
      generateIntern(uuid, gen_flags);
      return getStringIntern(uuid, string_flags);
   }
};

DLLLOCAL extern qore_classid_t CID_UUID;

#endif
