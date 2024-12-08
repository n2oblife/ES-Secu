/* Header file for parsing the packet protocol used in assignment 2
 *
 * Copyright 2018 TUM
 * Created: 2018-11-15 Florian Wilde <florian.wilde@tum.de>
 **************************************************************************************************/

#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <stdint.h>
#include "crypto.h"
#include "base64url.h"

/* Type definitions */
typedef struct {
  uint8_t *text;
  size_t textLen;
  uint8_t *nonce;
} pt_s;

typedef struct {
  uint8_t *text;
  size_t textLen;
} ct_s;

enum packetizerState {packetizer_wait4SoH,
                      packetizer_collectHeader,
                      packetizer_wait4SoT,
                      packetizer_collectText,
                      packetizer_wait4EoT,
                      packetizer_complete };

enum packetizerErrors {packetizer_Successful,
                       packetizer_IllegalCharacter,
                       packetizer_HeaderIncorrectSize,
                       packetizer_HeaderDecodingFailed,
                       packetizer_NonceAllocationFailed,
                       packetizer_TextAllocationFailed,
                       packetizer_TextIncorrectSize,
                       packetizer_TextDecodingFailed,
                       packetizer_CiphertextAllocationFailed,
                       packetizer_CiphertextTooLarge };

#define PACKETIZER_TEXTLEN_LEN ((size_t) 3)
#define PACKETIZER_HEADER_LEN ((size_t) PACKETIZER_TEXTLEN_LEN + CRYPTO_NONCE_BYTES)
#define PACKETIZER_HEADER_B64_LEN ((size_t) BASE64_LENGTH(PACKETIZER_HEADER_LEN))

/* Function definitions */
enum packetizerErrors packetizerReceive(pt_s *job);
enum packetizerErrors packetizerSend(ct_s *output);

#endif/*PACKETIZER_H*/
