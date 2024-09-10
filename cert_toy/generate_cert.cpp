#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <iostream>

void generate_certificate(RSA* rsa) {
    X509 *x509 = X509_new();  // Create new X509 structure

    // Set the version
    X509_set_version(x509, 2);  // Version 3 certificate

    // Set the serial number
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

    // Set the validity period (from now to 365 days in the future)
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 365 * 24 * 60 * 60);

    // Set the public key for the certificate
    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, rsa);  // Assign RSA key to PKEY
    X509_set_pubkey(x509, pkey);

    // Set the subject name (basic information)
    X509_NAME *name = X509_get_subject_name(x509);
    X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char*)"US", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char*)"My Organization", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"My Certificate", -1, -1, 0);

    // Set issuer name (in self-signed certs, subject and issuer are the same)
    X509_set_issuer_name(x509, name);

    // Sign the certificate with our private key
    X509_sign(x509, pkey, EVP_sha256());

    // Write the certificate to a file
    FILE *cert_file = fopen("certificate.pem", "wb");
    PEM_write_X509(cert_file, x509);
    fclose(cert_file);

    // Clean up
    EVP_PKEY_free(pkey);
    X509_free(x509);

    std::cout << "Certificate generated and saved." << std::endl;
}

int main() {
    OpenSSL_add_all_algorithms();

    // Generate RSA key
    int bits = 2048;
    unsigned long e = RSA_F4;
    RSA *rsa = RSA_generate_key(bits, e, nullptr, nullptr);
    if (!rsa) {
        std::cerr << "Error generating RSA key" << std::endl;
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // Save private key to file
    FILE *priv_file = fopen("private_key.pem", "wb");
    PEM_write_RSAPrivateKey(priv_file, rsa, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(priv_file);

    FILE *pub_file = fopen("public_key.pem", "wb");
    PEM_write_RSAPublicKey(pub_file, rsa);
    fclose(pub_file);

    // Generate and save certificate
    generate_certificate(rsa);

    // Free the RSA structure
    RSA_free(rsa);

    return 0;
}
