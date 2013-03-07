/**
  AES encryption/decryption demo program using OpenSSL EVP apis
  gcc -Wall openssl_aes.c -lcrypto

  this is public domain code.

  Saju Pillai (saju.pillai@gmail.com)
**/

#include "gwan.h"    // G-WAN exported functions
#pragma link "crypto"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

/**
 * Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
int aes_initialize(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx,
             EVP_CIPHER_CTX *d_ctx)
{
  int i, nrounds = 5;
  unsigned char key[32], iv[32];

  /*
   * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
   * nrounds is the number of times the we hash the material. More rounds are more secure but
   * slower.
   */
  i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
  if (i != 32) {
    printf("Key size is %d bits - should be 256 bits\n", i);
    return -1;
  }

  EVP_CIPHER_CTX_init(e_ctx);
  EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_CIPHER_CTX_init(d_ctx);
  EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  return 0;
}


int aes_initialize_own(EVP_CIPHER_CTX *e_ctx,
						EVP_CIPHER_CTX *d_ctx,
						unsigned char iv[],
						unsigned char key[]) {

  EVP_CIPHER_CTX_init(e_ctx);
  EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_CIPHER_CTX_init(d_ctx);
  EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  return 0;
}

int AES_BLOCK_SIZE = 32;
/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
  /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
  int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
  unsigned char *ciphertext = malloc(c_len);

  /* allows reusing of 'e' for multiple encryption cycles */
  EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

  /* update ciphertext, c_len is filled with the length of ciphertext generated,
    *len is the size of plaintext in bytes */
  EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

  /* update ciphertext with the final remaining bytes */
  EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);

  *len = c_len + f_len;
  return ciphertext;
}

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{
  /* because we have padding ON, we must allocate an extra cipher block size of memory */
  int p_len = *len, f_len = 0;
  unsigned char *plaintext = malloc(p_len + AES_BLOCK_SIZE);

  EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
  EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);

  *len = p_len + f_len;
  return plaintext;
}

/*int main(int argc, char *argv[]) {
   "opaque" encryption, decryption ctx structures that libcrypto uses to record
     status of enc/dec operations
  EVP_CIPHER_CTX en, de;

   8 bytes to salt the key_data during key generation. This is an example of
     compiled in salt. We just read the bit pattern created by these two 4 byte
     integers on the stack as 64 bits of contigous salt material -
     ofcourse this only works if sizeof(int) >= 4
  unsigned int salt[] = {12345, 54321};
  int key_data_len, i;
  unsigned char *input[] = {"809029467|721299820BroadcomCARD-35230:wuaki1362156941", NULL};

   the key_data is read from the argument list
  unsigned char *key_data = "bb8a2d02a5a55122d5da5681a04ca5cd2d300ec33eb72437681f9d00ba4b9386";//(unsigned char *)argv[1];
  unsigned char* iv = "2d177a67960da95030d7326686d1d4ca";
  key_data_len = strlen(key_data);

  printf("KEY lenght: %d\n", key_data_len);

   gen key and iv. init the cipher ctx object
  if (aes_initialize(key_data, key_data_len, iv, &en, &de)) {
    printf("Couldn't initialize AES cipher\n");
    return -1;
  }

   encrypt and decrypt each input string and compare with the original
  for (i = 0; input[i]; i++) {
    char *plaintext;
    unsigned char *ciphertext;
    int olen, len;

     The enc/dec functions deal with binary data and not C strings. strlen() will
       return length of the string without counting the '\0' string marker. We always
       pass in the marker byte to the encrypt/decrypt functions so that after decryption
       we end up with a legal C string
    olen = len = strlen(input[i])+1;

    ciphertext = aes_encrypt(&en, (unsigned char *)input[i], &len);
    plaintext = (char *)aes_decrypt(&de, ciphertext, &len);

    printf("OK: \"%s\"\n", ciphertext);

    unsigned char sign[strlen(ciphertext)];
    s_snprintf(sign, 255, "%B", ciphertext);
    printf("Base64 Sig: %s\n", sign);

    if (strncmp(plaintext, input[i], olen))
      printf("FAIL: enc/dec failed for \"%s\"\n", input[i]);
    else
      printf("OK: enc/dec ok for \"%s\"\n", plaintext);

    printf("ddddd %x", ('h' << 4) + ('d' << 16));

    free(ciphertext);
    free(plaintext);
  }

  EVP_CIPHER_CTX_cleanup(&en);
  EVP_CIPHER_CTX_cleanup(&de);

  return 0;
}*/
