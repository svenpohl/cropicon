Cropicon
========

Cropicon is a C++ script for cropcircle-like SVG-image generation from the first bits of a SHA-256 hash.

![Sample Cropicon images](example.jpg "Cropicoin examples")


Usage
-----

cropicon.o

1. argument -> SHA256 hashvalue.
2. argument -> output SVG-filename.
 

./cropicoin.o [hash_sha256] [output.svg]

Example
-------
	./cropicoin.o 9636fab63995a32e7875b0b5b9c2883cd599b5370bd0da9d2ff6f4e73ea130ec mycropcircle.svg


Notes
-----
2^16 = 65536 main shapes, including the rotations, 2^24 (~16,7 million) different cropcircle-images.


Comments
--------
	Thank's to Jeff for some inspiration!
		
