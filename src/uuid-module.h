/* -*- mode: c++; indent-tabs-mode: nil -*- */
/*
  uuid-module.h
  
  Qore Programming Language

  Copyright 2010 - 2011 David Nichols

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

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include <qore/Qore.h>

#if (defined _WIN32 || defined __WIN32__) && ! defined __CYGWIN__
#define WIN_UUID 1
#else
#include <uuid.h>
#endif

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

#ifdef WIN_UUID
typedef UUID q_uuid_t;
#else
#ifdef OSSP_UUID
typedef uuid_t* q_uuid_t;
#else
typedef uuid_t q_uuid_t;
#endif
#endif

#define Q_UUID_LEN 36

class QoreUUID : public AbstractPrivateData {
protected:
   q_uuid_t uuid;

   DLLLOCAL static QoreStringNode *getStringIntern(q_uuid_t &uuid, int flags = QUF_NONE) {
      if (flags == QUF_NONE)
         flags = QUF_LOWER_CASE;

#ifdef WIN_UUID
      UCHAR *StringUuid = 0;
#ifdef DEBUG
      RPC_STATUS rc =
#endif
      UuidToString(&uuid, &StringUuid);
#ifdef DEBUG
      assert(rc == RPC_S_OK);
#endif
      QoreStringNode *str = new QoreStringNode((const char *)StringUuid);
      RpcStringFree(&StringUuid);
      if (flags & QUF_UPPER_CASE)
         str->toupr();
      else
         str->tolwr();
#else // WIN_UUID
#ifdef OSSP_UUID
      size_t len = 0;
#ifdef DEBUG
      char *buf = 0;
      uuid_rc_t rc = uuid_export(uuid, UUID_FMT_STR, &buf, &len);
      if (rc != UUID_RC_OK)
         printd(0, "uuid_export error: %d: %s\n", rc, uuid_error(rc));
      assert(rc == UUID_RC_OK);
#else // !DEBUG
      uuid_export(uuid, UUID_FMT_STR, &buf, &len);
#endif // DEBUG
      QoreStringNode *str = new QoreStringNode(buf, len, len + 1, QCS_DEFAULT);
      if (flags & QUF_UPPER_CASE)
         str->toupr();
      else
         str->tolwr();
#else // OSSP_UUID
      QoreStringNode *str = new QoreStringNode;
      str->allocate(Q_UUID_LEN + 1);

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
#endif // HAVE_UUID_UNPARSE_CASE
#endif // OSSP_UUID
#endif // WIN_UUID

      return str;
   }

   DLLLOCAL static void generateIntern(q_uuid_t &uuid, int flags = QUF_NONE) {
#ifdef WIN_UUID
      UuidCreate(&uuid);
#else
#ifdef OSSP_UUID
      if (flags & QUF_TIME)
         uuid_make(uuid, UUID_MAKE_V1);
      else if (!(flags & QUF_EMPTY))
         uuid_make(uuid, UUID_MAKE_V4);
#else
      if (flags & QUF_RANDOM)
         uuid_generate_random(uuid);
      else if (flags & QUF_TIME)
         uuid_generate_time(uuid);
      else if (flags & QUF_EMPTY)
         uuid_clear(uuid);
      else
         uuid_generate(uuid);
#endif
#endif
   }

public:
   DLLLOCAL QoreUUID(int flags = QUF_NONE) {
#ifdef OSSP_UUID
      uuid_create(&uuid);
#endif
      generateIntern(uuid, flags);
   }

   DLLLOCAL QoreUUID(const QoreString &uuid_str, ExceptionSink *xsink) {
#ifdef OSSP_UUID
      uuid_create(&uuid);
#endif
      set(uuid_str, xsink);
   }

   DLLLOCAL QoreUUID(const QoreUUID &old) {
#ifdef WIN_UUID
      // FIXME: test
      memcpy(&uuid, &old.uuid, sizeof(UUID));
#else
#ifdef OSSP_UUID
      uuid_create(&uuid);
      uuid_clone(old.uuid, &uuid);
#else
      uuid_copy(uuid, const_cast<QoreUUID &>(old).uuid);
#endif
#endif
   }

#ifdef OSSP_UUID
   DLLLOCAL ~QoreUUID() {
      uuid_destroy(uuid);
   }
#endif

   DLLLOCAL void generate(int flags = QUF_NONE) {
      generateIntern(uuid, flags);
   }

   DLLLOCAL QoreStringNode *getErrorString(ExceptionSink *xsink, const QoreString &uuid_str, const char *err = "cannot parse string") {
      QoreStringNode *desc = new QoreStringNode(err);
      desc->concat(": argument '");
      desc->concat(&uuid_str, xsink);
      if (desc->strlen() > 255) {
         desc->terminate(255);
         desc->concat("...");
      }
      desc->concat("' as a UUID string (must be in format like ex: '1b4e28ba-2fa1-11d2-883f-b9a761bde3fb')");
      return desc;
   }

   DLLLOCAL int set(const QoreString &uuid_str, ExceptionSink *xsink) {
#ifdef WIN_UUID
      RPC_STATUS rc = UuidFromString((unsigned char *)uuid_str.getBuffer(), &uuid);
      if (rc == RPC_S_OK)
         return 0;

      xsink->raiseException("UUID-STRING-ERROR", getErrorString(xsink, uuid_str));
      return -1;
#else
#ifdef OSSP_UUID
      uuid_rc_t rc = uuid_import(uuid, UUID_FMT_STR, uuid_str.getBuffer(), uuid_str.strlen());
      if (rc == UUID_RC_OK)
         return 0;

      xsink->raiseException("UUID-STRING-ERROR", getErrorString(xsink, uuid_str, uuid_error(rc)));
      return -1;
#else
      if (uuid_parse((char *)uuid_str.getBuffer(), uuid)) {
         xsink->raiseException("UUID-STRING-ERROR", getErrorString(xsink, uuid_str));
         return -1;
      }
      return 0;
#endif
#endif
   }

   DLLLOCAL QoreStringNode *toString(int flags = QUF_NONE) {
      return getStringIntern(uuid, flags);
   }

   DLLLOCAL bool isNull() const {
#ifdef WIN_UUID
      RPC_STATUS rc;
      UuidIsNil((UUID *)&uuid, &rc);
      return rc;
#else
#ifdef OSSP_UUID
      int result;
      uuid_isnil(uuid, &result);
      return result;
#else
      return (bool)uuid_is_null(const_cast<QoreUUID *>(this)->uuid);
#endif
#endif
   }

   DLLLOCAL void clear() {
#ifdef WIN_UUID
      UuidCreateNil(&uuid);
#else
#ifdef OSSP_UUID
      uuid_destroy(uuid);
      uuid_create(&uuid);
#else
      uuid_clear(uuid);
#endif
#endif
   }

   DLLLOCAL int compare(const QoreUUID &other) const {
#ifdef WIN_UUID
      RPC_STATUS rc;
      UuidCompare((UUID*)&uuid, (UUID*)&other.uuid, &rc);
      return rc;
#else
#ifdef OSSP_UUID
      int result;
      uuid_compare(uuid, other.uuid, &result);
      return result;
#else
      return uuid_compare(const_cast<QoreUUID*>(this)->uuid, const_cast<QoreUUID &>(other).uuid);
#endif
#endif
   }

   DLLLOCAL static QoreStringNode *get(int string_flags = QUF_NONE, int gen_flags = QUF_NONE) {
      q_uuid_t uuid;
#ifdef OSSP_UUID
      uuid_create(&uuid);
#endif
      generateIntern(uuid, gen_flags);
      return getStringIntern(uuid, string_flags);
   }
};

DLLLOCAL extern qore_classid_t CID_UUID;

#endif
