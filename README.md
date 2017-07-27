# Cropicon

Cropicon is a C++ for cropcircle-like SVG-image generation from the first bits of a SHA-256 hash.

![Sample Cropicon images](example.jpg "Cropicoin examples")


## Usage:

cropicon.o

1. argument -> SHA256 hashvalue.
2. argument -> output SVG-filename.
 

./cropicoin.o [hash_sha256] [output.svg]

Example:
./cropicoin.o 9636fab63995a32e7875b0b5b9c2883cd599b5370bd0da9d2ff6f4e73ea130ec mycropcircle.svg


