#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <iostream>

void generate_key() {
    int bits = 2048;  // Key length
    unsigned long e = RSA_F4;  // Public exponent

    // Generate RSA key
    RSA *rsa = RSA_generate_key(bits, e, nullptr, nullptr);
    if (!rsa) {
        std::cerr << "Error generating RSA key" << std::endl;
        ERR_print_errors_fp(stderr);
        return;
    }

    // Save private key
    FILE *priv_file = fopen("private_key.pem", "wb");
    PEM_write_RSAPrivateKey(priv_file, rsa, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(priv_file);

    // Save public key
    FILE *pub_file = fopen("public_key.pem", "wb");
    PEM_write_RSA_PUBKEY(pub_file, rsa);
    fclose(pub_file);

    // Clean up
    RSA_free(rsa);
    std::cout << "Keys generated and saved." << std::endl;
}

int main() {
    OpenSSL_add_all_algorithms();  // Initialize OpenSSL algorithms
    generate_key();
    return 0;
}
