#!/usr/bin/env qore

%requires uuid >= 1.1

# the uuid module requires qore 0.8.0+, so we know we have types
%require-types
%new-style
%strict-args
%enable-all-warnings

%requires QUnit

%exec-class uuid_test

class uuid_test inherits QUnit::Test {
    constructor() : Test("uuid", "1.0") {
        addTestCase("default", \defaultTest());
        addTestCase("random", \randomTest());
        addTestCase("time", \timeTest());
        addTestCase("literal", \literalTest());
        addTestCase("parsed", \parsedTest());

        set_return_value(main());
    }

    defaultTest() {
        UUID uuid();
        doTests(uuid, "default");
    }

    randomTest() {
        UUID uuid();
        uuid.generate(UUID::Random);
        doTests(uuid, "random");
    }

    timeTest() {
        UUID uuid();
        uuid.generate(UUID::Time);
        doTests(uuid, "time");
    }

    literalTest() {
        UUID uuid("1b4e28ba-2fa1-11d2-883f-b9a761bde3fb");
        doTests(uuid, "literal");
    }

    parsedTest() {
        UUID uuid("1b4e28ba-2fa1-11d2-883f-b9a761bde3fb");
        string str = UUID::get();
        uuid = new UUID(str);
        string str2 = uuid.toString();
        assertEq(str, str2, sprintf("static UUID::get(), type: %s", "parsed"));
        doTests(uuid, "parsed");
    }

    doTests(UUID uuid, string type) {
        assertFalse(uuid.isNull(), sprintf("UUID::isNull, type: %s", type));

        if (UUID::HAVE_UNPARSE_CASE) {
            assertTrue(uuid.toString(UUID::LowerCase) =~ /^[\-0-9a-f]+$/, sprintf("UUID::toString(LowerCase), type: %s", type));
            assertTrue(uuid.toString(UUID::UpperCase) =~ /^[\-0-9A-F]+$/, sprintf("UUID::toString(UpperCase), type: %s", type));
        }

        UUID other = uuid.copy();
        assertEq(0, uuid.compare(other), sprintf("UUID::copy(), type: %s", type));

        uuid.clear();
        assertTrue(uuid.isNull(), sprintf("UUID::clear(), type: %s", type));
        assertEq(-1, uuid.compare(other), sprintf("UUID::compare(), type: %s", type));

        uuid.set("ffffffff-ffff-ffff-ffff-ffffffffffff");
        assertEq(1, uuid.compare(other), sprintf("UUID::set(), type: %s", type));
    }
}
