#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// PEM 형식의 인증서로부터 공개 키를 로드하는 함수
EVP_PKEY* load_public_key_from_cert(const char* cert_file) {
    FILE* cert_fp = fopen(cert_file, "r");
    if (!cert_fp) {
        cerr << "Error opening certificate file\n";
        return nullptr;
    }

    X509* cert = PEM_read_X509(cert_fp, nullptr, nullptr, nullptr);
    fclose(cert_fp);

    if (!cert) {
        cerr << "Error reading certificate\n";
        return nullptr;
    }

    // 인증서에서 공개 키 추출
    EVP_PKEY* pub_key = X509_get_pubkey(cert);
    X509_free(cert);

    if (!pub_key) {
        cerr << "Error extracting public key from certificate\n";
    }

    return pub_key;
}

// 서명 검증 함수
bool verify_signature(EVP_PKEY* pub_key, const unsigned char* message, size_t message_len,
                      const unsigned char* signature, size_t signature_len) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        cerr << "Error creating context\n";
        return false;
    }

    if (EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, pub_key) <= 0) {
        cerr << "Error initializing digest verify\n";
        EVP_MD_CTX_free(ctx);
        return false;
    }

    if (EVP_DigestVerifyUpdate(ctx, message, message_len) <= 0) {
        cerr << "Error updating digest verify\n";
        EVP_MD_CTX_free(ctx);
        return false;
    }

    int result = EVP_DigestVerifyFinal(ctx, signature, signature_len);
    EVP_MD_CTX_free(ctx);

    return (result == 1);
}

// 파일에서 서명을 로드하는 함수
vector<unsigned char> load_signature(const char* sig_file) {
    ifstream file(sig_file, ios::binary | ios::ate);
    if (!file) {
        cerr << "Error opening signature file\n";
        return {};
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<unsigned char> signature(size);
    if (!file.read(reinterpret_cast<char*>(signature.data()), size)) {
        cerr << "Error reading signature file\n";
        return {};
    }

    return signature;
}

int main() {
    const char* cert_file = "certificate.pem";
    const char* sig_file = "signature.bin";
    const char* message = "This is a message to sign";

    // 인증서로부터 공개 키 로드
    EVP_PKEY* pub_key = load_public_key_from_cert(cert_file);
    if (!pub_key) {
        return 1;
    }

    // 서명 파일 로드
    vector<unsigned char> signature = load_signature(sig_file);
    if (signature.empty()) {
        return 1;
    }

    // 서명 검증
    if (verify_signature(pub_key, (unsigned char*)message, strlen(message), signature.data(), signature.size())) {
        cout << "Signature is valid\n";
    } else {
        cerr << "Signature is invalid\n";
    }

    // 공개 키 해제
    EVP_PKEY_free(pub_key);

    return 0;
}
