shrink
======

[![Build Status](https://travis-ci.org/conformal/shrink.png?branch=master)]
(https://travis-ci.org/conformal/shrink)

The shrink library provides a single API into several compression
algorithms. It enables developers to easily add compression and
decompression functionality to an existing code base. Currently it
supports:

- LZO
- LZ77
- LZMA

All these fine algorithms have pros and cons.
LZO is the fastest by an order of magnitude but trades of compression
ratio for speed. LZ77 is the middle of the road on both speed and
compression ration. LZMA is slow but compresses the best.  The idea of
this library is to provide an app writer with the capability of using
any compression/decompression algorithm without having to understand the
intricate parts. Now there is no excuse to not add compression to any
code!

## License

shrink is licensed under the liberal ISC License.
