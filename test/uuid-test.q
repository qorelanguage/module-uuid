#!/usr/bin/env qore

%requires uuid >= 1.1

# the uuid module requires qore 0.8.0+, so we know we have types
%require-types
%requires UnitTest

%exec-class uuid_test

const opts = 
    ( "help"   : "help,h",
      "verbose" : "verbose,v"
    );

class uuid_test inherits UnitTest {

    constructor() : UnitTest() {
        my GetOpt $g(opts);
        my hash $o = $g.parse2(\$ARGV);
        if ($o.help)
            $.usage();

        $.setVerbose($o.verbose);

        my UUID $uuid();        
        $.doTests($uuid, "default");
        $uuid.generate(UUID::Random);
        $.doTests($uuid, "random");
        $uuid.generate(UUID::Time);
        $.doTests($uuid, "time");

        $uuid = new UUID("1b4e28ba-2fa1-11d2-883f-b9a761bde3fb");
        $.doTests($uuid, "literal");

        my string $str = UUID::get();
        $uuid = new UUID($str);
        my string $str2 = $uuid.toString();
        $.testValue($str, $str2, sprintf("static UUID::get(), type: %s", "parsed"));
        $.doTests($uuid, "parsed");

        printf("%d tests completed successfully, %d error%s\n", $.testCount(), $.errors(), $.errors() == 1 ? "" : "s");
    }

    doTests(UUID $uuid, string $type) {
        $.testValue($uuid.isNull(), False, sprintf("UUID::isNull, type: %s", $type));

        if (UUID::HAVE_UNPARSE_CASE) {
            $.testValue($uuid.toString(UUID::LowerCase) =~ /^[\-0-9a-f]+$/, True, sprintf("UUID::toString(LowerCase), type: %s", $type));
            $.testValue($uuid.toString(UUID::UpperCase) =~ /^[\-0-9A-F]+$/, True, sprintf("UUID::toString(UpperCase), type: %s", $type));
        }

        my UUID $other = $uuid.copy();
        $.testValue($uuid.compare($other), 0, sprintf("UUID::copy(), type: %s", $type));

        $uuid.clear();
        $.testValue($uuid.isNull(), True, sprintf("UUID::clear(), type: %s", $type));
        $.testValue($uuid.compare($other), -1, sprintf("UUID::compare(), type: %s", $type));

        $uuid.set("ffffffff-ffff-ffff-ffff-ffffffffffff");
        $.testValue($uuid.compare($other), 1, sprintf("UUID::set(), type: %s", $type));
    }

    static usage() {
        printf("usage: %s [options]
 -h,--help        this help text
", get_script_name());
      exit(1);
   }

}
