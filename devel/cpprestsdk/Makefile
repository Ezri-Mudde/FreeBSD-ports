# $FreeBSD$

PORTNAME=	cpprestsdk
PORTVERSION=	2.9.1
DISTVERSIONPREFIX=	v
PORTREVISION=	13
CATEGORIES=	devel

MAINTAINER=	projects@phess.net
COMMENT=	Microsoft C++ REST SDK

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/../license.txt

BROKEN_powerpc64=	fails to compile: threadpool.h:59:25: conversion to 'int' from 'size_t {aka long unsigned int}' may alter its value

LIB_DEPENDS=	libboost_system.so:devel/boost-libs

USES=		cmake compiler:c++11-lib ssl
USE_LDCONFIG=	yes
USE_GITHUB=	yes
GH_ACCOUNT=	Microsoft
WRKSRC_SUBDIR=	Release
CMAKE_OFF=	BUILD_SAMPLES BUILD_TESTS

.include <bsd.port.pre.mk>

.if ${SSL_DEFAULT} == base
BROKEN_FreeBSD_12=	use of undeclared identifier 'SSL_R_SHORT_READ'
BROKEN_FreeBSD_13=	use of undeclared identifier 'SSL_R_SHORT_READ'
.endif

.include <bsd.port.post.mk>
