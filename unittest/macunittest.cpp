/**
 * Copyright (C)  2004  Brad Hards <bradh@frogmouth.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "macunittest.h"
#include "qca.h"
#include <iostream>

MACUnitTest::MACUnitTest()
    : Tester()
{

}

void MACUnitTest::allTests()
{
    QCA::Initializer init;

    QCString macResult; // used as the actual result

    if(!QCA::isSupported("hmac(md5)"))
	SKIP("HMAC(MD5) not supported!\n");
    else {
	// These tests are from RFC2202, Section 2.
	// The first three are also in the Appendix to RFC2104
	QCA::HMAC md5hmac1( "md5" );
	QCA::SymmetricKey key1;
	key1 = QCString( "Jefe" );
	key1.resize( 4 );
	md5hmac1.setup( key1 );
	QSecureArray data1 = QCString( "what do ya want for nothing?" );
	data1.resize( 28 );
	md5hmac1.update( data1 );
	CHECK( QCA::arrayToHex( md5hmac1.final() ), QString( "750c783e6ab0b503eaa86e310a5db738" ) );

	QCA::HMAC md5hmac2( "md5" );
	QCA::SymmetricKey key2( QCA::hexToArray( "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b" ) );
	md5hmac2.setup( key2 );
	QSecureArray data2 = QCString( "Hi There" );
	data2.resize( 8 );
	md5hmac2.update( data2 );
	CHECK( QCA::arrayToHex( md5hmac2.final() ), QString( "9294727a3638bb1c13f48ef8158bfc9d" ) );

	// test reuse
	md5hmac2.clear();
	QCA::SymmetricKey key3( QCA::hexToArray( "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" ) );
	md5hmac2.setup ( key3 );
	QSecureArray data3( 50 );
	for (unsigned int i = 0; i < data3.size(); i++ )
	    data3[ i ] = 0xDD;
	md5hmac2.update( data3 );
	CHECK( QCA::arrayToHex( md5hmac2.final() ), QString( "56be34521d144c88dbb8c733f0e8b3f6" ) );

	QCA::HMAC md5hmac4( "md5" );
	QCA::SymmetricKey key4 ( QCA::hexToArray( "0102030405060708090a0b0c0d0e0f10111213141516171819") );
	md5hmac4.setup( key4 );
	QSecureArray data4( 50 );
	for (unsigned int i = 0; i < data4.size(); i++ )
	    data4[ i ] = 0xcd;
	md5hmac4.update( data4 );
	CHECK( QCA::arrayToHex( md5hmac4.final() ), QString( "697eaf0aca3a3aea3a75164746ffaa79" ) );

	QCA::HMAC md5hmac5( "md5" );
	QCA::SymmetricKey key5 ( QCA::hexToArray( "0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c" ) );
	md5hmac5.setup( key5 );
	QSecureArray data5 = QCString( "Test With Truncation" );
	data5.resize( 20 );
    	md5hmac5.update( data5 );
	CHECK( QCA::arrayToHex( md5hmac5.final() ), QString( "56461ef2342edc00f9bab995690efd4c" ) );

	QCA::HMAC md5hmac6( "md5" );
	QCA::SymmetricKey key6( 80 );
	for (unsigned int i = 0; i < key6.size(); i++)
	    key6[ i ] = 0xaa;
	md5hmac6.setup( key6 );
	QSecureArray data6 = QCString( "Test Using Larger Than Block-Size Key - Hash Key First" );
	data6.resize( 54 );
    	md5hmac6.update( data6 );
	CHECK( QCA::arrayToHex( md5hmac6.final() ), QString( "6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd" ) );

	QCA::HMAC md5hmac7( "md5" );
	md5hmac7.setup( key6 ); // same as previous test, so just reuse
	QSecureArray data7 = QCString( "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data" );
	data7.resize( 73 );
    	md5hmac7.update( data7 );
	CHECK( QCA::arrayToHex( md5hmac7.final() ), QString( "6f630fad67cda0ee1fb1f562db3aa53e" ) );
    }

    if(!QCA::isSupported("hmac(sha1)"))
	SKIP("HMAC(SHA1) not supported!\n");
    else {
	// These tests are from RFC2202, Section 3.
	QCA::HMAC test1; // should be default
	QCA::SymmetricKey key1 ( QCA::hexToArray( "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b" ) );
	test1.setup( key1 );
	QSecureArray data1 = QCString( "Hi There" );
	data1.resize( 8 );
    	test1.update( data1 );
	CHECK( QCA::arrayToHex( test1.final() ), QString( "b617318655057264e28bc0b6fb378c8ef146be00" ) );

	QCA::HMAC test2( "sha1" ); 
	QCA::SymmetricKey key2;
	key2 = QCString( "Jefe" );
	key2.resize( 4 );
	test2.setup( key2 );
	QSecureArray data2 = QCString( "what do ya want for nothing?" );
	data2.resize( 28 );
    	test2.update( data2 );
	CHECK( QCA::arrayToHex( test2.final() ), QString( "effcdf6ae5eb2fa2d27416d5f184df9c259a7c79" ) );

	QCA::HMAC test3;
	QCA::SymmetricKey key3( QCA::hexToArray( "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" ) );
	test3.setup( key3 );
	QSecureArray data3( 50 );
	for ( unsigned int i = 0; i < data3.size(); i++ )
	    data3[ i ] = 0xDD;
	test3.update( data3 );
	CHECK( QCA::arrayToHex( test3.final() ), QString( "125d7342b9ac11cd91a39af48aa17b4f63f175d3" ) );

	QCA::HMAC test4;
	QCA::SymmetricKey key4( QCA::hexToArray( "0102030405060708090a0b0c0d0e0f10111213141516171819" ) );
	test4.setup( key4 );
	QSecureArray data4( 50 );
	for ( unsigned int i = 0; i < data4.size(); i++ )
	    data4[ i ] = 0xcd;
	test4.update( data4 );
	CHECK( QCA::arrayToHex( test4.final() ), QString( "4c9007f4026250c6bc8414f9bf50c86c2d7235da" ) );

	QCA::HMAC test5; // should be default
	QCA::SymmetricKey key5 ( QCA::hexToArray( "0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c" ) );
	test5.setup( key5 );
	QSecureArray data5 = QCString( "Test With Truncation" );
	data5.resize( 20 );
    	test5.update( data5 );
	CHECK( QCA::arrayToHex( test5.final() ), QString( "4c1a03424b55e07fe7f27be1d58bb9324a9a5a04" ) );

	QCA::HMAC test6; // should be default
	QCA::SymmetricKey key6( 80 );
	for ( unsigned int i = 0; i < key6.size(); i++ )
	    key6[i] = 0xAA;
	test6.setup( key6 );
	QSecureArray data6 = QCString( "Test Using Larger Than Block-Size Key - Hash Key First" );
	data6.resize( 54 );
    	test6.update( data6 );
	CHECK( QCA::arrayToHex( test6.final() ), QString( "aa4ae5e15272d00e95705637ce8a3b55ed402112" ) );

	test6.clear();
	test6.setup( key6 ); // reuse test, same key
	QSecureArray data7 = QCString( "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data" );
	data7.resize( 73 );
    	test6.update( data7 );
	CHECK( QCA::arrayToHex( test6.final() ), QString( "e8e99d0f45237d786d6bbaa7965c7808bbff1a91" ) );

    }

}

