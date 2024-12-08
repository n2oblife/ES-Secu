/* Source file for parsing the packet protocol used in assignment 2
 *
 * Copyright 2018 TUM
 * Created: 2018-11-15 Florian Wilde <florian.wilde@tum.de>
 **************************************************************************************************/

#include "VirtualSerial.h"
#include "base64url.h"
#include "packetizer.h"

int16_t packetizerReceiveByteBlocking(void) {
  /* Wait until at least one byte is received and if so return it */
  while(CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface) <= 0) {
    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
  }
  return CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
}

enum packetizerErrors packetizerReadHeader(pt_s *job) {
  /* We received a start of header (SoH, 0x01) and now gather all the bytes that belong to the
     header up to and including the start of text (SoT, 0x02) to see if the header is too long */
  char headerB64[PACKETIZER_HEADER_B64_LEN] = { 0 };
  char header[PACKETIZER_HEADER_LEN] = { 0 };
  size_t header_len = sizeof(header);
  int16_t rbuf = 0;
 
  for(size_t headerB64idx = 0; headerB64idx < PACKETIZER_HEADER_B64_LEN; ) {
    rbuf = packetizerReceiveByteBlocking();

    if(rbuf > 0xFF) {
      /* Error in CDC_Device_ReceiveByte(), ignore it */
    } else if(rbuf == 0x02) {
      /* A premature SoT means the header is too short */
      return packetizer_HeaderIncorrectSize;
    } else if(isbase64(rbuf) || rbuf == '=') {
      headerB64[headerB64idx++] = rbuf;
    } else {
      /* Return appropriate error if we encounter a character not in the base64url alphabet.
         Because isbase64() considers padding to be not part of the alphabet, allow it also. */
      return packetizer_IllegalCharacter;
    }
  }
  /* Here we wait for the SoT to check that the header is not too long */
  if(packetizerReceiveByteBlocking() != 0x02) {
    return packetizer_HeaderIncorrectSize;
  }
  /* We received a header of correct size, so remove base64url encoding and put content into job */
  if(base64_decode(headerB64, PACKETIZER_HEADER_B64_LEN, header, &header_len)) {
    if(header_len == sizeof(header)) {
      job->textLen = header[2] << 16 | header[1] << 8 | header[0];
      job->nonce = malloc(CRYPTO_NONCE_BYTES);
      if(job->nonce != NULL) {
        memcpy(job->nonce, header + 3, CRYPTO_NONCE_BYTES);
      } else {
        return packetizer_NonceAllocationFailed;
      }
    } else {
      /* If decoded length does not match, header is also of incorrect length */
      return packetizer_HeaderIncorrectSize;
    }
  } else {
    /* Decoding failed, e.g. because padding was incorrect */
    return packetizer_HeaderDecodingFailed;
  }
  return packetizer_Successful;
}

enum packetizerErrors packetizerReadText(pt_s *job) {
  /* We received a correct header, now allocate space for the text and receive it up to and
     including the end of text (EoT, 0x03) to see if the text is too long. Then remove base64url
     encoding */
  enum packetizerErrors ret = packetizer_Successful;
  int16_t rbuf = 0;
  size_t textB64_len = job->textLen * sizeof(char), text_len = 0;
  char *textB64 = calloc(job->textLen, sizeof(char)), *text = NULL;
  if(textB64 == NULL) {
    return packetizer_TextAllocationFailed;
  }
 
  /* Allocate memory for text (in base64 yet) and receive it */
  for(size_t textB64idx = 0; textB64idx < job->textLen; ) {
    rbuf = packetizerReceiveByteBlocking();

    if(rbuf > 0xFF) {
      /* Error in CDC_Device_ReceiveByte(), ignore it */
    } else if(rbuf == 0x03) {
      /* A premature EoT means the text is too short */
      ret = packetizer_TextIncorrectSize;
      break;
    } else if(isbase64(rbuf) || rbuf == '=') {
      textB64[textB64idx++] = rbuf;
    } else {
      /* Return appropriate error if we encounter a character not in the base64url alphabet.
         Because isbase64() considers padding to be not part of the alphabet, allow it also. */
      ret = packetizer_IllegalCharacter;
      break;
    }
  }
  if(ret == packetizer_Successful) {
    /* Here we wait for the EoT to check that the text is not too long */
    if(packetizerReceiveByteBlocking() != 0x03) {
      ret = packetizer_TextIncorrectSize;
    }
  }
  if(ret == packetizer_Successful) {
    /* We received a text of correct size, so remove base64url encoding and put link into job */
    if(base64_decode_alloc(textB64, textB64_len, &text, &text_len)) {
      job->textLen = text_len;
      if(text != NULL) {
        job->text = (uint8_t *) text;
      } else {
        /* If text pointer is NULL, malloc failed */
        ret = packetizer_TextAllocationFailed;
      }
    } else {
      /* Decoding failed, e.g. because padding was incorrect */
      ret = packetizer_TextDecodingFailed;
    }
  }

  /* Regardless of our outcome, we have to free the textB64, because we loose the pointer to it on
     return */
  free(textB64);
  return ret;
}
  
enum packetizerErrors packetizerReceive(pt_s *job) {
  enum packetizerErrors ret;

  /* Discard bytes until we find a start of header (SoH, 0x01) */
  while(packetizerReceiveByteBlocking() != 0x01);
  /* Read header and fill in job or forward error */
  ret = packetizerReadHeader(job);
  if(ret != packetizer_Successful) {
    return ret;
  }
  /* Read text and add link to it to job or forward error */
  ret = packetizerReadText(job);
  if(ret != packetizer_Successful) {
    return ret;
  }
  return packetizer_Successful;
}

enum packetizerErrors packetizerSend(ct_s *output) {
  enum packetizerErrors ret = packetizer_Successful;
  char *textB64 = NULL;
  size_t textB64_len = 0;

  /* Encode ciphertext into base64url, return fail if not successful */
  textB64_len = base64_encode_alloc((char *) output->text, output->textLen, &textB64);
  if(textB64 == NULL) {
    if(output->textLen > 0 && textB64_len <= 0) {
      /* Size computation overflow in base64url.c */
      ret = packetizer_CiphertextTooLarge;
    } else {
      ret = packetizer_CiphertextAllocationFailed;
    }
  } else {
    CDC_Device_SendByte(&VirtualSerial_CDC_Interface, 0x02);
    CDC_Device_SendData(&VirtualSerial_CDC_Interface, textB64, textB64_len);
    CDC_Device_SendByte(&VirtualSerial_CDC_Interface, 0x03);
  }

  return ret;
}
