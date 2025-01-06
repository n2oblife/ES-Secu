#include "VirtualSerial.h"
#include "packetizer.h"
#include <sodium.h>

int main(void) {
    extern uint8_t g_chipid[16];              // Unique chip ID
    uint8_t key[32] = {0};  // 256-bit key derived from chip ID
    
    // Create the 32-bit key by concatenating the chip ID with itself
    // memcpy(key, g_chipid, 16);
    // memcpy(key + 16, g_chipid, 16);
    for (int i = 0; i < 16; i++) {
        key[i] = g_chipid[i];
        key[i + 16] = g_chipid[i];
    }
    
    pt_s plaintext = {0};
    ct_s ciphertext = {0};


    // Set custom RNG before initializing libsodium
    randombytes_set_implementation(&randombytes_stir);
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized; it is not safe to use */
    }

    // Initialize USB
    USB_Init();

    while (1) {
        // Receive and parse packets
        if (packetizerReceive(&plaintext) == 0) {
            // Generate nonce if not provided
            if (plaintext.nonce == NULL) {
                plaintext.nonce = sodium_malloc(crypto_secretbox_NONCEBYTES);
                if (plaintext.nonce == NULL) {
                    continue; // Handle memory allocation failure
                }
                randombytes_buf(plaintext.nonce, crypto_secretbox_NONCEBYTES);
            }

            // Allocate memory for ciphertext
            ciphertext.text = sodium_malloc(plaintext.textLen + crypto_secretbox_MACBYTES);
            if (ciphertext.text == NULL) {
                sodium_free(plaintext.nonce);
                plaintext.nonce = NULL;
                continue; // Handle memory allocation failure
            }

            // Encrypt the plaintext using libsodium
            if (crypto_secretbox_easy(ciphertext.text, plaintext.text, plaintext.textLen,
                                      plaintext.nonce, key) == 0) {
                ciphertext.textLen = plaintext.textLen + crypto_secretbox_MACBYTES;
                packetizerSend(&ciphertext);  // Send the ciphertext back
            }

            // Free memory
            sodium_free(plaintext.nonce);
            plaintext.nonce = NULL;
            sodium_free(ciphertext.text);
            ciphertext.text = NULL;
            free(plaintext.text);
            plaintext.text = NULL;
        }
    }
}
