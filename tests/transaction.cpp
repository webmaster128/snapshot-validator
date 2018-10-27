#include <transaction.h>

#include "catch.hpp"

using Catch::Matchers::Equals;

TEST_CASE("Transaction") {
    SECTION("serialization of type 0 with no asset data works") {
        Transaction transaction(
                    0,
                    865708731,
                    bytes_t{
                        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
                    },
                    10010344879730196491ul,
                    123456789ul,
                    10000000, // fee, not serialized
                    bytes_t{},
                    0 // not serialized
                    );

        auto serialized = transaction.serialize();

        REQUIRE_THAT(serialized,
                     Equals(bytes_t{
                                0x00, // type
                                0xbb, 0xaa, 0x99, 0x33, // timestamp (le)
                                0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                                0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                                0x8a, 0xeb, 0xe3, 0xa1, 0x8b, 0x78, 0x00, 0x0b, // recipient (be)
                                0x15, 0xcd, 0x5b, 0x07, 0x00, 0x00, 0x00, 0x00, // amount (le)
                            }));
    }

    SECTION("serialization of type 0 with asset data works") {
        Transaction transaction(
                    0,
                    865708731,
                    bytes_t{
                        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
                    },
                    10010344879730196491ul,
                    123456789ul,
                    10000000, // fee, not serialized
                    bytes_t{
                        // 64 chars memo "The nice memo I attach to that money for the whole world to read" (UTF-8 encoded)
                        0x54, 0x68, 0x65, 0x20, 0x6e, 0x69, 0x63, 0x65, 0x20, 0x6d, 0x65, 0x6d, 0x6f, 0x20, 0x49, 0x20,
                        0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x20, 0x74, 0x6f, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20, 0x6d,
                        0x6f, 0x6e, 0x65, 0x79, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x74, 0x68, 0x65, 0x20, 0x77, 0x68, 0x6f,
                        0x6c, 0x65, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x20, 0x74, 0x6f, 0x20, 0x72, 0x65, 0x61, 0x64,
                    },
                    0 // dappid, not serialized
                    );

        auto serialized = transaction.serialize();

        REQUIRE_THAT(serialized,
                     Equals(bytes_t{
                                // type
                                0x00,
                                // timestamp (le)
                                0xbb, 0xaa, 0x99, 0x33,
                                // pubkey
                                0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                                0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                                // recipient (be)
                                0x8a, 0xeb, 0xe3, 0xa1, 0x8b, 0x78, 0x00, 0x0b,
                                // amount (le)
                                0x15, 0xcd, 0x5b, 0x07, 0x00, 0x00, 0x00, 0x00,
                                // memo
                                0x54, 0x68, 0x65, 0x20, 0x6e, 0x69, 0x63, 0x65, 0x20, 0x6d, 0x65, 0x6d, 0x6f, 0x20, 0x49, 0x20,
                                0x61, 0x74, 0x74, 0x61, 0x63, 0x68, 0x20, 0x74, 0x6f, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20, 0x6d,
                                0x6f, 0x6e, 0x65, 0x79, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x74, 0x68, 0x65, 0x20, 0x77, 0x68, 0x6f,
                                0x6c, 0x65, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x20, 0x74, 0x6f, 0x20, 0x72, 0x65, 0x61, 0x64,
                            }));
    }

    SECTION("id of type 0 with no asset data works and one signature works") {
        Transaction transaction(
                    0,
                    865708731,
                    bytes_t{
                        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
                    },
                    10010344879730196491ul,
                    123456789ul,
                    10000000, // fee, not serialized
                    bytes_t{},
                    0 // not serialized
                    );

        auto signature = bytes_t{0x26, 0x27, 0x28, 0x29};
        auto id = transaction.id(signature, bytes_t{});

        // integer of little endian
        // $ echo 00bbaa993300112233445566778899aabbccddeeff00112233445566778899aabbccddeeff8aebe3a18b78000b15cd5b070000000026272829 | xxd -r -p - | sha256sum | head -c 16 && echo
        // 445505dc3ee55bdb
        REQUIRE(id == 15806479375328957764ul);
    }
}