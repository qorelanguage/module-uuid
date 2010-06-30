#!/usr/bin/env qore

%requires uuid >= 1.1

# the uuid module requires qore 0.8.0+, so we know we have types
%require-types

%exec-class uuid_test

const opts = 
    ( "help"   : "help,h"
    );

class uuid_test {
    private {
	int $.errors = 0;
	hash $.ehash;
    }

    constructor() {
	my GetOpt $g(opts);
	my hash $o = $g.parse2(\$ARGV);
	if ($o.help)
	    $.usage();

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
	$.test_value($str, $str2, "parsed", "static UUID::get()");
	$.doTests($uuid, "parsed");

	printf("%d tests completed successfully, %d error%s\n", elements $.ehash, $.errors, $.errors == 1 ? "" : "s");
    }

    doTests(UUID $uuid, string $type) {
	printf("%s UUID %n\n", $type, $uuid.toString());

	$.test_value($uuid.isNull(), False, $type, "UUID::isNull()");
	if (UUID::HAVE_UNPARSE_CASE) {
	    $.test_value($uuid.toString(UUID::LowerCase) =~ /^[\-0-9a-f]+$/, True, $type, "UUID::toString(LowerCase)");
	    $.test_value($uuid.toString(UUID::UpperCase) =~ /^[\-0-9A-F]+$/, True, $type, "UUID::toString(UpperCase)");
	}

	my UUID $other = $uuid.copy();
	$.test_value($uuid.compare($other), 0, $type, "UUID::copy()");

	$uuid.clear();
	#printf("%s UUID %n\n", $type, $uuid.toString());
	$.test_value($uuid.isNull(), True, $type, "UUID::clear()");
	$.test_value($uuid.compare($other), -1, $type, "UUID::compare()");

	$uuid.set("ffffffff-ffff-ffff-ffff-ffffffffffff");
	$.test_value($uuid.compare($other), 1, $type, "UUID::set()");
    }

    static usage() {
	printf("usage: %s [options]
 -h,--help        this help text
", get_script_name());
      exit(1);
   }

    test_value(any $v1, any $v2, string $type, string $msg) {
	if ($v1 === $v2)
	    stdout.printf("OK: %s %s test\n", $type, $msg);
	else {
	    stdout.printf("ERROR: %s %s test failed! (%N != %N)\n", $type, $msg, $v1, $v2);
	    ++$.errors;
	}
	$.ehash.($type + "-" + $msg) = True;
    }
}
