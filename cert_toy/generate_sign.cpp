#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// PEM 형식의 개인 키를 로드하는 함수
EVP_PKEY* load_private_key(const char* priv_key_file) {
    FILE* priv_key_fp = fopen(priv_key_file, "r");
    if (!priv_key_fp) {
        cerr << "Error opening private key file\n";
        return nullptr;
    }

    EVP_PKEY* priv_key = PEM_read_PrivateKey(priv_key_fp, nullptr, nullptr, nullptr);
    fclose(priv_key_fp);

    if (!priv_key) {
        cerr << "Error reading private key\n";
    }

    return priv_key;
}

// 서명 생성 함수
bool sign_message(EVP_PKEY* priv_key, const unsigned char* message, size_t message_len,
                  vector<unsigned char>& signature) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        cerr << "Error creating context\n";
        return false;
    }

    if (EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, priv_key) <= 0) {
        cerr << "Error initializing digest sign\n";
        EVP_MD_CTX_free(ctx);
        return false;
    }

    if (EVP_DigestSignUpdate(ctx, message, message_len) <= 0) {
        cerr << "Error updating digest sign\n";
        EVP_MD_CTX_free(ctx);
        return false;
    }

    size_t sig_len = 0;
    if (EVP_DigestSignFinal(ctx, nullptr, &sig_len) <= 0) {
        cerr << "Error getting signature size\n";
        EVP_MD_CTX_free(ctx);
        return false;
    }

    signature.resize(sig_len);
    if (EVP_DigestSignFinal(ctx, signature.data(), &sig_len) <= 0) {
        cerr << "Error generating signature\n";
        EVP_MD_CTX_free(ctx);
        return false;
    }

    EVP_MD_CTX_free(ctx);
    return true;
}

// 서명을 파일에 저장하는 함수
void save_signature(const vector<unsigned char>& signature, const char* sig_file) {
    ofstream file(sig_file, ios::binary);
    if (!file) {
        cerr << "Error opening signature file for writing\n";
        return;
    }
    file.write(reinterpret_cast<const char*>(signature.data()), signature.size());
    file.close();
}

int main() {
    const char* priv_key_file = "private_key.pem";
    const char* sig_file = "signature.bin";
    const char* message = "This is a message to sign";

    // 개인 키 로드
    EVP_PKEY* priv_key = load_private_key(priv_key_file);
    if (!priv_key) {
        return 1;
    }

    // 서명 생성
    vector<unsigned char> signature;
    if (sign_message(priv_key, (unsigned char*)message, strlen(message), signature)) {
        cout << "Signature generated successfully\n";
        save_signature(signature, sig_file);  // 서명을 파일에 저장
        cout << "Signature saved to " << sig_file << endl;
    } else {
        cerr << "Failed to generate signature\n";
    }

    // 개인 키 해제
    EVP_PKEY_free(priv_key);

    return 0;
}
