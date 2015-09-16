**Aarni** is a small and easy-to-use file encryptor. It's based on [Qt 4](http://qt.nokia.com/).

Currently, [AES-128](http://www.csrc.nist.gov/publications/fips/fips197/fips-197.pdf) in the [XTS mode](http://en.wikipedia.org/wiki/Disk_encryption_theory#XTS) (approved as the [IEEE 1619 Standard](https://siswg.net/)) is used to encrypt data. [SHA-384](http://csrc.nist.gov/publications/fips/fips180-3/fips180-3_final.pdf) is used to generate the encryption key and the initial tweak value from the password inputed by the user. All files are compressed before encryption.

**NOTE** If you forget the password you use, the only way I know to decrypt the file is to crack it by brutal force. So, be careful ;o

Your suggestion is warmly welcome!

Really sorry for forgetting where I downloaded these icons :( Probably some where at http://www.kde-look.org/