/*
 * Copyright 2019 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <openssl/crypto.h>
#include <openssl/core_numbers.h>
#include <openssl/sha.h>
#include <openssl/params.h>
#include <openssl/core_names.h>
#include "internal/core_mkdigest.h"
#include "internal/provider_algs.h"
#include "internal/sha.h"

static OSSL_OP_digest_set_params_fn sha1_set_params;

/* Special set_params method for SSL3 */
static int sha1_set_params(void *vctx, const OSSL_PARAM params[])
{
    int cmd = 0;
    size_t msg_len = 0;
    const void *msg = NULL;
    const OSSL_PARAM *p;
    SHA_CTX *ctx = (SHA_CTX *)vctx;

    if (ctx != NULL && params != NULL) {
        p = OSSL_PARAM_locate(params, OSSL_DIGEST_PARAM_CMD);
        if (p != NULL && !OSSL_PARAM_get_int(p, &cmd))
            return 0;
        p = OSSL_PARAM_locate(params, OSSL_DIGEST_PARAM_MSG);
        if (p != NULL && !OSSL_PARAM_get_octet_ptr(p, &msg, &msg_len))
            return 0;
        return sha1_ctrl(ctx, cmd, msg_len, (void *)msg);
    }
    return 0;
}

OSSL_FUNC_DIGEST_CONSTRUCT_PARAMS(sha1, SHA_CTX,
                           SHA_CBLOCK, SHA_DIGEST_LENGTH,
                           SHA1_Init, SHA1_Update, SHA1_Final,
                           sha1_set_params)

OSSL_FUNC_DIGEST_CONSTRUCT(sha224, SHA256_CTX,
                           SHA256_CBLOCK, SHA224_DIGEST_LENGTH,
                           SHA224_Init, SHA224_Update, SHA224_Final)

OSSL_FUNC_DIGEST_CONSTRUCT(sha256, SHA256_CTX,
                           SHA256_CBLOCK, SHA256_DIGEST_LENGTH,
                           SHA256_Init, SHA256_Update, SHA256_Final)

OSSL_FUNC_DIGEST_CONSTRUCT(sha384, SHA512_CTX,
                           SHA512_CBLOCK, SHA384_DIGEST_LENGTH,
                           SHA384_Init, SHA384_Update, SHA384_Final)

OSSL_FUNC_DIGEST_CONSTRUCT(sha512, SHA512_CTX,
                           SHA512_CBLOCK, SHA512_DIGEST_LENGTH,
                           SHA512_Init, SHA512_Update, SHA512_Final)

OSSL_FUNC_DIGEST_CONSTRUCT(sha512_224, SHA512_CTX,
                           SHA512_CBLOCK, SHA224_DIGEST_LENGTH,
                           sha512_224_init, SHA512_Update, SHA512_Final)

OSSL_FUNC_DIGEST_CONSTRUCT(sha512_256, SHA512_CTX,
                           SHA512_CBLOCK, SHA256_DIGEST_LENGTH,
                           sha512_256_init, SHA512_Update, SHA512_Final)

