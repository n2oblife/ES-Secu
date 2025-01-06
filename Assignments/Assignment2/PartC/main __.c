#include "VirtualSerial.h"
#include "packetizer.h"
#include <sodium.h>

int main(void) {
    uint8_t g_chip_id[16]; // Unique Chip ID
    uint8_t key[32];             // 256-bit key derived from the chip ID

    // Retrieve the chip's unique ID
    // get_chip_id(g_chip_id);
    
    // Generate the key by concatenating the chip ID with itself
    for (int i = 0; i < 16; i++) {
        key[i] = g_chip_id[i];
        key[i + 16] = g_chip_id[i];
    }

    pt_s plaintext = {0};


    // Initialize USB
    USB_Init();

    // Set randombytes implementation if needed
    randombytes_set_implementation(NULL); // Using default implementation
    if (sodium_init() < 0) {
        /* Panic! The library couldn't be initialized; it is not safe to use */
        while (1);
    }

    while (1) {
        /* Consume incoming data and parse it
           If so, it will fill the plaintext structure */
        if (packetizerReceive(&plaintext) == 0) {
            // Allocate memory for ciphertext
            ct_s ciphertext = {
                .text = sodium_malloc(plaintext.textLen + crypto_secretbox_MACBYTES),
                .textLen = 0,
            };

            if (ciphertext.text == NULL) {
                /* Memory allocation failed */
                free(plaintext.text);
                plaintext.text = NULL;
                continue;
            }

            // Ensure a nonce is available
            if (plaintext.nonce == NULL) {
                plaintext.nonce = sodium_malloc(crypto_secretbox_NONCEBYTES);
                if (plaintext.nonce == NULL) {
                    sodium_free(ciphertext.text);
                    ciphertext.text = NULL;
                    free(plaintext.text);
                    plaintext.text = NULL;
                    continue;
                }
                randombytes_buf(plaintext.nonce, crypto_secretbox_NONCEBYTES);
            }

            // Encrypt the plaintext using libsodium
            if (crypto_secretbox_easy(ciphertext.text, plaintext.text, plaintext.textLen,
                                      plaintext.nonce, key) == 0) {
                ciphertext.textLen = plaintext.textLen + crypto_secretbox_MACBYTES;
                packetizerSend(&ciphertext); // Send ciphertext back
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
